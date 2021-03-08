#pragma once

#include "Iterator.h"
#include "../Combination/CombinationWrapper.h"
#include "../Converter.h"
#include "../Position.h"

namespace CombinatorNamespace {
	template<typename Combination, class Request, typename Element>
	class DereferencedIterator : public Iterator {
		public:
			DereferencedIterator(const Position combinationSize) :
					positions(new Position[combinationSize]),
					combinationWrapper(Converter<Combination, Request>::template createCombinationWrapper<Element>(
							combinationSize
					)) {}
			~DereferencedIterator() {
				delete combinationWrapper;
				delete[] positions;
			}
			virtual Combination& operator*() const = 0;
		protected:
			Position* const positions;
			mutable CombinationWrapper<Combination, Request>* combinationWrapper;
	};
}
