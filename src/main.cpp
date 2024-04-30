#include "SparseMatrix.hpp"
#include "chrono.hpp"

int main()
{   
    // TEST 1: Create Sparse Matrix using uncompressed storage technique
    algebra::SparseMatrix<double, StorageOrder::RowMajor> mat(4, 4, false);
    mat(0, 0) = 1.3;
    mat(1, 1) = 2.3;
    mat(3, 2) = 4.3;
    std::cout << "Original uncompressed matrix:" << std::endl;
    mat.print();

    // Access and print an element from the uncompressed matrix
    std::cout << "Element at (2, 2) in uncompressed matrix: " << mat(1, 1) << std::endl;

    // Compress the matrix and modify an element
    std::cout << "\nCompressing the matrix..." << std::endl;
    mat.compress();
    mat(1, 1) = 52.5;
    std::cout << "Compressed matrix:" << std::endl;
    mat.print();

    // Uncompress the matrix
    std::cout << "\nUncompressing the matrix..." << std::endl;
    mat.uncompress();
    std::cout << "Uncompressed matrix:" << std::endl;
    mat.print();
    
    // TEST 2: Implementation of the Matrix reader format for a simple sparse matrix
    std::string filename_matrix="Simple_Matrix.mtx";
    algebra::SparseMatrix<double,StorageOrder::ColumnMajor> mat1(filename_matrix, false);
    mat1.print();

    //TEST 3: Matrix-vector multiplication
    std::string filename="Insp_131.mtx";
    algebra::SparseMatrix<double,StorageOrder::ColumnMajor> mat2(filename, false);
    std::vector<double> vec(131);
    Timings::Chrono chronometer;
    chronometer.start();
    std::vector<double> result=mat2*vec;
    chronometer.stop();
    double time=chronometer.wallTime();
    std::cout<<"Time for uncompressed: "<<time<<" msec"<<std::endl;

    mat2.compress();

    chronometer.start();
    std::vector<double> result1=mat2*vec;
    chronometer.stop();
    double time1=chronometer.wallTime();
    std::cout<<"Time for compressed: "<<time1<<" msec "<<std::endl;
    return 0;
}