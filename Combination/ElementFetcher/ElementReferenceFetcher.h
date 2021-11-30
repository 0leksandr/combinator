#pragma once

#include "../../Position.h"

namespace CombinatorNamespace {
	template<typename Element>
	class ElementReferenceFetcher {
		public:
			template<class Request>
			static const Element& get(
					const Request& request,
					const Position elementPosition,
					const Position combinationPosition
			) { // TODO: override
				return request.template getElementReference<Element>(elementPosition, combinationPosition);
			}
	};
}
