#include "Sparse_Matrix.hpp"
#include <iostream>


int main()
{   
    //using SparseMatrix = algebra::Sparse_Matrix;
    //using RowMajor = algebra::StorageOrder::RowMajor;
    //using ColumnMajor = algebra::StorageOrder::ColumnMajor;

    // TEST 1: Create Sparse Matrix using uncompressed and compressed storage technique and perform the conversion from one form to the others and vice-versa
    algebra::Sparse_Matrix<double, algebra::StorageOrder::RowMajor> mat;
    mat.resize(3, 3);
    mat.compress();
    mat(0, 0) = 1.0;
    mat(1, 1) = 2.0;
    mat(2, 2) = 3.0;
    
 
    // TEST 2: Implementation of the Matrix reader format
 //   matrixFileName = ifl("matrixFileName", "../../MatrixData/spd/Insp_131.mtx");
 //   std::string matrixFile(matrixFileName);
 //   SparseMatrix<double, RowMajor> mat1(matrixFile);

    return 0;
}