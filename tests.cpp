#pragma once

#include <array>
#include <cmath>
#include <map>
#include <random>
#include <vector>
#include "Combinator.h"
#include <my/macro.cpp>

// TODO: test combinations of combinations

template<class Container>
void assertEquals(const Container& a, const Container& b) {
	Assert(a.size() == b.size());
	for (int c = 0; c < a.size(); ++c) Assert(a[c] == b[c]);
}
template<class Container>
void assertElementsDiffer(const Container& a, const Container& b) {
	Assert(a.size() == b.size());
	bool equal = true;
	for (int c = 0; c < a.size(); ++c) {
		if (a[c] != b[c]) {
			equal = false;
			break;
		}
	}
	Assert(!equal);
}
template<class Combination>
void assertCombinationsUnique(std::vector<Combination> combinations) {
	for (int c = 0; c < combinations.size(); ++c) {
		for (int d = c+1; d < combinations.size(); ++d) {
			assertElementsDiffer(combinations[c], combinations[d]);
		}
	}
}
template<class Container>
void assertOrdered(const Container& container) {
	if (container.size() > 1) {
		for (int c = 0; c < container.size() - 1; ++c) {
			Assert(container[c] < container[c+1]);
		}
	}
}
template<class Container>
void assertElementsUnique(const Container& container) {
	for (int c = 0; c < container.size(); ++c) {
		for (int d = c+1; d < container.size(); ++d) {
			Assert(container[c] != container[d]);
		}
	}
}
template<class Container, class Element>
void assertIn(const Container& container, const Element& element) {
	for (const auto& e : container) if (e == element) return;
	Assert(false);
}

void testPassed() {
	myPrint("Test passed\n");
}

template<unsigned NrElementsInCombination, unsigned NrCombinations, class Combinator = nullptr_t>
void testCombinator(const Combinator& combinations) {
	double expectedOrdered[NrCombinations][NrElementsInCombination] = {
			{1., 2.},
			{1., 3.},
			{1., 4.},
			{2., 3.},
			{2., 4.},
			{3., 4.},
	};
	Assert(combinations.size() == NrCombinations);
	unsigned c(0);
	for (auto combination : combinations) {
		Assert(combination.size() == NrElementsInCombination);
		Assert(combinations[c].size() == NrElementsInCombination);
		for (unsigned d = 0; d < NrElementsInCombination; d++) {
			double expected = expectedOrdered[c][d];
			Assert(combination[d] == expected);
			Assert(combinations[c][d] == expected);
		}
		c++;
	}
	testPassed();
}
void testCombinatorSameCombination() {
	const unsigned nrElementsInCombination = 2;
	testCombinator<nrElementsInCombination, 6>(Combinator(
			std::vector<double>({1, 2, 3, 4}),
			nrElementsInCombination
	));
}
template<class Combination>
void testCombinatorCustomCombination() {
	const unsigned nrElementsInCombination = 2;
	testCombinator<nrElementsInCombination, 6>(Combinator<std::vector<double>, Combination>(
			std::vector<double>({1, 2, 3, 4}),
			nrElementsInCombination
	));
}
template<unsigned NrElementsInCombination, unsigned NrCombinations, class Permutator = nullptr_t>
void testPermutator(Permutator permutations) {
	double expectedShuffled[NrCombinations][NrElementsInCombination] = {
			{1., 2.},
			{2., 1.},
			{3., 1.},
			{4., 1.},
			{1., 3.},
			{2., 3.},
			{3., 2.},
			{4., 2.},
			{1., 4.},
			{2., 4.},
			{3., 4.},
			{4., 3.},
	};
	Assert(permutations.size() == NrCombinations);
	unsigned c(0);
	for (auto permutation : permutations) {
		Assert(permutation.size() == NrElementsInCombination);
		Assert(permutations[c].size() == NrElementsInCombination);
		for (unsigned d = 0; d < NrElementsInCombination; d++) {
			double expected = expectedShuffled[c][d];
			Assert(permutation[d] == expected);
			Assert(permutations[c][d] == expected);
		}
		c++;
	}
	testPassed();
}
void testPermutatorSameCombination() {
	const unsigned nrElementsInCombination = 2;
	testPermutator<nrElementsInCombination, 12>(Permutator(
			std::vector<double>({1, 2, 3, 4}),
			nrElementsInCombination
	));
}
template<class Combination>
void testPermutatorCustomCombination() {
	const unsigned nrElementsInCombination = 2;
	testPermutator<nrElementsInCombination, 12>(Permutator<std::vector<double>, Combination>(
			std::vector<double>({1, 2, 3, 4}),
			nrElementsInCombination
	));
}
void testPermutatorNoSize() {
	const auto elements = std::vector<double>({1, 2, 3, 4});
	Permutator<std::vector<double>> permutations1(elements,4);
	Permutator<std::vector<double>> permutations2(elements);
	Assert(permutations1.size() == permutations2.size());
	for (int c = 0; c < permutations1.size(); ++c) {
		assertEquals(permutations1[c], permutations2[c]);
	}
	testPassed();
}
template<class Combination, class Combinator>
void testList(
		const Combinator& combinator,
		const unsigned expectedNrCombinations,
		const bool expectOrdered,
		const bool expectElementsUnique
) {
	Assert(combinator.size() == expectedNrCombinations);
	std::vector<Combination> combinations;
	combinations.reserve(combinator.size());
	unsigned c(0);
	for (const auto& combination : combinator) {
		assertEquals(combination, combinator[c++]);
		if (expectOrdered) assertOrdered(combination);
		if (expectElementsUnique) assertElementsUnique(combination);
		combinations.push_back(combination);
	}
	Assert(combinations.size() == expectedNrCombinations);
	assertCombinationsUnique(combinations);
	testPassed();
}
template<class Container, class Combination>
void testCartesian(
		const std::vector<Container>& containers,
		const unsigned expectedNrCombinations
) {
	const auto combinator = Cartesian(containers);
	Assert(combinator.size() == expectedNrCombinations);
	std::vector<Combination> combinations;
	combinations.reserve(combinator.size());
	unsigned c(0);
	for (const auto& combination : combinator) {
		assertEquals(combination, combinator[c++]);
		for (int d = 0; d < containers.size(); ++d) assertIn(containers[d], combination[d]);
		combinations.push_back(combination);
	}
	Assert(combinations.size() == expectedNrCombinations);
	assertCombinationsUnique(combinations);
	testPassed();
}
void testCartesianSizeOverflow() {
	const unsigned nrContainers = 100; // 2^100 > 10e30
	const unsigned nrElementsInContainer = 2;
	std::vector<std::vector<int>> containers;
	containers.reserve(nrContainers);
	for (unsigned c = 0; c < nrContainers; ++c) {
		std::vector<int> container;
		container.reserve(nrElementsInContainer);
		for (int d = 0; d < nrElementsInContainer; ++d) container.push_back(d);
		containers.push_back(container);
	}

	bool exThrown = false;
	try {
		(Cartesian(containers));
	} catch (std::overflow_error&) {
		exThrown = true;
	}
	Assert(exThrown);
	testPassed();
}
void testCartesianConstValues() {
	class NonCopyable {
		public:
			NonCopyable() = default;
			NonCopyable& operator=(const NonCopyable& other) {
				Assert(false);
			}
	};
	for (const auto& c : Cartesian<std::vector<NonCopyable>>{{{NonCopyable{}}, {NonCopyable{}}}}) {}
	testPassed();
}
void tests() {
	testCombinatorSameCombination();
	testCombinatorCustomCombination<std::vector<double>>();
	testCombinatorCustomCombination<std::array<double, 2>>();
	testPermutatorSameCombination();
	testPermutatorCustomCombination<std::vector<double>>();
	testPermutatorCustomCombination<std::array<double, 2>>();
	testPermutatorNoSize();

	#define INPUT1 <std::vector<double>>({1, 2, 3, 4}, 2)
	testList<std::vector<double>>(Combinator INPUT1, 6, true, true);
	testList<std::vector<double>>(Permutator INPUT1, 12, false, true);
	testList<std::vector<double>>(MultiPermutator INPUT1, 16, false, false);

	#define COMBINATION std::array<double, 3>
	#define INPUT2 <std::array<double, 8>, COMBINATION>({1, 2, 3, 4, 5, 6, 7, 8}, 3)
	testList<COMBINATION >(Combinator INPUT2, 56, true, true);
	testList<COMBINATION >(Permutator INPUT2, 336, false, true);
	testList<COMBINATION >(MultiPermutator INPUT2, 512, false, false);

	testCartesian<std::vector<int>, std::vector<int>>(
			std::vector<std::vector<int>>{
					std::vector<int>{1, 2},
					std::vector<int>{3, 4},
			},
			4
	);
	testCartesian<std::vector<int>, std::vector<int>>(
			std::vector<std::vector<int>>{
					std::vector<int>{1, 2, 3},
					std::vector<int>{3, 4, 5},
					std::vector<int>{5, 6, 7},
					std::vector<int>{7, 8, 9},
			},
			81
	);
	testCartesianSizeOverflow();
//	testCartesianConstValues();
}

