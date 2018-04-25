
#ifndef DISKERROR_PCRE2_BASE_H
#define DISKERROR_PCRE2_BASE_H

#include "flags/Base.h"
#include "flags/Compile.h"
#include "flags/Match.h"

class Pcre2Base : public Php::Base
{
protected:
	std::string _regex_string;
	pcre2_code *_regex_compiled;
	pcre2_match_data *_match_data;
	pcre2_match_context *_mcontext;
	pcre2_jit_stack *_jit_stack;

public:
	Pcre2Base();

	int64_t compileFlags;
	Php::Value getCompileFlags() const
	{ return compileFlags; }
	void setCompileFlags(const Php::Value &v)
	{ compileFlags = v.numericValue(); }    // test for error?

	int64_t matchFlags;
	Php::Value getMatchFlags() const
	{ return matchFlags; }
	void setMatchFlags(const Php::Value &v)
	{ matchFlags = v.numericValue(); }    // test for error?

	void __construct(Php::Parameters &);

	void compile(Php::Parameters &);

	void setRegex(Php::Parameters &);
	Php::Value getRegex() const;

	void __destruct();
	~Pcre2Base();

};

Php::Value whatAmI();

#endif    //	DISKERROR_PCRE2_BASE_H
