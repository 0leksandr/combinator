#pragma once

#include "Iterator.h"
#include "../Converter.h"
#include "../Position.h"

namespace CombinatorNamespace {
	template<class Combination>
	class DereferencedIterator : public Iterator {
		public:
			explicit DereferencedIterator(const Combination combination, const Position combinationSize) :
					positions(new Position[combinationSize]),
					combination(combination) {}
			~DereferencedIterator() {
				delete[] positions;
				Converter::deleteCombination(combination);
			}
			virtual Combination& operator*() const = 0;
		protected:
			Position* const positions;
			mutable Combination combination;
	};
}
