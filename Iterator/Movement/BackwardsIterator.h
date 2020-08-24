#pragma once

#include "../Iterator.h"

class BackwardsIterator : public Iterator {
	protected:
		virtual void operator--() = 0;
};
