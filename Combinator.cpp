#pragma once

#include <vector>
#include <cmath>
#include <array>
#include <boost/assert.hpp>

#include <iostream> // TODO: remove

// TODO: UnorderedCombinator from variadic list of collections
// TODO: ShuffledCombinator::begin and MultiChoiceCombinator::begin return some Walker
// TODO: remove Container from main template

namespace Combinator {
    typedef size_t Position;

	#define Assert BOOST_ASSERT

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

	template<class Container>
	class Converter {
		public:
			explicit Converter(const FixedRequest<Container>* const request) :
					request(request) {}
			template<class Element>
			std::vector<Element> construct(std::vector<Element>*) const {
				std::vector<Element> vec;
				initVector(vec);
				return vec;
			}
			template<class Element, unsigned long size>
			std::array<Element, size> construct(std::array<Element, size>*) const {
				Assert(size == request->length);
				return std::array<Element, size>();
			}
			template<class Element>
			Element* construct(Element**) const {
				return new Element[request->length];
			}
			template<class C>
			C construct(C*) const {
				throw std::runtime_error("Unsupported combination type");
			}

			template<class Element>
			void prepare(std::vector<Element>& combination) const {
				if (combination.size() != request->length) {
					combination.clear();
					initVector(combination);
				}
			}
			template<class C>
			void prepare(C& combination) const {}

			template<class Element, size_t size>
			Element* getElement(std::array<Element*, size>*, Element& element) const {
				return &element;
			}
			template<class Element>
			Element* getElement(Element***, Element& element) const {
				return &element;
			}
			template<class C, class Element>
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
			const FixedRequest<Container>* const request;

			template<class Element>
			void initVector(std::vector<Element>& vec) const {
				vec.reserve(request->length);
				for (Position c = 0; c < request->length; c++) {
					vec.push_back(request->elements[c]);
				}
			}
	};

	class IndexedIterator {
		public:
			explicit IndexedIterator(const Position index) : index(index) {}
			bool operator!=(const IndexedIterator& other) const {
				return this->index != other.index;
			}
		protected:
			Position index;
	};
    template<class Container, class Combination>
    class ForwardIterator : public IndexedIterator {
        public:
            explicit ForwardIterator(const FixedRequest<Container>* const request) :
					IndexedIterator(0),
                    request(request),
                    positions(new Position[request->length]),
                    converter(request),
                    combination(converter.construct(&combination)) {}
            ~ForwardIterator() {
                delete[] positions;
                converter.destruct(combination);
            }

			[[nodiscard]] virtual Position size() const = 0;

            virtual void operator++() = 0;
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
            Position* const positions;

			[[nodiscard]] Position nrElements() const {
				return request->elements.size();
			}
        private:
            const Converter<Container> converter;
            Combination combination;
    };
	template<class Container, class Combination>
	class BackwardsIterator : public ForwardIterator<Container, Combination> {
		public:
			explicit BackwardsIterator(const FixedRequest<Container>* const request) :
					ForwardIterator<Container, Combination>(request) {}
		protected:
			virtual void operator--() = 0;
	};
	template<class Container, class Combination>
	class RandomAccessIterator : public BackwardsIterator<Container, Combination> {
		public:
			explicit RandomAccessIterator(const FixedRequest<Container>* const request) :
					BackwardsIterator<Container, Combination>(request) {}
			virtual void go(Position index) = 0;
	};

	template<class Container, class Combination>
	class UniqueElementsIterator : public RandomAccessIterator<Container, Combination> {
		public:
			explicit UniqueElementsIterator(const FixedRequest<Container>* const request) :
					RandomAccessIterator<Container, Combination>(request) {
				for (Position c = 0; c < request->length; c++) this->positions[c] = c;
			}
	};

