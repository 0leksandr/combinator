#pragma once

#include <vector>
#include "../Position.h"

namespace CombinatorNamespace {
	template<class Container>
	class MultisetRequest {
		public:
			const std::vector<Container> containers;

			MultisetRequest(const std::vector<Container>& containers) : containers(containers) {}
//			template<class T>
//			MultisetRequest(const T&& containers) : containers(containers) {}
			Position combinationSize() const {
				return containers.size();
			}

			template<typename Element>
			Element& getElementReference(const Position elementPosition, const Position combinationPosition) const {
				return (Element&)containers[combinationPosition][elementPosition];
			}
			template<typename Element>
			const Element getElementCopy(const Position elementPosition, const Position combinationPosition) const {
				return containers[combinationPosition][elementPosition];
			}
			template<typename ElementAddress>
			ElementAddress getElementAddress(const Position elementPosition, const Position combinationPosition) const {
				return (ElementAddress)&containers[combinationPosition][elementPosition];
			}
	};
}
