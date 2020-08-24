#pragma once

#include "../DereferencedIterator.h"
#include "../Movement/PositionedIterator.h"
#include "../SizedIterator.h"
#include "../../Converter.h"
#include "../../Request/ComposeRequest.h"

template<class Container, Position NrContainers, class Combination>
class ComposeIterator :
		public DereferencedIterator<Container, Combination>,
		public PositionedIterator,
		public SizedIterator {
	public:
		ComposeIterator(const ComposeRequest<Container, NrContainers>* const request) :
				DereferencedIterator<Container, Combination>(
						request->containers.size(),
						Converter<Combination>::initCombination(
								&this->combination,
								request->containers.size(),
								request->containers[0][0]
						)
				),
				SizedIterator(getSize(request)),
				request(request) {}
		Combination& operator*() const override {
			for (Position c = 0; c < request->containers.size(); c++) {
				this->combination[c] = request->containers[c][this->positions[c]];
			}
			return this->combination;
		}
	private:
		const ComposeRequest<Container, NrContainers>* const request;

		static Position getSize(const ComposeRequest<Container, NrContainers>* request) {
			Position _size = 1;
			for (const auto& container : request->containers) _size *= container.size();
			return _size;
		}
};
