#pragma once

#include "../../Position.h"

namespace CombinatorNamespace {
	class ElementFetcher {
		public:
			template<class Request, typename Element>
			static Element& get(
					const Request& request,
					const Position elementPosition,
					const Position combinationPosition
			) { // TODO: override
				return request.template getElement<Element>(elementPosition, combinationPosition);
			}
	};
}
