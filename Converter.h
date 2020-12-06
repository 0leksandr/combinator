#pragma once

#include <array>
#include <vector>
#include "Combination/ElementFetcher/ElementFetcher.h"
#include "Combination/ElementFetcher/ElementAddressFetcher.h"
#include "Combination/ArrayCombination.h"
#include "Combination/StdArrayCombination.h"
#include "Combination/VectorCombination.h"
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
			[[maybe_unused]] static CombinationWrapper<Combination, Request>* createCombinationWrapper(
					std::array<Element, Size> *,
					const Position,
					const Element& example
			) {
				return new StdArrayCombination<Request, ElementFetcher, Element, Size>{};
			}
			template<typename Element, size_t Size>
			[[maybe_unused]] static CombinationWrapper<Combination, Request>* createCombinationWrapper(
					std::array<Element*, Size> *,
					const Position,
					const Element& example
			) {
				return new StdArrayCombination<Request, ElementAddressFetcher, Element*, Size>{};
			}
			template<typename Element>
			[[maybe_unused]] static CombinationWrapper<Combination, Request>* createCombinationWrapper(
					std::vector<Element> *,
					const Position,
					const Element& example
			) {
				return new VectorCombination<Request, ElementFetcher, Element>{};
			}
			template<typename Element>
			[[maybe_unused]] static CombinationWrapper<Combination, Request>* createCombinationWrapper(
					std::vector<Element*> *,
					const Position,
					const Element& example
			) {
				return new VectorCombination<Request, ElementAddressFetcher, Element*>{};
			}
			template<typename Element>
			[[maybe_unused]] static CombinationWrapper<Combination, Request>* createCombinationWrapper(
					Element* *,
					const Position size,
					const Element& example
			) {
				return new ArrayCombination<Request, ElementFetcher, Element>{size};
			}
			template<typename Element>
			[[maybe_unused]] static CombinationWrapper<Combination, Request>* createCombinationWrapper(
					Element** *,
					const Position size,
					const Element& example
			) {
				return new ArrayCombination<Request, ElementAddressFetcher, Element*>{size};
			}

			template<typename T>
			static T* tplToParam() {
				return nullptr;
			}
	};
}
