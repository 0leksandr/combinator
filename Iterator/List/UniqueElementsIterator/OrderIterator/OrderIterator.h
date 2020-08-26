#pragma once

#include "../UniqueElementsIterator.h"
#include "../../../../Position.h"
#include "../../../../Request/FixedRequest.h"

template<class Container, class Combination>
class OrderIterator : public UniqueElementsIterator<Container, Combination> {
	public:
		explicit OrderIterator(const FixedRequest<Container>& request) :
				UniqueElementsIterator<Container, Combination>(request) {}
		static Position size(const FixedRequest<Container>& request) {
			return nPerM(request.elements.size(), request.length);
		}
	protected:
		static Position nPerM(const Position n, const Position m) {
			Position res(1);
			for (Position c = 0; c < m; c++) {
				res *= n - c;
				res /= c + 1;
			}
			return res;
		}
};
