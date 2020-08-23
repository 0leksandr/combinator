#pragma once

#include "ListIterator.h"
#include "../BackwardsIterator.h"
#include "../ForwardIterator.h"
#include "../../Request/FixedRequest.h"

template<class Container, class Combination>
class BackwardsListIterator : // TODO: BidirectionalListIterator
		public ForwardIterator,
		public BackwardsIterator,
		public ListIterator<Container, Combination> {
	public:
		explicit BackwardsListIterator(const FixedRequest<Container>* const request) :
				ListIterator<Container, Combination>(request),
				BackwardsIterator() {}
};
