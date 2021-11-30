#pragma once

#include <vector>
#include "CombinationWrapper.h"

namespace CombinatorNamespace {
	template<class Request, class ElementFetcher, typename CombinationElement>
	class VectorCombination : public CombinationWrapper<std::vector<CombinationElement>, Request> {
		public:
			std::vector<CombinationElement>& get(const Request& request, Position* const positions) const override {
				combination = std::vector<CombinationElement>{};
				combination.reserve(request.combinationSize());
				for (Position c = 0, to = request.combinationSize(); c < to; c++) {
					combination.push_back(ElementFetcher::template get<Request>(request, positions[c], c));
				}
				return combination;
			}
		private:
			mutable std::vector<CombinationElement> combination;
	};
}
