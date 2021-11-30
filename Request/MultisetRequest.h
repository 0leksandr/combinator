#pragma once

#include <vector>
#include "../Position.h"

namespace CombinatorNamespace {
	template<class Container>
	class MultisetRequest {
		public:
			explicit MultisetRequest(const std::vector<Container>& containers) : containers(containers) {}
//			template<class T>
//			MultisetRequest(const T&& containers) : containers(containers) {}

			[[nodiscard]] Position combinationSize() const {
				return containers.size();
			}
			[[nodiscard]] Position nrContainers() const {
				return this->containers.size();
			}
			[[nodiscard]] Position containerSize(const Position containerIdx) const {
				return this->containers[containerIdx].size();
			}

			template<typename Element>
			Element& getElementReference(const Position elementPosition, const Position combinationPosition) const {
				return (Element&)containers[combinationPosition][elementPosition];
			}
			template<typename Element>
			Element getElementCopy(const Position elementPosition, const Position combinationPosition) const {
				return containers[combinationPosition][elementPosition];
			}
			template<typename Element>
			Element* getElementAddress(const Position elementPosition, const Position combinationPosition) const {
				return (Element*)&containers[combinationPosition][elementPosition];
			}
		private:
			const std::vector<Container> containers;
	};
}
