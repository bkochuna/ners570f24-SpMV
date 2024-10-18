#include "SpMV.hpp"

#include <iostream>

// Traditionally, main takes two arguments: argc and argv.
// These are the number of command line arguments and the arguments themselves.
// int main(int argc, char* argv[])
// However, we don't need them for this example.
int main()
{
  std::cout << "Hello World!\n";

  SpMV::SparseMatrix<float>* ptr_A = new SpMV::SparseMatrix_ELL<float>(1, 5);

  // New scoping unit. This means variables defined in here, stay here.
  {
    SpMV::SparseMatrix_ELL<double> A = SpMV::SparseMatrix_ELL<double>(2,2);
    A.setCoefficient(1, 1, 1.0);
  }
  ptr_A->setCoefficient(0, 3, 4.0);

  delete(ptr_A);


  return 0;
}
