### Combinator

Service that generates collection of subsets from passed collection(s) (`std::array` or `std::vector`).

Example:
```c++
Combinator(std::vector<int>({1, 2, 3}), 2) ~= {
    {1, 2},
    {1, 3},
    {2, 3},
};
Permutator(std::vector<int>({1, 2, 3}), 2) ~= {
    {1, 2},
    {1, 3},
    {2, 1},
    {2, 3},
    {3, 1},
    {3, 2},
};
MultiPermutator(std::vector<int>({1, 2, 3}), 2) ~= {
    {1, 1},
    {2, 1},
    {3, 1},
    {1, 2},
    {2, 2},
    {3, 2},
    {1, 3},
    {2, 3},
    {3, 3},
};
Cartesian(std::vector<std::vector<int>>({std::vector<int>{1, 2}, {3, 4}})) ~= {
    {1, 3},    
    {2, 3},
    {1, 4},
    {2, 4},
};
```

Functions as forward-iterator and random-access-iterator, so that allows to generate arbitrary big sequenses, requiring minimum amount of memory.

Accepts collections of any objects (integers are shows as example).

Returns sets of `std::array` or `std::vector` (required type should be declared as template argument).
