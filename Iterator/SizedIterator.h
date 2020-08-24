#pragma once

#include "Iterator.h"
#include "../Position.h"

class SizedIterator : public Iterator {
	public:
		template<class Request>
		static Position size(const Request& request) { // TODO: virtual
			throw 0;
		}
};
