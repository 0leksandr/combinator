#pragma once

#include "ForwardIterator.h"
#include "../FixedRequest.h"

template<class Container, class Combination>
class BackwardsIterator : public ForwardIterator<Container, Combination> {
	public:
		explicit BackwardsIterator(const FixedRequest<Container>* const request) :
				ForwardIterator<Container, Combination>(request) {}
	protected:
		virtual void operator--() = 0;
};
