#pragma once

#include <vector>
#include <cmath>
#include <array>
#include <boost/assert.hpp>

namespace Combinator {
    typedef unsigned long position; // TODO: Position

    template<class element, class Container, class Combination> // TODO: Element
    class ForwardIterator;

	#define Assert BOOST_ASSERT

//	template<class element>
//	class Container {
//		public:
//			virtual element operator[](position _position) const = 0;
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

    template<class element, class Container, class Combination>
    class FixedCombinator {
            friend class ForwardIterator<element, Container, Combination>;
        public:
			virtual Combination& operator[](position index) const = 0;
			[[nodiscard]] position size() const {
						Assert(_size > 0);
				return _size;
			}
            ForwardIterator<element, Container, Combination> begin() const {
                return ForwardIterator<element, Container, Combination>(this, 0);
            }
            ForwardIterator<element, Container, Combination> end() const {
                return ForwardIterator<element, Container, Combination>(this, this->size());
            }
        protected:
    		const FixedRequest<Combination> request;
			const position _size;
            mutable ForwardIterator<element, Container, Combination> current;

            virtual void next(ForwardIterator<element, Container, Combination>& iterator) const = 0;
            FixedCombinator(Container elements, const position length, const position size) :
					request(elements, length),
					_size(size),
                    current(this, 0) {}
            FixedCombinator(const FixedCombinator<element, Container, Combination>& other) :
					request(other.request),
					_size(other._size),
                    current(this, 0) {}
			[[nodiscard]] position nrElements() const {
				return request.elements.size();
			}
			void first(position* const positions) const {
				for (position c = 0; c < this->request.length; c++) positions[c] = c;
			}
    };

    template<class element, class Container, class Combination = Container>
    class OrderedCombinator;
    template<class element, class Container, class Combination = Container>
    class ShuffledCombinator;

    template<class element, class Container, class Combination>
    class ForwardIterator {
            friend class OrderedCombinator<element, Container, Combination>;
            friend class ShuffledCombinator<element, Container, Combination>;
            class Converter;
        public:
            ForwardIterator(
                    const FixedCombinator<element, Container, Combination>* const combinator,
                    const position index
            ):
                    combinator(combinator),
                    request(&combinator->request),
                    index(index),
                    positions(new position[combinator->request.length]),
                    converter(&combinator->request),
                    combination(converter.construct(&combination)) {
                combinator->first(positions);
            }
            ~ForwardIterator() {
                delete[] positions;
                converter.destruct(combination);
            }
            void operator++() {
                combinator->next(*this);
                index++;
            }
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
            position index;
            position* const positions;
        private:
            const FixedCombinator<element, Container, Combination>* const combinator;
            const FixedRequest<Container>* const request;
            const Converter converter;
            Combination combination;

