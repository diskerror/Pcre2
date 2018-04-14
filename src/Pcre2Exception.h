
#ifndef DISKERROR_PCRE2_EXCEPTION_H
#define DISKERROR_PCRE2_EXCEPTION_H

class Pcre2Exception : public Php::Exception
{
public:
	Pcre2Exception() : Php::Exception(""){};
	Pcre2Exception(std::string message) : Php::Exception(message) {}
	Pcre2Exception(int32_t);
};

#endif	//	DISKERROR_PCRE2_EXCEPTION_H
