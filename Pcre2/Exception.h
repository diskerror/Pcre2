
#ifndef DISKERROR_PCRE2_EXCEPTION_H
#define DISKERROR_PCRE2_EXCEPTION_H


namespace Pcre2 {

class Exception : public std::exception
{
	int32_t _err;

public:
	Exception(int32_t err);

	const char *what() const noexcept override;
};

}	//	namespace Pcre2

#endif	//	DISKERROR_PCRE2_EXCEPTION_H
