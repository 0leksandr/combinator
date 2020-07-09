#pragma once

#include "BackwardsIterator.h"
#include "../Position.h"
#include "../Request/FixedRequest.h"

template<class Container, class Combination>
class RandomAccessIterator : public BackwardsIterator<Container, Combination> {
	public:
		explicit RandomAccessIterator(const FixedRequest<Container>* const request) :
				BackwardsIterator<Container, Combination>(request) {}
		virtual void go(Position index) = 0;
};
