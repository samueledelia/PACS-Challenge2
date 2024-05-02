#include "SparseMatrix.hpp"
#include "chrono.hpp"

// Function to test matrix-vector multiplication and measure time
template<typename T, StorageOrder Order>
double testMatrixVectorMultiplication(const algebra::SparseMatrix<T, Order>& matrix, const std::vector<T>& vec) {
    Timings::Chrono chronometer;
    chronometer.start();
    std::vector<T> result = matrix * vec;
    chronometer.stop();
    return chronometer.wallTime();
}

int main() {
    // TEST 1: Create Sparse Matrix using uncompressed storage technique
    {
        algebra::SparseMatrix<double, StorageOrder::RowMajor> mat(4, 4, false);
        mat(0, 0) = 1.3;
        mat(1, 1) = 2.3;
        mat(3, 2) = 4.3;
        
        std::cout << "Original uncompressed matrix:" << std::endl;
        mat.print();
        
        std::cout << "Element at (2, 2) in uncompressed matrix: " << mat(1, 1) << std::endl;
        
        std::cout << "\nCompressing the matrix..." << std::endl;
        mat.compress();
        mat(1, 1) = 52.5;
        
        std::cout << "Compressed matrix:" << std::endl;
        mat.print();
        
        std::cout << "\nUncompressing the matrix..." << std::endl;
        mat.uncompress();
        
        std::cout << "Uncompressed matrix:" << std::endl;
        mat.print();
    }
    
    // TEST 2: Read and print matrix from file
    {
        std::string filename_matrix = "Simple_Matrix.mtx";
        algebra::SparseMatrix<double, StorageOrder::ColumnMajor> mat1(filename_matrix, false);
        
        std::cout << "Matrix read from file \"" << filename_matrix << "\":" << std::endl;
        mat1.print();
    }
    
    // TEST 3: Matrix-vector multiplication and time measurement
    {
        std::string filename = "Insp_131.mtx";
        algebra::SparseMatrix<double, StorageOrder::ColumnMajor> mat2(filename, false);
        std::vector<double> vec(131);
        
        // Measure time for uncompressed multiplication
        double time_uncompressed = testMatrixVectorMultiplication(mat2, vec);
        std::cout << "Time for uncompressed multiplication: " << time_uncompressed << " msec" << std::endl;
        
        // Measure time for compressed multiplication
        mat2.compress();
        double time_compressed = testMatrixVectorMultiplication(mat2, vec);
        std::cout << "Time for compressed multiplication: " << time_compressed << " msec" << std::endl;
    }

    return 0;
}