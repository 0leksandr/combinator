#pragma once

#include "../ElementType.h"
#include "../Iterator/Multiset/MultisetFIterator.h"
#include "../Iterator/Multiset/MultisetRAIterator.h"
#include "../Request/ContainerWrapper/ContainerWrapper.h"
#include "../Request/MultisetRequest.h"
#include "FixedSizeCombinator.h"

namespace CombinatorNamespace {
	template<
			class Containers,
			class Combination,
//			bool ReferenceContainer, // TODO: use
			class Container = typename ElementType<Containers>::type,
			typename Element = typeof(Container[]),
			class Request = MultisetRequest<Containers>
	>
	class Cartesian : public FixedSizeCombinator<
			Combination,
			Request,
			MultisetFIterator<Combination, Request, Element>,
			MultisetRAIterator<Combination, Request, Element>
	> {
		public:
			explicit Cartesian(const Containers& containers) : FixedSizeCombinator<
					Combination,
					Request,
					MultisetFIterator<Combination, Request, Element>,
					MultisetRAIterator<Combination, Request, Element>
			>(Request(containers)) {}
	};
}
