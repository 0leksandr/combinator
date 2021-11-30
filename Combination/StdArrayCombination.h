#pragma once

#include <array>
#include "CombinationWrapper.h"

namespace CombinatorNamespace {
	template<class Request, class ElementFetcher, typename CombinationElement, size_t Size>
	class StdArrayCombination : public CombinationWrapper<std::array<CombinationElement, Size>, Request> {
		public:
			std::array<CombinationElement, Size>& get(
					const Request& request,
					Position* const positions
			) const override {
				for (Position c = 0, to = request.combinationSize(); c < to; c++) {
					combination[c] = ElementFetcher::template get<Request>(request, positions[c], c);
				}
				return combination;
			}
		private:
			mutable std::array<CombinationElement, Size> combination;
	};
}
