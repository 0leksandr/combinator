#pragma once

#include <tuple>
#include "../Position.h"

#include "my/macro.cpp"

namespace CombinatorNamespace {
	namespace {
		template<unsigned c, class... Containers>
		struct ContainerSelector {
			static Position containerSize(const unsigned containerIdx, const std::tuple<Containers...>& containers) {
				if (c == containerIdx+1) return std::get<c-1>(containers).size();
				return ContainerSelector<c - 1, Containers...>::containerSize(containerIdx, containers);
			}
			template<typename Element>
			static const Element& getElementReference(
					const std::tuple<Containers...>& containers,
					const unsigned containerIdx,
					const Position elementIdx
			) {
				if (c == containerIdx+1) return (const Element&)std::get<c-1>(containers)[elementIdx];
				return ContainerSelector<c - 1, Containers...>::template getElementReference<Element>(
						containers,
						containerIdx,
						elementIdx
				);
			}
			template<typename Element>
			static Element getElementCopy(
					const std::tuple<Containers...>& containers,
					const unsigned containerIdx,
					const Position elementIdx
			) {
				if (c == containerIdx+1) return std::get<c-1>(containers)[elementIdx];
				return ContainerSelector<c - 1, Containers...>::template getElementCopy<Element>(
						containers,
						containerIdx,
						elementIdx
				);
			}
			template<typename Element>
			static Element* getElementAddress(
					const std::tuple<Containers...>& containers,
					const unsigned containerIdx,
					const Position elementIdx
			) {
				if (c == containerIdx+1) return (Element*)&std::get<c-1>(containers)[elementIdx];
				return ContainerSelector<c - 1, Containers...>::template getElementAddress<Element>(
						containers,
						containerIdx,
						elementIdx
				);
			}
		};

		template<class... Containers>
		struct ContainerSelector<0, Containers...> {
			static Position containerSize(const unsigned, const std::tuple<Containers...>&) {
				assert(false);
			}
			template<typename Element>
			static const Element& getElementReference(
					const std::tuple<Containers...>&,
					const unsigned,
					const Position
			) {
				assert(false);
			}
			template<typename Element>
			static Element getElementCopy(const std::tuple<Containers...>&, const unsigned, const Position) {
				assert(false);
			}
			template<typename Element>
			static Element* getElementAddress(
					const std::tuple<Containers...>&,
					const unsigned,
					const Position
			) {
				assert(false);
			}
		};
	}

	template<class... Containers>
	class VariadicMultisetRequest {
		public:
			explicit VariadicMultisetRequest(Containers... containers) :
					containers(containers...) { // TODO: std::make_tuple?
				static_assert(sizeof...(Containers) >= 1);
			}

			[[nodiscard]] Position combinationSize() const {
				return sizeof...(Containers);
			}
			[[nodiscard]] Position containerSize(const Position containerIdx) const {
				return ContainerSelector<sizeof...(Containers), Containers...>::containerSize(
						containerIdx,
						this->containers
				);
			}

			template<typename Element>
			const Element& getElementReference(
					const Position elementPosition,
					const Position combinationPosition
			) const {
				return ContainerSelector<sizeof...(Containers), Containers...>::template getElementReference<Element>(
						this->containers,
						unsigned(combinationPosition),
						elementPosition
				);
			}
			template<typename Element>
			Element getElementCopy(const Position elementPosition, const Position combinationPosition) const {
				return ContainerSelector<sizeof...(Containers), Containers...>::template getElementCopy<Element>(
						this->containers,
						unsigned(combinationPosition),
						elementPosition
				);
			}
			template<typename Element>
			Element* getElementAddress(
					const Position elementPosition,
					const Position combinationPosition
			) const {
				return ContainerSelector<sizeof...(Containers), Containers...>
				        ::template getElementAddress<Element*>(
								this->containers,
								unsigned(combinationPosition),
								elementPosition
						);
			}
		private:
			std::tuple<Containers...> containers;
	};
}
