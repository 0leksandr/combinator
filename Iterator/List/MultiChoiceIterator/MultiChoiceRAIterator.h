#pragma once

#include "MultiChoiceListIterator.h"
#include "../../Movement/RandomAccessIterator.h"
#include "../../../Position.h"
#include "../../../Request/FixedRequest.h"

template<class Container, class Combination>
class MultiChoiceRAIterator :
		public MultiChoiceListIterator<Container, Combination>,
		public RandomAccessIterator {
	public:
		explicit MultiChoiceRAIterator(const FixedRequest<Container>& request) :
				MultiChoiceListIterator<Container, Combination>(request) {}
	protected:
		void go(Position index) override {
			const Position nrElements(this->nrElements());
			for (Position c = 0; c < this->request.length; c++) {
				this->positions[c] = index % nrElements;
				index /= nrElements;
			}
		}
};
