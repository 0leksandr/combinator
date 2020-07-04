#pragma once

#include <vector>
#include <cmath>
#include <array>
#include <boost/assert.hpp>

namespace Combinator {
    typedef unsigned long position; // TODO: Position

    template<class element, class Container, class Combination> // TODO: Element
    class ForwardIterator;
	template<class element, class Container, class Combination>
	class RandomAccessIterator;

	#define Assert BOOST_ASSERT

//	template<class element>
//	class Container {
//		public:
//			virtual element operator[](position _position) const = 0;
//			[[nodiscard]] virtual size_t size() const = 0;
//	};

    template<class Container>
    class FixedRequest {
    	public:
			const Container elements;
			const position length;

			FixedRequest(
					const Container elements,
					const position length
			) : elements(elements), length(length) {}
    };

    template<class element, class Container, class Combination, class ForwardIterator>
    class FixedCombinator {
        public:
//			virtual RandomAccessIterator<element, Container, Combination>* newIterator() const = 0;
//			FixedCombinator(Container elements, const position length) :
//					request(elements, length),
//					current(newIterator()) {}

			Combination& operator[](position index) const {
				current->go(index);
				return **current;
			}
            ForwardIterator begin() const {
                return ForwardIterator(&request, 0);
            }
            ForwardIterator end() const {
                return ForwardIterator(&request, this->size()); // TODO: keep as private const
            }
			position size() const {
				return current->size();
			}
        protected:
    		const FixedRequest<Combination> request;
            mutable RandomAccessIterator<element, Container, Combination>* current;

            explicit FixedCombinator(const FixedRequest<Combination> request) :
					request(request),
                    current(newIterator()) {}
            FixedCombinator( // TODO: remove?
            		const FixedCombinator<element, Container, Combination, ForwardIterator>& other
			) :
					request(other.request),
                    current(newIterator()) {}
			~FixedCombinator() {
            	delete current;
            }
    	private:
			RandomAccessIterator<element, Container, Combination>* newIterator() const { // TODO: remove?
				return new ForwardIterator(&request, 0);
            }
    };

    template<class element, class Container, class Combination = Container>
    class OrderedCombinator;
    template<class element, class Container, class Combination = Container>
    class ShuffledCombinator;

    template<class element, class Container, class Combination>
    class ForwardIterator {
            class Converter;
        public:
            ForwardIterator(
					const FixedRequest<Container>* const request,
                    const position index // TODO: do not pass to constructor
            ):
                    request(request),
                    index(index),
                    positions(new position[request->length]),
                    converter(request),
                    combination(converter.construct(&combination)) {
                first(); // TODO: so, index is ignored?
            }
            ~ForwardIterator() {
                delete[] positions;
                converter.destruct(combination);
            }

			[[nodiscard]] virtual position size() const = 0; // TODO: static

            virtual void operator++() = 0;
            bool operator!=(const ForwardIterator<element, Container, Combination>& other) const {
                return index != other.index;
            }
            Combination& operator*() {
                converter.prepare(combination);
                for (position c = 0; c < request->length; c++)
                    combination[c] = converter.getElement(
                            &combination,
                            request->elements[positions[c]]
                    );
                return combination;
            }
        protected:
			const FixedRequest<Container>* const request;
            position index;
            position* const positions;

			[[nodiscard]] position nrElements() const { // TODO: remove?
				return request->elements.size();
			}
			void first() {
				for (position c = 0; c < this->request->length; c++) positions[c] = c;
			}
        private:
            const Converter converter;
            Combination combination;

            class Converter { // TODO: move outside
                public:
                    explicit Converter(const FixedRequest<Combination>* const request) :
                            request(request) {}
                    std::vector<element> construct(std::vector<element>*) const {
                        std::vector<element> vec;
                        initVector(vec);
                        return vec;
                    }
//                    template<class _element>
//                    simpleArray<_element> construct(simpleArray<_element>*) const {
//                        return simpleArray<_element>(request->length);
//                    }
                    template<class _element, unsigned long size>
                    std::array<_element, size> construct(std::array<_element, size>*) const {
                        Assert(size == request->length);
                        return std::array<_element, size>();
                    }
                    template<class _element>
                    _element* construct(_element**) const {
                        return new _element[request->length];
                    }
                    template<class C>
                    C construct(C*) const {
                    	throw std::runtime_error("Unsupported combination type");
                    }

