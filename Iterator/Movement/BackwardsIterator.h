#pragma once

#include "../Iterator.h"

namespace CombinatorNamespace {
	class BackwardsIterator : public Iterator {
		protected:
			virtual void operator--() = 0;
	};
}
