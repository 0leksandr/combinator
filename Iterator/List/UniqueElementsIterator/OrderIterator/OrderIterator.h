#pragma once

#include "../UniqueElementsIterator.h"
#include "../../../../Position.h"
#include "../../../../Request/FixedRequest.h"

template<class Container, class Combination>
class OrderIterator : public UniqueElementsIterator<Container, Combination> {
	public:
		explicit OrderIterator(const FixedRequest<Container>* const request) :
				UniqueElementsIterator<Container, Combination>(request),
				_size(nPerM(request->elements.size(), request->length)) {}
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
};
