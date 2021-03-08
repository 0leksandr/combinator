#pragma once

#include <vector>
#include "Combinator/Cartesian.h"
#include "Combinator/Combinator.h"
#include "Combinator/FixedSizeCombinator.h"
#include "Combinator/FixedSizedSingleSetCombinator.h"
#include "Combinator/MultiPermutator.h"
#include "Combinator/Permutator.h"
#include "Position.h"

// TODO: Cartesian from variadic list of collections?
// TODO: make Permutator::begin return some Walker
// TODO: static combination size => static CombinationWrapper

namespace CombinatorNamespace {
	template<class Container, class Combination>
	using ConditionalCombination = typename std::conditional<
	        std::is_same_v<Combination, nullptr_t>,
			Container,
			Combination
	>::type;
}

template<class Combination = nullptr_t, bool ReferenceContainer = false, class Container>
auto Combinator(const Container& elements, const CombinatorNamespace::Position length) {
	return CombinatorNamespace::Combinator<
			Container,
			CombinatorNamespace::ConditionalCombination<Container, Combination>,
			ReferenceContainer
	>{elements, length};
}

template<class Combination = nullptr_t, bool ReferenceContainer = false, class Container>
auto Permutator(const Container& elements, const CombinatorNamespace::Position length) {
	return CombinatorNamespace::Permutator<
			Container,
			CombinatorNamespace::ConditionalCombination<Container, Combination>,
			ReferenceContainer
	>{elements, length};
}
template<class Combination = nullptr_t, bool ReferenceContainer = false, class Container>
auto Permutator(const Container& elements) {
	return CombinatorNamespace::Permutator<
			Container,
			CombinatorNamespace::ConditionalCombination<Container, Combination>,
			ReferenceContainer
	>{elements};
}

template<class Combination = nullptr_t, bool ReferenceContainer = false, class Container>
auto MultiPermutator(const Container& elements, const CombinatorNamespace::Position length) {
	return CombinatorNamespace::MultiPermutator<
			Container,
			CombinatorNamespace::ConditionalCombination<Container, Combination>,
			ReferenceContainer
	>{elements, length};
}

template<class Combination = nullptr_t, bool ReferenceContainer = false, class Container>
auto Cartesian(const std::vector<Container>& containers) {
	return CombinatorNamespace::Cartesian<
			Container,
			CombinatorNamespace::ConditionalCombination<Container, Combination>,
			ReferenceContainer
	>{containers};
}
