
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

void Pcre2Base::__construct(Php::Parameters &pIn)
{
	if (compileFlags == NULL) {
		compileFlags = new Flags();
		if (pIn.size() > 1 && !pIn[1].isNull())
			compileFlags->set((int64_t) pIn[1]);
		else
			compileFlags->set(Compile::UTF);
	}

	if (matchFlags == = NULL) {
		matchFlags = new Flags
		if (pIn.size() > 2 && !pIn[2].isNull())
			matchFlags->set((int64_t) pIn[2]);
		else
			matchFlags->set(Match::NOTEMPTY);
	}

	if (pIn.size() > 0 && !pIn[0].isNull() && pIn[0] != "") {
		Php::Parameters pNew;
		pNew.emplace_back(pIn[0]);
		compile(pNew);
	}
}

Php::Value Pcre2Base::compile(Php::Parameters &p)
{
	if (p.size() > 0 && !p[0].isNull()) {
		_regex_string = p[0];
	}

	if (p.size() > 1 && !p[1].isNull()) {
		compileFlags->set((int64_t) p[1]);
	}

	if (_regex_string == "") {
		throw Exception(-1);
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
		throw Exception(errorcode);

	_mcontext = pcre2_match_context_create(NULL);
	if (_mcontext == NULL)
		throw Exception(-1);

	if (compileFlags->hasFlags(DISKERROR_PCRE2_DO_JIT)) {
		int jitError = pcre2_jit_compile(_regex_compiled, PCRE2_JIT_COMPLETE);
		if (jitError)
			throw Exception(jitError);

		_jit_stack = pcre2_jit_stack_create(32 * 1024, 1024 * 1024, NULL);
		pcre2_jit_stack_assign(_mcontext, NULL, _jit_stack);
	}

	_match_data = pcre2_match_data_create_from_pattern(_regex_compiled, NULL);
	if (_match_data == NULL)
		throw Exception(-1);

	return this;
}

Php::Value Pcre2Base::setRegex(Php::Parameters &p)
{
	if(p[0] == "")
		throw Exception("regex string cannot be empty")

	_regex_string = p[0];

	if (p.size() > 1 && !p[1].isNull()) {
		compileFlags->set((int64_t) p[1]);
	}

	return this;
}

Php::Value Pcre2Base::getRegex()
{
	return _regex_string;
}

Pcre2Base::__destruct()
{
	_regex_string = "";

	if (matchFlags != NULL) {
		delete matchFlags;
		matchFlags = NULL;
	}

	if (compileFlags != NULL) {
		delete compileFlags;
		compileFlags = NULL;
	}

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
