#pragma once

#include "../DereferencedIterator.h"
#include "../SizedIterator.h"
#include "../../Converter.h"
#include "../../Request/MultisetRequest.h"

template<class Container, Position NrContainers, class Combination>
class MultisetIterator :
		public DereferencedIterator<Container, Combination>,
		public SizedIterator {
	public:
		explicit MultisetIterator(const MultisetRequest<Container, NrContainers>& request) :
				DereferencedIterator<Container, Combination>(
						Converter<Combination>::initCombination(
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
		static Position size(const MultisetRequest<Container, NrContainers>& request) {
			Position size = 1;
			for (const auto& container : request.containers) size *= container.size();
			return size;
		}
	protected:
		const MultisetRequest<Container, NrContainers>& request;
};
