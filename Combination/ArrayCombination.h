#pragma once

#include <array>
#include "CombinationWrapper.h"

namespace CombinatorNamespace {
	template<class Request, class ElementFetcher, typename Element>
	class ArrayCombination : public CombinationWrapper<Element*, Request> {
		public:
			ArrayCombination(const Position size) : combination(new Element[size]) {}
			~ArrayCombination() {
				delete[] combination;
			}
			Element*& get(const Request& request, Position* const positions) const override {
				for (Position c = 0, to = request.combinationSize(); c < to; c++) {
					combination[c] = ElementFetcher::template get<Request, Element>(request, positions[c], c);
				}
				return combination;
			}
		private:
			mutable Element* combination;
	};
}
