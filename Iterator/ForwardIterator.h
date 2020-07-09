#pragma once

#include "IndexedIterator.h"
#include "../Converter.h"
#include "../Position.h"
#include "../Request/FixedRequest.h"

template<class Container, class Combination>
class ForwardIterator : public IndexedIterator {
	public:
		explicit ForwardIterator(const FixedRequest<Container>* const request) :
				IndexedIterator(0),
				request(request),
				positions(new Position[request->length]),
				combination(Converter<Combination>::initCombination(
						&combination,
						request->length,
						request->elements[0]
				)) {}
		~ForwardIterator() {
			delete[] positions;
			Converter<Container>::deleteCombination(combination);
		}

		[[nodiscard]] virtual Position size() const = 0;

		virtual void operator++() = 0;
		Combination& operator*() {
			for (Position c = 0; c < request->length; c++) {
				combination[c] = request->elements[positions[c]];
			}
			return combination;
		}
	protected:
		const FixedRequest<Container>* const request;
		Position* const positions;

		[[nodiscard]] Position nrElements() const {
			return request->elements.size();
		}
	private:
		Combination combination;
};
