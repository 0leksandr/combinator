#pragma once

#include <array>
#include <vector>
#include "Combination/StdArrayCombination.h"
#include "Combination/StdArrayPCombination.h"
#include "Combination/VectorCombination.h"
#include "Combination/VectorPCombination.h"
#include "Position.h"

namespace CombinatorNamespace {
	template<typename Combination, class Request>
	class Converter { // TODO: CombinationInitializer
		public:
			template<typename Element>
			static CombinationWrapper<Combination, Request>* createCombinationWrapper(
					const Position size,
					const Element& example
			) {
				return createCombinationWrapper(tplToParam<Combination>(), size, example);
			}
		private:
			template<typename Element, size_t Size>
			static CombinationWrapper<Combination, Request>* createCombinationWrapper(
					std::array<Element, Size>*,
					const Position size,
					const Element& example
			) {
				return new StdArrayCombination<Request, Element, Size>{};
			}
			template<typename Element, size_t Size>
			static CombinationWrapper<Combination, Request>* createCombinationWrapper(
					std::array<Element*, Size>*,
					const Position size,
					const Element& example
			) {
				return new StdArrayPCombination<Request, Element, Size>{};
			}
			template<typename Element>
			static CombinationWrapper<Combination, Request>* createCombinationWrapper(
					std::vector<Element>*,
					const Position size,
					const Element& example
			) {
				return new VectorCombination<Request, Element>{};
			}
			template<typename Element>
			static CombinationWrapper<Combination, Request>* createCombinationWrapper(
					std::vector<Element*>*,
					const Position size,
					const Element& example
			) {
				return new VectorPCombination<Request, Element>{size};
			}
			template<typename T>
			static T* tplToParam() {
				return nullptr;
			}
	};
}
