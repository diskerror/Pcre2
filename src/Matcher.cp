
#include "Matcher.h"
#include "MatchData.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
Php::Value Matcher::hasMatch(Php::Parameters &p) const
{
	int32_t offset = 0;
	if (p.size() > 1) {
		offset = p[1];
		if (offset < 0) {
			offset = 0;
		}
	}

	MatchData match_data(_regex_compiled);

	//	do match
	int32_t matchCount = pcre2_match(
		_regex_compiled,
		(const PCRE2_UCHAR *) p[0].stringValue().c_str(),
		PCRE2_ZERO_TERMINATED,
		offset,
		(uint32_t)(matchFlags & 0x00000000FFFFFFFF),
		match_data(),
		_mcontext
	);

	if (matchCount < PCRE2_ERROR_NOMATCH) {
		PCRE2_UCHAR eMessage[256];
		pcre2_get_error_message(matchCount, eMessage, sizeof(eMessage));
		throw Php::Exception((const char *) eMessage);
	}

	if (matchCount == PCRE2_ERROR_NOMATCH) {
		return false;
	}

	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
Php::Value Matcher::match(Php::Parameters &p) const
{
	const char *subject = (const char *) p[0].stringValue().c_str();

	int32_t offset = 0;
	if (p.size() > 1) {
		offset = p[1];
		if (offset < 0) {
			offset = 0;
		}
	}

	MatchData match_data(_regex_compiled);

	//	do match
	int32_t matchCount = pcre2_match(
		_regex_compiled,
		(const PCRE2_UCHAR *) subject,
		PCRE2_ZERO_TERMINATED,
		offset,
		(uint32_t)(matchFlags & 0x00000000FFFFFFFF),
		match_data(),
		_mcontext
	);

	if (matchCount < PCRE2_ERROR_NOMATCH) {
		PCRE2_UCHAR eMessage[256];
		pcre2_get_error_message(matchCount, eMessage, sizeof(eMessage));
		throw Php::Exception((const char *) eMessage);
	}

	std::vector<std::string> output;

	if (matchCount == PCRE2_ERROR_NOMATCH) {
		return output;    //	empty array
	}

	PCRE2_SIZE *ovector = pcre2_get_ovector_pointer(match_data());
	for (int32_t i = 0; i < matchCount; i++) {
		output.emplace_back((const char *) (subject + ovector[2 * i]), (size_t)(ovector[2 * i + 1] - ovector[2 * i]));
	}

	return output;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//	INCOMPLETE!!!!!
Php::Value Matcher::matchAll(Php::Parameters &p) const
{
	const char *subject = (const char *) p[0].stringValue().c_str();

	int32_t offset = 0;
	if (p.size() > 1) {
		offset = p[1];
		if (offset < 0) {
			offset = 0;
		}
	}

	MatchData match_data(_regex_compiled);

	std::vector<int32_t> workspace(100, 0);

	//	do match
	int32_t matchCount = pcre2_dfa_match(
		_regex_compiled,
		(const PCRE2_UCHAR *) subject,
		PCRE2_ZERO_TERMINATED,
		offset,
		(uint32_t)(matchFlags & 0x00000000FFFFFFFF),
		match_data(),
		_mcontext,
		workspace.data(),
		workspace.size()
	);

	if (matchCount < PCRE2_ERROR_NOMATCH) {
		PCRE2_UCHAR eMessage[256];
		pcre2_get_error_message(matchCount, eMessage, sizeof(eMessage));
		throw Php::Exception((const char *) eMessage);
	}

	std::vector<std::string> output;
	if (matchCount == PCRE2_ERROR_NOMATCH) {
		return output;    //	empty array
	}

	PCRE2_SIZE *ovector = pcre2_get_ovector_pointer(match_data());
	PCRE2_SIZE i;
	for (i = 0; i < (PCRE2_SIZE) matchCount; i++) {
		output.emplace_back((const char *) (subject + ovector[2 * i]), (size_t)(ovector[2 * i + 1] - ovector[2 * i]));
	}

	return output;
}
