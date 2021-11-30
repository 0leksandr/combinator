#pragma once

#include "../../Position.h"

namespace CombinatorNamespace {
	template<typename Element>
	class ElementAddressFetcher {
		public:
			template<class Request>
			static Element* get(
					const Request& request,
					const Position elementPosition,
					const Position combinationPosition
			) {
				return request.template getElementAddress<Element>(elementPosition, combinationPosition);
			}
	};
}
