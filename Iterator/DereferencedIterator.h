#pragma once

#include "Iterator.h"
#include "../Converter.h"
#include "../Position.h"

template<class Container, class Combination>
class DereferencedIterator : public Iterator {
	public:
		explicit DereferencedIterator(const Position combinationSize, const Combination combination) :
				positions(new Position[combinationSize]),
				combination(combination) {}
		~DereferencedIterator() {
			delete[] positions;
			Converter<Container>::deleteCombination(combination);
		}
		virtual Combination& operator*() const = 0;
	protected:
		Position* const positions;
		mutable Combination combination;
};
