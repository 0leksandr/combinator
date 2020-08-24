#pragma once

#include <array>
#include "Iterator/Compose/ComposeFIterator.h"
#include "Iterator/Compose/ComposeRAIterator.h"
#include "Iterator/List/MultiChoiceIterator/MultiChoiceFIterator.h"
#include "Iterator/List/MultiChoiceIterator/MultiChoiceRAIterator.h"
#include "Iterator/List/UniqueElementsIterator/OrderIterator/ComboIterator.h"
#include "Iterator/List/UniqueElementsIterator/OrderIterator/Walker.h"
#include "Iterator/List/UniqueElementsIterator/ShuffleIterator/ShuffleIterator.h"
#include "Position.h"
#include "Request/ComposeRequest.h"
#include "Request/FixedRequest.h"

// TODO: UnorderedCombinator from variadic list of collections
// TODO: ShuffledCombinator::begin return some Walker
// TODO: remove Container from main template

namespace Combinator {
	template<class Container, class Combination, class Request, class ForwardIterator, class RandomAccessIterator>
	class FixedCombinator {
		public:
			Combination& operator[](Position index) const {
				if (current == nullptr) current = newIterator();
				current->operator[](index);
				return **current;
			}
			ForwardIterator begin() const {
				return ForwardIterator(request);
			}
			PositionedIterator end() const {
				return _end;
			}
			Position size() const {
				return ForwardIterator::size(request);
			}
		protected:
			explicit FixedCombinator(const Request& request) :
					request(request),
					current(nullptr),
					_end(size()) {}
			~FixedCombinator() {
				if (current != nullptr) delete current;
			}
		private:
			const Request request;
			mutable RandomAccessIterator* current;
			const PositionedIterator _end;

			RandomAccessIterator* newIterator() const {
				return new RandomAccessIterator(request);
			}
	};

	template<class Container, class Combination, class ForwardIterator, class RandomAccessIterator>
	class FixedListCombinator : public FixedCombinator<
			Container,
			Combination,
			FixedRequest<Container>,
			ForwardIterator,
			RandomAccessIterator
	> {
		protected:
			FixedListCombinator(const Container& elements, const Position length) : FixedCombinator<
					Container,
					Combination,
					FixedRequest<Container>,
					ForwardIterator,
					RandomAccessIterator
			>(FixedRequest<Container>(elements, length)) {}
	};

	template<class Container, class Combination = Container>
	class OrderedCombinator : public FixedListCombinator<
			Container,
			Combination,
			Walker<Container, Combination>,
			ComboIterator<Container, Combination>
	> {
		public:
			OrderedCombinator(const Container& elements, const Position length) :
					FixedListCombinator<
							Container,
							Combination,
							Walker<Container, Combination>,
							ComboIterator<Container, Combination>
					>(elements, length) {}
	};

	template<class Container, class Combination = Container>
	class ShuffledCombinator : public FixedListCombinator<
			Container,
			Combination,
			ShuffleIterator<Container, Combination>,
			ShuffleIterator<Container, Combination>
	> {
		public:
			ShuffledCombinator(const Container& elements, const Position length):
					FixedListCombinator<
							Container,
							Combination,
							ShuffleIterator<Container, Combination>,
							ShuffleIterator<Container, Combination>
					>(elements, length) {}
	};

	template<class Container, class Combination = Container>
	class MultiChoiceCombinator : public FixedListCombinator<
			Container,
			Combination,
			MultiChoiceFIterator<Container, Combination>,
			MultiChoiceRAIterator<Container, Combination>
	> {
		public:
			MultiChoiceCombinator(const Container& elements, const Position length):
					FixedListCombinator<
							Container,
							Combination,
							MultiChoiceFIterator<Container, Combination>,
							MultiChoiceRAIterator<Container, Combination>
					>(elements, length) {}
	};

	template<class Container, Position NrContainers, class Combination>
	class ComposeCombinator : public FixedCombinator<
			Container,
			Combination,
			ComposeRequest<Container, NrContainers>,
			ComposeFIterator<Container, NrContainers, Combination>,
			ComposeRAIterator<Container, NrContainers, Combination>
	> {
		public:
			explicit ComposeCombinator(const std::array<Container, NrContainers>& containers) : FixedCombinator<
					Container,
					Combination,
					ComposeRequest<Container, NrContainers>,
					ComposeFIterator<Container, NrContainers, Combination>,
					ComposeRAIterator<Container, NrContainers, Combination>
			>(ComposeRequest<Container, NrContainers>(containers)) {}
	};
}
