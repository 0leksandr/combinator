#pragma once

#include "Iterator.h"
#include "../Position.h"

class IndexedIterator : public Iterator { // TODO: PositionedIterator?
	public:
		explicit IndexedIterator(const Position index) : Iterator(), index(index) {}
		bool operator!=(const IndexedIterator& other) const {
			return this->index != other.index;
		}
	protected:
		Position index;
};
