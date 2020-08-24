#pragma once

#include "CandidateOrderIterator.h"
#include "Hunter.h"
#include "Mathematician.h"
#include "OrderIterator.h"
#include "Walker.h"
#include "../../../Movement/RandomAccessIterator.h"
#include "../../../../Assert.h"
#include "../../../../Position.h"
#include "../../../../Request/FixedRequest.h"

template<class Container, class Combination>
class ComboIterator :
		public OrderIterator<Container, Combination>,
		public RandomAccessIterator {
	private:
		std::vector<CandidateOrderIterator<Container, Combination>*> iterators;
	public:
		explicit ComboIterator(const FixedRequest<Container>* const request) :
				OrderIterator<Container, Combination>(request),
				iterators() {
			iterators.push_back(new Walker<Container, Combination>(request));
			iterators.push_back(new Hunter<Container, Combination>(request));
			iterators.push_back(new Mathematician<Container, Combination>(request));
		}
		~ComboIterator() {
			for (auto* iterator : iterators) delete iterator;
		}
	protected:
		void go(Position index) override {
			auto estimated((Position)-1);
			CandidateOrderIterator<Container, Combination>* chosen(nullptr);
			for (auto* iterator : iterators) {
				Position myBet = iterator->estimate(index);
				if (myBet < estimated) {
					chosen = iterator;
					estimated = myBet;
				}
			}
			Assert(chosen != nullptr);
			chosen->operator[](index);
			for (Position c = 0; c < this->request->length; c++) {
				this->positions[c] = chosen->getPosition(c);
			}
		}
};
