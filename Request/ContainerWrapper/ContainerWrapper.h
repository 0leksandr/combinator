#pragma once

#include <vector>

template<class Container, bool ContainerReference>
class ContainerWrapper {
	public:
		explicit ContainerWrapper(const Container& container) : container(container) {}
//		typename std::result_of<decltype(&Container::operator[])(Container)>::type operator[](const size_t i) const {
//		decltype(&Container::operator[](std::declval<size_t>))(size_t) operator[](const size_t i) const {
//		decltype(this->container[0]) operator[](const size_t i) const {
		auto& operator[](const size_t i) const {
			return this->container[i];
		}
		[[nodiscard]] size_t size() const {
			return this->container.size();
		}
		static std::vector<ContainerWrapper> wrapVector(const std::vector<Container>& containers) {
			std::vector<ContainerWrapper> v;
			v.reserve(containers.size());
			for (const auto& container : containers) v.template emplace_back(container);
			return v;
		}
	private:
		const typename std::conditional<ContainerReference, const Container&, const Container>::type container;
};
