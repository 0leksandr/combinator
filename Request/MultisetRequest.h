#pragma once

#include <array>
#include "../Position.h"

template<class Container, Position NrContainers>
class MultisetRequest {
	public:
		const std::array<Container, NrContainers> containers;

		MultisetRequest(const std::array<Container, NrContainers>& containers) : containers(containers) {}
};
