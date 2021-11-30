#pragma once

#include <array>
#include "CombinationWrapper.h"

namespace CombinatorNamespace {
	template<class Request, class ElementFetcher, typename CombinationElement>
	class ArrayCombination : public CombinationWrapper<CombinationElement*, Request> {
		public:
			explicit ArrayCombination(const Position size) : combination(new CombinationElement[size]) {}
			~ArrayCombination() {
				delete[] combination;
			}
			CombinationElement*& get(const Request& request, Position* const positions) const override {
				for (Position c = 0, to = request.combinationSize(); c < to; c++) {
					combination[c] = ElementFetcher::template get<Request>(request, positions[c], c);
				}
				return combination;
			}
		private:
			mutable CombinationElement* combination;
	};
}
