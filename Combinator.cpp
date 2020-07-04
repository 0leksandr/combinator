#pragma once

#include <vector>
#include <cmath>
#include <array>
#include <boost/assert.hpp>

namespace Combinator {
    typedef unsigned long Position;

    template<class Element, class Container, class Combination>
    class ForwardIterator;
	template<class Element, class Container, class Combination>
	class RandomAccessIterator;

	#define Assert BOOST_ASSERT

//	template<class Element>
//	class Container {
//		public:
//			virtual Element operator[](Position position) const = 0;
//			[[nodiscard]] virtual size_t size() const = 0;
//	};

    template<class Container>
    class FixedRequest {
    	public:
			const Container elements;
			const Position length;

			FixedRequest(
					const Container elements,
					const Position length
			) : elements(elements), length(length) {}
    };

	template<class Element, class Combination>
	class Converter {
		public:
			explicit Converter(const FixedRequest<Combination>* const request) :
					request(request) {}
			std::vector<Element> construct(std::vector<Element>*) const {
				std::vector<Element> vec;
				initVector(vec);
				return vec;
			}
			// template<class element>
			// simpleArray<element> construct(simpleArray<element>*) const {
			// 	return simpleArray<element>(request->length);
			// }
			template<class element, unsigned long size>
			std::array<element, size> construct(std::array<element, size>*) const {
				Assert(size == request->length); // TODO: remove?
				return std::array<element, size>();
			}
			template<class element>
			element* construct(element**) const {
				return new element[request->length];
			}
			template<class C>
			C construct(C*) const {
				throw std::runtime_error("Unsupported combination type");
			}

			void prepare(std::vector<Element>& combination) const {
				if (combination.size() != request->length) {
					combination.clear();
					initVector(combination);
				}
			}
			template<class C>
			void prepare(C& combination) const {}

			// Element* getElement(simpleArray<Element*>*, Element& element) const {
			// 	return &element;
			// }
			template<size_t size>
			Element* getElement(std::array<Element*, size>*, Element& element) const {
				return &element;
			}
			Element* getElement(Element***, Element& element) const {
				return &element;
			}
			template<class C>
			const Element& getElement(C*, const Element& element) const {
				return element;
			}

			template<class element>
			void destruct(element*& combination) const {
				delete combination;
			}
			template<class C>
			void destruct(C& combination) const {}
		private:
			const FixedRequest<Combination>* const request;

			void initVector(std::vector<Element>& vec) const {
				vec.reserve(request->length);
				for (Position c = 0; c < request->length; c++) {
					vec.push_back(request->elements[c]);
				}
			}
	};

    template<class Element, class Container, class Combination, class ForwardIterator>
    class FixedCombinator {
        public:
//			virtual RandomAccessIterator<Element, Container, Combination>* newIterator() const = 0;
//			FixedCombinator(Container elements, const Position length) :
//					request(elements, length),
//					current(newIterator()) {}

			Combination& operator[](Position index) const {
				current->go(index);
				return **current;
			}
            ForwardIterator begin() const {
                return ForwardIterator(&request, 0);
            }
            ForwardIterator end() const {
                return ForwardIterator(&request, this->size()); // TODO: keep as private const
            }
			Position size() const {
				return current->size();
			}
        protected:
    		const FixedRequest<Combination> request;
            mutable RandomAccessIterator<Element, Container, Combination>* current;

            explicit FixedCombinator(const FixedRequest<Combination> request) :
					request(request),
                    current(newIterator()) {}
            FixedCombinator( // TODO: remove?
            		const FixedCombinator<Element, Container, Combination, ForwardIterator>& other
			) :
					request(other.request),
                    current(newIterator()) {}
			~FixedCombinator() {
            	delete current;
            }
    	private:
			RandomAccessIterator<Element, Container, Combination>* newIterator() const { // TODO: remove?
				return new ForwardIterator(&request, 0);
            }
    };

