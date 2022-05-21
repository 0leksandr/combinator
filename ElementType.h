#pragma once

#include <array>
#include <vector>

namespace CombinatorNamespace {
	template<typename T>
	struct ElementType {
		//	static_assert(false);
		typedef T type;
	};

	template<typename T>
	struct ElementType<std::vector<T>> {
		typedef T type;
	};

	template<typename T, size_t Size>
	struct ElementType<std::array<T, Size>> {
		typedef T type;
	};
}
