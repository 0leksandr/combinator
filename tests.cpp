#pragma once

#include <vector>
#include "Combinator.h"
#include "my/macro.cpp"

// TODO: test combinations of combinations
// TODO: check RandomAccessListIterator with big "jumps"
// TODO: check case with Combination = collection of addresses of elements (`Converter::getElement`)

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

template<class Combination> void testOrdered() {
	const unsigned NR_ELEMENTS_IN_COMBINATION = 2;
	OrderedCombinator<std::vector<double>, Combination> combinations(
			std::vector<double>({1, 2, 3, 4}),
			NR_ELEMENTS_IN_COMBINATION
	);
	const unsigned NR_COMBINATIONS = 6;
	double expectedOrdered[NR_COMBINATIONS][2] = {
			{1., 2.},
			{1., 3.},
			{1., 4.},
			{2., 3.},
			{2., 4.},
			{3., 4.},
	};
	Assert(combinations.size() == NR_COMBINATIONS);
	unsigned c(0);
	for (auto combination : combinations) {
		Assert(combination.size() == NR_ELEMENTS_IN_COMBINATION);
		Assert(combinations[c].size() == NR_ELEMENTS_IN_COMBINATION);
		for (unsigned d = 0; d < NR_ELEMENTS_IN_COMBINATION; d++) {
			double expected = expectedOrdered[c][d];
			Assert(combination[d] == expected);
			Assert(combinations[c][d] == expected);
		}
		c++;
	}
	myPrint("Test passed\n");
}
template<class Combination> void testShuffled() {
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
template<class Container, class Combination, class Combinator>
void test(
		const Container& input,
		const unsigned combinationLength,
		const unsigned expectedNrCombinations,
		const bool expectOrdered,
		const bool expectElementsUnique
) {
	auto combinator = Combinator(input, combinationLength);
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
void tests() {
	testOrdered<std::vector<double>>();
	testOrdered<std::array<double, 2>>();
	testShuffled<std::vector<double>>();
	testShuffled<std::array<double, 2>>();

	test<
	        std::vector<double>,
			std::vector<double>,
			OrderedCombinator<std::vector<double>>
	>({1, 2, 3, 4}, 2, 6, true, true);
	test<
	        std::vector<double>,
			std::vector<double>,
			ShuffledCombinator<std::vector<double>>
	>({1, 2, 3, 4}, 2, 12, false, true);
	test<
			std::vector<double>,
			std::vector<double>,
			MultiChoiceCombinator<std::vector<double>>
	>({1, 2, 3, 4}, 2, 16, false, false);

	test<
			std::array<double, 8>,
			std::array<double, 3>,
			OrderedCombinator<std::array<double, 8>, std::array<double, 3>>
	>({1, 2, 3, 4, 5, 6, 7, 8}, 3, 56, true, true);
	test<
			std::array<double, 8>,
			std::array<double, 3>,
			ShuffledCombinator<std::array<double, 8>, std::array<double, 3>>
	>({1, 2, 3, 4, 5, 6, 7, 8}, 3, 336, false, true);
	test<
			std::array<double, 8>,
			std::array<double, 3>,
			MultiChoiceCombinator<std::array<double, 8>, std::array<double, 3>>
	>({1, 2, 3, 4, 5, 6, 7, 8}, 3, 512, false, false);
}