    template<class Element, class Container, class Combination = Container>
    class OrderedCombinator;
    template<class Element, class Container, class Combination = Container>
    class ShuffledCombinator;

    template<class Element, class Container, class Combination>
    class ForwardIterator {
        public:
            ForwardIterator(
					const FixedRequest<Container>* const request,
                    const Position index // TODO: do not pass to constructor
            ):
                    request(request),
                    index(index),
                    positions(new Position[request->length]),
                    converter(request),
                    combination(converter.construct(&combination)) {
                first(); // TODO: so, index is ignored?
            }
            ~ForwardIterator() {
                delete[] positions;
                converter.destruct(combination);
            }

			[[nodiscard]] virtual Position size() const = 0; // TODO: static

            virtual void operator++() = 0;
            bool operator!=(const ForwardIterator<Element, Container, Combination>& other) const {
                return index != other.index;
            }
            Combination& operator*() {
                converter.prepare(combination);
                for (Position c = 0; c < request->length; c++)
                    combination[c] = converter.getElement(
                            &combination,
                            request->elements[positions[c]]
                    );
                return combination;
            }
        protected:
			const FixedRequest<Container>* const request;
            Position index;
            Position* const positions;

			[[nodiscard]] Position nrElements() const { // TODO: remove?
				return request->elements.size();
			}
			void first() {
				for (Position c = 0; c < this->request->length; c++) positions[c] = c;
			}
        private:
            const Converter<Element, Combination> converter;
            Combination combination;
    };
	template<class Element, class Container, class Combination>
	class BackwardsIterator : public ForwardIterator<Element, Container, Combination> {
		public:
			BackwardsIterator(
					const FixedRequest<Container>* const request,
					const Position index
			) : ForwardIterator<Element, Container, Combination>(request, index) {}
		protected:
			virtual void operator--() = 0;
	};
	template<class Element, class Container, class Combination>
	class RandomAccessIterator : public BackwardsIterator<Element, Container, Combination> {
		public:
			RandomAccessIterator(
					const FixedRequest<Container>* const request,
					const Position index
			) : BackwardsIterator<Element, Container, Combination>(request, index) {}
			virtual void go(Position index) = 0;
	};
	template<class Element, class Container, class Combination>
	class OrderIterator : public RandomAccessIterator<Element, Container, Combination> {
		public:
			OrderIterator(
					const FixedRequest<Container>* const request,
					const Position index
			) :
					RandomAccessIterator<Element, Container, Combination>(request, index),
					_size(nPerM(request->elements.size(), request->length)) {}
			[[nodiscard]] virtual Position estimate(Position index) const = 0; // TODO: not public?
			void operator++() override {
				increment(this->request->length - 1);
				++this->index;
			}
			void operator--() override {
				decrement(this->request->length - 1);
				--this->index;
			}
			[[nodiscard]] Position getPosition(const Position position) const { // TODO: for ComboIterator and Hunter. Refactor
				return this->positions[position];
			}
		protected:
			[[nodiscard]] Position size() const override {
				return _size;
			}
			template<typename Float = float>
			static Position nPerM(const Position n, const Position m) {
				Float res(1.);
				for (Position c = 0; c < m; c++) {
					res *= Float(n - c);
					res /= Float(c + 1);
				}
				return (Position)res;
			}
		private:
			const Position _size;

