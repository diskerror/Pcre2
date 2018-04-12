#include "Base.h"

Flags::Base::Base()
{
	_flags = 0;
	_hasChanged = true;
}

void Flags::Base::__construct(Php::Parameters &p)
{
	if (p.size() > 0)
		_flags = p[0].numericValue();
}

inline void Flags::Base::_setChanged(int64_t flags)
{
	if (flags != _flags) {
		_flags = flags;
		_hasChanged = true;
	}
}

Php::Value Flags::Base::add(Php::Parameters &p)
{
	_setChanged(_flags | p[0].numericValue());
	return this;
}

Php::Value Flags::Base::remove(Php::Parameters &p)
{
	_setChanged(_flags & ~p[0].numericValue());
	return this;
}

Php::Value Flags::Base::clear()
{
	_setChanged(0);
	return this;
}

Php::Value Flags::Base::set(Php::Parameters &p)
{
	_setChanged(p[0].numericValue());
	return this;
}

inline void Flags::Base::set(int64_t flags)
{
	_setChanged(flags);
}

Php::Value Flags::Base::hasFlag(Php::Parameters &p) const
{
	return (bool) (_flags & p[0].numericValue());
}

inline bool Flags::Base::hasFlag(int64_t whichFlags) const
{
	return (bool) (_flags & whichFlags);
}

Php::Value Flags::Base::get(Php::Parameters &p) const
{
	return _flags & p[0].numericValue();
}

int64_t Flags::Base::get(int64_t whichFlags) const
{
	return _flags & whichFlags;
}

Php::Value Flags::Base::getChanged() const
{
	return _hasChanged;
}

Php::Value Flags::Base::clearChanged()
{
	_hasChanged = false;
	return this;
}
