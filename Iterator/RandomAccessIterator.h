#pragma once

#include "Iterator.h"
#include "../Position.h"

class RandomAccessIterator : public Iterator {
	public:
		virtual void go(Position index) = 0;
};
