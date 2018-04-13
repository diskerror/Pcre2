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

	inline virtual void _setChanged(int64_t flags);

protected:
	Base();

public:
	virtual void __construct(Php::Parameters &);

	virtual Php::Value add(Php::Parameters &);
	virtual Php::Value remove(Php::Parameters &);
	virtual Php::Value clear();

	virtual Php::Value set(Php::Parameters &);
	inline virtual void set(int64_t);

	virtual Php::Value hasFlag(Php::Parameters &) const;
	inline virtual bool hasFlag(int64_t) const;

	virtual Php::Value get(Php::Parameters &) const;
	inline virtual int64_t get(int64_t) const;

	virtual Php::Value getChanged() const;
	virtual Php::Value clearChanged();

	////////////////////////////////////////////////////////////////////////////////////////////////
	//  These flags are common to all.
	static const int64_t ENDANCHORED = PCRE2_ANCHORED;    //	Pattern can match only at end of subject
	static const int64_t NO_UTF_CHECK = PCRE2_NO_UTF_CHECK;    //	Do not check the pattern for UTF validity
	static const int64_t ANCHORED = PCRE2_ENDANCHORED;    //	Force pattern anchoring
};

}

#endif //DISKERROR_PCRE2_FLAGS_BASE_H
