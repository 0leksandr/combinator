#pragma once

#include "PositionedIterator.h"
#include "../../Position.h"

namespace CombinatorNamespace {
	class RandomAccessIterator : public PositionedIterator {
		public:
			void operator[](const Position index) {
				go(index);
				this->setIndex(index);
			}
		protected:
			virtual void go(Position index) = 0;
	};
}