	template<class Container, class Combination>
	class OrderIterator : public UniqueElementsIterator<Container, Combination> {
		public:
			explicit OrderIterator(const FixedRequest<Container>* const request) :
					UniqueElementsIterator<Container, Combination>(request),
					_size(nPerM(request->elements.size(), request->length)) {}
			void operator++() override {
				increment(this->request->length - 1);
				++this->index;
			}
			void operator--() override {
				decrement(this->request->length - 1);
				--this->index;
			}
			[[nodiscard]] Position size() const override {
				return _size;
			}
		protected:
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
			[[nodiscard]] Position maxPosition(const Position position) const {
				return this->nrElements() + position - this->request->length;
			}
	};
	template<class Container, class Combination>
	class CandidateOrderIterator : public OrderIterator<Container, Combination> {
		public:
			explicit CandidateOrderIterator(const FixedRequest<Container>* const request) :
					OrderIterator<Container, Combination>(request) {}
			[[nodiscard]] virtual Position estimate(Position index) const = 0;
			[[nodiscard]] Position getPosition(const Position position) const {
				return this->positions[position];
			}
	};
	template<class Container, class Combination>
	class Walker : public CandidateOrderIterator<Container, Combination> {
		public:
			explicit Walker(const FixedRequest<Container>* const request) :
					CandidateOrderIterator<Container, Combination>(request) {}
			Walker(const Walker& other) :
					CandidateOrderIterator<Container, Combination>(other.request) {
				this->index = other.index;
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
				#pragma clang diagnostic push
				#pragma ide diagnostic ignored "bugprone-infinite-loop"
					while (this->index < index) this->operator++();
					while (this->index > index) this->operator--();
				#pragma clang diagnostic pop
			}
			[[nodiscard]] Position getIndex() const {
				return this->index;
			}
	};
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
	template<class Container, class Combination>
	class Mathematician : public CandidateOrderIterator<Container, Combination> {
		public:
			explicit Mathematician(const FixedRequest<Container>* const request) :
					CandidateOrderIterator<Container, Combination>(request) {
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
	template<class Container, class Combination>
	class ComboIterator : public OrderIterator<Container, Combination> {
		private:
			std::vector<CandidateOrderIterator<Container, Combination>*> iterators;
		public:
			explicit ComboIterator(const FixedRequest<Container>* const request) :
					OrderIterator<Container, Combination>(request),
					iterators() {
				iterators.push_back(new Walker<Container, Combination>(request));
				iterators.push_back(new Hunter<Container, Combination>(request));
				iterators.push_back(new Mathematician<Container, Combination>(request));
			}
			~ComboIterator() {
				for (auto* iterator : iterators) delete iterator;
			}
			void go(Position index) override {
				auto estimated((Position)-1);
				CandidateOrderIterator<Container, Combination>* chosen(nullptr);
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
	};

	template<class Container, class Combination>
	class ShuffleIterator : public UniqueElementsIterator<Container, Combination> {
		public:
			explicit ShuffleIterator(const FixedRequest<Container>* const request) :
					UniqueElementsIterator<Container, Combination>(request),
					_size(nPerM(request->elements.size(), request->length)) {}
			void go(Position index) override {
				this->index = index;
				Position nrElements(this->nrElements());
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

	template<class Container, class Combination>
	class MultiChoiceFIterator : public ForwardIterator<Container, Combination> {
		public:
			explicit MultiChoiceFIterator(const FixedRequest<Container>* const request) :
					ForwardIterator<Container, Combination>(request),
					_size(pow(request->elements.size(), request->length)) {
				for (int c = 0; c < request->length; ++c) this->positions[c] = 0;
			}
			void operator++() override {
				increment(0);
				++this->index;
			}
			[[nodiscard]] Position size() const override {
				return _size;
			}
		private:
			const Position _size; // TODO: MultiChoiceSizedIterator?

			static Position pow(const Position a, const Position b) {
				Position res(1);
				for (int c = 0; c < b; ++c) res *= a;
				return res;
			}
			void increment(const Position position) {
				if (position < this->request->length) {
					if (++(this->positions[position]) == this->nrElements()) {
						this->positions[position] = 0;
						increment(position + 1);
					}
				}
			}
	};
	template<class Container, class Combination>
	class MultiChoiceRAIterator : public RandomAccessIterator<Container, Combination> {
		public:
			explicit MultiChoiceRAIterator(const FixedRequest<Container>* const request) :
					RandomAccessIterator<Container, Combination>(request),
					_size(pow(request->elements.size(), request->length)) {
				for (int c = 0; c < request->length; ++c) this->positions[c] = 0;
			}
			void go(Position index) override {
				this->index = index;
				const Position nrElements(this->nrElements());
				for (Position c = 0; c < this->request->length; c++) {
					this->positions[c] = index % nrElements;
					index /= nrElements;
				}
			}
			// TODO: are these necessary?
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
			const Position _size; // TODO: remove

			static Position pow(const Position a, const Position b) {
				Position res(1);
				for (int c = 0; c < b; ++c) res *= a;
				return res;
			}
	};

	template<class Container, class Combination, class ForwardIterator, class RandomAccessIterator>
	class FixedCombinator {
		public:
			Combination& operator[](Position index) const {
				current->go(index);
				return **current;
			}
			ForwardIterator begin() const {
				return ForwardIterator(&request);
			}
			IndexedIterator end() const {
				return _end;
			}
			Position size() const {
				return current->size();
			}
		protected:
			FixedCombinator(const Container& elements, const Position length) :
					request(elements, length),
					current(newIterator()),
					_end(size()) {}
			FixedCombinator(
					const FixedCombinator<
							Container,
							Combination,
							ForwardIterator,
							RandomAccessIterator
					>& other
			) :
					request(other.request),
					current(newIterator()),
					_end(other._end) {}
			~FixedCombinator() {
				delete current;
			}
		private:
			const FixedRequest<Container> request;
			mutable RandomAccessIterator* current;
			const IndexedIterator _end;

			RandomAccessIterator* newIterator() const {
				return new RandomAccessIterator(&request);
			}
	};

	template<class Container, class Combination = Container>
	class OrderedCombinator : public FixedCombinator<
	        Container,
	        Combination,
	        Walker<Container, Combination>,
			ComboIterator<Container, Combination>
	> {
		public:
			OrderedCombinator(const Container& elements, const Position length) :
					FixedCombinator<
					        Container,
					        Combination,
					        Walker<Container, Combination>,
							ComboIterator<Container, Combination>
					>(elements, length) {}
	};

    template<class Container, class Combination = Container>
    class ShuffledCombinator : public FixedCombinator<
            Container,
            Combination,
            ShuffleIterator<Container, Combination>,
			ShuffleIterator<Container, Combination>
	> {
        public:
			ShuffledCombinator(const Container& elements, const Position length):
					FixedCombinator<
					        Container,
					        Combination,
					        ShuffleIterator<Container, Combination>,
							ShuffleIterator<Container, Combination>
					>(elements, length) {}
    };

	template<class Container, class Combination = Container>
	class MultiChoiceCombinator : public FixedCombinator<
			Container,
			Combination,
			MultiChoiceFIterator<Container, Combination>,
			MultiChoiceRAIterator<Container, Combination>
	> {
		public:
			MultiChoiceCombinator(const Container& elements, const Position length):
					FixedCombinator<
							Container,
							Combination,
							MultiChoiceFIterator<Container, Combination>,
							MultiChoiceRAIterator<Container, Combination>
					>(elements, length) {}
	};
}
