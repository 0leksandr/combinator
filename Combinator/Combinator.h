#pragma once

#include "../Iterator/List/UniqueElementsIterator/OrderIterator/ComboIterator.h"
#include "../Iterator/List/UniqueElementsIterator/OrderIterator/Walker.h"
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
	class Combinator : public FixedSizedSingleSetCombinator<
			ContainerWrapperAlias,
			Combination,
			Walker<ContainerWrapperAlias, Combination>,
			ComboIterator<ContainerWrapperAlias, Combination>
	> {
		public:
			Combinator(const Container& elements, const Position length) :
					FixedSizedSingleSetCombinator<
							ContainerWrapperAlias,
							Combination,
							Walker<ContainerWrapperAlias, Combination>,
							ComboIterator<ContainerWrapperAlias, Combination>
					>(ContainerWrapperAlias{elements}, length) {}
	};
}
