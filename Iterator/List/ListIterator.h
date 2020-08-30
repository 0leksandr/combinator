#pragma once

#include "../DereferencedIterator.h"
#include "../../Converter.h"
#include "../../Position.h"
#include "../../Request/FixedSizeRequest.h"

template<class Container, class Combination>
class ListIterator : public DereferencedIterator<Combination> {
	public:
		explicit ListIterator(const FixedSizeRequest<Container>& request) :
				DereferencedIterator<Combination>(
						Converter::initCombination(
								&this->combination,
								request.length,
								request.elements[0]
						),
						request.length
				),
				request(request) {}
		Combination& operator*() const override {
			for (Position c = 0; c < request.length; c++) {
				this->combination[c] = request.elements[this->positions[c]];
			}
			return this->combination;
		}
	protected:
		const FixedSizeRequest<Container>& request;

		[[nodiscard]] Position nrElements() const {
			return request.elements.size();
		}
};
