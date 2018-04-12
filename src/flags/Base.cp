#include "Base.h"

Flags::Base::Base()
{
	_flags = 0;
	_hasChanged = true;
}

void Flags::Base::__construct(Php::Parameters &)
{
	if(p.size() > 0)
		_flags = p[0];
}

inline void Flags::Base::_setChanged(int64_t flags) const
{
	if (flags != _flags) {
		_flags = flags;
		_hasChanged = true;
	}
}

Php::Value Flags::Base::add(Php::Parameters &) const
{
	_setChanged(_flags | (int64_t) p[0]);
	return this;
}

Php::Value Flags::Base::remove(Php::Parameters &) const
{
	_setChanged(_flags & ~((int64_t)p[0]));
	return $this;
}

Php::Value Flags::Base::clear() const
{
	_setChanged(0);
	return $this;
}

Php::Value Flags::Base::set(Php::Parameters &) const
{
	_setChanged((int64_t) p[0]);
	return $this;
}

void Flags::Base::set(int64_t flags) const
{
	_setChanged(flags);
}

Php::Value Flags::Base::hasFlag(Php::Parameters &) const
{
	return (bool) (_flags & (int64_t) p[0]);
}

bool Flags::Base::hasFlag(int64_t whichFlags) const
{
	return (bool) (_flags & whichFlags);
}

Php::Value Flags::Base::get(Php::Parameters &) const
{
	return _flags & (int64_t) p[0];
}

int64_t Flags::Base::get(int64_t whichFlags) const
{
	return _flags & whichFlags;
}

Php::Value Flags::Base::getChanged() const
{
	return _hasChanged;
}

Php::Value Flags::Base::clearChanged() const
{
	_hasChanged = false;
	return $this;
}
