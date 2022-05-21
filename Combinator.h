#pragma once

#include <array>
#include <vector>
#include "Combinator/Cartesian.h"
#include "Combinator/Combinator.h"
#include "Combinator/MultiPermutator.h"
#include "Combinator/Permutator.h"
#include "Combinator/VariadicCartesian.h"
#include "ElementType.h"
#include "Position.h"

// TODO: Cartesian from variadic list of collections?
// TODO: make Permutator::begin return some Walker
// TODO: static combination size => static CombinationWrapper
// TODO: throw out-of-bounds exceptions
// TODO: "natural" combinations order (sort by first element, then second etc.)
// TODO: exceptions instead of `Assert`
// TODO: CombinatorNamespace > anonymous namespace
// TODO: remove `my`

// Граблі:
// - не можна зробити загальний інтерфейс для Request'ів, тому що альтернативи:
//   - `template<typename Element>` має бути в методах, не в класі загалом, тому що тоді треба тягнути Element в
//     Request, а з ними - і на найвищий рівень
//   - шаблонний метод не можна зробити віртуальним (`template<> virtual void f()` - помилка) :c

namespace CombinatorNamespace {
	template<class Combination, class Container>
	using Coalesce = typename std::conditional<
	        std::is_same_v<Combination, nullptr_t>,
			Container,
			Combination
	>::type;
}

template<class Combination = nullptr_t, bool ReferenceContainer = false, class Container>
auto Combinator(const Container& elements, const CombinatorNamespace::Position length) {
	return CombinatorNamespace::Combinator<
			Container,
			CombinatorNamespace::Coalesce<Combination, Container>,
			ReferenceContainer
	>{elements, length};
}

template<class Combination = nullptr_t, bool ReferenceContainer = false, class Container>
auto Permutator(const Container& elements, const CombinatorNamespace::Position length) {
	return CombinatorNamespace::Permutator<
			Container,
			CombinatorNamespace::Coalesce<Combination, Container>,
			ReferenceContainer
	>{elements, length};
}
template<class Combination = nullptr_t, bool ReferenceContainer = false, class Container>
auto Permutator(const Container& elements) {
	return CombinatorNamespace::Permutator<
			Container,
			CombinatorNamespace::Coalesce<Combination, Container>,
			ReferenceContainer
	>{elements};
}

template<class Combination = nullptr_t, bool ReferenceContainer = false, class Container>
auto MultiPermutator(const Container& elements, const CombinatorNamespace::Position length) {
	return CombinatorNamespace::MultiPermutator<
			Container,
			CombinatorNamespace::Coalesce<Combination, Container>,
			ReferenceContainer
	>{elements, length};
}

template<class Combination = nullptr_t, class Container>
auto Cartesian(const std::vector<Container>& containers) {
	return CombinatorNamespace::Cartesian<
			std::vector<Container>,
			CombinatorNamespace::Coalesce<
			        Combination,
					std::vector<typename CombinatorNamespace::ElementType<Container>::type>
			>
	>{containers};
}
template<class Combination = nullptr_t, class Container, CombinatorNamespace::Position Size>
auto Cartesian(const std::array<Container, Size>& containers) {
	return CombinatorNamespace::Cartesian<
			std::array<Container, Size>,
			CombinatorNamespace::Coalesce<
			        Combination,
					std::array<typename CombinatorNamespace::ElementType<Container>::type, Size>
			>
	>{containers};
}

template<class Combination = nullptr_t, class Container, class... Containers>
auto VariadicCartesian(Container container, Containers... containers) {
	return CombinatorNamespace::VariadicCartesian<
			Container,
			CombinatorNamespace::Coalesce<Combination, Container>, // TODO: think about
			typeof(Container[]),
			Containers...
	>{container, containers...};
}

template<class Combination = nullptr_t, bool ReferenceContainer = false, class Container>
auto Combinations(const Container& elements, const CombinatorNamespace::Position length) {
	return Combinator<Combination, ReferenceContainer, Container>(elements, length);
}
template<class Combination = nullptr_t, bool ReferenceContainer = false, class Container>
auto Permutations(const Container& elements, const CombinatorNamespace::Position length) {
	return Permutator<Combination, ReferenceContainer, Container>(elements, length);
}
template<class Combination = nullptr_t, bool ReferenceContainer = false, class Container>
auto Permutations(const Container& elements) {
	return Permutator<Combination, ReferenceContainer, Container>(elements);
}
template<class Combination = nullptr_t, bool ReferenceContainer = false, class Container>
auto MultiPermutations(const Container& elements, const CombinatorNamespace::Position length) {
	return MultiPermutator<Combination, ReferenceContainer, Container>(elements, length);
}
template<class Combination = nullptr_t, class Containers>
auto CartesianProducts(const Containers& containers) {
	return Cartesian<Combination>(containers);
}
template<class Combination = nullptr_t, class Container1, class Container2, class... Containers>
auto CartesianProducts(Container1 container1, Container2 container2, Containers... containers) {
	return VariadicCartesian<Combination, Container1, Container2, Containers...>(
			container1,
			container2,
			containers...
	);
}
