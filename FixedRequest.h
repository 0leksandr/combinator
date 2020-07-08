#pragma once

#include "Position.h"

template<class Container>
class FixedRequest {
	public:
		const Container elements;
		const Position length;

		FixedRequest(
				const Container elements,
				const Position length
		) : elements(elements), length(length) {}
};
