#ifndef SPARSEMATRIX_HPP
#define SPARSEMATRIX_HPP

#include <iostream>
#include <algorithm>
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
struct MyComparator {
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
struct MyComparator<StorageOrder::ColumnMajor>{
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
        using MapType = std::map<IndexArray, T,MyComparator<Order>>;            // map (i,j) --> A_{i,j}
        
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
        SparseMatrix(std::size_t rows,std::size_t cols, bool compressed_): num_rows(rows), num_cols(cols), compressed(compressed_) {};
        SparseMatrix(std::string filename, bool compressed_);                                                  // Constructor that takes the filename of the Matrix Market format file
        
        // Read mtx file
        void read_mtx(std::string filename);

        // Print in mtx format
        void print();

        // Method to resize the Sparse Matrix
        void resize(std::size_t rows, std::size_t cols);

        // Method to compress the Sparse Matrix
        void compress();

        // Method to uncompress the matrix
        void uncompress();

        // Method to check if the Sparse Matrix is compressed
        bool is_compressed() const {
            return compressed;
        }

        // Const call operator to access elements
        T operator()(std::size_t i, std::size_t j) const;

        // Non-const call operator for adding elements
        T& operator()(std::size_t i, std::size_t j);

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
algebra::SparseMatrix<T, Order>::SparseMatrix(std::string filename, bool compressed_) {
    compressed = compressed_;
    // Open the file
    std::ifstream file(filename);
    // Check if the file is opened successfully
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return;
    }

    // Read the first line of the file to get matrix dimensions
    std::string line;
    // M = num of rows, N = num of cols, L = num of non zero element in the matrix
    std::size_t M, N, L;
    while (getline(file, line) && line[0] == '%');
    std::stringstream ss(line);
    if (!(ss >> M >> N >> L)) {
        std::cerr << "Invalid matrix market file format" << std::endl;
        file.close();
        return;
    }
    num_rows = M;
    num_cols = N;

