#pragma once

#include "../DereferencedIterator.h"
#include "../../Converter.h"
#include "../../Position.h"
#include "../../Request/FixedRequest.h"

template<class Container, class Combination>
class ListIterator : public DereferencedIterator<Combination> {
	public:
		explicit ListIterator(const FixedRequest<Container>* const request) :
				request(request),
				positions(new Position[request->length]),
				combination(Converter<Combination>::initCombination(
						&combination,
						request->length,
						request->elements[0]
				)) {}
		~ListIterator() {
			delete[] positions;
			Converter<Container>::deleteCombination(combination);
		}
		Combination& operator*() const override {
			for (Position c = 0; c < request->length; c++) {
				combination[c] = request->elements[positions[c]];
			}
			return combination;
		}
	protected:
		const FixedRequest<Container>* const request;
		Position* const positions;
		[[nodiscard]] Position nrElements() const {
			return this->request->elements.size();
		}
	private:
		mutable Combination combination;
};
