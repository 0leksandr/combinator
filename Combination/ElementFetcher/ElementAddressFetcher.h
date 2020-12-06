#pragma once

#include "../../Position.h"

namespace CombinatorNamespace {
	class ElementAddressFetcher {
		public:
			template<class Request, typename ElementAddress>
			static ElementAddress get(
					const Request& request,
					const Position elementPosition,
					const Position combinationPosition
			) {
				return request.template getElementAddress<ElementAddress>(elementPosition, combinationPosition);
			}
	};
}
