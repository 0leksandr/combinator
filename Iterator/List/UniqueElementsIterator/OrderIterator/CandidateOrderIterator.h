#pragma once

#include "OrderIterator.h"
#include "../../../Movement/RandomAccessIterator.h"
#include "../../../../Position.h"
#include "../../../../Request/FixedRequest.h"

template<class Container, class Combination>
class CandidateOrderIterator :
		public OrderIterator<Container, Combination>,
		public RandomAccessIterator {
	public:
		explicit CandidateOrderIterator(const FixedRequest<Container>& request) :
				OrderIterator<Container, Combination>(request) {}
		[[nodiscard]] virtual Position estimate(Position index) const = 0;
		[[nodiscard]] Position getPosition(const Position position) const {
			return this->positions[position];
		}
};
