#pragma once

#include <array>
#include "CombinationWrapper.h"
#include "../Iterator/List/ListIterator.h"
#include "../Request/FixedSizeRequest.h"

namespace CombinatorNamespace {
	template<class Container, class Element, size_t Size>
	class StdArrayCombination : public CombinationWrapper<std::array<Element, Size>, FixedSizeRequest<Container>> {
		public:
			std::array<Element, Size>& get(
					const FixedSizeRequest<Container>& request,
					Position* const positions
			) const override {
				for (Position c = 0; c < request.length; c++) {
					combination[c] = request.elements[positions[c]];
				}
				return combination;
			}
		private:
			mutable std::array<Element, Size> combination;
	};
}
