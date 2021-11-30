#pragma once

#include "MultisetIterator.h"
#include "../Movement/ForwardIterator.h"
#include "../../Position.h"
#include "../../Request/MultisetRequest.h"

namespace CombinatorNamespace {
	template<class Container, class Combination>
	class MultisetFIterator :
			public MultisetIterator<Container, Combination>,
			public ForwardIterator {
		public:
			explicit MultisetFIterator(const MultisetRequest<Container>& request) :
					MultisetIterator<Container, Combination>(request) {}
			void increment() override {
				increment(0);
			}
		private:
			void increment(const Position position) {
				if (position == this->request.combinationSize()) return;
				if (++(this->positions[position]) == this->request.containerSize(position)) {
					this->positions[position] = 0;
					increment(position + 1);
				}
			}
	};
}
