#pragma once

#include "BackwardsListIterator.h"
#include "../RandomAccessIterator.h"
#include "../../Position.h"
#include "../../Request/FixedRequest.h"

template<class Container, class Combination>
class RandomAccessListIterator : // TODO: remove?
		public BackwardsListIterator<Container, Combination>, // TODO: do not extend from BackwardsListIterator
		public RandomAccessIterator {
	public:
		explicit RandomAccessListIterator(const FixedRequest<Container>* const request) :
				BackwardsListIterator<Container, Combination>(request),
				RandomAccessIterator() {}
};
