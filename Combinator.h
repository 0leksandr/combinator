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

template<class Container, class Combination, class ForwardIterator, class RandomAccessIterator>
class FixedSizedSingleSetCombinator : public FixedSizeCombinator<
		Combination,
		CombinatorNamespace::FixedSizeRequest<Container>,
		ForwardIterator,
		RandomAccessIterator
> {
	protected:
		FixedSizedSingleSetCombinator(const Container& elements,const CombinatorNamespace::Position length) :
				FixedSizeCombinator<
						Combination,
						CombinatorNamespace::FixedSizeRequest<Container>,
						ForwardIterator,
						RandomAccessIterator
				>(CombinatorNamespace::FixedSizeRequest<Container>(elements, length)) {}
};

template<class Container, class Combination = Container>
class Combinator : public FixedSizedSingleSetCombinator<
		Container,
		Combination,
		CombinatorNamespace::Walker<Container, Combination>,
		CombinatorNamespace::ComboIterator<Container, Combination>
> {
	public:
		Combinator(const Container& elements, const CombinatorNamespace::Position length) :
				FixedSizedSingleSetCombinator<
						Container,
						Combination,
						CombinatorNamespace::Walker<Container, Combination>,
						CombinatorNamespace::ComboIterator<Container, Combination>
				>(elements, length) {}
};

template<class Container, class Combination = Container>
class Permutator : public FixedSizedSingleSetCombinator<
		Container,
		Combination,
		CombinatorNamespace::PermutationIterator<Container, Combination>,
		CombinatorNamespace::PermutationIterator<Container, Combination>
> {
	public:
		explicit Permutator(const Container& elements):
				FixedSizedSingleSetCombinator<
						Container,
						Combination,
						CombinatorNamespace::PermutationIterator<Container, Combination>,
						CombinatorNamespace::PermutationIterator<Container, Combination>
				>(elements, elements.size()) {}
		Permutator(const Container& elements, const CombinatorNamespace::Position length):
				FixedSizedSingleSetCombinator<
						Container,
						Combination,
						CombinatorNamespace::PermutationIterator<Container, Combination>,
						CombinatorNamespace::PermutationIterator<Container, Combination>
				>(elements, length) {}
};

template<class Container, class Combination = Container>
class MultiPermutator : public FixedSizedSingleSetCombinator<
		Container,
		Combination,
		CombinatorNamespace::MultiPermutationFIterator<Container, Combination>,
		CombinatorNamespace::MultiPermutationRAIterator<Container, Combination>
> {
	public:
		explicit MultiPermutator(const Container& elements):
				FixedSizedSingleSetCombinator<
						Container,
						Combination,
						CombinatorNamespace::MultiPermutationFIterator<Container, Combination>,
						CombinatorNamespace::MultiPermutationRAIterator<Container, Combination>
				>(elements, elements.size()) {}
		MultiPermutator(const Container& elements, const CombinatorNamespace::Position length):
				FixedSizedSingleSetCombinator<
						Container,
						Combination,
						CombinatorNamespace::MultiPermutationFIterator<Container, Combination>,
						CombinatorNamespace::MultiPermutationRAIterator<Container, Combination>
				>(elements, length) {}
};

template<class Container, class Combination = Container>
class Cartesian : public FixedSizeCombinator<
		Combination,
		CombinatorNamespace::MultisetRequest<Container>,
		CombinatorNamespace::MultisetFIterator<Container, Combination>,
		CombinatorNamespace::MultisetRAIterator<Container, Combination>
> {
	public:
		// template<class... Containers>
		// explicit Cartesian(const Container& container, const Containers&&... containers) : FixedSizeCombinator<
		// 		Combination,
		// 		MultisetRequest<Containers...>,
		// 		MultisetFIterator<Combination, Containers...>,
		// 		MultisetRAIterator<Combination, Containers...>
		// >(MultisetRequest<Containers...>(std::forward<Containers>(containers)...)) {}
		explicit Cartesian(const std::vector<Container>& containers) : FixedSizeCombinator<
				Combination,
				CombinatorNamespace::MultisetRequest<Container>,
				CombinatorNamespace::MultisetFIterator<Container, Combination>,
				CombinatorNamespace::MultisetRAIterator<Container, Combination>
		>(CombinatorNamespace::MultisetRequest<Container>{containers}) {}
};
