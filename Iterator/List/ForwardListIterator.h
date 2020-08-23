#pragma once

#include "ListIterator.h"
#include "../ForwardIterator.h"
#include "../../Converter.h"
#include "../../Position.h"
#include "../../Request/FixedRequest.h"

template<class Container, class Combination>
class ForwardListIterator : // TODO: remove
		public ForwardIterator,
		public ListIterator<Container, Combination> {
	public:
		explicit ForwardListIterator(const FixedRequest<Container>* const request) :
				ForwardIterator(),
				ListIterator<Container, Combination>(request) {}
//	protected:
//		[[nodiscard]] Position nrElements() const {
//			return this->request->elements.size();
//		}
};
