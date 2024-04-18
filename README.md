# Challenge 2 of the PACS Course (Sparse Matrix Library)

This is a C++ library for working with sparse matrices. Sparse matrices are matrices that have a large number of zero elements. This library provides a way to efficiently store and manipulate sparse matrices in memory.

## Features

- Implementation of sparse matrices using different storage formats
- Support for matrix resizing, compression, and access operations
- Basic arithmetic operations (addition, subtraction, multiplication)
- Matrix-vector multiplication
- Header-only implementation for easy integration into existing projects

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
3. Use the algebra::Sparse_Matrix class to work with sparse matrices!!

## Things to do:
-To simplify life for me and my assistants, put in your .bashrc file the statement
export PACS_ROOT=<the directory where the Examples are>
So that in the MAkefile you can indicate with $(PACS_ROOT)/include and $(PACS_ROOT)/lib the folders with the header and libraries of the course. In this way, I do not need to edit your Makefile!
-Try to learn to use Doxygen to comment on your function/classes/methods. There are plenty of examples in the Examples code.
-Printout in low-level functions Normally you do not want cout<< in low-level functions. Printout is made in the main or in specialized functions called at a high level in the code. This is for efficiency since printouts are computationally expensive. Sometimes, you may want to have a verbose option for debugging or testing. This is a case where using CPP macros is fine.  

#ifndef NDEBUG

... print

#endif

or

#ifdef VERBOSE

print

#endif  

Another technique is the callback technique, an example is in the NewtonSolver Example

If it is an error message, use std::cerr not std::cout.