			void increment(const Position position) const {
				if (++this->positions[position] > maxPosition(position)) {
					if (position != 0) { // preventing fail on end
						increment(position - 1);
						this->positions[position] = this->positions[position - 1] + 1;
					}
				}
			}
			void decrement(const Position position) const {
				--this->positions[position];
				if (position > 0 && this->positions[position] == this->positions[position - 1]) {
					for (Position c = position; c < this->request->length; c++) {
						this->positions[c] = maxPosition(c);
					}
					decrement(position - 1);
				}
			}
			[[nodiscard]] Position maxPosition(const Position position) const { // TODO: inline?
				return this->nrElements() + position - this->request->length;
			}
	};
	template<class Element, class Container, class Combination>
	class Walker : public OrderIterator<Element, Container, Combination> {
		public:
			explicit Walker(
					const FixedRequest<Container>* const request,
					const Position index
			) :
					OrderIterator<Element, Container, Combination>(request, index) {}
			Walker(const Walker& other) :
					OrderIterator<Element, Container, Combination>(other.request, other.index) {
				for (Position c = 0; c < this->request->length; c++) {
					this->positions[c] = other.positions[c];
				}
			}
			[[nodiscard]] Position estimate(const Position index) const override {
				if (index > this->index) return index - this->index;
				if (index < this->index) return this->index - index;
				return 0;
			}
			void go(const Position index) override {
				while (this->index < index) this->operator++();
				while (this->index > index) this->operator--();
			}
			[[nodiscard]] Position getIndex() const {
				return this->index;
			}
	};
	template<class Element, class Container, class Combination>
	class Hunter : public OrderIterator<Element, Container, Combination> {
		public:
			explicit Hunter(
					const FixedRequest<Container>* const request,
					const Position index
			) :
					OrderIterator<Element, Container, Combination>(request, index) {
				const Position nrGuardians = (Position)sqrt(this->size()) + 1;
				reactionTime = this->size() / nrGuardians; // TODO: check
				Walker<Element, Container, Combination> patrol(request, index);
				while (patrol.getIndex() < this->size() - 1) {
					patrol.operator++();
					if ((patrol.getIndex() + reactionTime / 2) % reactionTime == 0) {
						guardians.push_back(patrol);
					}
				}
			}
			[[nodiscard]] Position estimate(const Position index) const override {
				return guardian(index).estimate(index);
			}
			void go(const Position index) override {
				Walker<Element, Container, Combination> envoy = guardian(index);
				envoy.go(index);
				for (Position c = 0; c < this->request->length; c++) {
					this->positions[c] = envoy.getPosition(c);
				}
				this->index = index;
			}
		private:
			std::vector<Walker<Element, Container, Combination>> guardians;
			Position reactionTime;

