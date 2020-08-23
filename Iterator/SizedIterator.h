#pragma once

#include "Iterator.h"
#include "../Position.h"

class SizedIterator : public Iterator {
	public:
		SizedIterator() : Iterator() {}
		[[nodiscard]] virtual Position size() const = 0;
};
