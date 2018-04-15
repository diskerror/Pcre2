
#include "Replacer.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
void Replacer::__construct(Php::Parameters &p)
{
	_replacement = p.size() > 1 ? p[1].buffer() : "";

	if (p.size() > 3 && !p[3].isNull())
		matchFlags |= p[3].numericValue();
	else
		matchFlags |= Flags::Replace::GLOBAL;

	if (p.size() > 1)
		p.erase(p.begin() + 1);
	Pcre2Base::__construct(p);
}

void Replacer::setReplacement(Php::Parameters &p)
{
	_replacement = p[0].stringValue();

	if (p.size() > 1 && !p[1].isNull()) {
		matchFlags = p[1].numericValue();
	}
}

Php::Value Replacer::getReplacement() const
{
	return _replacement;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
Php::Value Replacer::replace(Php::Parameters &p) const
{
	PCRE2_SIZE subjectLen = p[0].size();
	PCRE2_SIZE bufferSize = (subjectLen < 2048) ? 4096 : (subjectLen * 1.3);
	PCRE2_UCHAR outputBuffer[bufferSize];

	int res = pcre2_substitute(
		_regex_compiled,
		(const PCRE2_UCHAR *) (const char *) p[0].buffer(),    //	subject,
		PCRE2_ZERO_TERMINATED,
		(PCRE2_SIZE)(p.size() > 1 ? p[1].numericValue() : 0),    //	offset
		(uint32_t) (matchFlags & 0x00000000FFFFFFFF),    //	options
		_match_data,
		_mcontext,        //	match context
		(PCRE2_SPTR) _replacement.c_str(),
		PCRE2_ZERO_TERMINATED,
		outputBuffer,
		&bufferSize
	);

	if (res < PCRE2_ERROR_NOMATCH)
		handleNumericError(res);

	return Php::Value((const char *) outputBuffer, (int) bufferSize);
}

void Replacer::__destruct()
{
	Pcre2Base::__destruct();
	_replacement.clear();
	_replacement.shrink_to_fit();
}
