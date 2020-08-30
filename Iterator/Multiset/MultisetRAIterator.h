#pragma once

#include "MultisetIterator.h"
#include "../Movement/RandomAccessIterator.h"
#include "../../Position.h"
#include "../../Request/MultisetRequest.h"

template<class Container, class Combination>
class MultisetRAIterator :
		public MultisetIterator<Combination, Container>,
		public RandomAccessIterator {
	public:
		explicit MultisetRAIterator(const MultisetRequest<Container>& request) :
				MultisetIterator<Combination, Container>(request) {}
	protected:
		void go(Position index) override {
			for (Position c = 0; c < this->request.containers.size(); c++) {
				const Position nrElements(this->request.containers[c].size());
				this->positions[c] = index % nrElements;
				index /= nrElements;
			}
		}
};


