#pragma once

#include "Iterator.h"

class BackwardsIterator : public Iterator { // TODO: BidirectionalIterator ?
	public:
		BackwardsIterator() : Iterator() {}
	protected:
		virtual void operator--() = 0;
};
