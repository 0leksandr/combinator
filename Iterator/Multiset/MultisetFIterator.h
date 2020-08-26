#pragma once

#include "MultisetIterator.h"
#include "../Movement/ForwardIterator.h"
#include "../../Position.h"
#include "../../Request/MultisetRequest.h"

template<class Container, Position NrContainers, class Combination>
class MultisetFIterator :
		public MultisetIterator<Container, NrContainers, Combination>,
		public ForwardIterator {
	public:
		explicit MultisetFIterator(const MultisetRequest<Container, NrContainers>& request) :
				MultisetIterator<Container,NrContainers,Combination>(request) {}
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