            class Converter {
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
                    template<unsigned long size>
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
    class OrderedCombinator : public FixedCombinator<element, Container, Combination> {
            class OrderIterator;
            class Hunter;
            class Mathematician;
        public:
            OrderedCombinator(Container elements, const position length):
                    FixedCombinator<element, Container, Combination>(
                            elements,
                            length,
                            this->nPerM(elements.size(), length)
                    ),
                    iterators() {}
			OrderedCombinator(const OrderedCombinator<element, Container, Combination>& other):
					FixedCombinator<element, Container, Combination>(other),
					iterators() {}
			~OrderedCombinator() {
				for (auto* iterator : iterators) delete iterator;
			}
            Combination& operator[](position index) const override {
                if (iterators.empty()) initIterators();
                auto estimated((position)-1);
                OrderIterator* chosen(nullptr);
                for (auto* iterator : iterators) {
                    position myBet = iterator->estimate(index);
                    if (myBet < estimated) {
                        chosen = iterator;
                        estimated = myBet;
                    }
                }
                Assert(chosen != nullptr);
                chosen->go(index);
                for (position c = 0; c < this->request.length; c++) {
					this->current.positions[c] = chosen->positions[c];
				}
                return *this->current;
            }
        protected:
            void next(ForwardIterator<element, Container, Combination>& iterator) const override {
                next(iterator.positions);
            }
        private:
            mutable std::vector<OrderIterator*> iterators;
            void initIterators() const {
                iterators.push_back(new Hunter(this));
                iterators.push_back(new Mathematician(this));
            }
            void increment(position* const positions, const position _position) const {
                if (++positions[_position] > maxPosition(_position)) {
                    if (_position != 0) { // preventing fail on end
                        increment(positions, _position - 1);
                        positions[_position] = positions[_position - 1] + 1;
                    }
                }
            }
            void decrement(position* const positions, const position _position) const {
                --positions[_position];
                if (_position > 0 && positions[_position] == positions[_position - 1]) {
                    for (position c = _position; c < this->request.length; c++) positions[c] = maxPosition(c);
                    decrement(positions, _position - 1);
                }
            }
            void next(position* const positions) const {
                increment(positions, this->request.length - 1);
            }
            void previous(position* const positions) const {
                decrement(positions, this->request.length - 1);
            }
            position maxPosition(const position _position) const { // TODO: inline?
                return this->nrElements() + _position - this->request.length;
            }
            template<typename Float = float>
            position nPerM(const position n, const position m) const {
                Float res(1.);
                for (position c = 0; c < m; c++) {
                    res *= Float(n - c);
                    res /= Float(c + 1);
                }
                return (position)res;
            }

