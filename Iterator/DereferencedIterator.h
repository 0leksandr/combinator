#pragma once

#include "Iterator.h"

template<class Combination>
class DereferencedIterator : public Iterator {
	public:
		DereferencedIterator() : Iterator() {}
		virtual Combination& operator*() const = 0;
};
