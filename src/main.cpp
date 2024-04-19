#include "SparseMatrix.hpp"



int main()
{   
    // TEST 1: Create Sparse Matrix using uncompressed and compressed storage technique and perform the conversion from one form to the others and vice-versa
    algebra::SparseMatrix<double, StorageOrder::RowMajor> mat;
    mat.resize(4, 4);
    mat.uncompress();               // I'm only using uncompressed state!!!!!!!!!!
    mat(0, 0) = 1.3;
    mat(1, 1) = 2.3;
    mat(3, 2) = 4.3;
    mat.print();
    std::cout << "Element at (2, 2) in uncompressed matrix: " << mat(2-1,2-1) << std::endl;


    // TEST 2: Implementation of the Matrix reader format
    std::string filename_matrix="Matrix.mtx";
    algebra::SparseMatrix<double,StorageOrder::ColumnMajor> mat1(filename_matrix);
    mat1.print();
/*
    //TEST 3: Matrix-vector multiplication
    std::string filename="Insp_131.mtx";
    algebra::SparseMatrix<double,StorageOrder::ColumnMajor> mat2(filename);
   */ 

    return 0;
}