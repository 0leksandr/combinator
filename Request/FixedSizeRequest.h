#pragma once

#include "../Position.h"

namespace CombinatorNamespace {
	template<class Container>
	class FixedSizeRequest {
		public:
			const Container elements;
			const Position length;

			FixedSizeRequest(
					const Container elements,
					const Position length
			) : elements(elements), length(length) {}
			Position combinationSize() const { // TODO: override
				return length;
			}

			template<typename Element>
			/*const*/ Element& getElement(const Position elementPosition, const Position) const { // TODO: override
				// TODO: hope it's ok
				return (Element&)elements[elementPosition];
			}
			template<typename ElementAddress>
			ElementAddress getElementAddress(const Position elementPosition, const Position) const {
				// TODO: hope it's ok
				return (ElementAddress)&elements[elementPosition];
			}
	};
}
