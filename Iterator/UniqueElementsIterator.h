#pragma once

#include "RandomAccessIterator.h"
#include "../FixedRequest.h"
#include "../Position.h"

template<class Container, class Combination>
class UniqueElementsIterator : public RandomAccessIterator<Container, Combination> {
	public:
		explicit UniqueElementsIterator(const FixedRequest<Container>* const request) :
				RandomAccessIterator<Container, Combination>(request) {
			for (Position c = 0; c < request->length; c++) this->positions[c] = c;
		}
};
