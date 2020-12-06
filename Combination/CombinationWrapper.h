#pragma once

#include "../Position.h"

namespace CombinatorNamespace {
	template<typename Combination, class Request>
	class CombinationWrapper {
		public:
			virtual Combination& get(const Request& request, Position* const positions) const = 0;
	};
}
