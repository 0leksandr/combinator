#pragma once

#include "../Iterator/Movement/PositionedIterator.h"
#include "../Position.h"

namespace CombinatorNamespace {
	template<class Combination, class Request, class ForwardIterator, class RandomAccessIterator>
	class FixedSizeCombinator {
		public:
			Combination& operator[](Position index) const {
				if (current == nullptr) current = new RandomAccessIterator(request);
				current->operator[](index);
				return current->operator*();
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
}