template<class Combinator, class RandomFunc>
void testForwardAndRAEquality(
		const Combinator& combinator,
		const unsigned expectedNrCombinations,
		RandomFunc rand
) {
	dump(combinator);
	dump(expectedNrCombinations);
	Assert(combinator.size() == expectedNrCombinations);

	std::map<unsigned, std::vector<unsigned>> orderedCombinations;
	std::vector<unsigned> picks;
	picks.reserve(expectedNrCombinations);
	for (int c = 0; c < expectedNrCombinations; ++c) picks.push_back(c);
	std::shuffle(picks.begin(), picks.end(), rand);
	for (int c = 0; c < picks.size(); ++c) {
		if (!(c%100000)) dump(float(c)/expectedNrCombinations);
		const auto pick = picks[c];
		orderedCombinations[pick] = combinator[pick];
	}
	unsigned c(0);
	for (const auto& combination : combinator) {
		assertEquals(combination, orderedCombinations[c++]);
	}
}
void testBigJumps() {
	for (const int seed : {1, 23, 456}) {
		dump(seed);
		auto rand = std::default_random_engine(seed);
		for (const auto nrElements : {10, 50, 150, 300}) {
			dump(nrElements);
			std::vector<unsigned> elements;
			elements.reserve(nrElements);
			for (unsigned c = 0; c < nrElements; ++c) elements.push_back(c);

			testForwardAndRAEquality(
					Combinator<std::vector<unsigned>>(elements, 3),
					1
					* nrElements
					* (nrElements - 1)
					* (nrElements - 2)
					/ 1
					/ 2
					/ 3,
					rand
			);

			testForwardAndRAEquality(
					Permutator<std::vector<unsigned>>(elements, 3),
					nrElements * (nrElements - 1) * (nrElements - 2),
					rand
			);

			testForwardAndRAEquality(
					MultiPermutator<std::vector<unsigned>>(elements, 3),
					nrElements * nrElements * nrElements,
					rand
			);
		}
	}
}
