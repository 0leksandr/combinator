#pragma once

#include "../ForwardIterator.h"
#include "../../Converter.h"
#include "../../Position.h"
#include "../../Request/ComposeRequest.h"

template<class Container, Position NrContainers, class Combination>
class ComposeFIterator : public ForwardIterator {
	public:
		explicit ComposeFIterator(const ComposeRequest<Container, NrContainers>* request) :
				ForwardIterator(),
				request(request),
				positions(new Position[request->length]),
				combination(Converter<Combination>::initCombination(
						&combination,
						request->containers.size(),
						request->containers[0][0] // TODO: check if exists
				)),
				_size(getSize(request)) {}
		~ComposeFIterator() { // TODO: lots is copied from ForwardListIterator. Extract into 1 place (ForwardIterator?)
			delete[] positions;
			Converter<Container>::deleteCombination(combination);
		}
		[[nodiscard]] Position size() const override {
			return _size;
		}
		void operator++() override {
			increment(0);
			++this->index;
		}
		Combination& operator*() const override {
			for (Position c = 0; c < request->containers.size(); c++) {
				combination[c] = request->containers[c][positions[c]];
			}
			return combination;
		}
	private:
		const ComposeRequest<Container, NrContainers>* request;
		Position* const positions;
		mutable Combination combination;
		const Position _size;

		static Position getSize(const ComposeRequest<Container, NrContainers>* request) {
			Position _size = 1;
			for (const auto& container : request->containers) _size *= container.size();
			return _size;
		}
		void increment(const Position position) {
			if (position < request->containers.size()) {
				if (++(positions[position]) == request->containers[position].size()) {
					positions[position] = 0;
					increment(position + 1);
				}
			}
		}
};
