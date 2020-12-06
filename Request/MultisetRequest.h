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
			Position combinationSize() const {
				return containers.size();
			}
			template<typename Element>
			const Element& getElement(const Position elementPosition, const Position combinationPosition) const {
				return containers[combinationPosition][elementPosition];
			}
			template<typename Element>
			Element* getElementAddress(const Position elementPosition, const Position combinationPosition) const {
				// TODO: hope it's ok
				return (Element*)&containers[combinationPosition][elementPosition];
			}
	};
}
