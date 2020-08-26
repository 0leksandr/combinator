#pragma once

#include "MultiPermutationIterator.h"
#include "../../Movement/RandomAccessIterator.h"
#include "../../../Position.h"
#include "../../../Request/FixedSizeRequest.h"

template<class Container, class Combination>
class MultiPermutationRAIterator :
		public MultiPermutationIterator<Container, Combination>,
		public RandomAccessIterator {
	public:
		explicit MultiPermutationRAIterator(const FixedSizeRequest<Container>& request) :
				MultiPermutationIterator<Container, Combination>(request) {}
	protected:
		void go(Position index) override {
			const Position nrElements(this->nrElements());
			for (Position c = 0; c < this->request.length; c++) {
				this->positions[c] = index % nrElements;
				index /= nrElements;
			}
		}
};
