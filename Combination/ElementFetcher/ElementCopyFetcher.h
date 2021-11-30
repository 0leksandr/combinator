#pragma once

#include "../../Position.h"

namespace CombinatorNamespace {
	template<typename Element>
	class ElementCopyFetcher {
		public:
			template<class Request>
			static Element get(
					const Request& request,
					const Position elementPosition,
					const Position combinationPosition
			) {
				return request.template getElementCopy<Element>(elementPosition, combinationPosition);
			}
	};
}
