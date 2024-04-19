# Challenge 2 of the PACS Course (Sparse Matrix Library)

This is a C++ library for working with sparse matrices. Sparse matrices are matrices that have a large number of zero elements. This library provides a way to efficiently store and manipulate sparse matrices in memory.


## Usage

To use this library in your C++ project, follow these steps:

1. Clone the repository to your local machine:

```
git clone git@github.com:samueledelia/PACS-Challenge2.git
```
2. Include the `Sparse_Matrix.hpp` header file in your project:

```cpp
#include "Sparse_Matrix.hpp"
```
3. Run the provided tests to see examples of library usage and to verify code integrity and to measure its performance.

4. Use the algebra::Sparse_Matrix class to work with sparse matrices!!

## Things to do:
- Implementation of sparse matrices using different storage formats
- Support for matrix resizing, compression, and access operations
- Matrix-vector multiplication
- Header-only implementation for easy integration into existing projects
- Everyone should be able to run the Makefile!
- Try to learn to use Doxygen to comment on your function/classes/methods.
- Add Chrono.hpp to time the speed!