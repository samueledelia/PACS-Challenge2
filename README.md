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
## Notes for the usage
The matrix is implemented using a class template of the following kind `Matrix<T,StorageOrder>`
There are two kind of storage technique you can use:
- Coordinate map: is a dynamic storage techinique and allow to add new non zero easly. It is characterize by the following private member:
    - `data`: using the following type `std::map<std::array<std::size_t, 2>, T>`
- Compressed Sparse Row/Column: is a Compressed storage techniques. we store three vector:
    - `first_indexes`: of length the number of rows ( or column in case of CSC) plus one, contains the starting index for the elements of each row ( column).
    - `second_indexes`: of length the number of nonzeroes, contains the corresponding column (rows) index.
    - `values`: is the vector of values, again of length the number of non-zeroes.
Regarding the call operator we can make some observations:
- you cannot add a element in compressed state but only change the state of an existing non zero element
## Things to do:
- compress and uncompress method
- Matrix-vector multiplication
- Header-only implementation for easy integration into existing projects
- Everyone should be able to run the Makefile!
- Try to learn to use Doxygen to comment on your function/classes/methods.
- Add Chrono.hpp to time the speed!