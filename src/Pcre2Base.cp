
#include "Pcre2Base.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
Pcre2Base::Pcre2Base()
{
	_regex_string = "";
	_regex_compiled = NULL;
	_mcontext = NULL;
	_jit_stack = NULL;
	_match_data = NULL;

	compileFlags = NULL;
	matchFlags = NULL;
}

void Pcre2Base::__construct(Php::Parameters &p)
{
	if (compileFlags == NULL) {
		compileFlags = new Flags::Compile();
		if (p.size() > 1 && !p[1].isNull())
			compileFlags->set(p[1].numericValue());
	}

	if (matchFlags == NULL) {
		matchFlags = new Flags::Match();
		if (p.size() > 2 && !p[2].isNull())
			matchFlags->set(p[2].numericValue());
	}

	if (p.size() > 0 && !p[0].isNull() && p[0] != "") {
		p.resize(1);
		compile(p);
	}
}

Php::Value Pcre2Base::compile(Php::Parameters &p)
{
	if (p.size() > 0 && !p[0].isNull()) {
		_regex_string = p[0].rawValue();
	}

	if (p.size() > 1 && !p[1].isNull()) {
		compileFlags->set(p[1].numericValue());
	}

	if (_regex_string == "") {
		throw Pcre2Exception("regular expression string cannot be empty");
	}

	int errorcode;
	PCRE2_SIZE erroroffset;

	_regex_compiled = pcre2_compile(
		(PCRE2_SPTR) _regex_string.c_str(),
		PCRE2_ZERO_TERMINATED,
		(uint32_t) compileFlags->get(0x00000000FFFFFFFF),
		&errorcode,
		&erroroffset,
		NULL    //	match context
	);

	if (_regex_compiled == NULL)
		throw Pcre2Exception(errorcode);

	_mcontext = pcre2_match_context_create(NULL);
	if (_mcontext == NULL)
		throw Pcre2Exception("match context returned null, could not obtain memory");

	if (compileFlags->hasFlag(Flags::Compile::DO_JIT)) {
		int32_t jitError = pcre2_jit_compile(_regex_compiled, PCRE2_JIT_COMPLETE);
		if (jitError)
			throw Pcre2Exception(jitError);

		_jit_stack = pcre2_jit_stack_create(32 * 1024, 1024 * 1024, NULL);
		pcre2_jit_stack_assign(_mcontext, NULL, _jit_stack);
	}

	_match_data = pcre2_match_data_create_from_pattern(_regex_compiled, NULL);
	if (_match_data == NULL)
		throw Pcre2Exception("match data returned null, could not obtain memory");

	return this;
}

Php::Value Pcre2Base::setRegex(Php::Parameters &p)
{
	if(p[0].stringValue() == "")
		throw Pcre2Exception("regular expression string cannot be empty");

	_regex_string = p[0].stringValue();

	if (p.size() > 1 && !p[1].isNull()) {
		compileFlags->set(p[1].numericValue());
	}

	return this;
}

Php::Value Pcre2Base::getRegex() const
{
	return _regex_string;
}

void Pcre2Base::__destruct()
{
	if (_match_data != NULL) {
		pcre2_match_data_free(_match_data);
		_match_data = NULL;
	}

	if (_jit_stack != NULL) {
		pcre2_jit_stack_free(_jit_stack);
		_jit_stack = NULL;
	}

	if (_mcontext != NULL) {
		pcre2_match_context_free(_mcontext);
		_mcontext = NULL;
	}

	if (_regex_compiled != NULL) {
		pcre2_code_free(_regex_compiled);
		_regex_compiled = NULL;
	}

	if (matchFlags != NULL) {
		delete matchFlags;
		matchFlags = NULL;
	}

	if (compileFlags != NULL) {
		delete compileFlags;
		compileFlags = NULL;
	}

	_regex_string.clear();
	_regex_string.shrink_to_fit();
}

Pcre2Base::~Pcre2Base()
{
	if (matchFlags != NULL)
		delete matchFlags;

	if (compileFlags != NULL)
		delete compileFlags;

	if (_match_data != NULL)
		pcre2_match_data_free(_match_data);

	if (_jit_stack != NULL)
		pcre2_jit_stack_free(_jit_stack);

	if (_mcontext != NULL)
		pcre2_match_context_free(_mcontext);

	if (_regex_compiled != NULL)
		pcre2_code_free(_regex_compiled);
}
