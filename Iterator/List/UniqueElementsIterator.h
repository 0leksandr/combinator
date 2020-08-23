#pragma once

#include "ListIterator.h"
#include "../../Position.h"
#include "../../Request/FixedRequest.h"

template<class Container, class Combination>
class UniqueElementsIterator : public ListIterator<Container, Combination> {
	public:
		explicit UniqueElementsIterator(const FixedRequest<Container>* const request) :
				ListIterator<Container, Combination>(request) {
			for (Position c = 0; c < request->length; c++) this->positions[c] = c;
		}
};
