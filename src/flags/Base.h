//
// Created by Reid Woodbury Jr on 4/11/18.
//

#ifndef DISKERROR_PCRE2_FLAGS_BASE_H
#define DISKERROR_PCRE2_FLAGS_BASE_H

#include "../Flags.h"

namespace Flags {

class Base : public ::Php::Base
{
	int64_t _flags;
	bool _hasChanged;

	inline virtual void _setChanged(int64_t flags) const;

public:
	Base();

	virtual void __construct(Php::Parameters &);

	virtual Php::Value add(Php::Parameters &) const;
	virtual Php::Value remove(Php::Parameters &) const;
	virtual Php::Value clear() const;

	virtual Php::Value set(Php::Parameters &) const;
	inline virtual void set(int64_t) const;

	virtual Php::Value hasFlag(Php::Parameters &) const;
	inline virtual bool hasFlag(int64_t) const;

	virtual Php::Value get(Php::Parameters &) const;
	inline virtual int64_t get(int64_t) const;

	virtual Php::Value getChanged() const;
	virtual Php::Value clearChanged() const;

	////////////////////////////////////////////////////////////////////////////////////////////////
	//  These flags are common to all.
	const int64_t ENDANCHORED = PCRE2_ANCHORED;    //	Pattern can match only at end of subject
	const int64_t NO_UTF_CHECK = PCRE2_NO_UTF_CHECK;    //	Do not check the pattern for UTF validity
	const int64_t ANCHORED = PCRE2_ENDANCHORED;    //	Force pattern anchoring
};

}

#endif //DISKERROR_PCRE2_FLAGS_BASE_H
