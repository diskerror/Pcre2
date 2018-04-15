#include "Base.h"

Flags::Base::Base()
{
	_flags = NO_UTF_CHECK;
	_hasChanged = true;
}

void Flags::Base::__construct(Php::Parameters &p)
{
	if (p.size() > 0)
		_flags = p[0].numericValue();
}

void Flags::Base::add(Php::Parameters &p)
{
	_setChanged(_flags | p[0].numericValue());
}

void Flags::Base::remove(Php::Parameters &p)
{
	_setChanged(_flags & ~p[0].numericValue());
}

void Flags::Base::clear()
{
	_setChanged(0);
}

void Flags::Base::set(Php::Parameters &p)
{
	_setChanged(p[0].numericValue());
}

Php::Value Flags::Base::hasFlag(Php::Parameters &p) const
{
	return (bool) (_flags & p[0].numericValue());
}

Php::Value Flags::Base::get(Php::Parameters &p) const
{
	return _flags & p[0].numericValue();
}

Php::Value Flags::Base::getChanged() const
{
	return _hasChanged;
}

void Flags::Base::clearChanged()
{
	_hasChanged = false;
}