            class OrderIterator {
                public:
					position* const positions;
                    explicit OrderIterator(const OrderedCombinator<
                            element,
                            Container,
                            Combination
                    >* const combinator):
                            positions(new position[combinator->request.length]),
                            combinator(combinator) {}
                    ~OrderIterator() {
                        delete[] positions;
                    }
                    [[nodiscard]] virtual position estimate(position index) const = 0;
                    virtual void go(position index) = 0;
                protected:
                    const OrderedCombinator<element, Container, Combination>* const combinator;
            };
            class Walker : public OrderIterator {
                public:
					position location;
                    explicit Walker(const OrderedCombinator<
                            element,
                            Container,
                            Combination
                    >* const combinator):
                            OrderIterator(combinator),
                            location(0) {
                        this->combinator->first(this->positions);
                    }
					Walker(const Walker& other):
							OrderIterator(other.combinator),
							location(other.location) {
						for (position c = 0; c < this->combinator->request.length; c++) {
							this->positions[c] = other.positions[c];
						}
					}
                    [[nodiscard]] position estimate(const position index) const override {
                        if (index > location) return index - location;
                        if (index < location) return location - index;
						return 0;
                    }
                    void go(const position index) override {
                        while (location < index) forward();
                        while (location > index) back();
                    }
                    void forward() {
                        this->combinator->next(this->positions);
                        ++location;
                    }
                    void back() {
                        this->combinator->previous(this->positions);
                        --location;
                    }
            };
            class Hunter : public OrderIterator {
                public:
                    explicit Hunter(const OrderedCombinator<
                            element,
                            Container,
                            Combination
                    >* const combinator):
                            OrderIterator(combinator),
                            nrElements(combinator->nrElements()) {
                        const position nrGuardians = (position)sqrt(this->combinator->size()) + 1;
                        reactionTime = this->combinator->size() / nrGuardians; // TODO: check
                        Walker patrol(this->combinator);
                        while (patrol.location < this->combinator->size() - 1) {
                            patrol.forward();
                            if ((patrol.location + reactionTime / 2) % reactionTime == 0) {
								guardians.push_back(patrol);
							}
                        }
                    }
                    [[nodiscard]] position estimate(const position index) const override {
                        return guardian(index).estimate(index);
                    }
                    void go(const position index) override {
                        Walker envoy = guardian(index);
                        envoy.go(index);
                        for (position c = 0; c < this->combinator->request.length; c++)
                            this->positions[c] = envoy.positions[c];
                    }
                private:
                    std::vector<Walker> guardians;
                    const position nrElements;
                    position reactionTime;
                    Walker guardian(const position index) const {
                        return guardians[index / reactionTime];
                    }
            };
            class Mathematician : public OrderIterator {
                public:
                    explicit Mathematician(const OrderedCombinator<
                            element,
                            Container,
                            Combination
                    >* const combinator):
                            OrderIterator(combinator) {
                        avgEstimation = avgNrSteps();
                    }
                    [[nodiscard]] position estimate(position index) const override {
                        return avgEstimation; // TODO: test
                    }
                    void go(position index) override {
                        position nrElements(this->combinator->nrElements());
                        for (position c = 0; c < this->combinator->request.length; c++) {
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
                        if (_position == this->combinator->request.length - 1)
                            return step(index, index);
                        step res(0, 0);
                        position lastNPerM(0);
                        while (res.beginningOfX <= index) { // TODO: optimize
                            res.beginningOfX += lastNPerM = this->combinator->nPerM(
                                    nrElements - res.x - 1,
                                    this->combinator->request.length - _position - 1
                            );
                            res.x++;
                        }
                        if (res.x > 0)
                            --res.x;
                        res.beginningOfX -= lastNPerM;
                        return res;
                    }
                    void insertUnique(const position _position, position value) const {
                        if (_position > 0)
                            value += this->positions[_position - 1] + 1;
                        this->positions[_position] = value;
                    }
                    [[nodiscard]] position avgNrSteps() const {
                        float totalAvgNrSteps(0.f);
                        position prevValue(0);
                        for (position c = 0; c < this->combinator->request.length; c++) {
                            float nrSteps = avgNrSteps(c, prevValue);
                            prevValue = getStep(
                                    c,
                                    this->combinator->nrElements() - prevValue,
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
                        position maxValue = this->combinator->nrElements()
                                            + _position
                                            + 1
                                            - this->combinator->request.length;
                        for (position c = minValue; c < maxValue; c++)
                            sum += this->combinator->nPerM(
                                    this->combinator->nrElements() - c - 1,
                                    this->combinator->request.length - _position - 1
                            );
                        return sum / float(maxValue - minValue + 1);
                    }
            };
    };

    template<class element, class Container, class Combination>
    class ShuffledCombinator : public FixedCombinator<element, Container, Combination> {
        public:
            ShuffledCombinator(Container elements, const position length):
                    FixedCombinator<element, Container, Combination>(
                            elements,
                            length,
                            this->nPerM(elements.size(), length)
                    ) {}
            Combination& operator[](const position index) const override {
                move(this->current, index);
                return *this->current;
            }
        protected:
            void next(ForwardIterator<element, Container, Combination>& iterator) const override {
                move(iterator, iterator.index + 1);
            }
        private:
            void move(
                    const ForwardIterator<element, Container, Combination>& iterator,
                    position index
            ) const {
                position nrElements(this->nrElements());
                for (position c = 0; c < this->request.length; c++) {
                    insertUnique(iterator, c, index % nrElements);
                    index /= nrElements--;
                }
            }
            void insertUnique(
                    const ForwardIterator<element, Container, Combination>& iterator,
                    const position _position,
                    position value
            ) const {
                auto prevValue((position)-1);
                do {
                    position add(0); // TODO: move up?
                    for (position c = 0; c < _position; c++) {
						if ((prevValue + 1 < iterator.positions[c] + 1) &&
							(iterator.positions[c] <= value)
						) {
							++add;
						}
					}
                    prevValue = value;
                    value += add;
                } while (prevValue != value);
                iterator.positions[_position] = value;
            }
            [[nodiscard]] position nPerM(const position n, const position m) const {
                return nPerM0(n, m);
            }
            [[nodiscard]] position nPerM0(const position n, const position m) const {
                if (m > 1)
                    return n * nPerM0(n - 1, m - 1);
                return n;
            }
            [[nodiscard]] position nPerM1(position n, position m) const {
                position res(n);
                while (m --> 1)
                    res *= --n;
                return res;
            }
            [[nodiscard]] position nPerM2(position n, position m) const {
                position res(n);
                while (--m > 0)
                    res *= --n;
                return res;
            }
    };
}
