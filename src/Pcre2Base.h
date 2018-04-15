
#ifndef DISKERROR_PCRE2_BASE_H
#define DISKERROR_PCRE2_BASE_H

#include "flags/Base.h"
#include "flags/Compile.h"
#include "flags/Match.h"

class Pcre2Base : public Php::Base
{
protected:
	std::string         _regex_string;
	pcre2_code          *_regex_compiled;
	pcre2_match_data    *_match_data;
	pcre2_match_context *_mcontext;
	pcre2_jit_stack     *_jit_stack;

public:
	Pcre2Base();

//	Flags::Compile *compileFlags;
//	Flags::Match *matchFlags;
	int64_t compileFlags;
	int64_t matchFlags;

	void __construct(Php::Parameters &);

	void compile(Php::Parameters &);

	void setRegex(Php::Parameters &);
	Php::Value getRegex() const;

	void __destruct();
	~Pcre2Base();


	static void handleNumericError(int32_t err)
	{
		const uint32_t messgLen = 1024;
		PCRE2_UCHAR8 message[messgLen];

		pcre2_get_error_message(err, message, messgLen);

		throw Php::Exception((const char *) message);
	}
};

#endif    //	DISKERROR_PCRE2_BASE_H
