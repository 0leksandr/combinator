#pragma once

#include "Iterator.h"
#include "../Position.h"

class SizedIterator : public Iterator {
	public:
		[[nodiscard]] virtual Position size() const = 0;
};
