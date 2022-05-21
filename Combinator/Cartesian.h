#pragma once

#include "../Iterator/Multiset/MultisetFIterator.h"
#include "../Iterator/Multiset/MultisetRAIterator.h"
#include "../Request/ContainerWrapper/ContainerWrapper.h"
#include "../Request/MultisetRequest.h"
#include "FixedSizeCombinator.h"

namespace CombinatorNamespace {
	template<
			class Container,
			class Combination,
			bool ReferenceContainer,
//			class Container = CombinatorNamespace::ElementType
			class ContainerWrapperAlias = ContainerWrapper<Container, ReferenceContainer>,
			typename Element = typeof(ContainerWrapperAlias[]),
			class Request = MultisetRequest<ContainerWrapperAlias>
	>
	class Cartesian : public FixedSizeCombinator<
			Combination,
			Request,
			MultisetFIterator<Combination, Request, Element>,
			MultisetRAIterator<Combination, Request, Element>
	> {
		public:
			explicit Cartesian(const std::vector<Container>& containers) : FixedSizeCombinator<
					Combination,
					Request,
					MultisetFIterator<Combination, Request, Element>,
					MultisetRAIterator<Combination, Request, Element>
			>(Request(ContainerWrapperAlias::wrapContainer(containers))) {}
	};
}
