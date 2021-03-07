#pragma once

#include "../Iterator/List/UniqueElementsIterator/PermutationIterator/PermutationIterator.h"
#include "../Position.h"
#include "../Request/ContainerWrapper/ContainerWrapper.h"
#include "FixedSizedSingleSetCombinator.h"

namespace CombinatorNamespace {
	template<
			class Container,
			class Combination,
			bool ReferenceContainer,
			class ContainerWrapperAlias = ContainerWrapper<Container, ReferenceContainer>
	>
	class Permutator : public FixedSizedSingleSetCombinator<
			ContainerWrapperAlias,
			Combination,
			PermutationIterator<ContainerWrapperAlias, Combination>,
			PermutationIterator<ContainerWrapperAlias, Combination>
	> {
		public:
			explicit Permutator(const Container& elements):
					FixedSizedSingleSetCombinator<
							ContainerWrapperAlias,
							Combination,
							PermutationIterator<ContainerWrapperAlias, Combination>,
							PermutationIterator<ContainerWrapperAlias, Combination>
					>(ContainerWrapper<Container, ReferenceContainer>{elements}, elements.size()) {}
			Permutator(const Container& elements, const Position length):
					FixedSizedSingleSetCombinator<
							ContainerWrapperAlias,
							Combination,
							PermutationIterator<ContainerWrapperAlias, Combination>,
							PermutationIterator<ContainerWrapperAlias, Combination>
					>(ContainerWrapperAlias{elements}, length) {}
	};
}
