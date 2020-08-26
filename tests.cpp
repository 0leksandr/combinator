#pragma once

#include <array>
#include <cmath>
#include <map>
#include <random>
#include <vector>
#include "Combinator.h"
#include <my/macro.cpp>

// TODO: test combinations of combinations

using namespace Combinator;

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

template<class Combination>
void testOrdered() {
	const unsigned nrElementsInCombination = 2;
	OrderedCombinator<std::vector<double>, Combination> combinations(
			std::vector<double>({1, 2, 3, 4}),
			nrElementsInCombination
	);
	const unsigned nrCombinations = 6;
	double expectedOrdered[nrCombinations][nrElementsInCombination] = {
			{1., 2.},
			{1., 3.},
			{1., 4.},
			{2., 3.},
			{2., 4.},
			{3., 4.},
	};
	Assert(combinations.size() == nrCombinations);
	unsigned c(0);
	for (auto combination : combinations) {
		Assert(combination.size() == nrElementsInCombination);
		Assert(combinations[c].size() == nrElementsInCombination);
		for (unsigned d = 0; d < nrElementsInCombination; d++) {
			double expected = expectedOrdered[c][d];
			Assert(combination[d] == expected);
			Assert(combinations[c][d] == expected);
		}
		c++;
	}
	myPrint("Test passed\n");
}
template<class Combination>
void testShuffled() {
	const unsigned NR_ELEMENTS_IN_COMBINATION = 2;
	ShuffledCombinator<std::vector<double>, Combination> combinations(
			std::vector<double>({1, 2, 3, 4}),
			NR_ELEMENTS_IN_COMBINATION
	);
	const unsigned NR_COMBINATIONS = 12;
	double expectedShuffled[NR_COMBINATIONS][2] = {
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
	Assert(combinations.size() == NR_COMBINATIONS);
	unsigned c(0);
	for (auto combination : combinations) {
		Assert(combination.size() == NR_ELEMENTS_IN_COMBINATION);
		Assert(combinations[c].size() == NR_ELEMENTS_IN_COMBINATION);
		for (unsigned d = 0; d < NR_ELEMENTS_IN_COMBINATION; d++) {
			double expected = expectedShuffled[c][d];
			Assert(combination[d] == expected);
			Assert(combinations[c][d] == expected);
		}
		c++;
	}
	myPrint("Test passed\n");
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
	myPrint("Test passed\n");
}
template<class Container, Position NrContainers, class Combination>
void testCompose(
		const std::array<Container, NrContainers>& containers,
		const unsigned expectedNrCombinations
) {
	const auto combinator = ComposeCombinator<Container, NrContainers, Combination>(containers);
	Assert(combinator.size() == expectedNrCombinations);
	std::vector<Combination> combinations;
	combinations.reserve(combinator.size());
	unsigned c(0);
	for (const auto& combination : combinator) {
		assertEquals(combination, combinator[c++]);
		for (int d = 0; d < NrContainers; ++d) assertIn(containers[d], combination[d]);
		combinations.push_back(combination);
	}
	Assert(combinations.size() == expectedNrCombinations);
	assertCombinationsUnique(combinations);
	myPrint("Test passed\n");
}
void tests() {
	testOrdered<std::vector<double>>();
	testOrdered<std::array<double, 2>>();
	testShuffled<std::vector<double>>();
	testShuffled<std::array<double, 2>>();

	#define INPUT1 <std::vector<double>>({1, 2, 3, 4}, 2)
	testList<std::vector<double>>(OrderedCombinator INPUT1, 6, true, true);
	testList<std::vector<double>>(ShuffledCombinator INPUT1, 12, false, true);
	testList<std::vector<double>>(MultiChoiceCombinator INPUT1, 16, false, false);

	#define COMBINATION std::array<double, 3>
	#define INPUT2 <std::array<double, 8>, COMBINATION>({1, 2, 3, 4, 5, 6, 7, 8}, 3)
	testList<COMBINATION >(OrderedCombinator INPUT2, 56, true, true);
	testList<COMBINATION >(ShuffledCombinator INPUT2, 336, false, true);
	testList<COMBINATION >(MultiChoiceCombinator INPUT2, 512, false, false);

	testCompose<std::vector<int>, 2, std::vector<int>>(
			std::array<std::vector<int>, 2>{
					std::vector<int>{1, 2},
					std::vector<int>{3, 4},
			},
			4
	);
	testCompose<std::vector<int>, 4, std::vector<int>>(
			std::array<std::vector<int>, 4>{
					std::vector<int>{1, 2, 3},
					std::vector<int>{3, 4, 5},
					std::vector<int>{5, 6, 7},
					std::vector<int>{7, 8, 9},
			},
			81
	);
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
		if (!(c%10000)) dump(float(c)/expectedNrCombinations);
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
					OrderedCombinator<std::vector<unsigned>>(elements, 3),
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
					ShuffledCombinator<std::vector<unsigned>>(elements, 3),
					nrElements * (nrElements - 1) * (nrElements - 2),
					rand
			);

			testForwardAndRAEquality(
					MultiChoiceCombinator<std::vector<unsigned>>(elements, 3),
					nrElements * nrElements * nrElements,
					rand
			);
		}
	}
}
