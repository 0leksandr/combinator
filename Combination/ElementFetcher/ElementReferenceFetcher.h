#pragma once

#include "../../Position.h"

namespace CombinatorNamespace {
	class ElementReferenceFetcher {
		public:
			template<class Request, typename Element>
			static Element& get(
					const Request& request,
					const Position elementPosition,
					const Position combinationPosition
			) { // TODO: override
				return request.template getElementReference<Element>(elementPosition, combinationPosition);
			}
	};
}
