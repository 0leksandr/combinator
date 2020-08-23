#pragma once

#include "CandidateOrderIterator.h"
#include "../../../Position.h"
#include "../../../Request/FixedRequest.h"

template<class Container, class Combination>
class Walker : public CandidateOrderIterator<Container, Combination> {
	public:
		explicit Walker(const FixedRequest<Container>* const request) :
				CandidateOrderIterator<Container, Combination>(request) {}
		Walker(const Walker& other) :
				CandidateOrderIterator<Container, Combination>(other.request) {
			this->index = other.index;
			for (Position c = 0; c < this->request->length; c++) {
				this->positions[c] = other.positions[c];
			}
		}
		[[nodiscard]] Position estimate(const Position index) const override {
			if (index > this->index) return index - this->index;
			if (index < this->index) return this->index - index;
			return 0;
		}
		void go(const Position index) override {
			#pragma clang diagnostic push
			#pragma ide diagnostic ignored "LoopDoesntUseConditionVariableInspection"
				while (this->index < index) this->operator++();
				while (this->index > index) this->operator--();
			#pragma clang diagnostic pop
		}
		[[nodiscard]] Position getIndex() const { // TODO: protected?
			return this->index;
		}
};
