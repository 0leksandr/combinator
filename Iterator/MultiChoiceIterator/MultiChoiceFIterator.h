#pragma once

#include "../ForwardIterator.h"
#include "../../Position.h"
#include "../../Request/FixedRequest.h"

template<class Container, class Combination>
class MultiChoiceFIterator : public ForwardIterator<Container, Combination> {
	public:
		explicit MultiChoiceFIterator(const FixedRequest<Container>* const request) :
				ForwardIterator<Container, Combination>(request),
				_size(pow(request->elements.size(), request->length)) {
			for (int c = 0; c < request->length; ++c) this->positions[c] = 0;
		}
		void operator++() override {
			increment(0);
			++this->index;
		}
		[[nodiscard]] Position size() const override {
			return _size;
		}
	private:
		const Position _size; // TODO: MultiChoiceSizedIterator?

		static Position pow(const Position a, const Position b) {
			Position res(1);
			for (int c = 0; c < b; ++c) res *= a;
			return res;
		}
		void increment(const Position position) {
			if (position < this->request->length) {
				if (++(this->positions[position]) == this->nrElements()) {
					this->positions[position] = 0;
					increment(position + 1);
				}
			}
		}
};
