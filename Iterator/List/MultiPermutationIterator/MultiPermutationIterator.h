#pragma once

#include "../ListIterator.h"
#include "../../SizedIterator.h"
#include "../../../Request/FixedSizeRequest.h"

template<class Container, class Combination>
class MultiPermutationIterator :
		public ListIterator<Container, Combination>,
		public SizedIterator {
	public:
		explicit MultiPermutationIterator(const FixedSizeRequest<Container>& request) :
				ListIterator<Container, Combination>(request) {
			for (int c = 0; c < request.length; ++c) this->positions[c] = 0;
		}
		static Position size(const FixedSizeRequest<Container>& request) {
			return pow(request.elements.size(), request.length);
		}
	private:
		static Position pow(const Position a, const Position b) {
			Position res(1);
			for (int c = 0; c < b; ++c) res *= a;
			return res;
		}
};
