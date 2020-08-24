#pragma once

#include "CandidateOrderIterator.h"
#include "../../../Movement/BackwardsIterator.h"
#include "../../../Movement/ForwardIterator.h"
#include "../../../Movement/MobileIteratorMacros.h"
#include "../../../../Position.h"
#include "../../../../Request/FixedRequest.h"

template<class Container, class Combination>
class Walker :
		public CandidateOrderIterator<Container, Combination>,
		public ForwardIterator,
		public BackwardsIterator {
	MOBILE_ITERATOR;
	public:
		explicit Walker(const FixedRequest<Container>* const request) :
				CandidateOrderIterator<Container, Combination>(request) {}
		Walker(const Walker& other) :
				CandidateOrderIterator<Container, Combination>(other.request) {
			this->setIndex(other.getIndex());
			for (Position c = 0; c < this->request->length; c++) {
				this->positions[c] = other.positions[c];
			}
		}
		[[nodiscard]] Position estimate(const Position index) const override {
			if (index > this->getIndex()) return index - this->getIndex();
			if (index < this->getIndex()) return this->getIndex() - index;
			return 0;
		}
		[[nodiscard]] Position getWalkerIndex() const {
			return this->getIndex();
		}
	protected:
		void go(const Position index) override {
			#pragma clang diagnostic push
			#pragma ide diagnostic ignored "LoopDoesntUseConditionVariableInspection"
				while (this->getIndex() < index) this->operator++();
				while (this->getIndex() > index) this->operator--();
			#pragma clang diagnostic pop
		}
		void operator--() override {
			decrement(this->request->length - 1);
			this->setIndex(this->getIndex() - 1);
		}
		void increment() override {
			increment(this->request->length - 1);
		}
	private:
		void increment(const Position position) {
			if (++this->positions[position] > maxPosition(position)) {
				if (position != 0) { // preventing fail on end
					increment(position - 1);
					this->positions[position] = this->positions[position - 1] + 1;
				}
			}
		}
		void decrement(const Position position) const {
			--this->positions[position];
			if (position > 0 && this->positions[position] == this->positions[position - 1]) {
				for (Position c = position; c < this->request->length; c++) {
					this->positions[c] = maxPosition(c);
				}
				decrement(position - 1);
			}
		}
		[[nodiscard]] Position maxPosition(const Position position) const {
			return this->nrElements() + position - this->request->length;
		}
};
