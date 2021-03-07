#pragma once

#include "../../Position.h"

namespace CombinatorNamespace {
	class ElementCopyFetcher {
		public:
			template<class Request, typename Element>
			static const Element get(
					const Request& request,
					const Position elementPosition,
					const Position combinationPosition
			) {
				return request.template getElementCopy<Element>(elementPosition, combinationPosition);
			}
	};
}
