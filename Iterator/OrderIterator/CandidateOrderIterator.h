#pragma once

#include "OrderIterator.h"
#include "../../Position.h"
#include "../../Request/FixedRequest.h"

template<class Container, class Combination>
class CandidateOrderIterator : public OrderIterator<Container, Combination> {
	public:
		explicit CandidateOrderIterator(const FixedRequest<Container>* const request) :
				OrderIterator<Container, Combination>(request) {}
		[[nodiscard]] virtual Position estimate(Position index) const = 0;
		[[nodiscard]] Position getPosition(const Position position) const {
			return this->positions[position];
		}
};
