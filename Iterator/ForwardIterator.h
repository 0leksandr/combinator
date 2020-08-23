#pragma once

#include "Iterator.h"

class ForwardIterator : public Iterator {
	public:
		virtual void operator++() = 0;
};
