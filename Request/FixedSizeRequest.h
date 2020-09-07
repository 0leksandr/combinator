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
	};
}
