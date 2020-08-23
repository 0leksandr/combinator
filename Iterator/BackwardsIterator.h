#pragma once

#include "Iterator.h"

class BackwardsIterator : public Iterator {
	public:
		BackwardsIterator() : Iterator() {}
	protected:
		virtual void operator--() = 0;
};
