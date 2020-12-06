#pragma once

#include <vector>
#include "CombinationWrapper.h"

namespace CombinatorNamespace {
	template<class Request, class ElementFetcher, typename Element>
	class VectorCombination : public CombinationWrapper<std::vector<Element>, Request> {
		public:
			std::vector<Element>& get(const Request& request, Position* const positions) const override {
				combination = std::vector<Element>{};
				combination.reserve(request.combinationSize());
				for (Position c = 0, to = request.combinationSize(); c < to; c++) {
					combination.push_back(ElementFetcher::template get<Request, Element>(request, positions[c], c));
				}
				return combination;
			}
		private:
			mutable std::vector<Element> combination;
	};
}
