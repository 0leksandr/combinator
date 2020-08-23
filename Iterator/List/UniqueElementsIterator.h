#pragma once

#include "ListIterator.h"
#include "../ForwardIterator.h"
#include "../PositionedIterator.h"
#include "../RandomAccessIterator.h"
#include "../SizedIterator.h"
#include "../../Position.h"
#include "../../Request/FixedRequest.h"

template<class Container, class Combination>
class UniqueElementsIterator :
		public ListIterator<Container, Combination>,
		public SizedIterator,
		public PositionedIterator,
		public ForwardIterator,
		public RandomAccessIterator {
	public:
		explicit UniqueElementsIterator(const FixedRequest<Container>* const request) :
				ListIterator<Container, Combination>(request),
				SizedIterator(),
				PositionedIterator(0),
				ForwardIterator(),
				RandomAccessIterator() {
			for (Position c = 0; c < request->length; c++) this->positions[c] = c;
		}
};
