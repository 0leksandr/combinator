#pragma once

#include "../Position.h"

class IndexedIterator {
	public:
		explicit IndexedIterator(const Position index) : index(index) {}
		bool operator!=(const IndexedIterator& other) const {
			return this->index != other.index;
		}
	protected:
		Position index;
};
