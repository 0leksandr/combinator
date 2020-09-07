#pragma once

#include <vector>
#include "../Position.h"

namespace CombinatorNamespace {
	template<class Container>
	class MultisetRequest {
		public:
			const std::vector<Container> containers;

	//		MultisetRequest(const Containers&&... containers) : containers{std::forward<Containers>(containers)...} {}
			MultisetRequest(const std::vector<Container>& containers) : containers(containers) {}
	//		template<class T>
	//		MultisetRequest(const T&& containers) : containers(containers) {}
	};
}