                    void prepare(std::vector<element>& _combination) const {
                        if (_combination.size() != request->length) {
                            _combination.clear();
                            initVector(_combination);
                        }
                    }
                    template<class C>
                    void prepare(C& _combination) const {}

//                    element* getElement(simpleArray<element*>*, element& _element) const {
//                        return &_element;
//                    }
                    template<size_t size>
                    element* getElement(std::array<element*, size>*, element& _element) const {
                        return &_element;
                    }
                    element* getElement(element***, element& _element) const {
                        return &_element;
                    }
                    template<class C>
                    const element& getElement(C*, const element& _element) const {
                        return _element;
                    }

                    template<class _element>
                    void destruct(_element*& _combination) const {
                        delete _combination;
                    }
                    template<class C>
                    void destruct(C& _combination) const {}
                private:
                    const FixedRequest<Combination>* const request;
                    void initVector(std::vector<element>& vec) const {
                        vec.reserve(request->length);
                        for (position c = 0; c < request->length; c++) {
							vec.push_back(request->elements[c]);
						}
                    }
            };
    };
	template<class element, class Container, class Combination>
	class BackwardsIterator : public ForwardIterator<element, Container, Combination> {
		public:
			BackwardsIterator(
					const FixedRequest<Container>* const request,
					const position index
			) : ForwardIterator<element, Container, Combination>(request, index) {}
		protected:
			virtual void operator--() = 0;
	};
	template<class element, class Container, class Combination>
	class RandomAccessIterator : public BackwardsIterator<element, Container, Combination> {
		public:
			RandomAccessIterator(
					const FixedRequest<Container>* const request,
					const position index
			) : BackwardsIterator<element, Container, Combination>(request, index) {}
			virtual void go(position index) = 0;
	};
	template<class element, class Container, class Combination>
	class OrderIterator : public RandomAccessIterator<element, Container, Combination> {
		public:
			OrderIterator(
					const FixedRequest<Container>* const request,
					const position index
			) :
					RandomAccessIterator<element, Container, Combination>(request, index),
					_size(nPerM(request->elements.size(), request->length)) {}
			[[nodiscard]] virtual position estimate(position index) const = 0; // TODO: not public?
			void operator++() override {
				increment(this->request->length - 1);
				++this->index;
			}
			void operator--() override {
				decrement(this->request->length - 1);
				--this->index;
			}
			[[nodiscard]] position getPosition(const position _position) const { // TODO: for ComboIterator and Hunter. Refactor
				return this->positions[_position];
			}
		protected:
			[[nodiscard]] position size() const override {
				return _size;
			}
			template<typename Float = float>
			static position nPerM(const position n, const position m) {
				Float res(1.);
				for (position c = 0; c < m; c++) {
					res *= Float(n - c);
					res /= Float(c + 1);
				}
				return (position)res;
			}
		private:
			const position _size;

