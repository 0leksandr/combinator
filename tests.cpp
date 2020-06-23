#pragma once

#include <vector>
#include "Combinator.cpp"
#include "my/macro.cpp"

using namespace Combinator;

void testOrdered() {
	std::vector<double> vec;
	for (unsigned c = 0; c < 4; c++) vec.push_back(c + 1);

	const unsigned NR_ELEMENTS_IN_COMBINATION = 2;
	OrderedCombinator<double, std::vector<double>, std::vector<double>> combinations(
			vec,
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
	position c(0);
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
void testShuffled() {
	std::vector<double> vec;
	for (unsigned c = 0; c < 4; c++)
		vec.push_back(c + 1);

	const unsigned NR_ELEMENTS_IN_COMBINATION = 2;
	ShuffledCombinator<double, std::vector<double>, std::vector<double>> combinations(
			vec,
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
	position c(0);
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
	testOrdered();
	testShuffled();
}
