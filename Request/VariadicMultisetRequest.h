#pragma once

#include <tuple>
#include "../Position.h"

#include "my/macro.cpp"

namespace CombinatorNamespace {
	template<class... Containers>
	class VariadicMultisetRequest {
		public:
			explicit VariadicMultisetRequest(Containers... containers) :
					containers(containers...)
			{
				static_assert(sizeof...(Containers) >= 2);
				ContainerSelector<sizeof...(Containers) - 1, true>::checkElementType(this->containers);
			}

			[[nodiscard]] Position combinationSize() const {
				return sizeof...(Containers);
			}
			[[nodiscard]] Position containerSize(const Position containerIdx) const {
				return ContainerSelector<sizeof...(Containers), true>::containerSize(
						containerIdx,
						this->containers
				);
			}

			template<typename Element>
			const Element& getElementReference(
					const Position elementPosition,
					const Position combinationPosition
			) const {
				return ContainerSelector<sizeof...(Containers), true>::template getElementReference<Element>(
						this->containers,
						unsigned(combinationPosition),
						elementPosition
				);
			}
			template<typename Element>
			Element getElementCopy(const Position elementPosition, const Position combinationPosition) const {
				return ContainerSelector<sizeof...(Containers), true>::template getElementCopy<Element>(
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
				return ContainerSelector<sizeof...(Containers), true>
				        ::template getElementAddress<Element*>(
								this->containers,
								unsigned(combinationPosition),
								elementPosition
						);
			}
		private:
			std::tuple<Containers...> containers;

			template<unsigned c, bool b>
			struct ContainerSelector {
				static void checkElementType(const std::tuple<Containers...>& containers) {
					static_assert(std::is_same_v<
							typeof(std::get<0>(containers)[666]),
							typeof(std::get<c-1>(containers)[666])
					>);
					ContainerSelector<c - 1, b>::checkElementType(containers);
				}
				static Position containerSize(const unsigned containerIdx, const std::tuple<Containers...>& containers) {
					if (c == containerIdx+1) return std::get<c-1>(containers).size();
					return ContainerSelector<c - 1, b>::containerSize(containerIdx, containers);
				}
				template<typename Element>
				static const Element& getElementReference(
						const std::tuple<Containers...>& containers,
						const unsigned containerIdx,
						const Position elementIdx
				) {
					if (c == containerIdx+1) return (const Element&)std::get<c-1>(containers)[elementIdx];
					return ContainerSelector<c - 1, b>::template getElementReference<Element>(
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
					return ContainerSelector<c - 1, b>::template getElementCopy<Element>(
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
					return ContainerSelector<c - 1, b>::template getElementAddress<Element>(
							containers,
							containerIdx,
							elementIdx
					);
				}
			};

			template<bool b> // TODO: `template<>` when C++ will support it inside a class
			struct ContainerSelector<0, b> {
				static void checkElementType(const std::tuple<Containers...>&) {}
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
	};
}
