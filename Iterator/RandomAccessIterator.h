#pragma once

#include "Iterator.h"
#include "../Position.h"

class RandomAccessIterator : public Iterator {
	public:
		explicit RandomAccessIterator() : Iterator() {}
		virtual void go(Position index) = 0;
};
