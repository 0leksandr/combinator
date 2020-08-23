#pragma once

#include "../UniqueElementsIterator.h"
#include "../../BackwardsIterator.h"
#include "../../ForwardIterator.h"
#include "../../IndexedIterator.h"
#include "../../RandomAccessIterator.h"
#include "../../SizedIterator.h"
#include "../../../Position.h"
#include "../../../Request/FixedRequest.h"

template<class Container, class Combination>
class ShuffleIterator :
		public UniqueElementsIterator<Container, Combination>,
		public SizedIterator,
		public IndexedIterator,
		public ForwardIterator,
		public BackwardsIterator,
		public RandomAccessIterator {
	public:
		explicit ShuffleIterator(const FixedRequest<Container>* const request) :
				UniqueElementsIterator<Container, Combination>(request),
				SizedIterator(),
				IndexedIterator(0),
				ForwardIterator(),
				BackwardsIterator(),
				RandomAccessIterator(),
				_size(nPerM(request->elements.size(), request->length)) {}
		void go(Position index) override {
			this->index = index;
			Position nrElements(this->nrElements());
			for (Position c = 0; c < this->request->length; c++) {
				insertUnique(c, index % nrElements);
				index /= nrElements--;
			}
		}
		void operator++() override {
			go(this->index + 1);
		}
		void operator--() override {
			go(this->index - 1);
		}
		[[nodiscard]] Position size() const override {
			return _size;
		}
	private:
		const Position _size;

		void insertUnique(
				const Position position,
				Position value
		) {
			auto prevValue((Position)-1); // TODO: 0
			do {
				Position add(0); // TODO: move up?
				for (Position c = 0; c < position; c++) {
					if (
							(prevValue + 1 < this->positions[c] + 1) &&
							(this->positions[c] <= value)
							) {
						++add;
					}
				}
				prevValue = value;
				value += add;
			} while (prevValue != value);
			this->positions[position] = value;
		}
		static Position nPerM(const Position n, const Position m) {
			return nPerM0(n, m);
		}
		static Position nPerM0(const Position n, const Position m) {
			if (m > 1) return n * nPerM0(n - 1, m - 1);
			return n;
		}
		static Position nPerM1(Position n, Position m) {
			Position res(n);
			while (m --> 1) res *= --n;
			return res;
		}
		static Position nPerM2(Position n, Position m) {
			Position res(n);
			while (--m > 0) res *= --n;
			return res;
		}
};
