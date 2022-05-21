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

			static auto wrapContainer(const std::vector<Container>& containers) {
				std::vector<ContainerWrapper> v;
				v.reserve(containers.size());
				for (const auto& container : containers) v.template emplace_back(container);
				return v;
			}
			template<Position Size>
			static auto wrapContainer(const std::array<Container, Size>& containers) {
				std::array<ContainerWrapper, Size> a;
				for (int c = 0; c < Size; ++c) a[c] = ContainerWrapper{containers[c]};
				return a;
			}
		private:
			const typename std::conditional<ContainerReference, const Container&, const Container>::type container;
	};
}
