#pragma once

#include "../RandomAccessIterator.h"
#include "../../FixedRequest.h"
#include "../../Position.h"

template<class Container, class Combination>
class MultiChoiceRAIterator : public RandomAccessIterator<Container, Combination> {
	public:
		explicit MultiChoiceRAIterator(const FixedRequest<Container>* const request) :
				RandomAccessIterator<Container, Combination>(request),
				_size(pow(request->elements.size(), request->length)) {
			for (int c = 0; c < request->length; ++c) this->positions[c] = 0;
		}
		void go(Position index) override {
			this->index = index;
			const Position nrElements(this->nrElements());
			for (Position c = 0; c < this->request->length; c++) {
				this->positions[c] = index % nrElements;
				index /= nrElements;
			}
		}
		// TODO: are these necessary?
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
		const Position _size; // TODO: remove

		static Position pow(const Position a, const Position b) {
			Position res(1);
			for (int c = 0; c < b; ++c) res *= a;
			return res;
		}
};
