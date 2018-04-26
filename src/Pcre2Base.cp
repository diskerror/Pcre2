
#include "Pcre2Base.h"
#include "flags/Compile.h"

Pcre2Base::Pcre2Base()
	: _regex_string("")
{
	_regex_compiled = NULL;
	_match_data = NULL;
	_mcontext = NULL;
	_jit_stack = NULL;

	compileFlags = 0;
	matchFlags = 0;
}

void Pcre2Base::__construct(Php::Parameters &p)
{
	if (p.size() > 1 && !p[1].isNull())
		compileFlags |= p[1].numericValue();
	else
		compileFlags |= (int64_t) Php::ini_get(EXT_NAME ".default_compile_flags").numericValue();

	if (p.size() > 2 && !p[2].isNull())
		matchFlags |= p[2].numericValue();
	else
		matchFlags |= (int64_t) Php::ini_get(EXT_NAME ".default_match_flags").numericValue();

	if (p.size() > 0 && !p[0].isNull() && p[0] != "") {
		p.resize(1);
		compile(p);
	}
}

void Pcre2Base::compile(Php::Parameters &p)
{
	if (p.size() > 0 && !p[0].isNull())
		_regex_string = p[0].rawValue();

	if (p.size() > 1 && !p[1].isNull())
		compileFlags = p[1].numericValue();

	if (p.size() > 2 && !p[2].isNull())
		matchFlags = p[2].numericValue();

	if (_regex_string == "")
		throw Php::Exception("regular expression string cannot be empty");

	int errorcode;
	PCRE2_SIZE erroroffset;

	_regex_compiled = pcre2_compile(
		(PCRE2_SPTR) _regex_string.c_str(),
		PCRE2_ZERO_TERMINATED,
		(uint32_t) compileFlags,
		&errorcode,
		&erroroffset,
		NULL    //	match context
	);

	if (_regex_compiled == NULL) {
		PCRE2_UCHAR eMessage[256];
		pcre2_get_error_message(errorcode, eMessage, sizeof(eMessage));

		char eOut[1024];
		std::snprintf(eOut, sizeof(eOut), "PCRE2 compilation failed at offset %d: %s", (int) erroroffset, eMessage);
		throw Php::Exception(eOut);
	}

	_match_data = pcre2_match_data_create_from_pattern(_regex_compiled, NULL);

	_mcontext = pcre2_match_context_create(NULL);
	if (_mcontext == NULL)
		throw Php::Exception("match context returned null, could not obtain memory");

	//  Apply JIT option
	if (compileFlags & Flags::Compile::JIT) {
		int32_t jitError = pcre2_jit_compile(_regex_compiled, PCRE2_JIT_COMPLETE);
		if (jitError) {
			PCRE2_UCHAR eMessage[256];
			pcre2_get_error_message(jitError, eMessage, sizeof(eMessage));
			throw Php::Exception((const char *) eMessage);
		}

		_jit_stack = pcre2_jit_stack_create(
			(PCRE2_SIZE) Php::ini_get(EXT_NAME ".jit_stack_min").numericValue() * 1024,
			(PCRE2_SIZE) Php::ini_get(EXT_NAME ".jit_stack_max").numericValue() * 1024 * 1024,
			NULL);

		if (_jit_stack == NULL)
			throw Php::Exception("an error occurred when creating JIT stack");

		pcre2_jit_stack_assign(_mcontext, NULL, _jit_stack);
	}
}

void Pcre2Base::setRegex(Php::Parameters &p)
{
	if (p[0].stringValue() == "")
		throw Php::Exception("regular expression string cannot be empty");

	_regex_string = p[0].stringValue();

	if (p.size() > 1 && !p[1].isNull()) {
		compileFlags = p[1].numericValue();
	}
}

Php::Value Pcre2Base::getRegex() const
{
	return _regex_string;
}

void Pcre2Base::__destruct()
{
	if (_mcontext != NULL) {
		pcre2_match_context_free(_mcontext);
		_mcontext = NULL;
	}

	if (_jit_stack != NULL) {
		pcre2_jit_stack_free(_jit_stack);
		_jit_stack = NULL;
	}

	if (_match_data != NULL) {
		pcre2_match_data_free(_match_data);
		_match_data = NULL;
	}

	if (_regex_compiled != NULL) {
		pcre2_code_free(_regex_compiled);
		_regex_compiled = NULL;
	}

	_regex_string.clear();
	_regex_string.shrink_to_fit();
}

Pcre2Base::~Pcre2Base()
{
	pcre2_match_context_free(_mcontext);
	pcre2_jit_stack_free(_jit_stack);
	pcre2_match_data_free(_match_data);
	pcre2_code_free(_regex_compiled);
}

Php::Value whatAmI()
{
	return "extension";
}
