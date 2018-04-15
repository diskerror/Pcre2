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

	inline void _setChanged(int64_t);

protected:
	Base();

public:
	void __construct(Php::Parameters &);

	void add(Php::Parameters &);
	inline void add(int64_t flags) { _setChanged(_flags | flags); }

	void remove(Php::Parameters &);
	void clear();

	void set(Php::Parameters &);
	inline void set(int64_t flags) { _setChanged(flags); }

	Php::Value hasFlag(Php::Parameters &) const;
	inline bool hasFlag(int64_t whichFlags) const { return _flags & whichFlags; };

	Php::Value get(Php::Parameters &) const;
	inline int64_t get(int64_t whichFlags) const { return _flags & whichFlags; };

	Php::Value getChanged() const;
	void clearChanged();

	////////////////////////////////////////////////////////////////////////////////////////////////
	//  These flags are common to all.
	static const int64_t ENDANCHORED = PCRE2_ENDANCHORED;    //	Pattern can match only at end of subject
	static const int64_t NO_UTF_CHECK = PCRE2_NO_UTF_CHECK;    //	Do not check the pattern for UTF validity
	static const int64_t ANCHORED = PCRE2_ANCHORED;    //	Force pattern anchoring
};

void Base::_setChanged(int64_t flags)
{
	if (flags != _flags) {
		_flags = flags;
		_hasChanged = true;
	}
}

}

#endif //DISKERROR_PCRE2_FLAGS_BASE_H
