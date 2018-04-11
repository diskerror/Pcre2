
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

	uint32_t _compileFlags;
	uint32_t _matchFlags;

	//	We can only get to this constructor from the child classes,
	//		which forces this class into the role of an abstract class.
	Pcre2Base();

public:
	virtual void __construct(Php::Parameters &);

	void compile(Php::Parameters &);

	virtual ~Pcre2Base();

};

#endif	//	DISKERROR_PCRE2_BASE_H
