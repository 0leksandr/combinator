#pragma once

#include "Iterator/List/MultiChoiceIterator/MultiChoiceFIterator.h"
#include "Iterator/List/MultiChoiceIterator/MultiChoiceRAIterator.h"
#include "Iterator/List/UniqueElementsIterator/OrderIterator/ComboIterator.h"
#include "Iterator/List/UniqueElementsIterator/OrderIterator/Walker.h"
#include "Iterator/List/UniqueElementsIterator/ShuffleIterator/ShuffleIterator.h"
#include "Position.h"

// TODO: UnorderedCombinator from variadic list of collections
// TODO: ShuffledCombinator::begin return some Walker
// TODO: remove Container from main template

namespace Combinator {
	template<class Container, class Combination, class ForwardIterator, class RandomAccessIterator>
	class FixedListCombinator {
		public:
			Combination& operator[](Position index) const {
				current->operator[](index);
				return **current;
			}
			ForwardIterator begin() const {
				return ForwardIterator(&request);
			}
			PositionedIterator end() const {
				return _end;
			}
			Position size() const {
				return current->size();
			}
		protected:
			FixedListCombinator(const Container& elements, const Position length) :
					request(elements, length),
					current(newIterator()),
					_end(size()) {}
			FixedListCombinator(
					const FixedListCombinator<
							Container,
							Combination,
							ForwardIterator,
							RandomAccessIterator
					>& other
			) :
					request(other.request),
					current(newIterator()),
					_end(other._end) {}
			~FixedListCombinator() {
				delete current;
			}
		private:
			const FixedRequest<Container> request;
			mutable RandomAccessIterator* current;
			const PositionedIterator _end;

			RandomAccessIterator* newIterator() const {
				return new RandomAccessIterator(&request);
			}
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

//	template<class Container, Position NrContainers, class Combination>
//	class ComposeCombinator : public FixedListCombinator<
//			Container,
//			Combination
//	> {
//	};
}
