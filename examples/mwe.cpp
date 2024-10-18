#include "SpMV.hpp"

#include <iostream>

// Traditionally, main takes two arguments: argc and argv.
// These are the number of command line arguments and the arguments themselves.
// int main(int argc, char* argv[])
// However, we don't need them for this example.
int main()
{
  std::cout << "Hello World!\n";

  SpMV::SparseMatrix<float>* ptr_A = new SpMV::SparseMatrix_COO<float>(1, 5);

  // New scoping unit. This means variables defined in here, stay here.
  {
    SpMV::SparseMatrix_COO<double> A = SpMV::SparseMatrix_COO<double>(2,2);
    A.setCoefficient(1, 1, 1.0);

  }// once out of scoping unit, will automatically goto destruction

  ptr_A->setCoefficient(0, 3, 4.0);

  delete(ptr_A);

  // Use default constructor (creates a 1x1 matrix)
  SpMV::SparseMatrix_COO<float> A;

  return 0;
}
