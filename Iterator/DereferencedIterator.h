#pragma once

#include "Iterator.h"
#include "../Combination/CombinationWrapper.h"
#include "../Converter.h"
#include "../Position.h"

namespace CombinatorNamespace {
	template<typename Combination, class Request>
	class DereferencedIterator : public Iterator {
		public:
			template<typename Element>
			DereferencedIterator(
					const Position combinationSize,
					const Element& example
			) :
					positions(new Position[combinationSize]),
					combinationWrapper(Converter<Combination, Request>::template createCombinationWrapper<Element>(
							combinationSize,
							example
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
