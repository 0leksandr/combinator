#pragma once

#include "../DereferencedIterator.h"
#include "../../Converter.h"
#include "../../Position.h"
#include "../../Request/FixedRequest.h"

template<class Container, class Combination>
class ListIterator : public DereferencedIterator<Container, Combination> {
	public:
		explicit ListIterator(const FixedRequest<Container>* const request) :
				DereferencedIterator<Container, Combination>(
						request->length,
						Converter<Combination>::initCombination(
								&this->combination,
								request->length,
								request->elements[0]
						)
				),
				request(request) {}
		Combination& operator*() const override {
			for (Position c = 0; c < request->length; c++) {
				this->combination[c] = request->elements[this->positions[c]];
			}
			return this->combination;
		}
	protected:
		const FixedRequest<Container>* const request;

		[[nodiscard]] Position nrElements() const {
			return request->elements.size();
		}
};
