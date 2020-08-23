#pragma once

#include <cmath>
#include <vector>
#include "CandidateOrderIterator.h"
#include "Walker.h"
#include "../../../Position.h"
#include "../../../Request/FixedRequest.h"

template<class Container, class Combination>
class Hunter : public CandidateOrderIterator<Container, Combination> {
	public:
		explicit Hunter(const FixedRequest<Container>* const request) :
				CandidateOrderIterator<Container, Combination>(request) {
			const Position nrGuardians = (Position)sqrt(this->size()) + 1;
			reactionTime = this->size() / nrGuardians; // TODO: check
			Walker<Container, Combination> patrol(request);
			while (patrol.getIndex() < this->size() - 1) {
				patrol.operator++();
				if ((patrol.getIndex() + reactionTime / 2) % reactionTime == 0) {
					guardians.push_back(patrol);
				}
			}
		}
		[[nodiscard]] Position estimate(const Position index) const override {
			return guardian(index)->estimate(index);
		}
		void go(const Position index) override {
			auto envoy = guardian(index);
			envoy->go(index);
			for (Position c = 0; c < this->request->length; c++) {
				this->positions[c] = envoy->getPosition(c);
			}
			this->index = index;
		}
	private:
		std::vector<Walker<Container, Combination>> guardians;
		Position reactionTime;

		Walker<Container, Combination>* guardian(const Position index) const {
			return (Walker<Container, Combination>*)&guardians[index / reactionTime];
		}
};
