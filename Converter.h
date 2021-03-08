#pragma once

#include <array>
#include <vector>
#include "Combination/ElementFetcher/ElementAddressFetcher.h"
#include "Combination/ElementFetcher/ElementCopyFetcher.h"
#include "Combination/ElementFetcher/ElementReferenceFetcher.h"
#include "Combination/ArrayCombination.h"
#include "Combination/StdArrayCombination.h"
#include "Combination/VectorCombination.h"
#include "Position.h"

#include "my/macro.cpp"

namespace CombinatorNamespace {
	template<typename Combination, class Request>
	class Converter { // TODO: CombinationInitializer, Translator, CombinationFactory ?
		public:
			template<typename Element>
			static auto createCombinationWrapper(const Position size) {
				return createCombinationWrapper(tplToParam<Combination>(), size);
			}
		private:
			template<typename Element, size_t Size>
			[[maybe_unused]] static auto createCombinationWrapper(
					std::array<Element, Size> *,
					const Position size
			) {
				Assert(size == Size);
				return new StdArrayCombination<Request, ElementReferenceFetcher, Element, Size>{};
			}
			template<typename Element, size_t Size>
			[[maybe_unused]] static auto createCombinationWrapper(
					std::array<Element*, Size> *,
					const Position size
			) {
				Assert(size == Size);
				return new StdArrayCombination<Request, ElementAddressFetcher, Element*, Size>{};
			}

			template<typename Element>
			[[maybe_unused]] static auto createCombinationWrapper(
					std::vector<Element> *,
					const Position
			) {
				return new VectorCombination<Request, ElementReferenceFetcher, Element>{};
			}
			template<typename Element>
			[[maybe_unused]] static auto createCombinationWrapper(
					std::vector<Element*> *,
					const Position
			) {
				return new VectorCombination<Request, ElementAddressFetcher, Element*>{};
			}

			template<typename Element>
			[[maybe_unused]] static auto createCombinationWrapper(
					Element* *,
					const Position size
			) {
				return new ArrayCombination<Request, ElementReferenceFetcher, Element>{size};
			}
			template<typename Element>
			[[maybe_unused]] static auto createCombinationWrapper(
					Element** *,
					const Position size
			) {
				return new ArrayCombination<Request, ElementAddressFetcher, Element*>{size};
			}

			template<typename T>
			static T* tplToParam() {
				return nullptr;
			}
	};
}
