#pragma once

#include <cmath>
#include <vector>
#include "CandidateOrderIterator.h"
#include "Walker.h"
#include "../../../../Position.h"
#include "../../../../Request/FixedSizeRequest.h"

namespace CombinatorNamespace {
	template<class Container, class Combination>
	class Hunter : public CandidateOrderIterator<Container, Combination> {
		public:
			explicit Hunter(const FixedSizeRequest<Container>& request) :
					CandidateOrderIterator<Container, Combination>(request) {
				const auto size = OrderIterator<Container, Combination>::size(request);
				const Position nrGuardians = Hunter::min((Position)sqrt(size) + 1, size);
				reactionTime = size / nrGuardians; // TODO: check
				Walker<Container, Combination> patrol(request);
				const auto maxPatrolIndex = std::max(size, Position{2}) - 1;
				while (patrol.getWalkerIndex() < maxPatrolIndex) {
					patrol.operator++();
					if ((patrol.getWalkerIndex() + reactionTime / 2) % reactionTime == 0) {
						guardians.push_back(patrol);
					}
				}
			}
			[[nodiscard]] Position estimate(const Position index) const override {
				return guardian(index)->estimate(index);
			}
		protected:
			void go(const Position index) override {
				auto envoy = guardian(index);
				envoy->operator[](index);
				for (Position c = 0; c < this->request.length; c++) {
					this->positions[c] = envoy->getPosition(c);
				}
			}
		private:
			std::vector<Walker<Container, Combination>> guardians;
			Position reactionTime;

			Walker<Container, Combination>* guardian(const Position index) const {
				const auto guardianIndex = Hunter::min(index / reactionTime, guardians.size() - 1);
				return (Walker<Container, Combination>*)&guardians[guardianIndex];
			}
			static Position min(const Position a, const Position b) { // std::min doesn't support `unsigned long long`
				return (a < b) ? a : b;
			}
	};
}
