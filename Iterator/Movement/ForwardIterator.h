#pragma once

#include "PositionedIterator.h"
#include "../../Position.h"

namespace CombinatorNamespace {
	class ForwardIterator : public PositionedIterator {
		public:
			void operator++() {
				increment();
				this->setIndex(this->getIndex() + 1);
			}
		protected:
			virtual void increment() = 0;
	};
}
