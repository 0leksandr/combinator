#pragma once

#include "ComposeIterator.h"
#include "../Movement/RandomAccessIterator.h"
#include "../../Position.h"
#include "../../Request/ComposeRequest.h"

template<class Container, Position NrContainers, class Combination>
class ComposeRAIterator :
		public ComposeIterator<Container, NrContainers, Combination>,
		public RandomAccessIterator {
	public:
		explicit ComposeRAIterator(const ComposeRequest<Container, NrContainers>* const request) :
				ComposeIterator<Container, NrContainers, Combination>(request) {}
	protected:
		void go(Position index) override {
			for (Position c = 0; c < this->request->containers.size(); c++) {
				const Position nrElements(this->request->containers[c].size());
				this->positions[c] = index % nrElements;
				index /= nrElements;
			}
		}
};


