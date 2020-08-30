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
// TODO: optional permutation size

namespace Combinator {
	template<class Combination, class Request, class ForwardIterator, class RandomAccessIterator>
	class FixedSizeCombinator {
		public:
			Combination& operator[](Position index) const {
				if (current == nullptr) current = new RandomAccessIterator(request);
				current->operator[](index);
				return **current;
			}
			[[maybe_unused]] ForwardIterator begin() const {
				return ForwardIterator(request);
			}
			[[maybe_unused]] PositionedIterator end() const {
				return PositionedIterator(_size);
			}
			Position size() const {
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
			const Position _size;
			mutable RandomAccessIterator* current;
	};

	template<class Container, class Combination, class ForwardIterator, class RandomAccessIterator>
	class FixedSizedSingleSetCombinator : public FixedSizeCombinator<
			Combination,
			FixedSizeRequest<Container>,
			ForwardIterator,
			RandomAccessIterator
	> {
		protected:
			FixedSizedSingleSetCombinator(const Container& elements, const Position length) : FixedSizeCombinator<
					Combination,
					FixedSizeRequest<Container>,
					ForwardIterator,
					RandomAccessIterator
			>(FixedSizeRequest<Container>(elements, length)) {}
	};

	template<class Container, class Combination = Container>
	class OrderedCombinator : public FixedSizedSingleSetCombinator<
			Container,
			Combination,
			Walker<Container, Combination>,
			ComboIterator<Container, Combination>
	> {
		public:
			OrderedCombinator(const Container& elements, const Position length) :
					FixedSizedSingleSetCombinator<
							Container,
							Combination,
							Walker<Container, Combination>,
							ComboIterator<Container, Combination>
					>(elements, length) {}
	};

	template<class Container, class Combination = Container>
	class Permutator : public FixedSizedSingleSetCombinator<
			Container,
			Combination,
			PermutationIterator<Container, Combination>,
			PermutationIterator<Container, Combination>
	> {
		public:
			Permutator(const Container& elements, const Position length):
					FixedSizedSingleSetCombinator<
							Container,
							Combination,
							PermutationIterator<Container, Combination>,
							PermutationIterator<Container, Combination>
					>(elements, length) {}
	};

	template<class Container, class Combination = Container>
	class MultiPermutator : public FixedSizedSingleSetCombinator<
			Container,
			Combination,
			MultiPermutationFIterator<Container, Combination>,
			MultiPermutationRAIterator<Container, Combination>
	> {
		public:
			MultiPermutator(const Container& elements, const Position length):
					FixedSizedSingleSetCombinator<
							Container,
							Combination,
							MultiPermutationFIterator<Container, Combination>,
							MultiPermutationRAIterator<Container, Combination>
					>(elements, length) {}
	};

	template<class Combination, class Container>
	class Cartesian : public FixedSizeCombinator<
			Combination,
			MultisetRequest<Container>,
			MultisetFIterator<Container, Combination>,
			MultisetRAIterator<Container, Combination>
	> {
		public:
//			explicit Cartesian(const Containers&&... containers) : FixedSizeCombinator<
//					Combination,
//					MultisetRequest<Containers...>,
//					MultisetFIterator<Combination, Containers...>,
//					MultisetRAIterator<Combination, Containers...>
//			>(MultisetRequest<Containers...>(containers...)) {}
			explicit Cartesian(const std::vector<Container>& containers) : FixedSizeCombinator<
			        Combination,
			        MultisetRequest<Container>,
					MultisetFIterator<Container, Combination>,
					MultisetRAIterator<Container, Combination>
			>(containers) {}
	};
}
