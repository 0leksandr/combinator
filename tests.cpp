#pragma once

#include <vector>
#include "Combinator.cpp"
#include "my/macro.cpp"

// TODO: test combinations of combinations
// TODO: test combinations by checking: all combinations are unique, nr of combinations, (elements are ordered), (elements are unique)

using namespace Combinator;

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
void tests() {
	testOrdered<std::vector<double>>();
	testOrdered<std::array<double, 2>>();
	testShuffled<std::vector<double>>();
	testShuffled<std::array<double, 2>>();
}
