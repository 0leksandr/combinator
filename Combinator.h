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
// TODO: remove Container from main template

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

template<class Container, class Combination = Container, bool ReferenceContainer = false>
class Combinator : public FixedSizedSingleSetCombinator<
		ContainerWrapper<Container, ReferenceContainer>,
		Combination,
		CombinatorNamespace::Walker<ContainerWrapper<Container, ReferenceContainer>, Combination>,
		CombinatorNamespace::ComboIterator<ContainerWrapper<Container, ReferenceContainer>, Combination>
> {
	public:
		Combinator(const Container& elements, const CombinatorNamespace::Position length) :
				FixedSizedSingleSetCombinator<
						ContainerWrapperAlias,
						Combination,
						CombinatorNamespace::Walker<ContainerWrapperAlias, Combination>,
						CombinatorNamespace::ComboIterator<ContainerWrapperAlias, Combination>
				>(ContainerWrapperAlias{elements}, length) {}
	private:
		typedef ContainerWrapper<Container, ReferenceContainer> ContainerWrapperAlias;
};

template<class Container, class Combination = Container, bool ReferenceContainer = false>
class Permutator : public FixedSizedSingleSetCombinator<
		ContainerWrapper<Container, ReferenceContainer>,
		Combination,
		CombinatorNamespace::PermutationIterator<ContainerWrapper<Container, ReferenceContainer>, Combination>,
		CombinatorNamespace::PermutationIterator<ContainerWrapper<Container, ReferenceContainer>, Combination>
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
	private:
		typedef ContainerWrapper<Container, ReferenceContainer> ContainerWrapperAlias;
};

template<class Container, class Combination = Container, bool ReferenceContainer = false>
class MultiPermutator : public FixedSizedSingleSetCombinator<
		ContainerWrapper<Container, ReferenceContainer>,
		Combination,
		CombinatorNamespace::MultiPermutationFIterator<ContainerWrapper<Container, ReferenceContainer>, Combination>,
		CombinatorNamespace::MultiPermutationRAIterator<ContainerWrapper<Container, ReferenceContainer>, Combination>
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
	private:
		typedef ContainerWrapper<Container, ReferenceContainer> ContainerWrapperAlias;
};

template<class Container, class Combination = Container, bool ReferenceContainer = false>
class Cartesian : public FixedSizeCombinator<
		Combination,
		CombinatorNamespace::MultisetRequest<ContainerWrapper<Container, ReferenceContainer>>,
		CombinatorNamespace::MultisetFIterator<ContainerWrapper<Container, ReferenceContainer>, Combination>,
		CombinatorNamespace::MultisetRAIterator<ContainerWrapper<Container, ReferenceContainer>, Combination>
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
	private:
		typedef ContainerWrapper<Container, ReferenceContainer> ContainerWrapperAlias;
};
