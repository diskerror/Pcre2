
#include "Matcher.h"
#include "flags/Match.h"
#include "flags/Compile.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
Php::Value Matcher::hasMatch(Php::Parameters &p) const
{
	PCRE2_SIZE offset = (p.size() > 1 && p[1].numericValue() > 0) ? p[1].numericValue() : 0;

	//	do match
	int32_t matchCount = pcre2_match(
		_regex_compiled,
		(const PCRE2_UCHAR *) p[0].stringValue().c_str(),
		PCRE2_ZERO_TERMINATED,
		offset,
		(uint32_t)(matchFlags & 0x00000000FFFFFFFF),
		_match_data,
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
	PCRE2_SIZE offset = (p.size() > 1 && p[1].numericValue() > 0) ? p[1].numericValue() : 0;

	//	do match
	int32_t matchCount = pcre2_match(
		_regex_compiled,
		(const PCRE2_UCHAR *) subject,
		(PCRE2_SIZE) p[0].stringValue().size(),
		offset,
		(uint32_t)(matchFlags & 0x00000000FFFFFFFF),
		_match_data,
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

	PCRE2_SIZE *ovector = pcre2_get_ovector_pointer(_match_data);
	for (int32_t i = 0; i < matchCount; i++) {
		output.emplace_back((const char *) (subject + ovector[2 * i]), (size_t)(ovector[2 * i + 1] - ovector[2 * i]));
	}

	return output;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
Php::Value Matcher::matchAll(Php::Parameters &p) const
{
	std::string subject = p[0].stringValue();
	PCRE2_SIZE subjectLength = subject.size();
	PCRE2_SIZE offset = (p.size() > 1 && p[1].numericValue() > 0) ? p[1].numericValue() : 0;

	//	Do first match.
	int32_t matchCount = pcre2_match(
		_regex_compiled,
		(const PCRE2_UCHAR *) subject.c_str(),
		subjectLength,
		offset,
		(uint32_t)(matchFlags & 0x00000000FFFFFFFF),
		_match_data,
		_mcontext
	);

	if (matchCount < PCRE2_ERROR_NOMATCH) {
		PCRE2_UCHAR eMessage[256];
		pcre2_get_error_message(matchCount, eMessage, sizeof(eMessage));
		throw Php::Exception((const char *) eMessage);
	}

	std::vector<std::vector<std::string>> output;

	if (matchCount == PCRE2_ERROR_NOMATCH) {
		return output;    //	empty array
	}

	PCRE2_SIZE *ovector = pcre2_get_ovector_pointer(_match_data);

	if (ovector[0] > ovector[1])
		return output;

	std::vector<std::string> firstOutput;
	for (PCRE2_SIZE i = 0; i < (PCRE2_SIZE) matchCount; i++) {
		firstOutput.emplace_back(
			(const char *) (subject.c_str() + ovector[2 * i]),
			(size_t)(ovector[2 * i + 1] - ovector[2 * i])
		);
	}

	output.emplace_back(firstOutput);


	//  Find any additional matches.
	bool crlf_is_newline = true;
	bool utf8 = compileFlags | Flags::Compile::UTF;

	for (;;) {
		////////////////////////////////////////////////////////////////////////////////////////////
		//  from pcre2demo.c, line 342
		uint32_t options = 0;
		offset = ovector[1];   // Start at end of previous match

		if (ovector[0] == ovector[1]) {
			if (ovector[0] == subjectLength)
				break;
			options |= Flags::Match::NOTEMPTY_ATSTART | Flags::Match::ANCHORED;
		} else {
			PCRE2_SIZE startchar = pcre2_get_startchar(_match_data);
			if (offset <= startchar) {
				if (startchar >= subjectLength) break;  /* Reached end of subject.   */
				offset = startchar + 1;                 /* Advance by one character. */
				if (utf8) { /* If UTF-8, it may be more than one code unit. */
					for (; offset < subjectLength; offset++)
						if ((subject.c_str()[offset] & 0xc0) != 0x80) break;
				}
			}
		}

		int32_t matchCount = pcre2_match(
			_regex_compiled,
			(const PCRE2_UCHAR *) subject.c_str(),
			subjectLength,
			offset,
			options,
			_match_data,
			_mcontext
		);

		if (matchCount < PCRE2_ERROR_NOMATCH) {
			PCRE2_UCHAR eMessage[256];
			pcre2_get_error_message(matchCount, eMessage, sizeof(eMessage));
			throw Php::Exception((const char *) eMessage);
		}

		if (matchCount == PCRE2_ERROR_NOMATCH) {
			if (options == 0)
				break;                    /* All matches found */

			ovector[1] = offset + 1;              /* Advance one code unit */
			if (crlf_is_newline &&                /* If CRLF is a newline & */
				offset < subjectLength - 1 &&    /* we are at CRLF, */
				subject.c_str()[offset] == '\r' &&
				subject.c_str()[offset + 1] == '\n')
				ovector[1] += 1;                       /* Advance by one more. */
			else if (utf8)                              /* Otherwise, ensure we */
			{                                         /* advance a whole UTF-8 */
				while (ovector[1] < subjectLength)       /* character. */
				{
					if ((subject.c_str()[ovector[1]] & 0xc0) != 0x80) break;
					ovector[1] += 1;
				}
			}
			continue;    /* Go round the loop again */
		}

		std::vector<std::string> loopOutput;    //  erases previous
		PCRE2_SIZE i;
		for (i = 0; i < (PCRE2_SIZE) matchCount; i++) {
			loopOutput.emplace_back(
				(const char *) (subject.c_str() + ovector[2 * i]),
				(size_t)(ovector[2 * i + 1] - ovector[2 * i])
			);
		}

		output.emplace_back(loopOutput);
	}

	return output;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
Php::Value Matcher::test(Php::Parameters &p) const
{
	return (int64_t) Php::ini_get("diskerror_pcre2.default_replace_flags");
}
