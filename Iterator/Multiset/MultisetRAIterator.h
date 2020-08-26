#pragma once

#include "MultisetIterator.h"
#include "../Movement/RandomAccessIterator.h"
#include "../../Position.h"
#include "../../Request/MultisetRequest.h"

template<class Container, Position NrContainers, class Combination>
class MultisetRAIterator :
		public MultisetIterator<Container, NrContainers, Combination>,
		public RandomAccessIterator {
	public:
		explicit MultisetRAIterator(const MultisetRequest<Container, NrContainers>& request) :
				MultisetIterator<Container, NrContainers, Combination>(request) {}
	protected:
		void go(Position index) override {
			for (Position c = 0; c < this->request.containers.size(); c++) {
				const Position nrElements(this->request.containers[c].size());
				this->positions[c] = index % nrElements;
				index /= nrElements;
			}
		}
};


