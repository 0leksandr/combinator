#pragma once

#include "../Iterator/List/MultiPermutationIterator/MultiPermutationFIterator.h"
#include "../Iterator/List/MultiPermutationIterator/MultiPermutationRAIterator.h"
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
	class MultiPermutator : public FixedSizedSingleSetCombinator<
			ContainerWrapperAlias,
			Combination,
			MultiPermutationFIterator<ContainerWrapperAlias, Combination>,
			MultiPermutationRAIterator<ContainerWrapperAlias, Combination>
	> {
		public:
			explicit MultiPermutator(const Container& elements):
					FixedSizedSingleSetCombinator<
							ContainerWrapperAlias,
							Combination,
							MultiPermutationFIterator<ContainerWrapperAlias, Combination>,
							MultiPermutationRAIterator<ContainerWrapperAlias, Combination>
					>(ContainerWrapperAlias{elements}, elements.size()) {}
			MultiPermutator(const Container& elements, const Position length):
					FixedSizedSingleSetCombinator<
							ContainerWrapperAlias,
							Combination,
							MultiPermutationFIterator<ContainerWrapperAlias, Combination>,
							MultiPermutationRAIterator<ContainerWrapperAlias, Combination>
					>(ContainerWrapperAlias{elements}, length) {}
	};
}
