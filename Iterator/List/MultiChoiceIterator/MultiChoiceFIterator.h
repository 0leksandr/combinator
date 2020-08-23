#pragma once

#include "MultiChoiceListIterator.h"
#include "../../ForwardIterator.h"
#include "../../../Position.h"
#include "../../../Request/FixedRequest.h"

template<class Container, class Combination>
class MultiChoiceFIterator :
		public MultiChoiceListIterator<Container, Combination>,
		public ForwardIterator {
	public:
		explicit MultiChoiceFIterator(const FixedRequest<Container>* const request) :
				MultiChoiceListIterator<Container, Combination>(request),
				ForwardIterator() {
		}
		void operator++() override {
			increment(0);
			++this->index;
		}
	private:
		void increment(const Position position) {
			if (position < this->request->length) {
				if (++(this->positions[position]) == this->nrElements()) {
					this->positions[position] = 0;
					increment(position + 1);
				}
			}
		}
};
