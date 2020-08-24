#pragma once

#include "PositionedIterator.h"
#include "../Position.h"

class RandomAccessIterator : public PositionedIterator {
	public:
		void goWithIndex(const Position index) {
			go(index);
			this->index = index;
		}
	protected:
		virtual void go(Position index) = 0;
};
