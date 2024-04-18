#ifndef SPARSE_MATRIX_HPP
#define SPARSE_MATRIX_HPP

#include <iostream>
#include <vector>
#include <map>
#include <array>

namespace algebra // Algebra
{
    enum class StorageOrder {
    RowMajor,
    ColumnMajor
    };

    template <typename T,StorageOrder Order>
    class Sparse_Matrix{
    private:
        // Type definitions
        using IndexArray = std::array<std::size_t, 2>;
        using MapType = std::map<IndexArray, T>;            // map (i,j) --> A_{i,j}
        
        // Data members
        MapType data;
        std::size_t num_rows;
        std::size_t num_cols;
        bool compressed;

    public:
        // Constructor
        Sparse_Matrix() : num_rows(0), num_cols(0), compressed(false) {}

        // Method to resize the Sparse Matrix
 
        void resize(std::size_t rows, std::size_t cols) {
            num_rows = rows;
            num_cols = cols;
            data.clear();
            // Optionally, resize operations could be implemented here
        }

        // Method to compress the Sparse Matrix
        void compress() {
            // ?
            compressed = true;
        }

        // Method to uncompress the matrix
        void uncompress() {
            // ?
            compressed = false;
        }

        // Method to check if the Sparse Matrix is compressed
        bool is_compressed() const {
            return compressed;
        }

        // Const call operator to access elements
        T operator()(std::size_t i, std::size_t j) const {
            if (i >= get_rows() || j >= get_cols())
                return 0; // Out of bounds, return 0
            if (!compressed)
                return data.count({i, j}) ? data.at({i, j}) : 0;
            else {
                // Implementation for accessing elements in compressed mode
            }
        }

        // Non-const call operator for adding elements
        T& operator()(std::size_t i, std::size_t j) {
            if (i >= get_rows() || j >= get_cols())
                throw std::out_of_range("Index out of range");
            if (compressed)
                throw std::logic_error("Cannot add elements in compressed mode");
            return data[{i, j}];
        }

        // Method to get the number of rows
        std::size_t get_rows() const {
            return num_rows;
        }

        // Method to get the number of columns
        std::size_t get_cols() const {
            return num_cols;
        }

 //       // Friend operator for matrix-vector multiplication
 //       friend std::vector<T> operator*(const Sparse_Matrix<T, Order>& mat, const std::vector<T>& vec) {
 //           std::vector<T> result;
 //           // Implementation of matrix-vector multiplication TO BE DONE
 //           //return result;
 //           return 0;   //Placeholder
 //       }
    };

}   // namespace algebra

#endif  // SPARSE_MATRIX_HPP