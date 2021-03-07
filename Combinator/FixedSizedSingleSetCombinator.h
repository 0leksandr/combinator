#pragma once

#include "../Request/FixedSizeRequest.h"
#include "FixedSizeCombinator.h"

namespace CombinatorNamespace {
	template<class ContainerWrapper, class Combination, class ForwardIterator, class RandomAccessIterator>
	class FixedSizedSingleSetCombinator : public FixedSizeCombinator<
			Combination,
			FixedSizeRequest<ContainerWrapper>,
			ForwardIterator,
			RandomAccessIterator
	> {
		protected:
			template<class Container>
			FixedSizedSingleSetCombinator(const Container& elements, const CombinatorNamespace::Position length) :
					FixedSizeCombinator<
							Combination,
							CombinatorNamespace::FixedSizeRequest<ContainerWrapper>,
							ForwardIterator,
							RandomAccessIterator
					>(CombinatorNamespace::FixedSizeRequest<ContainerWrapper>(elements, length)) {}
	};
}
