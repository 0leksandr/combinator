#pragma once

#include "../Position.h"

template<class Container>
class FixedRequest { // TODO: FixedListRequest?
	public:
		const Container elements;
		const Position length;

		FixedRequest(
				const Container elements,
				const Position length
		) : elements(elements), length(length) {}
};
