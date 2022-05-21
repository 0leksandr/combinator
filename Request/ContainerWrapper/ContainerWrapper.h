#pragma once

#include <array>
#include <vector>
#include "../../Position.h"

namespace CombinatorNamespace {
	template<class Container, bool ContainerReference>
	class ContainerWrapper {
		public:
			explicit ContainerWrapper(const Container& container) : container(container) {}
			const auto& operator[](const size_t i) const {
				return this->container[i];
			}
			auto& operator[](const size_t i) {
				return this->container[i];
			}
			[[nodiscard]] size_t size() const {
				return this->container.size();
			}
		private:
			const typename std::conditional<ContainerReference, const Container&, const Container>::type container;
	};
}
