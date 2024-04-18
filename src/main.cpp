#include "Sparse_Matrix.hpp"



int main()
{   
    // TEST 1
    algebra::Sparse_Matrix<double, algebra::StorageOrder::RowMajor> mat;
    mat.resize(3, 3);
    mat(0, 0) = 1.0;
    mat(1, 1) = 2.0;
    mat(2, 2) = 3.0;
    mat.compress();

    return 0;
}