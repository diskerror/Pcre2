
#ifndef DISKERROR_PCRE2_BASE_H
#define DISKERROR_PCRE2_BASE_H


namespace Pcre2 {

class Base : public ::Php::Base {
protected:

	pcre2_code *_regex;
	pcre2_match_data *_match_data;
	pcre2_match_context *_mcontext;
	pcre2_jit_stack *_jit_stack;

	uint32_t _compileOptions;
	uint32_t _matchOptions;

	//	We can only get to this constructor from the child classes,
	//		which forces this class into the role of an abstract class.
	Base();

public:
	virtual void __construct(Php::Parameters &);

	void compile(Php::Parameters &);

	virtual ~Base();

};

}	//	namespace Pcre2

#endif	//	DISKERROR_PCRE2_BASE_H
