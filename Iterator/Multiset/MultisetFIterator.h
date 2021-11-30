#pragma once

#include "MultisetIterator.h"
#include "../Movement/ForwardIterator.h"
#include "../../Position.h"

namespace CombinatorNamespace {
	template<class Combination, class Request, typename Element>
	class MultisetFIterator :
			public MultisetIterator<Combination, Request, Element>,
			public ForwardIterator {
		public:
			explicit MultisetFIterator(const Request& request) :
					MultisetIterator<Combination, Request, Element>(request) {}
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
