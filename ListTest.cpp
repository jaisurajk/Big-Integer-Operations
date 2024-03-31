// Name: Jaisuraj Kaleeswaran
// CruzID: jkaleesw
// Assignment Name: pa6

#include "List.h"
#include <iostream>
#include <stdexcept>
#include <string>

using namespace std;

int main() {
  List A, B, C, D;
  for (long i = 1; i < 10; i++) {
    A.insertBefore(i);
    A.movePrev();
  }
  cout << "A = " << A << endl;
  for (long i = 1; i <= 5; i++) {
    A.moveNext();
  }
  cout << A.position() << endl;
  cout << A.findNext(3) << endl;
  cout << A.position() << endl;
  cout << A.findPrev(3) << endl;
  cout << A.position() << endl;
  A.clear();

  A.insertAfter(1);
  A.insertAfter(2);
  A.insertAfter(3);
  B.insertAfter(4);
  B.insertAfter(5);
  B.insertAfter(6);

  C = A.concat(B);
  cout << "C: " << C << endl;
  cout << "B: " << B << endl;
  if (C.length() != 6)
    cout << "1" << endl;
  cout << "length of C is: " << C.length() << endl;
  cout << "C.front: " << C.front() << endl;
  cout << "C.back: " << C.back() << endl;
  cout << "C.position: " << C.position() << endl;

  D = B;
  cout << "D: " << D << endl;
  cout << "D==B is " << (D == B ? "true" : "false") << endl;
  D.eraseBefore();
  cout << "D: " << D << endl;
  for (long i = 1; i < 15; i++) {
    D.insertBefore(i);
    D.movePrev();
  }
  D.eraseAfter();
  cout << "D: " << D << endl;
  D.setBefore(1);
  D.setAfter(15);
  cout << "D: " << D << endl;
  A.clear(), B.clear(), C.clear(), D.clear();
  return 0;
}
