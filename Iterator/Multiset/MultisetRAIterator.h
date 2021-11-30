#pragma once

#include "MultisetIterator.h"
#include "../Movement/RandomAccessIterator.h"
#include "../../Position.h"
#include "../../Request/MultisetRequest.h"

namespace CombinatorNamespace {
	template<class Container, class Combination>
	class MultisetRAIterator :
			public MultisetIterator<Container, Combination>,
			public RandomAccessIterator {
		public:
			explicit MultisetRAIterator(const MultisetRequest<Container>& request) :
					MultisetIterator<Container, Combination>(request) {}
		protected:
			void go(Position index) override {
				for (Position c = 0; c < this->request.nrContainers(); c++) {
					const Position nrElements(this->request.containerSize(c));
					this->positions[c] = index % nrElements;
					index /= nrElements;
				}
			}
	};
}


