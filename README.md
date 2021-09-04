# swl::optional
A standard conforming C++20 implementation of `std::optional`. 

C++20 came with new features that makes the implementation of `std::optional` 
(and other types based on unions) a lot simpler to write, faster to compile, and more debugger-friendly 
(ever crawled through all the `std::optional` base classes in your IDE debugger?). \
So if you're using `optional`a lot in C++20, switching to this implementation is free cookies. 

## Testing
The tests were adapted from [TartanLlama's optional](https://github.com/TartanLlama/optional) 
with some bits from the LLVM repo. Kudos to them. \
To run the tests : \
`mkdir ./test_out` \
`cd test_out` \
`cmake ../` \
`ctest --build-and-test ../ ./ --build-generator "Unix Makefiles"` (replace "Unix Makefiles" as needed) \
`make test` 

## Tested compilers
- GCC 11+
- GCC 10+ (the early 10.X versions have a bug in `std::construct_at`, which prevent to use it in a constexpr context, hence the tests might not compile)
- Clang will not fully work as its implementation of C++20 is still incomplete (Clang 12+ will work for trivially destructible type)
