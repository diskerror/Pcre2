
#include "Base.h"
#include "Exception.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
Pcre2::Base::Base() {
	_regex = NULL;
	_mcontext = NULL;
	_jit_stack = NULL;
	_match_data = NULL;

	_compileOptions = PCRE2_UTF;
	_matchOptions = 0;
}

void Pcre2::Base::__construct(Php::Parameters &p) {
	compile(p);
}

void Pcre2::Base::compile(Php::Parameters &p) {
	if (p.size() > 1) {
		_compileOptions = (uint32_t) p[1];
	}

	if (p.size() > 2) {
		_matchOptions = (uint32_t) p[2];
	}

	int errorcode;
	PCRE2_SIZE erroroffset;

	_regex = pcre2_compile(
		(PCRE2_SPTR) p[0],
		PCRE2_ZERO_TERMINATED,
		_compileOptions,
		&errorcode,
		&erroroffset,
		NULL    //	match context
	);

	if (_regex == NULL)
		throw Exception(errorcode);

	int jitError = pcre2_jit_compile(_regex, PCRE2_JIT_COMPLETE);
	if (jitError)
		throw Exception(jitError);

	_mcontext = pcre2_match_context_create(NULL);
	if (_mcontext == NULL)
		throw Exception(-1);

	_jit_stack = pcre2_jit_stack_create(8 * 1024, 256 * 1024, NULL);
	pcre2_jit_stack_assign(_mcontext, NULL, _jit_stack);

	_match_data = pcre2_match_data_create_from_pattern(_regex, NULL);
	if (_match_data == NULL)
		throw Exception(-1);
}

Pcre2::Base::~Base() {
	if (_match_data != NULL)
		pcre2_match_data_free(_match_data);

	if (_jit_stack != NULL)
		pcre2_jit_stack_free(_jit_stack);

	if (_mcontext != NULL)
		pcre2_match_context_free(_mcontext);

	if (_regex != NULL)
		pcre2_code_free(_regex);
}
