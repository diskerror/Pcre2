
#ifndef DISKERROR_PCRE2_BASE_H
#define DISKERROR_PCRE2_BASE_H

class Pcre2Base : public Php::Base
{
protected:
	std::string _regex_string;
	pcre2_code *_regex_compiled;
	pcre2_match_data *_match_data;
	pcre2_match_context *_mcontext;
	pcre2_jit_stack *_jit_stack;

public:
	explicit Pcre2Base();

	int64_t compileFlags;
	inline Php::Value getCompileFlags() const
	{ return compileFlags; }
	inline void setCompileFlags(const Php::Value &v)
	{ compileFlags = v.numericValue(); }    // test for error?

	int64_t matchFlags;
	inline Php::Value getMatchFlags() const
	{ return matchFlags; }
	inline void setMatchFlags(const Php::Value &v)
	{ matchFlags = v.numericValue(); }    // test for error?

	void __construct(Php::Parameters &);

	void compile(Php::Parameters &);

	inline void setRegex(Php::Parameters &);
	inline Php::Value getRegex() const;

	void __destruct();
	~Pcre2Base();

};

Php::Value whatAmI();

#endif    //	DISKERROR_PCRE2_BASE_H
