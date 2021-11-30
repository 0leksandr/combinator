#pragma once

#include "../Iterator/Multiset/MultisetFIterator.h"
#include "../Iterator/Multiset/MultisetRAIterator.h"
#include "../Request/ContainerWrapper/ContainerWrapper.h"
#include "../Request/VariadicMultisetRequest.h"
#include "FixedSizeCombinator.h"

namespace CombinatorNamespace {
	template<
			class Container, // TODO: bool ReferenceContainer
			class Combination,
			typename Element = typeof(Container[]),
			class... Containers
	>
	class VariadicCartesian : public FixedSizeCombinator<
			Combination,
			VariadicMultisetRequest<Container, Containers...>,
			MultisetFIterator<Combination, VariadicMultisetRequest<Container, Containers...>, Element>,
			MultisetRAIterator<Combination, VariadicMultisetRequest<Container, Containers...>, Element>
	> {
		public:
			explicit VariadicCartesian(
					Container container, // TODO: const &
					Containers... containers // TODO: const &
			) : FixedSizeCombinator<
					Combination,
					VariadicMultisetRequest<Container, Containers...>,
					MultisetFIterator<Combination, VariadicMultisetRequest<Container, Containers...>, Element>,
					MultisetRAIterator<Combination, VariadicMultisetRequest<Container, Containers...>, Element>
			>(VariadicMultisetRequest<Container, Containers...>(container, containers...)) {}
	};
}
