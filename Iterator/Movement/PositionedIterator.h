#pragma once

#include "../Iterator.h"
#include "../../Position.h"

namespace CombinatorNamespace {
	class PositionedIterator : public Iterator {
		public:
			explicit PositionedIterator(const Position index = 0) : index(index) {}
			virtual bool operator!=(const PositionedIterator& other) const {
				return this->index != other.index;
			}
		protected:
			[[nodiscard]] virtual Position getIndex() const {
				return index;
			}
			virtual void setIndex(const Position _index) {
				index = _index;
			}
		private:
			Position index;
	};
}
