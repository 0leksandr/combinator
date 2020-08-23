#pragma once

#include "Iterator.h"
#include "../Position.h"

class PositionedIterator : public Iterator {
	public:
		explicit PositionedIterator(const Position index) : Iterator(), index(index) {}
		bool operator!=(const PositionedIterator& other) const {
			return this->index != other.index;
		}
	protected:
		Position index;
};
