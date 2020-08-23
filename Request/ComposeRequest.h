#pragma once

#include "../Position.h"

template<class Container, Position NrContainers>
class ComposeRequest {
	public:
		const std::array<Container, NrContainers> containers;

		ComposeRequest(const std::array<Container, NrContainers>& containers) : containers(containers) {}
};
