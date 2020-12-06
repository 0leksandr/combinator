#pragma once

#include "../DereferencedIterator.h"
#include "../../Converter.h"
#include "../../Position.h"
#include "../../Request/FixedSizeRequest.h"

namespace CombinatorNamespace {
	template<class Container, class Combination>
	class ListIterator : public DereferencedIterator<Combination, FixedSizeRequest<Container>> {
		public:
			explicit ListIterator(const FixedSizeRequest<Container>& request) :
					DereferencedIterator<Combination, FixedSizeRequest<Container>>(
							request.length,
							request.elements[0]
					),
					request(request) {}
			Combination& operator*() const override {
				return this->combinationWrapper->get(request, this->positions);
			}
		protected:
			const FixedSizeRequest<Container>& request;

			[[nodiscard]] Position nrElements() const {
				return request.elements.size();
			}
	};
}
