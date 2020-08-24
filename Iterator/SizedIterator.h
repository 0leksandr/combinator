#pragma once

#include "Iterator.h"
#include "../Position.h"

class SizedIterator : public Iterator {
	public:
		SizedIterator(const Position size) : _size(size) {}
		[[nodiscard]] Position size() const {
			return _size;
		}
	private:
		const Position _size;
};