    if (!compressed){
        // Assemble COOmap matrix
        std::size_t row, col;
        T value;
        while (file >> row >> col >> value) {
            // Adjust indices to 0-based indexing and store data in map
            data[{row - 1, col - 1}] = value;
        }
    }else{
        if constexpr (Order == StorageOrder::ColumnMajor)
            std::cerr << "This option is not implemented!" << std::endl;
        
        int last_row = 1;
	    first_indexes.push_back(1);
	    for (int l = 0; l < L; l++){
	    	int row, col;
	    	T value;
	    	file >> row >> col >> value;
	    	second_indexes.push_back(col);
	    	values.push_back(value);
	    	if (row > last_row){
	    		last_row = row;
	    		first_indexes.push_back(second_indexes.size());
	    	}	
	}
	first_indexes.push_back(second_indexes.size() + 1);
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
    // M = num of rows, N = num of cols, L = num of non zero element in the matrix
    std::size_t M, N, L;
    while (getline(file, line) && line[0] == '%');
    std::stringstream ss(line);
    if (!(ss >> M >> N >> L)) {
        std::cerr << "Invalid matrix market file format" << std::endl;
        file.close();
        return;
    }
    num_rows = M;
    num_cols = N;

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

template<typename T, StorageOrder Order>
void algebra::SparseMatrix<T, Order>::resize(std::size_t rows, std::size_t cols){
    // We cannot resize if the matrix is compressed
    if(is_compressed())
        std::cerr<<"Error: We cannot resize if the matrix is compressed!"<<std::endl;
            
    // New data!!!
    std::map<std::array<std::size_t,2>,T,MyComparator<Order>> new_data;
            
    if constexpr(Order==StorageOrder::RowMajor){            // ROWMAJOR
        for(auto it=data.begin();it!=data.end();it++){
            std::size_t new_row=static_cast<std::size_t>((it->first[0]*num_cols + it->first[1])/cols);
            std::size_t new_col=static_cast<std::size_t>((it->first[0]*num_cols + it->first[1])%cols);
            // definition of the key for the map
            std::array<std::size_t,2> pos={new_row,new_col};
            // assignment of the value in the new map
            new_data[pos]=it->second;
            }
        }
        else{                                               // COLUMN MAJOR
            for(auto it=data.begin();it!=data.end();it++){
                std::size_t new_col=static_cast<std::size_t>((it->first[0] + it->first[1]*num_rows)/rows);
                std::size_t new_row=static_cast<std::size_t>((it->first[0] + it->first[1]*num_rows)%rows);
                // definition of the key for the map
                std::array<std::size_t,2> pos={new_row,new_col};
                // assignment of the value in the new map
                new_data[pos]=it->second;
                }        
        }
        // Re-adjust parameters
        data.clear();
        data=new_data;
        num_rows=rows;
        num_cols=cols;
        return;
}


template<typename T, StorageOrder Order>
void algebra::SparseMatrix<T,Order>::compress(){
    // Check if the matrix is already compressed
    if(compressed){
        std::cout << "Matrix already compressed" << std::endl;
        return;
    }

    // Determine the number of rows and columns based on storage order
    const size_t rowCount = (Order == StorageOrder::RowMajor) ? num_rows : num_cols;
    const size_t colCount = (Order == StorageOrder::RowMajor) ? num_cols : num_rows;

    // Resize vectors for storing compressed data
    first_indexes.resize(rowCount + 1);
    second_indexes.reserve(data.size());
    values.reserve(data.size());

    // Initialize index for storing compressed data
    size_t currentIndex = 0;

    // Initialize first index for the compressed matrix
    first_indexes[0] = 0;

    // Loop over each row or column
    for (size_t i = 0; i < rowCount; ++i) {
        // Find lower and upper bounds for the current row or column
        auto lower = data.lower_bound({(Order == StorageOrder::RowMajor) ? i : 0, (Order == StorageOrder::RowMajor) ? 0 : i});
        auto upper = data.upper_bound({(Order == StorageOrder::RowMajor) ? i : colCount - 1, (Order == StorageOrder::RowMajor) ? colCount - 1 : i});
        size_t count = 0; // Counter for non-zero elements in the current row or column

        // Loop over elements in the current row or column
        for (auto k = lower; k != upper; ++k) {
            // Store column or row index and value in the compressed format
            second_indexes.emplace_back((Order == StorageOrder::RowMajor) ? k->first[1] : k->first[0]);
            values.emplace_back(k->second);
            ++count; // Increment count of non-zero elements
        }

        // Update the next value of first_indexes according to the Compressed Sparse Row (CSR) format
        currentIndex += count;
        first_indexes[i + 1] = currentIndex;
    }

    // Clear the original elements
    data.clear();

    // Set the compressed flag to true
    compressed = true;

    // Inform the user about successful compression
    std::cout << "Matrix successfully compressed!!!!:)" << std::endl;
}

template<typename T, StorageOrder Order>
void algebra::SparseMatrix<T,Order>::uncompress(){
    // Check if matrix is already uncompressed
    if(!compressed){
        std::cout << "Matrix already uncompressed" << std::endl;
        return;
    }

    // Uncompressed mode
    if constexpr(Order == StorageOrder::RowMajor) {
        // Loop over the rows
        for(std::size_t i = 0; i < num_rows; ++i) {
            // Loop over non-zero elements of the row
            for(std::size_t j = first_indexes[i]; j < first_indexes[i + 1]; ++j) {
                // Define the key of the map
                std::array<std::size_t, 2> ii = {i, second_indexes[j]};
                // Assign the value
                data[ii] = values[j];
            }
        }
    } else {
        // Loop over the columns
        for(std::size_t i = 0; i < num_cols; ++i) {
            // Loop over non-zero elements of the column
            for(std::size_t j = first_indexes[i]; j < first_indexes[i + 1]; ++j) {
                // Define the key of the map
                std::array<std::size_t, 2> ii = {second_indexes[j], i};
                // Assign the value
                data[ii] = values[j];
            }
        }
    }

    // Set compressed value to false
    compressed = false;

    // Clear memory space of compressed matrix
    first_indexes.clear();
    second_indexes.clear();
    values.clear();
    std::cout << "Matrix successfully uncompressed!!!!!!!! :)" << std::endl;
}


template<typename T, StorageOrder Order>
T algebra::SparseMatrix<T,Order>::operator()(std::size_t i, std::size_t j) const {
    // Check if the indices are within bounds
    if (i >= get_rows() || j >= get_cols())
        return 0; // Out of bounds, return 0

    // If the matrix is uncompressed, use the data map for access
    if (!compressed)
        return data.count({i, j}) ? data.at({i, j}) : 0;
    else {
        // Compressed mode
        // Determine the start and end indexes for the loop based on storage order and indices
        const auto& idx = (Order == StorageOrder::RowMajor) ? first_indexes[i] : first_indexes[j];
        const auto& endIdx = (Order == StorageOrder::RowMajor) ? first_indexes[i + 1] : first_indexes[j + 1];
        // Search for the element within the compressed format
        for (std::size_t k = idx; k < endIdx; ++k) {
            if (second_indexes[k] == ((Order == StorageOrder::RowMajor) ? j : i)) {
                return values[k]; // Element found, return its value
            }
        }
        return 0; // Element not found, return default value
    }
}

// NOTICE: you cannot add a element in compressed state!!
template<typename T, StorageOrder Order>
T& algebra::SparseMatrix<T,Order>::operator()(std::size_t i, std::size_t j) {
    // Check if the indices are within bounds
    if (i >= get_rows() || j >= get_cols())
        throw std::out_of_range("Index out of range");

    // If the matrix is compressed
    if (compressed){
        // Compressed mode
        // Determine the start and end indexes for the loop based on storage order and indices
        const auto& idx = (Order == StorageOrder::RowMajor) ? first_indexes[i] : first_indexes[j];
        const auto& endIdx = (Order == StorageOrder::RowMajor) ? first_indexes[i + 1] : first_indexes[j + 1];
        // Search for the element within the compressed format
        for (std::size_t k = idx; k < endIdx; ++k) {
            if (second_indexes[k] == ((Order == StorageOrder::RowMajor) ? j : i)) {
                return values[k]; // Element found, return its value
            }
        }
        // If element not found, throw an exception
        throw std::out_of_range("Element not found");
    } else {
        // Uncompressed mode
        // Access the element in the data map, creating a new element with value 0 if not found
        return data[{i, j}];
    }
}
#endif  // SPARSEMATRIX_HPP