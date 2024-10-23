#include "SpMV.hpp"
 
#include <iostream>
#include <vector>
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

  }
  ptr_A->setCoefficient(0, 3, 4.0);

  delete(ptr_A);


  SpMV::SparseMatrix_CSR<double> Mat = SpMV::SparseMatrix_CSR<double>(4,4);
  Mat.setCoefficient(0,0,1.0);
  Mat.setCoefficient(0,1,7.0);
  Mat.setCoefficient(1,0,5.0);
  Mat.setCoefficient(1,2,3.0);
  Mat.setCoefficient(1,3,9.0);
  Mat.setCoefficient(2,1,2.0);
  Mat.setCoefficient(2,2,8.0);
  Mat.setCoefficient(3,3,6.0);

  // define b and x
  std::vector<double> b{0.0,0.0,0.0,0.0};
  std::vector<double> x{1.0,2.0,3.0,4.0};
  
  Mat.assembleStorage();

  std::cout << "Multiplying Ax=b for CSR!\n";

  // Mat.viewCSR();
  Mat.multyCSR(b,x);

  // print b

  std::cout << "b:\n";

  for (const auto& value : b) {
      std::cout << value << " ";
  }
  std::cout << std::endl;



  return 0;
}
