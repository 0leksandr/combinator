#pragma once

#include "../UniqueElementsIterator.h"
#include "../../FixedRequest.h"
#include "../../Position.h"

template<class Container, class Combination>
class OrderIterator : public UniqueElementsIterator<Container, Combination> {
	public:
		explicit OrderIterator(const FixedRequest<Container>* const request) :
				UniqueElementsIterator<Container, Combination>(request),
				_size(nPerM(request->elements.size(), request->length)) {}
		void operator++() override {
			increment(this->request->length - 1);
			++this->index;
		}
		void operator--() override {
			decrement(this->request->length - 1);
			--this->index;
		}
		[[nodiscard]] Position size() const override {
			return _size;
		}
	protected:
		template<typename Float = float>
		static Position nPerM(const Position n, const Position m) {
			Float res(1.);
			for (Position c = 0; c < m; c++) {
				res *= Float(n - c);
				res /= Float(c + 1);
			}
			return (Position)res;
		}
	private:
		const Position _size;

		void increment(const Position position) const {
			if (++this->positions[position] > maxPosition(position)) {
				if (position != 0) { // preventing fail on end
					increment(position - 1);
					this->positions[position] = this->positions[position - 1] + 1;
				}
			}
		}
		void decrement(const Position position) const {
			--this->positions[position];
			if (position > 0 && this->positions[position] == this->positions[position - 1]) {
				for (Position c = position; c < this->request->length; c++) {
					this->positions[c] = maxPosition(c);
				}
				decrement(position - 1);
			}
		}
		[[nodiscard]] Position maxPosition(const Position position) const {
			return this->nrElements() + position - this->request->length;
		}
};
