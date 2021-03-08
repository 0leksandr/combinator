#pragma once

#include "../DereferencedIterator.h"
#include "../../Converter.h"
#include "../../Position.h"
#include "../../Request/FixedSizeRequest.h"

namespace CombinatorNamespace {
	template<class Container, class Combination, typename Element = typeof(Container[])>
	class ListIterator : public DereferencedIterator<Combination, FixedSizeRequest<Container>, Element> {
		public:
			explicit ListIterator(const FixedSizeRequest<Container>& request) :
					DereferencedIterator<Combination, FixedSizeRequest<Container>, Element>(request.length),
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
