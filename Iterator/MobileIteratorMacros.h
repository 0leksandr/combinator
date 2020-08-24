#pragma once

// this actually shouldn't matter, how we convert
#define MOBILE_ITERATOR                                                   \
	public:                                                               \
		bool operator!=(const PositionedIterator& other) const override { \
			return ForwardIterator::operator!=(other);                    \
		}                                                                 \
	protected:                                                            \
		Position getIndex() const override {                              \
			return ForwardIterator::getIndex();                           \
		}                                                                 \
		void setIndex(const Position index) override {                    \
			ForwardIterator::setIndex(index);                             \
		}
