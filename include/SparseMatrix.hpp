#ifndef SPARSEMATRIX_HPP
#define SPARSEMATRIX_HPP

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <array>
#include <complex>
#include <fstream>
#include <sstream>

enum class StorageOrder {
    RowMajor,
    ColumnMajor
    };


// definition of the comparator for the map of the Matrix class, so that it will be ordered differently based on the StorageOrder this is the default one
template <StorageOrder S>
struct MyTypeComparator {
    bool operator()(const std::array<std::size_t,2>& lhs, const std::array<std::size_t,2>& rhs) const {
            // if the row is equal, the ordering is based on the column, otherwise it is based on the row
            if(lhs[0]==rhs[0])
                return lhs[1] < rhs[1];
            else return lhs[0] < rhs[0];
        }
};

// this is the specialization for the column type matrix, since there are only two possible cases, it chooses the default if it is Row-wise, while it chooses 
// the following if it is column-wise
template <>
struct MyTypeComparator<StorageOrder::ColumnMajor>{
    bool operator()(const std::array<std::size_t,2>& lhs, const std::array<std::size_t,2>& rhs) const {
            // if the column is equal, the ordering is based on the row, otherwise it is based on the column
            if(lhs[1]==rhs[1])
                return lhs[0] < rhs[0];
            else return lhs[1] < rhs[1];
    }
};


namespace algebra // Algebra namespace
{
    template <typename T,StorageOrder Order>
    class SparseMatrix{
    private:
        // Type definitions
        using IndexArray = std::array<std::size_t, 2>;
        using MapType = std::map<IndexArray, T,MyTypeComparator<Order>>;            // map (i,j) --> A_{i,j}
        
        bool compressed;

        // Data members for the Coordinate map (UNCOMPRESSED TECHNIQUE)
        MapType data;
        std::size_t num_rows;
        std::size_t num_cols;
        

        // vectors for the Compressed Sparse Row/Column (COMPRESSED TECHNIQUE)
        std::vector<std::size_t> first_indexes;
        std::vector<std::size_t> second_indexes;
        std::vector<T> values;

    public:
        // Constructors
        SparseMatrix() : num_rows(0), num_cols(0), compressed(false) {}                                        // Default Constructor  
        SparseMatrix(std::size_t rows,std::size_t cols): num_rows(rows), num_cols(cols), compressed(false) {}; // Constructors that takes the number of rows, columes and compressed boolean
        SparseMatrix(std::string filename);                                                                    // Constructor that takes the filename of the Matrix Market format file
        
        // Read mtx file
        void read_mtx(std::string filename);

        // Print in mtx format
        void print();

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
                // Compressed mode
                if constexpr(Order == StorageOrder::RowMajor) {
                    // Search for the element in the compressed row-major format
                    for (std::size_t k = first_indexes[i]; k < first_indexes[i + 1]; ++k) {
                        if (second_indexes[k] == j) {
                            return values[k]; // Element found, return its value
                            }
                        }
                    return 0; // Element not found, return default value
                } else {
                // Search for the element in the compressed column-major format
                    for (std::size_t k = first_indexes[j]; k < first_indexes[j + 1]; ++k) {
                        if (second_indexes[k] == i) {
                            return values[k]; // Element found, return its value
                        }
                    }
            return 0; // Element not found, return default value
        }
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
 //       friend std::vector<T> operator*(const SparseMatrix<T, Order>& mat, const std::vector<T>& vec) {
 //           std::vector<T> result;
 //           // Implementation of matrix-vector multiplication TO BE DONE
 //           //return result;
 //           return 0;   //Placeholder
 //       }
    
    };

}   // namespace algebra







// The following are method of Sparse Matrix Class to be put into the source file (.cpp)
template<typename T, StorageOrder Order>
algebra::SparseMatrix<T, Order>::SparseMatrix(std::string filename) {
    // Open the file
    std::ifstream file(filename);
    // Check if the file is opened successfully
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return;
    }

    // Read the first line of the file to get matrix dimensions
    std::string line;
    std::size_t numRows, numCols, numNonZeros;
    while (getline(file, line) && line[0] == '%');
    std::stringstream ss(line);
    if (!(ss >> numRows >> numCols >> numNonZeros)) {
        std::cerr << "Invalid matrix market file format" << std::endl;
        file.close();
        return;
    }
    num_rows = numRows;
    num_cols = numCols;

    // Read matrix data
    std::size_t row, col;
    T value;
    while (file >> row >> col >> value) {
        // Adjust indices to 0-based indexing and store data in map
        data[{row - 1, col - 1}] = value;
    }
    // Close the file
    file.close();
    std::cout << "Matrix read successfully from file: " << filename << std::endl;
}

template<typename T, StorageOrder Order>
void algebra::SparseMatrix<T, Order>::read_mtx(std::string filename) {
    // Clear previous data
    if (compressed) {
        first_indexes.clear();
        second_indexes.clear();
        values.clear();
    } else {
        data.clear();
    }

    // Open the file
    std::ifstream file(filename);
    // Error handling
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return;
    }

    // Read the first line of the file to get matrix dimensions
    std::string line;
    std::size_t numRows, numCols, numNonZeros;
    while (getline(file, line) && line[0] == '%');
    std::stringstream ss(line);
    if (!(ss >> numRows >> numCols >> numNonZeros)) {
        std::cerr << "Invalid matrix market file format" << std::endl;
        file.close();
        return;
    }
    num_rows = numRows;
    num_cols = numCols;

    // Read matrix data
    std::size_t row, col;
    T value;
    while (file >> row >> col >> value) {
        // Adjust indices to 0-based indexing and store data in map
        data[{row - 1, col - 1}] = value;
    }
    // Close the file
    file.close();

    std::cout << "Sparse Matrix read successfully from file: " << filename << std::endl;
    compressed = false;
}

template<typename T, StorageOrder Order>
void algebra::SparseMatrix<T, Order>::print() {
    if (!compressed) { // UNCOMPRESSED
        // Print matrix dimensions and number of non-zero elements
        std::cout << num_rows << " " << num_cols << " " << data.size() << std::endl;
        // Loop over elements of the map and print them
        for (const auto& iter : data) {
            // Adjust indices to 1-based indexing and print row, column, and value
            std::cout << iter.first[0] + 1 << " " << iter.first[1] + 1 << " " << iter.second << std::endl;
        }
    } else { // COMPRESSED
        // Print matrix dimensions and number of non-zero elements
        std::cout << num_rows << " " << num_cols << " " << values.size() << std::endl;
        if constexpr (Order == StorageOrder::RowMajor) { // ROW MAJOR FOR COMPRESSED
            // Loop over rows
            for (std::size_t i = 0; i < num_rows; i++) {
                // Loop over non-zero elements of the ith row
                for (std::size_t j = first_indexes[i]; j < first_indexes[i + 1]; j++) {
                    // Print row, column, and value
                    std::cout << i + 1 << " " << second_indexes[j] + 1 << " " << values[j] << std::endl;
                }
            }
        } else { // COLUMN MAJOR FOR COMPRESSED
            // Loop over columns
            for (std::size_t i = 0; i < num_cols; i++) {
                // Loop over non-zero elements of ith column
                for (std::size_t j = first_indexes[i]; j < first_indexes[i + 1]; j++) {
                    // Print row, column, and value
                    std::cout << second_indexes[j] + 1 << " " << i + 1 << " " << values[j] << std::endl;
                }
            }
        }
    }
}
#endif  // SPARSEMATRIX_HPP