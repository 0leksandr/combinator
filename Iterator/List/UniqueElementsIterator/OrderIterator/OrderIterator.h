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
		template<typename Float = float>
		static Position nPerM(const Position n, const Position m) {
			Float res(1.);
			for (Position c = 0; c < m; c++) {
				res *= Float(n - c);
				res /= Float(c + 1);
			}
			return (Position)res;
		}
};
