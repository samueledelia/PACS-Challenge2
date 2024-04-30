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
## Notes for Usage

### Storage Techniques

This library supports two storage techniques:

1. **Coordinate Map:**
    - This dynamic storage technique allows for easy addition of new non-zero elements.
    - It is characterized by the private member `data`, which is a `std::map<std::array<std::size_t, 2>, T>`.

2. **Compressed Sparse Row/Column (CSR/CSC):**
    - This compressed storage technique optimizes memory usage by storing only non-zero elements and their indices.
    - It consists of three vectors:
        - `first_indexes`: Contains the starting index for non-zero elements of each row (or column for CSC), plus one.
        - `second_indexes`: Contains the corresponding column (or row for CSC) indices of non-zero elements.
        - `values`: Stores the values of non-zero elements.

### Public Methods

Within the `algebra::Sparse_Matrix` class, the following public methods are available:

- Constructors:
    - Default constructor
    - Constructors accepting parameters for the number of rows, columns, and compression boolean
    - Constructor accepting the filename of a Matrix Market format file
- `read_mtx` method: Reads a Matrix Market format file or overwrites the matrix with new data from such a file.
- `print` method: Prints the matrix in Matrix Market format.
- `resize` method: Resizes the matrix while adjusting the stored elements accordingly.
- `compress` method: Converts the matrix to a compressed format (CSR/CSC).
- `uncompress` method: Reverts the compressed matrix to the coordinate map format.
- `is_compressed` method: Checks if the matrix is in compressed format.
- Const and non-const call operators: Allow for element access and modification.
- `get_row` and `get_cols` methods: Retrieve the number of rows and columns, respectively.

### Matrix-Vector Multiplication (* operator)

The `*` operator has been overloaded to facilitate matrix-vector multiplication with sparse matrices. This operator allows you to multiply a sparse matrix (`algebra::Sparse_Matrix`) by a vector (`std::vector<T>`) efficiently.

The syntax is:

```cpp
std::vector<T> result = sparse_matrix * vector;
```

- `sparse_matrix`: The sparse matrix object of type algebra::Sparse_Matrix<T, Order> to be multiplied.
- `vector`: The vector object of type std::vector<T> to be multiplied.