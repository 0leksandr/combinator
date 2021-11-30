#pragma once

#include "../DereferencedIterator.h"
#include "../SizedIterator.h"
#include "../../Converter.h"
#include "../../Request/MultisetRequest.h"

namespace CombinatorNamespace {
	template<class Container, class Combination, typename Element = typeof(Container[])>
	class MultisetIterator :
			public DereferencedIterator<Combination, MultisetRequest<Container>, Element>,
			public SizedIterator {
		public:
			explicit MultisetIterator(const MultisetRequest<Container>& request) :
					DereferencedIterator<Combination, MultisetRequest<Container>, Element>(request.combinationSize()),
					request(request) {
				for (int c = 0; c < request.combinationSize(); ++c) this->positions[c] = 0;
			}
			Combination& operator*() const override {
				return this->combinationWrapper->get(request, this->positions);
			}
			static Position size(const MultisetRequest<Container>& request) {
				Position size = 1;
				for (int c = 0; c < request.combinationSize(); ++c) {
					const auto prev = size; // TODO: would it be better (more performant) to declare it outside of the loop?
					size *= request.containerSize(c);
					if (prev != 0 && (size / prev != request.containerSize(c))) {
						throw std::overflow_error("Number of combinations exceeded max allowed number");
					}
				}
				return size;
			}
		protected:
			const MultisetRequest<Container>& request;
	};
}
