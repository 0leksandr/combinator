#pragma once

#include <array>
#include "CombinationWrapper.h"

namespace CombinatorNamespace {
	template<class Request, typename Element, size_t Size>
	class StdArrayCombination : public CombinationWrapper<std::array<Element, Size>, Request> {
		public:
			std::array<Element, Size>& get(const Request& request, Position* const positions) const override {
				for (Position c = 0, to = request.combinationSize(); c < to; c++) {
					combination[c] = request.template getElement<Element>(positions[c], c);
				}
				return combination;
			}
		private:
			mutable std::array<Element, Size> combination;
	};
}
