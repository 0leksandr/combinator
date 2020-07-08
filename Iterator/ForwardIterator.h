#pragma once

#include "IndexedIterator.h"
#include "../Converter.h"
#include "../FixedRequest.h"
#include "../Position.h"

template<class Container, class Combination>
class ForwardIterator : public IndexedIterator {
	public:
		explicit ForwardIterator(const FixedRequest<Container>* const request) :
				IndexedIterator(0),
				request(request),
				positions(new Position[request->length]),
				converter(request),
				combination(converter.construct(&combination)) {}
		~ForwardIterator() {
			delete[] positions;
			converter.destruct(combination);
		}

		[[nodiscard]] virtual Position size() const = 0;

		virtual void operator++() = 0;
		Combination& operator*() {
			converter.prepare(combination);
			for (Position c = 0; c < request->length; c++)
				combination[c] = converter.getElement(
						&combination,
						request->elements[positions[c]]
				);
			return combination;
		}
	protected:
		const FixedRequest<Container>* const request;
		Position* const positions;

		[[nodiscard]] Position nrElements() const {
			return request->elements.size();
		}
	private:
		const Converter<Container> converter;
		Combination combination;
};