			void increment(const position _position) const {
				if (++this->positions[_position] > maxPosition(_position)) {
					if (_position != 0) { // preventing fail on end
						increment(_position - 1);
						this->positions[_position] = this->positions[_position - 1] + 1;
					}
				}
			}
			void decrement(const position _position) const {
				--this->positions[_position];
				if (_position > 0 && this->positions[_position] == this->positions[_position - 1]) {
					for (position c = _position; c < this->request->length; c++) {
						this->positions[c] = maxPosition(c);
					}
					decrement(_position - 1);
				}
			}
			[[nodiscard]] position maxPosition(const position _position) const { // TODO: inline?
				return this->nrElements() + _position - this->request->length;
			}
	};
	template<class element, class Container, class Combination>
	class Walker : public OrderIterator<element, Container, Combination> {
		public:
			explicit Walker(
					const FixedRequest<Container>* const request,
					const position index
			) :
					OrderIterator<element, Container, Combination>(request, index) {}
			Walker(const Walker& other) :
					OrderIterator<element, Container, Combination>(other.request, other.index) {
				for (position c = 0; c < this->request->length; c++) {
					this->positions[c] = other.positions[c];
				}
			}
			[[nodiscard]] position estimate(const position index) const override {
				if (index > this->index) return index - this->index;
				if (index < this->index) return this->index - index;
				return 0;
			}
			void go(const position index) override {
				while (this->index < index) this->operator++();
				while (this->index > index) this->operator--();
			}
			[[nodiscard]] position getIndex() const {
				return this->index;
			}
	};
	template<class element, class Container, class Combination>
	class Hunter : public OrderIterator<element, Container, Combination> {
		public:
			explicit Hunter(
					const FixedRequest<Container>* const request,
					const position index
			) :
					OrderIterator<element, Container, Combination>(request, index) {
				const position nrGuardians = (position)sqrt(this->size()) + 1;
				reactionTime = this->size() / nrGuardians; // TODO: check
				Walker<element, Container, Combination> patrol(request, index);
				while (patrol.getIndex() < this->size() - 1) {
					patrol.operator++();
					if ((patrol.getIndex() + reactionTime / 2) % reactionTime == 0) {
						guardians.push_back(patrol);
					}
				}
			}
			[[nodiscard]] position estimate(const position index) const override {
				return guardian(index).estimate(index);
			}
			void go(const position index) override {
				Walker<element, Container, Combination> envoy = guardian(index);
				envoy.go(index);
				for (position c = 0; c < this->request->length; c++) {
					this->positions[c] = envoy.getPosition(c);
				}
				this->index = index;
			}
		private:
			std::vector<Walker<element, Container, Combination>> guardians;
			position reactionTime;

			Walker<element, Container, Combination> guardian(const position index) const { // TODO: link/reference
				return guardians[index / reactionTime];
			}
	};
	template<class element, class Container, class Combination>
	class Mathematician : public OrderIterator<element, Container, Combination> {
		public:
			explicit Mathematician(
					const FixedRequest<Container>* const request,
					const position index
			) :
					OrderIterator<element, Container, Combination>(request, index) {
				avgEstimation = avgNrSteps();
			}
			[[nodiscard]] position estimate(position index) const override {
				return avgEstimation; // TODO: test
			}
			void go(position index) override {
				this->index = index;
				position nrElements(this->nrElements());
				for (position c = 0; c < this->request->length; c++) {
					step _step = getStep(c, nrElements, index);
					nrElements -= _step.x + 1;
					index -= _step.beginningOfX;
					insertUnique(c, _step.x);
				}
			}
		private:
			position avgEstimation;
			struct step {
				step(
						const position x,
						const position beggingOfX
				): x(x), beginningOfX(beggingOfX) {}
				position x;
				position beginningOfX;
			};
			step getStep(
					const position _position,
					const position nrElements,
					const position index
			) const {
				if (_position == this->request->length - 1) return step(index, index);
				step res(0, 0);
				position lastNPerM(0);
				while (res.beginningOfX <= index) { // TODO: optimize
					res.beginningOfX += lastNPerM = this->nPerM(
							nrElements - res.x - 1,
							this->request->length - _position - 1
					);
					res.x++;
				}
				if (res.x > 0) --res.x;
				res.beginningOfX -= lastNPerM;
				return res;
			}
			void insertUnique(const position _position, position value) const {
				if (_position > 0) value += this->positions[_position - 1] + 1;
				this->positions[_position] = value;
			}
			[[nodiscard]] position avgNrSteps() const {
				float totalAvgNrSteps(0.f);
				position prevValue(0);
				for (position c = 0; c < this->request->length; c++) {
					float nrSteps = avgNrSteps(c, prevValue);
					prevValue = getStep(
							c,
							this->nrElements() - prevValue,
							(position)nrSteps
					).x;
					totalAvgNrSteps += nrSteps;
				}
				return (position)totalAvgNrSteps;
			}
			[[nodiscard]] float avgNrSteps(
					const position _position,
					const position minValue
			) const {
				float sum(0.f);
				position maxValue = this->nrElements()
									+ _position
									+ 1
									- this->request->length;
				for (position c = minValue; c < maxValue; c++)
					sum += this->nPerM(
							this->nrElements() - c - 1,
							this->request->length - _position - 1
					);
				return sum / float(maxValue - minValue + 1);
			}
	};
	template<class element, class Container, class Combination>
	class ComboIterator : public OrderIterator<element, Container, Combination> {
		private:
			mutable std::vector<OrderIterator<element, Container, Combination>*> iterators; // TODO: why mutable?
		public:
			ComboIterator(
					const FixedRequest<Container>* const request,
					const position index
			) :
					OrderIterator<element, Container, Combination>(request, index),
					iterators() {
				iterators.push_back(new Hunter<element, Container, Combination>(request, index));
				iterators.push_back(new Mathematician<element, Container, Combination>(request, index));
			}
			~ComboIterator() {
				for (auto* iterator : iterators) delete iterator;
			}
			void go(position index) override {
				auto estimated((position)-1);
				OrderIterator<element, Container, Combination>* chosen(nullptr);
				for (auto* iterator : iterators) {
					position myBet = iterator->estimate(index);
					if (myBet < estimated) {
						chosen = iterator;
						estimated = myBet;
					}
				}
				Assert(chosen != nullptr);
				chosen->go(index);
				for (position c = 0; c < this->request->length; c++) {
					this->positions[c] = chosen->getPosition(c);
				}
				this->index = index;
			}
			[[nodiscard]] position estimate(position index) const { // TODO: remove
				return 0;
			}
	};

