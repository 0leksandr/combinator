#pragma once

#include "../ListIterator.h"
#include "../../IndexedIterator.h"
#include "../../SizedIterator.h"
#include "../../../Request/FixedRequest.h"

template<class Container, class Combination>
class MultiChoiceListIterator :
		public ListIterator<Container, Combination>,
		public SizedIterator,
		public IndexedIterator {
	public:
		explicit MultiChoiceListIterator(const FixedRequest<Container>* const request) :
				ListIterator<Container, Combination>(request),
				SizedIterator(),
				IndexedIterator(0),
				_size(pow(request->elements.size(), request->length)) {
			for (int c = 0; c < request->length; ++c) this->positions[c] = 0;
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
