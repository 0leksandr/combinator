#pragma once

#include <vector>
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
			explicit operator std::vector<Combination>() const {
				std::vector<Combination> v;
				v.reserve(this->size());
				for (const auto combination : *this) {
					v.push_back(combination);
				}
				return v;
			}
		protected:
			explicit FixedSizeCombinator(const Request& request) :
					request(request),
					current(nullptr),
					_size(ForwardIterator::size(request)) {}
			~FixedSizeCombinator() {
				delete current;
			}
		private:
			const Request request;
			const Position _size;
			mutable RandomAccessIterator* current;
	};
}