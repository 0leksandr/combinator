#pragma once

#include "../ListIterator.h"
#include "../../SizedIterator.h"
#include "../../../Request/FixedRequest.h"

template<class Container, class Combination>
class MultiChoiceListIterator :
		public ListIterator<Container, Combination>,
		public SizedIterator {
	public:
		explicit MultiChoiceListIterator(const FixedRequest<Container>& request) :
				ListIterator<Container, Combination>(request) {
			for (int c = 0; c < request.length; ++c) this->positions[c] = 0;
		}
		static Position size(const FixedRequest<Container>& request) {
			return pow(request.elements.size(), request.length);
		}
	private:
		static Position pow(const Position a, const Position b) {
			Position res(1);
			for (int c = 0; c < b; ++c) res *= a;
			return res;
		}
};