			Walker<Element, Container, Combination> guardian(const Position index) const { // TODO: link/reference
				return guardians[index / reactionTime];
			}
	};
	template<class Element, class Container, class Combination>
	class Mathematician : public OrderIterator<Element, Container, Combination> {
		public:
			explicit Mathematician(
					const FixedRequest<Container>* const request,
					const Position index
			) :
					OrderIterator<Element, Container, Combination>(request, index) {
				avgEstimation = avgNrSteps();
			}
			[[nodiscard]] Position estimate(Position index) const override {
				return avgEstimation; // TODO: test
			}
			void go(Position index) override {
				this->index = index;
				Position nrElements(this->nrElements());
				for (Position c = 0; c < this->request->length; c++) {
					Step step = getStep(c, nrElements, index);
					nrElements -= step.x + 1;
					index -= step.beginningOfX;
					insertUnique(c, step.x);
				}
			}
		private:
			Position avgEstimation;
			struct Step {
				Step(
						const Position x,
						const Position beggingOfX
				): x(x), beginningOfX(beggingOfX) {}
				Position x;
				Position beginningOfX;
			};
			Step getStep(
					const Position position,
					const Position nrElements,
					const Position index
			) const {
				if (position == this->request->length - 1) return Step(index, index);
				Step res(0, 0);
				Position lastNPerM(0);
				while (res.beginningOfX <= index) { // TODO: optimize
					res.beginningOfX += lastNPerM = this->nPerM(
							nrElements - res.x - 1,
							this->request->length - position - 1
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
				float totalAvgNrSteps(0.f);
				Position prevValue(0);
				for (Position c = 0; c < this->request->length; c++) {
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
									- this->request->length;
				for (Position c = minValue; c < maxValue; c++)
					sum += this->nPerM(
							this->nrElements() - c - 1,
							this->request->length - position - 1
					);
				return sum / float(maxValue - minValue + 1);
			}
	};
	template<class Element, class Container, class Combination>
	class ComboIterator : public OrderIterator<Element, Container, Combination> {
		private:
			mutable std::vector<OrderIterator<Element, Container, Combination>*> iterators; // TODO: why mutable?
		public:
			ComboIterator(
					const FixedRequest<Container>* const request,
					const Position index
			) :
					OrderIterator<Element, Container, Combination>(request, index),
					iterators() {
				iterators.push_back(new Hunter<Element, Container, Combination>(request, index));
				iterators.push_back(new Mathematician<Element, Container, Combination>(request, index));
			}
			~ComboIterator() {
				for (auto* iterator : iterators) delete iterator;
			}
			void go(Position index) override {
				auto estimated((Position)-1);
				OrderIterator<Element, Container, Combination>* chosen(nullptr);
				for (auto* iterator : iterators) {
					Position myBet = iterator->estimate(index);
					if (myBet < estimated) {
						chosen = iterator;
						estimated = myBet;
					}
				}
				Assert(chosen != nullptr);
				chosen->go(index);
				for (Position c = 0; c < this->request->length; c++) {
					this->positions[c] = chosen->getPosition(c);
				}
				this->index = index;
			}
			[[nodiscard]] Position estimate(Position index) const { // TODO: remove
				return 0;
			}
	};

    template<class Element, class Container, class Combination>
    class OrderedCombinator : public FixedCombinator<Element, Container, Combination, ComboIterator<Element, Container, Combination>> {
        public:
			OrderedCombinator(Container elements, const Position length) :
					FixedCombinator<Element, Container, Combination, ComboIterator<Element, Container, Combination>>(
							FixedRequest<Combination>(elements, length)
					) {}
    };

	template<class Element, class Container, class Combination>
	class ShuffleIterator : public RandomAccessIterator<Element, Container, Combination> {
		public:
			ShuffleIterator(
					const FixedRequest<Container>* const request,
					const Position index
			) :
					RandomAccessIterator<Element, Container, Combination>(request, index),
					_size(nPerM(request->elements.size(), request->length)) {}
			void go(Position index) override {
				this->index = index;
				Position nrElements(this->nrElements()); // TODO: remove?
				for (Position c = 0; c < this->request->length; c++) {
					insertUnique(c, index % nrElements);
					index /= nrElements--;
				}
			}
			void operator++() override {
				go(this->index + 1);
			}
			void operator--() override {
				go(this->index - 1);
			}
			[[nodiscard]] Position size() const override {
				return _size;
			}
		private:
			const Position _size;

			void insertUnique(
					const Position position,
					Position value
			) {
				auto prevValue((Position)-1); // TODO: 0
				do {
					Position add(0); // TODO: move up?
					for (Position c = 0; c < position; c++) {
						if (
							(prevValue + 1 < this->positions[c] + 1) &&
							(this->positions[c] <= value)
						) {
							++add;
						}
					}
					prevValue = value;
					value += add;
				} while (prevValue != value);
				this->positions[position] = value;
			}
			static Position nPerM(const Position n, const Position m) {
				return nPerM0(n, m);
			}
			static Position nPerM0(const Position n, const Position m) {
				if (m > 1) return n * nPerM0(n - 1, m - 1);
				return n;
			}
			static Position nPerM1(Position n, Position m) {
				Position res(n);
				while (m --> 1) res *= --n;
				return res;
			}
			static Position nPerM2(Position n, Position m) {
				Position res(n);
				while (--m > 0) res *= --n;
				return res;
			}
	};

    template<class Element, class Container, class Combination>
    class ShuffledCombinator : public FixedCombinator<Element, Container, Combination, ShuffleIterator<Element, Container, Combination>> {
        public:
			ShuffledCombinator(Container elements, const Position length):
					FixedCombinator<Element, Container, Combination, ShuffleIterator<Element, Container, Combination>>(
							FixedRequest<Combination>(elements, length)
					) {}
    };
}
