# swl::optional
A standard conforming C++20 implementation of `std::optional`. 

C++20 came with new features that makes the implementation of `std::optional` 
(and other types based on unions) a lot simpler to write, faster to compile, and more debugger-friendly 
(ever crawled through all the `std::optional` base classes in your IDE debugger?). \
So if you're using `optional`a lot in C++20, switching to this implementation is free cookies. 

## Testing
The tests were adapted from [TartanLlama's optional](https://github.com/TartanLlama/optional) 
with some bits from the LLVM repo. Kudos to them. \
To build all the tests, compile `./tests/test_main.cpp` including 
the root of the repository in the search path, e.g. : \
`g++ -std=c++20 ./tests/test_main.cpp -I .`

## Tested compilers
- GCC 10+ (the early 10.X versions have a bug in `std::construct_at`, which prevent to use it in a constexpr context, hence the tests might not compile)

TODO : test more compilers
