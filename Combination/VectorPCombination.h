#pragma once

#include <vector>
#include "CombinationWrapper.h"

namespace CombinatorNamespace {
	template<class Request, typename Element>
	class VectorPCombination : public CombinationWrapper<std::vector<Element*>, Request> {
		public:
			VectorPCombination(const Position size) {
				combination.reserve(size);
				for (int c = 0; c < size; ++c) combination.push_back((Element*) nullptr);
			}
			std::vector<Element*>& get(const Request& request, Position* const positions) const override {
				for (Position c = 0, to = request.combinationSize(); c < to; c++) {
					combination[c] = request.template getElementAddress<Element>(positions[c], c);
				}
				return combination;
			}
		private:
			mutable std::vector<Element*> combination;
	};
}


