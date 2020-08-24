#pragma once

#include "ComposeIterator.h"
#include "../Movement/RandomAccessIterator.h"
#include "../../Position.h"
#include "../../Request/ComposeRequest.h"

#include "my/macro.cpp"

template<class Container, Position NrContainers, class Combination>
class ComposeRAIterator :
		public ComposeIterator<Container, NrContainers, Combination>,
		public RandomAccessIterator {
	public:
		ComposeRAIterator(const ComposeRequest<Container, NrContainers>* const request) :
				ComposeIterator<Container, NrContainers, Combination>(request) {}
	protected:
		void go(const Position position) override {
// TODO: implement
FIXME;
		}
};


