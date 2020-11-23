#pragma once

#include "../DereferencedIterator.h"
#include "../SizedIterator.h"
#include "../../Converter.h"
#include "../../Request/MultisetRequest.h"

namespace CombinatorNamespace {
	template<class Container, class Combination>
	class MultisetIterator :
			public DereferencedIterator<Combination>,
			public SizedIterator {
		public:
			explicit MultisetIterator(const MultisetRequest<Container>& request) :
					DereferencedIterator<Combination>(
							Converter::initCombination(
									&this->combination,
									request.containers.size(),
									request.containers[0][0]
							),
							request.containers.size()
					),
					request(request) {
				for (int c = 0; c < request.containers.size(); ++c) this->positions[c] = 0;
			}
			Combination& operator*() const override {
				for (Position c = 0; c < request.containers.size(); c++) {
					this->combination[c] = request.containers[c][this->positions[c]];
				}
				return this->combination;
			}
			static Position size(const MultisetRequest<Container>& request) {
				Position size = 1;
				for (const auto& container : request.containers) {
					const auto prev = size; // TODO: would it be better (more performant) to declare it outside of the loop?
					size *= container.size();
					if (size == 0 && prev != 0 && container.size() != 0) {
						throw std::overflow_error("Number of combinations exceeded max allowed number");
					}
				}
				return size;
			}
		protected:
			const MultisetRequest<Container>& request;
	};
}
