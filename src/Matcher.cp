
#include "Matcher.h"

int32_t Matcher::_basicMatch(Php::Parameters &p) const
{
	const char *subject = (const char *) p[0].stringValue().c_str();
	int32_t offset = 0;
	if (p.size() > 1) {
		offset = p[1];
		if (offset < 0) {
			offset = 0;
		}
	}

	//	do match
	int32_t matchCount = pcre2_match(
		_regex_compiled,
		(const PCRE2_UCHAR *) subject,
		PCRE2_ZERO_TERMINATED,
		offset,
		0,    //	options
		_match_data,
		NULL
	);

	if (matchCount < PCRE2_ERROR_NOMATCH) {
		throw Pcre2Exception(matchCount);
	}

	return matchCount;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
Php::Value Matcher::hasMatch(Php::Parameters &p) const
{
	int32_t matchCount = _basicMatch(p);

	if ( matchCount == PCRE2_ERROR_NOMATCH ) {
		return false;
	}

	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
Php::Value Matcher::match(Php::Parameters &p) const
{
	int32_t matchCount = _basicMatch(p);	//	updates _match_data

	std::vector<std::string> output;
	if (matchCount == PCRE2_ERROR_NOMATCH) {
		return output;	//	empty array
	}

	const char *subject = (const char *) p[0].stringValue().c_str();
	PCRE2_SIZE* ovector = pcre2_get_ovector_pointer(_match_data);
	PCRE2_SIZE i;
	for (i = 0; i < (PCRE2_SIZE) matchCount; i++) {
		output.emplace_back( (const char*) (subject + ovector[2 * i]), (size_t)(ovector[2 * i + 1] - ovector[2 * i]) );
	}

	return output;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//	INCOMPLETE!!!!!
Php::Value Matcher::matchAll(Php::Parameters &p) const
{
	int32_t matchCount = _basicMatch(p);	//	updates _match_data

	std::vector<std::string> output;
	if (matchCount == PCRE2_ERROR_NOMATCH) {
		return output;	//	empty array
	}

	const char *subject = (const char *) p[0].stringValue().c_str();
	PCRE2_SIZE* ovector = pcre2_get_ovector_pointer(_match_data);
	PCRE2_SIZE i;
	for (i = 0; i < (PCRE2_SIZE) matchCount; i++) {
		output.emplace_back( (const char*) (subject + ovector[2 * i]), (size_t)(ovector[2 * i + 1] - ovector[2 * i]) );
	}

	return output;
}
