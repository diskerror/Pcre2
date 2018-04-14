
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

	//	We can only get to this constructor from the child classes,
	//		which forces this class into the role of abstract.
	Pcre2Base();

public:
	Flags::Compile *compileFlags;
	Flags::Match *matchFlags;

	virtual void __construct(Php::Parameters &);

	virtual Php::Value compile(Php::Parameters &);

	virtual Php::Value setRegex(Php::Parameters &);
	virtual Php::Value getRegex() const;

	virtual void __destruct();
	virtual ~Pcre2Base();


	static void handleNumericError(int32_t err)
	{
		const uint32_t messgLen = 1024;
		PCRE2_UCHAR8 message[messgLen];

		pcre2_get_error_message(err, message, messgLen);

		throw Php::Exception((const char *) message);
	}
};

#endif    //	DISKERROR_PCRE2_BASE_H
