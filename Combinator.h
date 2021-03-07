#pragma once

#include <array>
#include "Iterator/Multiset/MultisetFIterator.h"
#include "Iterator/Multiset/MultisetRAIterator.h"
#include "Iterator/List/MultiPermutationIterator/MultiPermutationFIterator.h"
#include "Iterator/List/MultiPermutationIterator/MultiPermutationRAIterator.h"
#include "Iterator/List/UniqueElementsIterator/OrderIterator/ComboIterator.h"
#include "Iterator/List/UniqueElementsIterator/OrderIterator/Walker.h"
#include "Iterator/List/UniqueElementsIterator/PermutationIterator/PermutationIterator.h"
#include "Position.h"
#include "Request/ContainerWrapper/ContainerWrapper.h"
#include "Request/FixedSizeRequest.h"
#include "Request/MultisetRequest.h"

// TODO: Cartesian from variadic list of collections
// TODO: make Permutator::begin return some Walker

namespace CombinatorNamespace {
	template<class Combination, class Request, class ForwardIterator, class RandomAccessIterator>
	class FixedSizeCombinator {
		public:
			Combination& operator[](CombinatorNamespace::Position index) const {
				if (current == nullptr) current = new RandomAccessIterator(request);
				current->operator[](index);
				return current->operator*();
			}
			[[maybe_unused]] ForwardIterator begin() const {
				return ForwardIterator(request);
			}
			[[maybe_unused]] CombinatorNamespace::PositionedIterator end() const {
				return CombinatorNamespace::PositionedIterator(_size);
			}
			CombinatorNamespace::Position size() const {
				return _size;
			}
		protected:
			explicit FixedSizeCombinator(const Request& request) :
					request(request),
					current(nullptr),
					_size(ForwardIterator::size(request)) {}
			~FixedSizeCombinator() {
				if (current != nullptr) delete current;
			}
		private:
			const Request request;
			const CombinatorNamespace::Position _size;
			mutable RandomAccessIterator* current;
	};

	template<class ContainerWrapper, class Combination, class ForwardIterator, class RandomAccessIterator>
	class FixedSizedSingleSetCombinator : public FixedSizeCombinator<
			Combination,
			CombinatorNamespace::FixedSizeRequest<ContainerWrapper>,
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

	template<class Container, class Combination>
	using ConditionalCombination = typename std::conditional<
	        std::is_same_v<Combination, nullptr_t>,
			Container,
			Combination
	>::type;

	template<
	        class Container,
			class Combination,
			bool ReferenceContainer,
			class ContainerWrapperAlias = ContainerWrapper<Container, ReferenceContainer>
	>
	class Combinator : public FixedSizedSingleSetCombinator<
			ContainerWrapperAlias,
			Combination,
			CombinatorNamespace::Walker<ContainerWrapperAlias, Combination>,
			CombinatorNamespace::ComboIterator<ContainerWrapperAlias, Combination>
	> {
		public:
			Combinator(const Container& elements, const CombinatorNamespace::Position length) :
					FixedSizedSingleSetCombinator<
							ContainerWrapperAlias,
							Combination,
							CombinatorNamespace::Walker<ContainerWrapperAlias, Combination>,
							CombinatorNamespace::ComboIterator<ContainerWrapperAlias, Combination>
					>(ContainerWrapperAlias{elements}, length) {}
	};

	template<
	        class Container,
			class Combination,
			bool ReferenceContainer,
			class ContainerWrapperAlias = ContainerWrapper<Container, ReferenceContainer>
	>
	class Permutator : public FixedSizedSingleSetCombinator<
			ContainerWrapperAlias,
			Combination,
			CombinatorNamespace::PermutationIterator<ContainerWrapperAlias, Combination>,
			CombinatorNamespace::PermutationIterator<ContainerWrapperAlias, Combination>
	> {
		public:
			explicit Permutator(const Container& elements):
					FixedSizedSingleSetCombinator<
							ContainerWrapperAlias,
							Combination,
							CombinatorNamespace::PermutationIterator<ContainerWrapperAlias, Combination>,
							CombinatorNamespace::PermutationIterator<ContainerWrapperAlias, Combination>
					>(ContainerWrapper<Container, ReferenceContainer>{elements}, elements.size()) {}
			Permutator(const Container& elements, const CombinatorNamespace::Position length):
					FixedSizedSingleSetCombinator<
							ContainerWrapperAlias,
							Combination,
							CombinatorNamespace::PermutationIterator<ContainerWrapperAlias, Combination>,
							CombinatorNamespace::PermutationIterator<ContainerWrapperAlias, Combination>
					>(ContainerWrapperAlias{elements}, length) {}
	};

	template<
			class Container,
			class Combination,
			bool ReferenceContainer,
			class ContainerWrapperAlias = ContainerWrapper<Container, ReferenceContainer>
	>
	class MultiPermutator : public FixedSizedSingleSetCombinator<
			ContainerWrapperAlias,
			Combination,
			CombinatorNamespace::MultiPermutationFIterator<ContainerWrapperAlias, Combination>,
			CombinatorNamespace::MultiPermutationRAIterator<ContainerWrapperAlias, Combination>
	> {
		public:
			explicit MultiPermutator(const Container& elements):
					FixedSizedSingleSetCombinator<
							ContainerWrapperAlias,
							Combination,
							CombinatorNamespace::MultiPermutationFIterator<ContainerWrapperAlias, Combination>,
							CombinatorNamespace::MultiPermutationRAIterator<ContainerWrapperAlias, Combination>
					>(ContainerWrapperAlias{elements}, elements.size()) {}
			MultiPermutator(const Container& elements, const CombinatorNamespace::Position length):
					FixedSizedSingleSetCombinator<
							ContainerWrapperAlias,
							Combination,
							CombinatorNamespace::MultiPermutationFIterator<ContainerWrapperAlias, Combination>,
							CombinatorNamespace::MultiPermutationRAIterator<ContainerWrapperAlias, Combination>
					>(ContainerWrapperAlias{elements}, length) {}
	};

	template<
			class Container,
			class Combination,
			bool ReferenceContainer,
			class ContainerWrapperAlias = ContainerWrapper<Container, ReferenceContainer>
	>
	class Cartesian : public FixedSizeCombinator<
			Combination,
			CombinatorNamespace::MultisetRequest<ContainerWrapperAlias>,
			CombinatorNamespace::MultisetFIterator<ContainerWrapperAlias, Combination>,
			CombinatorNamespace::MultisetRAIterator<ContainerWrapperAlias, Combination>
	> {
		public:
			explicit Cartesian(const std::vector<Container>& containers) : FixedSizeCombinator<
					Combination,
					CombinatorNamespace::MultisetRequest<ContainerWrapperAlias>,
					CombinatorNamespace::MultisetFIterator<ContainerWrapperAlias, Combination>,
					CombinatorNamespace::MultisetRAIterator<ContainerWrapperAlias, Combination>
			>(
					CombinatorNamespace::MultisetRequest<ContainerWrapperAlias>{
						ContainerWrapperAlias::wrapVector(containers)
					}
			) {}
	};
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
auto MultiPermutator(const Container& elements) {
	return CombinatorNamespace::MultiPermutator<
			Container,
			CombinatorNamespace::ConditionalCombination<Container, Combination>,
			ReferenceContainer
	>{elements};
}

template<class Combination = nullptr_t, bool ReferenceContainer = false, class Container>
auto Cartesian(const std::vector<Container>& containers) {
	return CombinatorNamespace::Cartesian<
			Container,
			CombinatorNamespace::ConditionalCombination<Container, Combination>,
			ReferenceContainer
	>{containers};
}
