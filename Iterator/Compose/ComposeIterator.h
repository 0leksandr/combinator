#pragma once

#include "../DereferencedIterator.h"
#include "../PositionedIterator.h"
#include "../SizedIterator.h"

template<class Combination>
class ComposeIterator :
		public DereferencedIterator<Combination>,
		public PositionedIterator,
		public SizedIterator {
	public:
		ComposeIterator() :
				DereferencedIterator<Combination>(),
				PositionedIterator(0),
				SizedIterator() {}
};