    template<class element, class Container, class Combination>
    class OrderedCombinator : public FixedCombinator<element, Container, Combination, ComboIterator<element, Container, Combination>> {
        public:
			OrderedCombinator(Container elements, const position length) :
					FixedCombinator<element, Container, Combination, ComboIterator<element, Container, Combination>>(
							FixedRequest<Combination>(elements, length)
					) {}
    };

	template<class element, class Container, class Combination>
	class ShuffleIterator : public RandomAccessIterator<element, Container, Combination> {
		public:
			ShuffleIterator(
					const FixedRequest<Container>* const request,
					const position index
			) :
					RandomAccessIterator<element, Container, Combination>(request, index),
					_size(nPerM(request->elements.size(), request->length)) {}
			void go(position index) override {
				this->index = index;
				position nrElements(this->nrElements()); // TODO: remove?
				for (position c = 0; c < this->request->length; c++) {
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
			[[nodiscard]] position size() const override {
				return _size;
			}
		private:
			const position _size;

			void insertUnique(
					const position _position,
					position value
			) {
				auto prevValue((position)-1); // TODO: 0
				do {
					position add(0); // TODO: move up?
					for (position c = 0; c < _position; c++) {
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
				this->positions[_position] = value;
			}
			static position nPerM(const position n, const position m) {
				return nPerM0(n, m);
			}
			static position nPerM0(const position n, const position m) {
				if (m > 1) return n * nPerM0(n - 1, m - 1);
				return n;
			}
			static position nPerM1(position n, position m) {
				position res(n);
				while (m --> 1) res *= --n;
				return res;
			}
			static position nPerM2(position n, position m) {
				position res(n);
				while (--m > 0) res *= --n;
				return res;
			}
	};

    template<class element, class Container, class Combination>
    class ShuffledCombinator : public FixedCombinator<element, Container, Combination, ShuffleIterator<element, Container, Combination>> {
        public:
			ShuffledCombinator(Container elements, const position length):
					FixedCombinator<element, Container, Combination, ShuffleIterator<element, Container, Combination>>(
							FixedRequest<Combination>(elements, length)
					) {}
    };
}
