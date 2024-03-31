// Name: Jaisuraj Kaleeswaran
// CruzID: jkaleesw
// Assignment Name: pa6

#include "BigInteger.h"
#include "List.h"
#include <fstream>
#include <iostream>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

using namespace std;

int main(int argc, char **argv) {
  if (argc != 3) {
    fprintf(stderr, "Incorrect number of command line arguments, please "
                    "include a file in and file out only\n");
    exit(1);
  }
  std::ifstream in;
  std::ofstream out;
  in.open(argv[1]);
  if (!in.is_open()) {
    std::cerr << "Unable to open file " << argv[1] << " for reading"
              << std::endl;
    exit(1);
  }
  out.open(argv[2]);
  if (!out.is_open()) {
    std::cerr << "Unable to open file " << argv[2] << " for writing"
              << std::endl;
    exit(1);
  }
  std::string one;
  std::string two;

  in >> one;
  in >> two;

  BigInteger A(one);
  BigInteger B(two);

  BigInteger s2("2");
  BigInteger s3("3");
  BigInteger s9("9");
  BigInteger s16("16");

  out << A << endl << endl;
  out << B << endl << endl;
  out << A + B << endl << endl;
  out << A - B << endl << endl;
  out << A - A << endl << endl;
  out << A.mult(s3) - B.mult(s2) << endl << endl;
  out << A * B << endl << endl;
  out << A * A << endl << endl;
  out << B * B << endl << endl;
  out << s9 * A * A * A * A + s16 * B * B * B * B * B << endl;
  in.close(), out.close();
  return 0;
}