#pragma once

#include "MultisetIterator.h"
#include "../Movement/RandomAccessIterator.h"
#include "../../Position.h"

namespace CombinatorNamespace {
	template<class Combination, class Request, typename Element>
	class MultisetRAIterator :
			public MultisetIterator<Combination, Request, Element>,
			public RandomAccessIterator {
		public:
			explicit MultisetRAIterator(const Request& request) :
					MultisetIterator<Combination, Request, Element>(request) {}
		protected:
			void go(Position index) override {
				for (Position c = 0; c < this->request.combinationSize(); c++) {
					const Position nrElements(this->request.containerSize(c));
					this->positions[c] = index % nrElements;
					index /= nrElements;
				}
			}
	};
}


