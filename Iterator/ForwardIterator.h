#pragma once

#include "Iterator.h"

class ForwardIterator : public Iterator {
	public:
		ForwardIterator() : Iterator() {}

		virtual void operator++() = 0;
};
