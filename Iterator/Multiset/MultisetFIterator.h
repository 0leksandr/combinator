#pragma once

#include "MultisetIterator.h"
#include "../Movement/ForwardIterator.h"
#include "../../Position.h"
#include "../../Request/MultisetRequest.h"

namespace CombinatorNamespace {
	template<class Container, class Combination>
	class MultisetFIterator :
			public MultisetIterator<Combination, Container>,
			public ForwardIterator {
		public:
			explicit MultisetFIterator(const MultisetRequest<Container>& request) :
					MultisetIterator<Combination, Container>(request) {}
			void increment() override {
				increment(0);
			}
		private:
			void increment(const Position position) {
				if (position == this->request.containers.size()) return;
				if (++(this->positions[position]) == this->request.containers[position].size()) {
					this->positions[position] = 0;
					increment(position + 1);
				}
			}
	};
}
