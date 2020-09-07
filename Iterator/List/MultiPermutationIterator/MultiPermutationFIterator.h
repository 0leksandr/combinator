#pragma once

#include "MultiPermutationIterator.h"
#include "../../Movement/ForwardIterator.h"
#include "../../../Position.h"
#include "../../../Request/FixedSizeRequest.h"

namespace CombinatorNamespace {
	template<class Container, class Combination>
	class MultiPermutationFIterator :
			public MultiPermutationIterator<Container, Combination>,
			public ForwardIterator {
		public:
			explicit MultiPermutationFIterator(const FixedSizeRequest<Container>& request) :
					MultiPermutationIterator<Container, Combination>(request) {}
			void increment() override {
				increment(0);
			}
		private:
			void increment(const Position position) {
				if (position == this->request.length) return;
				if (++(this->positions[position]) == this->nrElements()) {
					this->positions[position] = 0;
					increment(position + 1);
				}
			}
	};
}
