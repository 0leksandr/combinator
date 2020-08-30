# combinator

Service, generating combinations of subsets from passed collection (array/std::array/vector, etc.).

F.e.,
```c++
OrderedCombinator<std::vector<int>>({1, 2, 3}, 2) = {
    {1, 2},
    {1, 3},
    {2, 3},
};
Permutator<std::vector<int>>({1, 2, 3}, 2) = {
    {1, 2},
    {1, 3},
    {2, 1},
    {2, 3},
    {3, 1},
    {3, 2},
};
MultiPermutator<std::vector<int>>({1, 2, 3}, 2) = {
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
Cartesian<std::vector<int>, 2, std::vector<int>>(
    std::array<std::vector<int>, 2>{std::vector<int>{1, 2}, {3, 4}}
) = {
    {1, 3},    
    {2, 3},
    {1, 4},
    {2, 4},
};
```

Functions as random-access iterator, so that allows to generate arbitrary big sequenses, requiring minimum amount of memory.

Accepts collections of any objects (integers are shows as example).

Returns iterables of different types (array/std::array/vector); required type should be declared as template argument when creating combinator object.
