#pragma once

#include "Iterator.h"

template<class Combination>
class DereferencedIterator : public Iterator {
	public:
		virtual Combination& operator*() const = 0;
};
