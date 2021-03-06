#pragma once

#include "CandidateOrderIterator.h"
#include "../../../../Position.h"
#include "../../../../Request/FixedSizeRequest.h"

namespace CombinatorNamespace {
	template<class Container, class Combination>
	class Mathematician : public CandidateOrderIterator<Container, Combination> {
		public:
			explicit Mathematician(const FixedSizeRequest<Container>& request) :
					CandidateOrderIterator<Container, Combination>(request) {
				avgEstimation = avgNrSteps();
			}
			[[nodiscard]] Position estimate(Position) const override {
				return avgEstimation; // TODO: test
			}
		protected:
			void go(Position index) override {
				Position nrElements(this->nrElements());
				for (Position c = 0; c < this->request.length; c++) {
					Step step = getStep(c, nrElements, index);
					nrElements -= step.x + 1;
					index -= step.beginningOfX;
					insertUnique(c, step.x);
				}
			}
		private:
			Position avgEstimation;
			struct Step {
				Step(const Position x,const Position beggingOfX) :
						x(x),
						beginningOfX(beggingOfX) {}
				Position x;
				Position beginningOfX;
			};
			Step getStep(
					const Position position,
					const Position nrElements,
					const Position index
			) const {
				if (position == this->request.length - 1) return Step(index, index);
				Step res(0, 0);
				Position lastNPerM(0);
				while (res.beginningOfX <= index) { // TODO: optimize
					res.beginningOfX += lastNPerM = this->nPerM(
							nrElements - res.x - 1,
							this->request.length - position - 1
					);
					res.x++;
				}
				if (res.x > 0) --res.x;
				res.beginningOfX -= lastNPerM;
				return res;
			}
			void insertUnique(const Position position, Position value) const {
				if (position > 0) value += this->positions[position - 1] + 1;
				this->positions[position] = value;
			}
			[[nodiscard]] Position avgNrSteps() const {
	return this->request.length * this->request.length; // TODO: make it right
				float totalAvgNrSteps(0.f);
				Position prevValue(0);
				for (Position c = 0; c < this->request.length; c++) {
					float nrSteps = avgNrSteps(c, prevValue);
					prevValue = getStep(
							c,
							this->nrElements() - prevValue,
							(Position)nrSteps
					).x;
					totalAvgNrSteps += nrSteps;
				}
				return (Position)totalAvgNrSteps;
			}
			[[nodiscard]] float avgNrSteps(
					const Position position,
					const Position minValue
			) const {
				float sum(0.f);
				Position maxValue = this->nrElements()
									+ position
									+ 1
									- this->request.length;
				for (Position c = minValue; c < maxValue; c++) {
					sum += this->nPerM(
							this->nrElements() - c - 1,
							this->request.length - position - 1
					);
				}
				return sum / float(maxValue - minValue + 1);
			}
	};
}
