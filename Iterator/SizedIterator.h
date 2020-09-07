#pragma once

#include "Iterator.h"
#include "../Position.h"

namespace CombinatorNamespace {
	class SizedIterator : public Iterator {
		public:
			template<class Request>
			static Position size(const Request& request) { // TODO: virtual
				throw 0;
			}
	};
}
