#pragma once

#include "ComposeIterator.h"
#include "../ForwardIterator.h"
#include "../../Position.h"
#include "../../Request/ComposeRequest.h"

template<class Container, Position NrContainers, class Combination>
class ComposeFIterator :
		public ComposeIterator<Container, NrContainers, Combination>,
		public ForwardIterator {
	public:
		explicit ComposeFIterator(const ComposeRequest<Container, NrContainers>* request) :
				ComposeIterator<Container,NrContainers,Combination>(request) {}
		void operator++() override {
			increment(0);
			++this->index;
		}
	private:
		void increment(const Position position) {
			if (position < this->request->containers.size()) {
				if (++(this->positions[position]) == this->request->containers[position].size()) {
					this->positions[position] = 0;
					increment(position + 1);
				}
			}
		}
};
