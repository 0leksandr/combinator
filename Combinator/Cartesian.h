#pragma once

#include "../Iterator/Multiset/MultisetFIterator.h"
#include "../Iterator/Multiset/MultisetRAIterator.h"
#include "../Request/ContainerWrapper/ContainerWrapper.h"
#include "FixedSizeCombinator.h"

namespace CombinatorNamespace {
	template<
			class Container,
			class Combination,
			bool ReferenceContainer,
			class ContainerWrapperAlias = ContainerWrapper<Container, ReferenceContainer>
	>
	class Cartesian : public FixedSizeCombinator<
			Combination,
			MultisetRequest<ContainerWrapperAlias>,
			MultisetFIterator<ContainerWrapperAlias, Combination>,
			MultisetRAIterator<ContainerWrapperAlias, Combination>
	> {
		public:
			explicit Cartesian(const std::vector<Container>& containers) : FixedSizeCombinator<
					Combination,
					MultisetRequest<ContainerWrapperAlias>,
					MultisetFIterator<ContainerWrapperAlias, Combination>,
					MultisetRAIterator<ContainerWrapperAlias, Combination>
			>(
					MultisetRequest<ContainerWrapperAlias>{
							ContainerWrapperAlias::wrapVector(containers)
					}
			) {}
	};
}
