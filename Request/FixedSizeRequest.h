#pragma once

#include "../Position.h"

namespace CombinatorNamespace {
	template<class Container>
	class FixedSizeRequest {
		public:
			const Container elements; // TODO: private
			const Position length;

			FixedSizeRequest(
					const Container elements,
					const Position length
			) : elements(elements), length(length) {}
			[[nodiscard]] Position combinationSize() const {
				return length;
			}

			template<typename Element>
			const Element& getElementReference(const Position elementPosition, const Position) const {
				return (const Element&)this->elements[elementPosition]; // TODO: `return this->elements[elementPosition];` ?
			}
			template<typename Element>
			Element getElementCopy(const Position elementPosition, const Position) const {
				return this->elements[elementPosition];
			}
			template<typename Element>
			Element* getElementAddress(const Position elementPosition, const Position) const {
				return (Element*)(&(this->elements[elementPosition]));
			}
	};
}
