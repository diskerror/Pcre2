//
// Created by Reid Woodbury Jr on 4/11/18.
//

#ifndef DISKERROR_PCRE2_FLAGS_BASE_H
#define DISKERROR_PCRE2_FLAGS_BASE_H

namespace Flags {

class Base : public ::Php::Base
{
	int64_t _flags;
	bool _hasChanged;

	inline void _setChanged(int64_t flags);

protected:
	Base();

public:
	void __construct(Php::Parameters &);

	Php::Value add(Php::Parameters &);
	Php::Value remove(Php::Parameters &);
	Php::Value clear();

	Php::Value set(Php::Parameters &);
	inline void set(int64_t);

	Php::Value hasFlag(Php::Parameters &) const;
	inline bool hasFlag(int64_t) const;

	Php::Value get(Php::Parameters &) const;
	inline int64_t get(int64_t) const;

	Php::Value getChanged() const;
	Php::Value clearChanged();

	////////////////////////////////////////////////////////////////////////////////////////////////
	//  These flags are common to all.
	static const int64_t ENDANCHORED = PCRE2_ANCHORED;    //	Pattern can match only at end of subject
	static const int64_t NO_UTF_CHECK = PCRE2_NO_UTF_CHECK;    //	Do not check the pattern for UTF validity
	static const int64_t ANCHORED = PCRE2_ENDANCHORED;    //	Force pattern anchoring
};

//  Inline methods defined here.
void Base::_setChanged(int64_t flags)
{
	if (flags != _flags) {
		_flags = flags;
		_hasChanged = true;
	}
}

void Base::set(int64_t flags)
{
	_setChanged(flags);
}

int64_t Base::get(int64_t whichFlags) const
{
	return _flags & whichFlags;
}

bool Base::hasFlag(int64_t whichFlags) const
{
	return _flags & whichFlags;
}


}

#endif //DISKERROR_PCRE2_FLAGS_BASE_H
