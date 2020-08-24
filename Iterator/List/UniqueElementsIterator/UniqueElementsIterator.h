#pragma once

#include "../ListIterator.h"
#include "../../SizedIterator.h"
#include "../../../Position.h"
#include "../../../Request/FixedRequest.h"

template<class Container, class Combination>
class UniqueElementsIterator :
		public ListIterator<Container, Combination>,
		public SizedIterator {
	public:
		UniqueElementsIterator(const FixedRequest<Container>* const request, const Position size) :
				ListIterator<Container, Combination>(request),
				SizedIterator(size) {
			for (Position c = 0; c < request->length; c++) this->positions[c] = c;
		}
};
