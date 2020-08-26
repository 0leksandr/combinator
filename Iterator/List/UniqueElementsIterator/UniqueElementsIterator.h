#pragma once

#include "../ListIterator.h"
#include "../../SizedIterator.h"
#include "../../../Position.h"
#include "../../../Request/FixedSizeRequest.h"

template<class Container, class Combination>
class UniqueElementsIterator :
		public ListIterator<Container, Combination>,
		public SizedIterator {
	public:
		explicit UniqueElementsIterator(const FixedSizeRequest<Container>& request) :
				ListIterator<Container, Combination>(request) {
			for (Position c = 0; c < request.length; c++) this->positions[c] = c;
		}
};
