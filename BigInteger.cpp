// Name: Jaisuraj Kaleeswaran
// CruzID: jkaleesw
// Assignment Name: pa6

#include "BigInteger.h"
#include "List.h"
#include <cctype>
#include <cmath>
#include <iostream>
#include <stdexcept>
#include <string>

using namespace std;

const ListElement base = 1000000000;
const int power = 9;

// Helper Functions

void remZero(List *L) {
  L->moveFront();
  while (L->length() > 0 && !L->peekNext()) {
    L->eraseAfter();
  }
}

List dummyMult(long s, List b, int *tracker) {
  long t = 0, c = 0;
  List L;
  for (b.moveBack(); b.position() > 0; b.movePrev()) {
    t = (b.peekPrev() * s) + c;
    c = t / base;
    t %= base;
    L.insertAfter(t);
  }
  if (c) {
    L.insertAfter(c);
  }
  L.moveBack();
  for (int i = 0; i < *tracker; i++) {
    L.insertAfter(0);
  }
  return L;
}

// Class Constructors & Destructors

BigInteger::BigInteger() { signum = 0; }

BigInteger::BigInteger(long x) {
  if (x > 0)
    signum = 1;
  else if (x < 0)
    signum = -1;
  else
    signum = 0;
  long y = x;
  while (y > 0) {
    digits.insertBefore(y % base);
    y /= base;
  }
}

BigInteger::BigInteger(std::string s) {
  if (!s.length()) {
    throw std::invalid_argument("BigInteger: Constructor: empty string");
  }
  signum = 1;
  if ((s[0] == '+') | (s[0] == '-')) {
    if (s[0] == '-') {
      signum = -1;
    }
    s = s.substr(1, s.length() - 1);
  }
  for (long unsigned int i = 0; i < s.length(); i++) {
    if (!std::isdigit(s[i])) {
      throw std::invalid_argument("BigInt Constructor is not a number");
    }
  }
  std::string number = "";
  List L;
  size_t most = 0, c_d = s.length();

  while (most < s.length() / power) {
    number = s.substr(c_d - power, power);
    digits.insertAfter(std::stol(number, nullptr, 10));
    c_d -= power;
    most++;
  }
  if (c_d) {
    number = s.substr(0, c_d);
    digits.insertAfter(std::stol(number, nullptr, 10));
  }
  remZero(&digits);
}

BigInteger::BigInteger(const BigInteger &N) {
  signum = N.signum;
  digits = N.digits;
}

// Access functions --------------------------------------------------------

int BigInteger::sign() const { return digits.length() ? signum : 0; }

int BigInteger::compare(const BigInteger &N) const {
  if (signum > N.signum)
    return 1;
  else if (signum < N.signum)
    return -1;
  else if (signum == 0 && N.signum == 0)
    return 0;
  const List &A = digits;
  const List &B = N.digits;
  int L = A.length();

  if (L != B.length()) {
    return (L > B.length()) ? signum : -N.signum;
  }
  List A_cp = A, B_cp = B;

  A_cp.moveFront(), B_cp.moveFront();
  for (int i = 0; i < L; i++) {
    if (A_cp.peekNext() > B_cp.peekNext())
      return signum;
    else if (A_cp.peekNext() < B_cp.peekNext())
      return -N.signum;
    A_cp.moveNext(), B_cp.moveNext();
  }
  return 0;
}

void BigInteger::makeZero() {
  digits.clear();
  signum = 0;
}

void BigInteger::negate() { signum *= -1; }

void normalizeList(List &L) {
  L.moveBack();
  int max = base;
  if (L.length() == 1) {
    long value = L.movePrev();
    if (value >= max)
      L.setAfter(value % base), L.insertBefore(value / base);
    return;
  }
  while (L.position() > 2) {
    long value = L.movePrev();
    if (value >= base) {
      long adder = value / base;
      L.setAfter(value % base);
      long before_val = L.peekPrev();
      L.setBefore(before_val + adder);
    }
    if (value < 0) {
      long new_val = value + base;
      L.setAfter(new_val);
      long before_val = L.peekPrev();
      L.setBefore(before_val - 1);
    }
  }
  // position is now at 1
  long value = L.movePrev(), adder = value / base;
  while (L.position() && adder) {
    if (value >= max) {
      adder = value / base;
      long new_val = value % base;
      L.setAfter(new_val);
      long before_val = L.peekPrev();
      L.setBefore(before_val + adder);
    }
    if (value < 0) {
      L.setAfter(value + base);
      long before_val = L.peekPrev();
      L.setBefore(before_val - 1);
    }
    value = L.movePrev();
  }
  if (value && value >= max)
    L.setAfter(value % base), L.insertBefore(value / base);
}

BigInteger BigInteger::add(const BigInteger &N) const {
  BigInteger A = *this, B = N, C;
  if (A.sign() > 0 && B.sign() < 0) {
    B.negate();
    return A.sub(B);
  } else if (A.sign() < 0 && B.sign() > 0) {
    A.negate();
    return B.sub(A);
  } else if (A.sign() < 0 && B.sign() < 0) {
    A.negate(), B.negate();
    C = A.add(B);
    C.negate();
    return C;
  }
  if (A > B) {
    return B.add(A);
  }
  List a = A.digits, b = B.digits, c = C.digits;
  long crry = 0, dummy = 0;

  a.moveBack(), b.moveBack();
  while (a.position() > 0 && b.position() > 0) {
    dummy = crry + a.peekPrev() + b.peekPrev();
    crry = dummy / base;
    dummy %= base;
    c.insertAfter(dummy), a.movePrev(), b.movePrev();
  }
  while (b.position() > 0) {
    dummy = crry + b.peekPrev();
    crry = dummy / base;
    dummy %= base;
    c.insertAfter(dummy), b.movePrev();
  }
  if (crry)
    c.insertAfter(crry);
  C.signum = 1, C.digits = c;
  return C;
}

BigInteger BigInteger::sub(const BigInteger &N) const{
	BigInteger B = *this, A = N, C;
	List a = A.digits, b = B.digits, c = C.digits;
	if (A == B){
		return C;
	}
	if (A.sign() && !B.sign()){
		A.negate();
		return A;
	}
	if (!A.sign() && B.sign()){
		return B;
	}
	if (A.sign() < 0 && B.sign() > 0){
		B.negate();
		C = A.add(B);
		C.negate();
		return C;
	}
	if (A.sign() > 0 && B.sign() < 0){
		A.negate();
		C = A.add(B);
		return C;
	}
	if (A.sign() < 0 && B.sign() < 0){
		A.negate(), B.negate();
		C = B.sub(A);
		C.negate();
		return C;
	}
	if (A < B){
		C = A.sub(B);
		C.negate();
		return C;
	}
	a.moveBack(), b.moveBack();
	long d = 0, dummy = 0;
	int i = b.position();
	while (i){
		if (a.peekPrev() - d < b.peekPrev()){
			dummy = a.peekPrev() + base - b.peekPrev() - d;
			d = 1;
		} else{
			dummy = a.peekPrev() - d - b.peekPrev();
			if (!(a.peekPrev() <= 0)){
				d = 0;
			}
		}
		
		c.insertAfter(dummy), a.movePrev(), b.movePrev();
		i--;
	}
	while (a.position() > 0){
		if (a.peekPrev() < d){
			dummy = a.peekPrev() + base - d;
			d = 1;
		} else {
			dummy = a.peekPrev() - d;
			if (a.peekPrev() > 0) {
				d = 0;
			}
		}
		c.insertAfter(dummy), a.movePrev();
	}
	C.digits = c;
	remZero(&(C.digits));
	C.signum = -1;
	return C;
}

BigInteger BigInteger::mult(const BigInteger &N) const {
  BigInteger B = *this, A = N, C;
  List a = A.digits, b = B.digits;
  int tracker = 0, pos = a.position();

  a.moveBack(), b.moveBack();
  for (int i = pos; i > 0; i--) {
    List dummy = dummyMult(a.peekPrev(), b, &tracker);
    BigInteger J;
    J.signum = 1, J.digits = dummy;
    C = C.add(J);
    a.movePrev();
    tracker++;
  }
  C.signum = A.signum * B.signum;
  return C;
}

// Other Functions
std::string BigInteger::to_string() {
  std::string product = "";
  if (!signum) {
    return "0";
  } else if (signum == -1) {
    product += "-";
  }
  digits.moveFront();
  while (digits.front() == 0 && digits.length() > 1) {
    digits.eraseAfter();
  }
  for (digits.moveFront(); digits.position() < digits.length();
       digits.moveNext()) {
    std::string A = std::to_string(digits.peekNext()), B = "";
    while ((int)(B.length() + A.length()) < power && digits.position() != 0) {
      B += '0';
    }
    product += (B + A);
  }

  return product;
}

// Overriden Operators

std::ostream &operator<<(std::ostream &stream, BigInteger N) {
  return stream << N.BigInteger::to_string();
}

bool operator==(const BigInteger &A, const BigInteger &B) {
  return !A.compare(B) ? 1 : 0;
}

bool operator<(const BigInteger &A, const BigInteger &B) {
  return A.compare(B) == -1 ? 1 : 0;
}

bool operator<=(const BigInteger &A, const BigInteger &B) {
  return (!A.compare(B) || A.compare(B) == -1) ? 1 : 0;
}

bool operator>(const BigInteger &A, const BigInteger &B) {
  return A.compare(B) == 1 ? true : false;
}

bool operator>=(const BigInteger &A, const BigInteger &B) {
  return (!A.compare(B) || A.compare(B) == 1) ? 1 : 0;
}

BigInteger operator+(const BigInteger &A, const BigInteger &B) {
  return A.add(B);
}

BigInteger operator+=(BigInteger &A, const BigInteger &B) {
  BigInteger num = A.add(B);
  A.digits = num.digits;
  A.signum = num.signum;
  return A;
}

BigInteger operator-(const BigInteger &A, const BigInteger &B) {
  return A.sub(B);
}

BigInteger operator-=(BigInteger &A, const BigInteger &B) {
  BigInteger num = A.sub(B);
  A.digits = num.digits;
  A.signum = num.signum;
  return A;
}

BigInteger operator*(const BigInteger &A, const BigInteger &B) {
  return A.mult(B);
}

BigInteger operator*=(BigInteger &A, const BigInteger &B) {
  BigInteger num = A.mult(B);
  A.digits = num.digits;
  A.signum = num.signum;
  return A;
}

// #include "BigInteger.h"
// #include "List.h"
// #include <cctype>
// #include <cmath>
// #include <iostream>
// #include <stdexcept>
// #include <string>

// using namespace std;

// const ListElement base = 1000000000;
// const int power = 9;

// // Helper Functions

// List dummyMult(long s, List *b, int *tracker) {
//   long t = 0, c = 0;
//   List L;
//   for (b->moveBack(); b->position() > 0; b->movePrev()) {
//     t = (b->peekPrev() * s) + c;
//     c = t / base;
//     t %= base;
//     L.insertAfter(t);
//   }

//   if (c > 0) {
//     L.insertAfter(c);
//   }
//   L.moveBack();

//   for (int i = 0; i < *tracker; i++) {
//     L.insertAfter(0);
//   }

//   return L;
// }

// void remZero(List *L) {
//   L->moveFront();
//   while (L->length() > 0 && !L->peekNext()) {
//     L->eraseAfter();
//   }
// }

// // Class Constructors & Destructors

// BigInteger::BigInteger() {
//   //	return;
//   signum = 0;
//   digits = List();
// }

// BigInteger::BigInteger(long x) {
//   if (!x) {
//     signum = 0;
//     digits.insertAfter(0);
//     return;
//   }

//   if (x < 0) {
//     signum = -1;
//     x = -x;
//   } else {
//     signum = 1;
//   }

//   while (x > 0) {
//     digits.insertBefore(x % base);
//     x /= base;
//   }
// }

// BigInteger::BigInteger(std::string s) {
//   int s_len = (int)s.length() - 1;
//   if (!s.length())
//     throw std::invalid_argument("BigInteger: Constructor: empty string");
//   if (s.length() == 1 && !isdigit(s[0]))
//     throw std::invalid_argument("BigInteger: Constructor: non-numeric
//     string");
//   if (s.length() != 0) {
//     for (int i = 1; i <= s_len; ++i) {
//       if (!isdigit(s[i])) {
//         throw std::invalid_argument(
//             "BigInteger: Constructor: non-numeric string");
//       }
//     }
//     if (isdigit(s[0]) || s[0] == '+') {
//       this->signum = 1;
//     } else if (s[0] == '-') {
//       this->signum = -1;
//     } else {
//       throw std::invalid_argument(
//           "BigInteger: Constructor: non-numeric string");
//     }
//     int i = s_len;
//     int lim;
//     if (isdigit(s[0])) {
//       lim = 0;
//     } else {
//       lim = 1;
//     }
//     if (i < power) {
//       if (s[0] == '-') {
//         s = s.substr(1, s_len);
//       }
//       if (s[0] == '+') {
//         s = s.substr(1, s_len);
//       }
//       long val_si = stol(s);
//       digits.insertAfter(val_si);
//     } else {
//       for (; i >= power; i = i - power) {
//         std::string val_s = s.substr(
//             i - power + 1, power); // fix this. Not storing correct value;
//         long val_si = stol(val_s);
//         digits.insertAfter(val_si);
//       }
//       std::string val_s;
//       for (; i >= lim; --i) {
//         val_s = s[i] + val_s;
//       }
//       long val_si = stol(val_s);
//       digits.insertAfter(val_si);
//     }
//   }
// }

// BigInteger::BigInteger(const BigInteger &N) {
//   this->signum = N.signum;
//   this->digits = N.digits;
// }

// // Access Functions

// int BigInteger::sign() const { return this->signum; }

// int BigInteger::compare(const BigInteger &N) const {
//   if (signum > N.signum) {
//     return 1;
//   } else if (signum < N.signum) {
//     return -1;
//   } else if (signum == 0 && N.signum == 0) {
//     return 0;
//   }

//   const List &A = digits;
//   const List &B = N.digits;
//   int L = (int)A.length();

//   if (L != (int)B.length()) {
//     return (L > (int)B.length()) ? signum : -N.signum;
//   }

//   List A_cp = A;
//   List B_cp = B;

//   A_cp.moveFront();
//   B_cp.moveFront();

//   for (int i = 0; i < L; i++) {
//     if (A_cp.peekNext() > B_cp.peekNext()) {
//       return signum;
//     }

//     else if (A_cp.peekNext() < B_cp.peekNext()) {
//       return -N.signum;
//     }

//     A_cp.moveNext();
//     B_cp.moveNext();
//   }

//   return 0;
// }

// // Manipulation Procedures

// void BigInteger::makeZero() {
//   digits.clear();
//   signum = 0;
// }

// void BigInteger::negate() { signum *= -1; }

// // BigInteger Arithmetic Operations

// BigInteger BigInteger::add(const BigInteger &N) const {
//   BigInteger A = *this;
//   BigInteger B = N;
//   BigInteger C;

//   if (A.sign() > 0 && B.sign() < 0) {
//     B.negate();
//     return A.sub(B);
//   } else if (A.sign() < 0 && B.sign() > 0) {
//     A.negate();
//     return B.sub(A);
//   } else if (A.sign() < 0 && B.sign() < 0) {
//     A.negate();
//     B.negate();
//     C = A.add(B);
//     C.negate();
//     return C;
//   }

//   if (A > B) {
//     return B.add(A);
//   }

//   List a = A.digits;
//   List b = B.digits;
//   List c = C.digits;

//   long crry = 0;
//   long dummy = 0;

//   a.moveBack();
//   b.moveBack();

//   while (a.position() > 0 && b.position() > 0) {
//     dummy = crry + a.peekPrev() + b.peekPrev();
//     crry = dummy / base;
//     dummy %= base;
//     c.insertAfter(dummy);
//     a.movePrev();
//     b.movePrev();
//   }

//   while (b.position() > 0) {
//     dummy = crry + b.peekPrev();
//     crry = dummy / base;
//     dummy %= base;
//     c.insertAfter(dummy);
//     b.movePrev();
//   }

//   if (crry > 0) {
//     c.insertAfter(crry);
//   }

//   C.signum = 1;
//   C.digits = c;
//   return C;
// }

// BigInteger BigInteger::sub(const BigInteger &N) const {

//   BigInteger B = *this;
//   BigInteger A = N;
//   BigInteger C;
//   List a = A.digits;
//   List b = B.digits;
//   List c = C.digits;

//   if (A == B) {
//     return C;
//   }

//   if (A.sign() && !B.sign()) {
//     A.negate();
//     return A;
//   }

//   if (!A.sign() && B.sign()) {
//     return B;
//   }

//   if (A.sign() < 0 && B.sign() > 0) {
//     B.negate();
//     C = A.add(B);
//     C.negate();
//     return C;
//   }

//   if (A.sign() > 0 && B.sign() < 0) {
//     A.negate();
//     C = A.add(B);
//     return C;
//   }

//   if (A.sign() < 0 && B.sign() < 0) {
//     A.negate();
//     B.negate();
//     C = B.sub(A);
//     C.negate();
//     return C;
//   }

//   if (A < B) {
//     C = A.sub(B);
//     C.negate();
//     return C;
//   }

//   a.moveBack();
//   b.moveBack();
//   long d = 0;
//   long dummy = 0;
//   int i = b.position();

//   while (!(i <= 0)) {
//     if (a.peekPrev() - d < b.peekPrev()) {
//       dummy = a.peekPrev() + base - b.peekPrev() - d;
//       d = 1;
//     } else {
//       dummy = a.peekPrev() - d - b.peekPrev();
//       if (!(a.peekPrev() <= 0)) {
//         d = 0;
//       }
//     }

//     c.insertAfter(dummy);
//     a.movePrev();
//     b.movePrev();
//     i--;
//   }

//   while (a.position() > 0) {
//     if (a.peekPrev() - d < 0) {
//       dummy = a.peekPrev() + base - 0 - d;
//       d = 1;
//     } else {
//       dummy = a.peekPrev() - d - 0;
//       if (!(a.peekPrev() <= 0)) {
//         d = 0;
//       }
//     }
//     c.insertAfter(dummy);
//     a.movePrev();
//   }

//   C.digits = c;
//   remZero(&(C.digits));
//   C.signum = -1;
//   return C;
// }

// void scalarMultList(List &L, ListElement m) {
//   L.moveFront();
//   while (L.position() < L.length()) {
//     L.setAfter(L.peekNext() * m);
//     L.moveNext();
//   }
// }

// void shiftList(List &L, int p) {
//   L.moveBack();
//   for (int i = 0; i < p; i++) {
//     L.insertAfter(0);
//   }
// }

// void sumList(List &S, List A, List B, int sgn) {
//   S.clear(); // Clear the result list
//   long carry = 0;

//   // Iterate through both lists
//   A.moveBack();
//   B.moveBack();
//   while (A.position() > 0 || B.position() > 0 || carry) {
//     long a = (A.position() > 0) ? A.peekPrev() : 0;
//     long b = (B.position() > 0) ? sgn * B.peekPrev() : 0;
//     long sum = a + b + carry;
//     carry = sum / base;
//     sum %= base;
//     S.moveFront();
//     S.insertBefore(sum);
//     if (A.position() > 0)
//       A.movePrev();
//     if (B.position() > 0)
//       B.movePrev();
//   }
//   while (S.length() > 1 && S.front() == 0) {
//     S.moveFront();
//     S.eraseAfter();
//   }
// }

// void normalizeList(List &L) {
//   L.moveBack();
//   int max = base;
//   if (L.length() == 1) {
//     long value = L.movePrev();
//     if (value >= max)
//       L.setAfter(value % base), L.insertBefore(value / base);
//     return;
//   }
//   while (L.position() > 2) {
//     long value = L.movePrev();
//     if (value >= base) {
//       long adder = value / base;
//       L.setAfter(value % base);
//       long before_val = L.peekPrev();
//       L.setBefore(before_val + adder);
//     }
//     if (value < 0) {
//       long new_val = value + base;
//       L.setAfter(new_val);
//       long before_val = L.peekPrev();
//       L.setBefore(before_val - 1);
//     }
//   }
//   // position is now at 1
//   long value = L.movePrev(), adder = value / base;
//   while (L.position() && adder) {
//     if (value >= max) {
//       adder = value / base;
//       long new_val = value % base;
//       L.setAfter(new_val);
//       long before_val = L.peekPrev();
//       L.setBefore(before_val + adder);
//     }
//     if (value < 0) {
//       L.setAfter(value + base);
//       long before_val = L.peekPrev();
//       L.setBefore(before_val - 1);
//     }
//     value = L.movePrev();
//   }
//   if (value && value >= max)
//     L.setAfter(value % base), L.insertBefore(value / base);
// }

// // BigInteger BigInteger::mult(const BigInteger &N) const {
// //   BigInteger result;
// //   List B = N.digits;

// //   // Check for the case where one of the operands is zero
// //   if (signum == 0 || N.signum == 0) {
// //     // result.makeZero();
// //     return result;
// //   }
// //   // result.signum = signum * N.signum;
// //   List temp;
// //   // List A = digits;

// //   List total;
// // //   int i = 0;

// // //   do {
// // //     temp = digits;
// // //     scalarMultList(temp, B.peekPrev());
// // //     shiftList(temp, i);
// // //     List faketotal = total;
// // //     sumList(total, faketotal, temp, 1);
// // //     normalizeList(total);
// // //     i++;
// // //     B.movePrev();
// // //   } while (B.position() > 0);

// //   B.moveBack();
// //   for (int i = 0; B.position() > 0; i++, B.movePrev()) {
// //       temp = digits;
// //       scalarMultList(temp, B.peekPrev());
// //       shiftList(temp, i);
// //       List faketotal = total;
// //       sumList(total, faketotal, temp, 1);
// //       normalizeList(total);
// //   }
// //   result.digits = total;
// //   // result.signum = signum * N.signum;
// //   if (signum == N.signum) {
// //     result.signum = 1;
// //   } else {
// //     result.signum = -1;
// //   }
// //   // normalizeList(result.digits);  // Normalize the result

// //   // cout << "bigint-STRING = " << result.digits.to_string() << endl;

// //   return result;
// // }

// // BigInteger result;
// // B.moveFront();
// //     int i = 0;
// //     while(B.position()<B.length()) {
// //         A.moveFront();
// //         while(A.position()<A.length()) {
// //             long x = A.peekNext() * B.peekNext();
// // result = result.add((BigInteger)x);
// //             A.moveNext();
// //         }
// //         i++;
// //         shiftList(temp, i);
// //         B.moveNext();
// //     }

// // BigInteger BigInteger::mult(const BigInteger &N) const {
// //   BigInteger B = *this;
// //   BigInteger A = N;
// //   BigInteger C;

// //   List a = A.digits;
// //   List b = B.digits;

// //   int tracker = 0;
// //   int pos = a.position();

// //   a.moveBack();
// //   b.moveBack();

// //   for (int i = pos; i > 0; i--) {
// //     List dummy = dummyMult(a.peekPrev(), &b, &tracker);
// //     BigInteger J;
// //     J.signum = 1;
// //     J.digits = dummy;
// //     C = C.add(J);
// //     a.movePrev();
// //     tracker++;
// //   }

// //   C.signum = A.signum * B.signum;
// //   return C;
// // }

// // Other Functions

// std::string BigInteger::to_string() {
//   std::string s = "";
//   if (this->signum == 0) {
//     s += '0';
//     return s;
//   } else {
//     if (this->signum == -1) {
//       s += '-';
//     }
//     digits.moveFront();
//     long valq = digits.moveNext();
//     while (valq == 0) {
//       valq = digits.moveNext();
//     }
//     digits.movePrev();
//     std::string val = std::to_string(digits.moveNext());
//     for (int i = 0; i < (int)val.length(); ++i) {
//       if (val[i] != '0') {
//         val = val.substr(i, val.length());
//         break;
//       }
//     }
//     s += val;
//     while (digits.position() != digits.length()) {
//       val = std::to_string(digits.moveNext());
//       if ((int)val.length() != power && digits.position() != 1) {
//         for (int i = (int)val.length(); i < power; ++i)
//           val = "0" + val;
//       }
//       s += val;
//     }
//     return s;
//   }
// }

// // Overriden Operators

// std::ostream &operator<<(std::ostream &stream, BigInteger N) {
//   return stream << N.BigInteger::to_string();
// }

// bool operator==(const BigInteger &A, const BigInteger &B) {
//   int iter = A.compare(B);
//   return (iter == 0) ? true : false;
// }

// bool operator<(const BigInteger &A, const BigInteger &B) {
//   int iter = A.compare(B);
//   return (iter == -1) ? true : false;
// }

// bool operator<=(const BigInteger &A, const BigInteger &B) {
//   int iter = A.compare(B);
//   return ((iter == 0) || (iter == -1)) ? true : false;
// }

// bool operator>(const BigInteger &A, const BigInteger &B) {
//   int iter = A.compare(B);
//   return (iter == 1) ? true : false;
// }

// bool operator>=(const BigInteger &A, const BigInteger &B) {
//   int iter = A.compare(B);
//   return ((iter == 0) || (iter == 1)) ? true : false;
// }

// BigInteger operator+(const BigInteger &A, const BigInteger &B) {
//   return A.add(B);
// }

// BigInteger operator+=(BigInteger &A, const BigInteger &B) {
//   BigInteger num = A.add(B);
//   A.digits = num.digits;
//   A.signum = num.signum;
//   return A;
// }

// BigInteger operator-(const BigInteger &A, const BigInteger &B) {
//   return A.sub(B);
// }

// BigInteger operator-=(BigInteger &A, const BigInteger &B) {
//   BigInteger num = A.sub(B);
//   A.digits = num.digits;
//   A.signum = num.signum;
//   return A;
// }

// BigInteger operator*(const BigInteger &A, const BigInteger &B) {
//   return A.mult(B);
// }

// BigInteger operator*=(BigInteger &A, const BigInteger &B) {
//   BigInteger num = A.mult(B);
//   A.digits = num.digits;
//   A.signum = num.signum;
//   return A;
// }

// // //
// //-----------------------------------------------------------------------------
// // // // BigInteger.cpp
// // // // Cpp file for the BigInteger ADT
// // //
// //-----------------------------------------------------------------------------
// // // #include<iostream>
// // // #include<string>
// // // #include<cmath>
// // // #include"BigInteger.h"
// // // #include"List.h"

// // // using namespace std;

// // // int power = 9;
// // // ListElement base = 100;

// // // BigInteger::BigInteger() {
// // //     signum = 0;
// // // }

// // // BigInteger::BigInteger(long x){
// // //     if (x > 0)
// // //         signum = 1;
// // //     else if (x < 0)
// // //         signum = -1;
// // //     else
// // //         signum = 0;
// // //     long y = x;
// // //     while (y > 0) {
// // //         digits.insertBefore(y % base);
// // //         y /= base;
// // //     }
// // // }

// // // BigInteger::BigInteger(std::string s){
// // //     if (s.length() == 1 && !isdigit(s[0]))
// // //         throw std::invalid_argument("BigInteger: Constructor:
// non-numeric
// // //         string");
// // //     if (s.length()) {
// // //         for (int i = 1; i < (int)s.length(); i++) {
// // //             if (!isdigit(s[i]))
// // //                 throw std::invalid_argument("BigInteger: Constructor:
// // //                 non-numeric string");
// // //         }
// // //         if (isdigit(s[0]) || s[0] == '+')
// // //             signum = 1;
// // //         else if (s[0] == '-')
// // //             signum = -1;
// // //         else
// // //             throw std::invalid_argument("BigInteger: Constructor:
// non-numeric
// // //             string");
// // //         std::string part_s, val_s;
// // //         int lim = isdigit(s[0]) ? 0 : 1, s_len = (int)s.length() - 1,
// leng =
// // //         s_len; if (leng < power) {
// // //             if (s[0] == '-' || s[0] == '+')
// // //                 part_s = s.substr(1,s_len);
// // //         } else {
// // //             for (; leng>=power; leng -= power) {
// // //                 val_s = s.substr(leng-power+1,power); // fix this. Not
// // //                 storing correct value; digits.insertAfter(stol(val_s));
// // //             }
// // //             for (; leng>=lim; leng--)
// // //                 part_s += s[leng];
// // //         }
// // //         digits.insertAfter(stol(part_s));
// // //         return;
// // //     }
// // //     throw std::invalid_argument("BigInteger: Constructor: empty
// string");
// // // }

// // // BigInteger::BigInteger(const BigInteger& N){
// // //     signum = N.signum;
// // //     digits = N.digits;
// // // }

// // // // Access functions
// --------------------------------------------------------

// // // int BigInteger::sign() const{
// // //     return digits.length() ? signum : 0;
// // // }

// // // int BigInteger::compare(const BigInteger& N) const{
// // //     BigInteger G = BigInteger::sub(N);
// // //     return G.signum;
// // // }

// // // // Manipulation procedures
// -------------------------------------------------

// // // void BigInteger::makeZero(){
// // //     List C = digits;
// // //     C.clear();
// // //     digits = C;
// // //     signum = 0;
// // // }

// // // void BigInteger::negate() {
// // //     signum *= -1;
// // // }

// // // // BigInteger Arithmetic operations
// ---------------------------------------

// // // // sumList()
// // // // Overwrites the state of S with A + sgn*B (considered as vectors).
// // // // Used by both sum() and sub().
// // // void sumList(List& S, List A, List B, int sgn) {
// // //     S.clear();  // Clear the result list
// // //     long carry = 0;

// // //     // Iterate through both lists
// // //     A.moveBack();
// // //     B.moveBack();
// // //     while (A.position() > 0 || B.position() > 0 || carry) {
// // //         long sum = carry;
// // //         if (A.position() > 0) {
// // //             sum += A.movePrev();
// // //         }
// // //         if (B.position() > 0) {
// // //             sum += sgn * B.movePrev();
// // //         }

// // //         S.insertBefore(sum % base);  // Insert the current digit
// // //         carry = sum / base;           // Update carry
// // //     }
// // // }

// // // void normalizeList(List& L) {
// // //     L.moveBack();
// // //     int max = base;
// // //     if (L.length() == 1) {
// // //         long value = L.movePrev();
// // //         if (value >= max)
// // //             L.setAfter(value%base), L.insertBefore(value/base);
// // //         return;
// // //     }
// // //     while (L.position() > 2) {
// // //         long value = L.movePrev();
// // //         if (value >= base) {
// // //             long adder = value/base;
// // //             L.setAfter(value%base);
// // //             long before_val = L.peekPrev();
// // //             L.setBefore(before_val + adder);
// // //         }
// // //         if (value < 0) {
// // //             long new_val = value + base;
// // //             L.setAfter(new_val);
// // //             long before_val = L.peekPrev();
// // //             L.setBefore(before_val -1);
// // //         }
// // //     }
// // //     //position is now at 1
// // //     long value = L.movePrev(), adder = value/base;
// // //     while (L.position() && adder) {
// // //         if (value >= max) {
// // //             adder = value/base;
// // //             long new_val = value%base;
// // //             L.setAfter(new_val);
// // //             long before_val = L.peekPrev();
// // //             L.setBefore(before_val + adder);
// // //         }
// // //         if (value < 0) {
// // //             L.setAfter(value + base);
// // //             long before_val = L.peekPrev();
// // //             L.setBefore(before_val-1);
// // //         }
// // //         value = L.movePrev();
// // //     }
// // //     if (value && value >= max)
// // //         L.setAfter(value%base), L.insertBefore(value/base);
// // // }

// // // // scalarMultList()
// // // // Multiplies L (considered as a vector) by m. Used by mult().
// // // void scalarMultList(List& L, ListElement m) {
// // //     L.moveBack();
// // //     long carry = 0;

// // //     while (L.position() > 0 || carry) {
// // //         long product = carry;
// // //         if (L.position() > 0) {
// // //             product += (m * L.movePrev());
// // //             cout << product << endl;
// // //         }

// // //         L.setAfter(product % base);  // Set the current digit
// // //         carry = product / base;       // Update carry
// // //     }
// // // }

// // // List scalar(List L, long multi, int shift) { // This function uses
// // // normalizeList()
// // //     List S;
// // //     if (multi == 0) {
// // //         S.insertAfter(0);
// // //         return S;
// // //     }
// // //     for (int i = 0; i < shift; ++i) {
// // //         S.insertAfter(0);
// // //     }
// // //     for (L.moveBack(); L.position();) {
// // //         long val = L.movePrev();
// // //         long new_val = multi*val;
// // //         S.insertAfter(new_val);
// // //     }
// // //     normalizeList(S);
// // //     return S;
// // // }

// // // // BigInteger BigInteger::add(const BigInteger& N) const {
// // // //     BigInteger Q;
// // // //     if (N.signum == -1 && this->signum == 1) {
// // // //         BigInteger NT = N;
// // // //         BigInteger TT = *this;
// // // //         NT.signum = 1;
// // // //         Q = TT.sub(NT);
// // // //         return Q;
// // // //     }
// // // //     else if (this->signum == -1 && N.signum == 1) {
// // // //         BigInteger P = *this;
// // // //         P.signum = 1;
// // // //         Q = N.sub(P);
// // // //         return Q;
// // // //     }
// // // //     else {
// // // //         Q.signum = this->signum;
// // // //         List A = this->digits;
// // // //         List B = N.digits;
// // // //         List C;
// // // //         if (A.length() >= B.length()) {
// // // //             B.moveBack();
// // // //             for (A.moveBack(); B.position() != 0;) {
// // // //                 long A_val = A.movePrev();
// // // //                 long B_val = B.movePrev();
// // // //                 long C_val = A_val+B_val;
// // // //                 C.insertAfter(C_val);
// // // //             }
// // // //             while (A.position()!= 0) {
// // // //                 long A_val = A.movePrev();
// // // //                 C.insertAfter(A_val);
// // // //             }
// // // //         }
// // // //         else {
// // // //             A.moveBack();
// // // //             for (B.moveBack(); A.position() != 0;) {
// // // //                 long A_val = A.movePrev();
// // // //                 long B_val = B.movePrev();
// // // //                 long C_val = A_val+B_val;
// // // //                 C.insertAfter(C_val);
// // // //             }
// // // //             while (B.position()!= 0) {
// // // //                 long B_val = B.movePrev();
// // // //                 C.insertAfter(B_val);
// // // //             }
// // // //         }
// // // //         normalizeList(C);
// // // //         Q.digits = C;
// // // //     }
// // // //     for (Q.digits.moveBack(); Q.digits.position();) {
// // // //         long val = Q.digits.movePrev();
// // // //         if (val != 0) {
// // // //             if (N.signum == this->signum) {
// // // //                 Q.signum = N.signum;
// // // //                 break;
// // // //             }
// // // //         }
// // // //     }
// // // //     return Q;
// // // // }

// // // BigInteger BigInteger::add(const BigInteger& N) const{
// // // 	BigInteger A = *this;
// // // 	BigInteger B = N;
// // // 	BigInteger C;

// // // 	if (A.sign() > 0 && B.sign() < 0){
// // // 		B.negate();
// // // 		return A.sub(B);
// // // 	}else if (A.sign() < 0 && B.sign() > 0){
// // // 		A.negate();
// // // 		return B.sub(A);
// // // 	}else if (A.sign() < 0 && B.sign() < 0){
// // // 		A.negate();
// // // 		B.negate();
// // // 		C = A.add(B);
// // // 		C.negate();
// // // 		return C;
// // // 	}

// // // /*if (N.signum == -1 && this->signum == 1) {
// // // 		sumList(sum, A, B, -1);
// // // 	}
// // // 	if (N.signum == 1 && this->signum == -1) {
// // // 		sumList(sum, B, A, -1);
// // // 	}
// // // 	if (N.signum == 1 && this->signum == 1) {
// // // 		sumList(sum, A, B, 1);
// // // 	}
// // // 	if (N.signum == -1 && this->signum == -1) {
// // // 		negateList(A);
// // //                 sumList(sum, A, B, -1);
// // //         }
// // // */

// // // 	if (A > B){
// // // 		return B.add(A);
// // // 	}

// // // 	List a = A.digits;
// // // 	List b = B.digits;
// // // 	List c = C.digits;

// // // 	long crry = 0;
// // // 	long dummy = 0;

// // // 	a.moveBack();
// // // 	b.moveBack();

// // // 	while (a.position() > 0 && b.position() > 0){
// // // 		dummy = crry + a.peekPrev() + b.peekPrev();
// // // 		crry = dummy / base;
// // // 		dummy %= base;
// // // 		c.insertAfter(dummy);
// // // 		a.movePrev();
// // // 		b.movePrev();
// // // 	}

// // // 	while (b.position() > 0){
// // // 		dummy = crry + b.peekPrev();
// // // 		crry = dummy / base;
// // // 		dummy %= base;
// // // 		c.insertAfter(dummy);
// // // 		b.movePrev();
// // // 	}

// // // 	if (crry > 0){
// // // 		c.insertAfter(crry);
// // // 	}

// // // 	C.signum = 1;
// // // 	C.digits = c;
// // // 	return C;
// // // }

// // // void remZero(List* L){
// // // 	L->moveFront();
// // // 	while (L->length() > 0 && !L->peekNext()){
// // // 		L->eraseAfter();
// // // 	}
// // // }

// // // BigInteger BigInteger::sub(const BigInteger &N) const{

// // // 	BigInteger B = *this;
// // // 	BigInteger A = N;
// // // 	BigInteger C;
// // // 	List a = A.digits;
// // // 	List b = B.digits;
// // // 	List c = C.digits;

// // // /*if (N.signum == -1 && this->signum == 1) {
// // //                 sumList(diff, A, B, 1);
// // //         }
// // //         if (N.signum == 1 && this->signum == -1) {
// // // 		negateList(A);
// // //                 sumList(diff, A, B, -1);
// // //         }
// // //         if (N.signum == 1 && this->signum == 1) {
// // //                 sumList(diff, A, B, -1);
// // //         }
// // //         if (N.signum == -1 && this->signum == -1) {
// // //                 sumList(diff, B, A, -1);
// // //         }*/

// // // 	if (A == B){
// // // 		return C;
// // // 	}

// // // 	if (A.sign() && !B.sign()){
// // // 		A.negate();
// // // 		return A;
// // // 	}

// // // 	if (!A.sign() && B.sign()){
// // // 		return B;
// // // 	}

// // // 	if (A.sign() < 0 && B.sign() > 0){
// // // 		B.negate();
// // // 		C = A.add(B);
// // // 		C.negate();
// // // 		return C;
// // // 	}

// // // 	if (A.sign() > 0 && B.sign() < 0){
// // // 		A.negate();
// // // 		C = A.add(B);
// // // 		return C;
// // // 	}

// // // 	if (A.sign() < 0 && B.sign() < 0){
// // // 		A.negate();
// // // 		B.negate();
// // // 		C = B.sub(A);
// // // 		C.negate();
// // // 		return C;
// // // 	}

// // // 	if (A < B){
// // // 		C = A.sub(B);
// // // 		C.negate();
// // // 		return C;
// // // 	}

// // // 	a.moveBack();
// // // 	b.moveBack();
// // // 	long d = 0;
// // // 	long dummy = 0;
// // // 	int i = b.position();

// // // 	while (!(i <= 0)){
// // // 		if (a.peekPrev() - d < b.peekPrev()){
// // // 			dummy = a.peekPrev() + base - b.peekPrev() - d;
// // // 			d = 1;
// // // 		} else{
// // // 			dummy = a.peekPrev() - d - b.peekPrev();
// // // 			if (!(a.peekPrev() <= 0)){
// // // 				d = 0;
// // // 			}
// // // 		}

// // // 		c.insertAfter(dummy);
// // // 		a.movePrev();
// // // 		b.movePrev();
// // // 		i--;
// // // 	}

// // // 	while (a.position() > 0){
// // // 		if (a.peekPrev() - d < 0){
// // // 			dummy = a.peekPrev() + base - d;
// // // 			d = 1;
// // // 		} else{
// // // 			dummy = a.peekPrev() - d;
// // // 			if (!(a.peekPrev() <= 0)){
// // // 				d = 0;
// // // 			}
// // // 		}
// // // 		c.insertAfter(dummy);
// // // 		a.movePrev();
// // // 	}

// // // 	C.digits = c;
// // // 	remZero(&(C.digits));
// // // 	C.signum = -1;
// // // 	return C;
// // // }

// // // // shiftList()
// // // // Prepends p zero digits to L, multiplying L by base^p. Used by
// mult().
// // // void shiftList(List& L, int p) {
// // //     L.moveBack();
// // //     for (int i = 0; i < p; i++) {
// // //         L.insertAfter(0);
// // //     }
// // // }

// // // List dummyMult(long s, List *b, int* tracker){
// // // 	long t = 0;
// // // 	long c = 0;
// // // 	List L;
// // // 	for (b->moveBack(); b->position() > 0; b->movePrev()){
// // // 		t = (b->peekPrev() * s) + c;
// // // 		c = t / base;
// // // 		t %= base;
// // // 		L.insertAfter(t);
// // // 	}

// // // 	if (c > 0){
// // // 		L.insertAfter(c);
// // // 	}
// // // 	L.moveBack();

// // // 	for (int i = 0; i < *tracker; i++){
// // // 		L.insertAfter(0);
// // // 	}

// // // 	return L;
// // // }

// // // BigInteger BigInteger::mult(const BigInteger &N) const{
// // // 	BigInteger B = *this;
// // // 	BigInteger A = N;
// // // 	BigInteger C;

// // // 	List a = A.digits;
// // // 	List b = B.digits;

// // // 	int tracker = 0;
// // // 	int pos = a.position();

// // // 	a.moveBack();
// // // 	b.moveBack();

// // // /*	for (int i = 0; i < B.length(); i++) {
// // // 		A = N.digits;
// // // 		scalarMultList(A,B.peekPrev());
// // // 		shiftList(A,i);
// // // 		sumList(prod,A,prod,1);
// // // 		normalizeList(prod);
// // // 		B.movePrev();
// // // */

// // // 	for (int i = pos; i > 0; i--){
// // // 		List dummy = dummyMult(a.peekPrev(), &b, &tracker);
// // // 		BigInteger J;
// // // 		J.signum = 1;
// // // 		J.digits = dummy;
// // // 		C = C.add(J);
// // // 		a.movePrev();
// // // 		tracker++;
// // // 	}

// // // 	C.signum = A.signum * B.signum;
// // // 	return C;
// // // }

// // // // BigInteger BigInteger::mult(const BigInteger& N) const {
// // // //     BigInteger Q;
// // // //     if (this->signum == 0 || N.signum == 0) {
// // // //         Q.digits.insertBefore(0);
// // // //         Q.signum = 0;
// // // //         return Q;
// // // //     }
// // // //     if (this->signum == N.signum) {
// // // //         Q.signum = 1;
// // // //     }
// // // //     if (this->signum != N.signum) {
// // // //         Q.signum = -1;
// // // //     }
// // // //     List A, B, C;
// // // //     if (this->digits.length() > N.digits.length()) {
// // // //         A  = this->digits;
// // // //         B = N.digits;
// // // //     }
// // // //     else {
// // // //         A = N.digits;
// // // //         B = this->digits;
// // // //     }
// // // //     C.insertBefore(0);
// // // //     B.moveBack();
// // // //     for (int shift = 0; B.position(); ++shift) {
// // // //         long multi = B.movePrev();
// // // //         if (multi != 0) {
// // // //             scalarMultList(A, multi);
// // // //             sumList(C, A, 1); //make an addlist function;
// // // //             copier(C, CD);
// // // //             normalize(C);
// // // //         }
// // // //     }
// // // //     Q.digits = C;
// // // //     return Q;

// // // // }

// // // // BigInteger BigInteger::mult(const BigInteger& N) const {
// // // //     BigInteger result;

// // // //     // Check for the case where one of the operands is zero
// // // //     if (signum == 0 || N.signum == 0) {
// // // //         result.makeZero();
// // // //         return result;
// // // //     }

// // // //     result.signum = signum * N.signum;

// // // //     List A = digits;
// // // //     List B = N.digits;

// // // //     A.moveBack();
// // // //     while (A.position() > 0) {
// // // //         long digitA = A.movePrev();
// // // //         List tempResult = B;
// // // //         scalarMultList(tempResult, digitA);  // Multiply B by the
// current
// // // digit of A
// // // //         // cout << tempResult << endl;
// // // //         shiftList(tempResult, digits.length() - A.position());  //
// Shift
// // // according to the position of A's current digit
// // // //         List temp = result.digits;
// // // //         sumList(temp, result.digits, tempResult, 1);
// // // //         normalizeList(temp);
// // // //         result.digits = temp;  // Assign the partial result to the
// overall
// // // result
// // // //     }

// // // //     // normalizeList(result.digits);  // Normalize the result

// // // //     return result;
// // // // }

// // // std::string BigInteger::to_string() {
// // //     std::string s = "";
// // //     if (signum) {
// // //         if (signum == -1)
// // //             s += '-';
// // //         digits.moveFront();
// // //         long valq = digits.moveNext();
// // //         while (!valq)
// // //             valq = digits.moveNext();
// // //         digits.movePrev();
// // //         std::string val = std::to_string(digits.moveNext());
// // //         for (int i = 0; i < (int)val.length(); ++i) {
// // //             if (val[i] != '0') {
// // //                 val = val.substr(i, val.length());
// // //                 break;
// // //             }
// // //         }
// // //         s += val;
// // //         while(digits.position() != digits.length()) {
// // //             val = std::to_string(digits.moveNext());
// // //             if ((int)val.length() != power && digits.position() != 1) {
// // //                 for (int i = (int)val.length(); i < power; i++)
// // //                     val = "0" + val;
// // //             }
// // //             s += val;
// // //         }
// // //     } else {
// // //         s += '0';
// // //     }
// // //     return s;
// // // }

// // // // Overriden Operators
// -----------------------------------------------------

// // // std::ostream& operator<<( std::ostream& stream, BigInteger N ){
// // //     return stream << N.BigInteger::to_string();
// // // }

// // // bool operator==( const BigInteger& A, const BigInteger& B ){
// // //     if (A.signum != B.signum)
// // //         return 0;
// // //     BigInteger A2 = A, B2 = B;
// // //     return (A2.to_string() == B2.to_string());
// // // }

// // // bool operator<( const BigInteger& A, const BigInteger& B ){
// // //     return A.compare(B) == -1 ? 1 : 0;
// // // }

// // // bool operator<=( const BigInteger& A, const BigInteger& B ){
// // //     return (A.compare(B) == -1 || !A.compare(B)) ? 1 : 0;
// // // }

// // // bool operator>( const BigInteger& A, const BigInteger& B ){
// // //     return A.compare(B) == 1 ? 1 : 0;
// // // }

// // // bool operator>=( const BigInteger& A, const BigInteger& B ){
// // //     return (A.compare(B) == 1 || !A.compare(B)) ? 1 : 0;
// // // }

// // // BigInteger operator+( const BigInteger& A, const BigInteger& B ){
// // //     return A.add(B);
// // // }

// // // BigInteger operator+=( BigInteger& A, const BigInteger& B ){
// // //     return A = A.add(B);
// // // }

// // // BigInteger operator-( const BigInteger& A, const BigInteger& B ){
// // //     return A.sub(B);
// // // }

// // // BigInteger operator-=( BigInteger& A, const BigInteger& B ){
// // //     return A = A.sub(B);
// // // }

// // // BigInteger operator*( const BigInteger& A, const BigInteger& B ){
// // //     return A.mult(B);
// // // }

// // // BigInteger operator*=( BigInteger& A, const BigInteger& B ){
// // //     return A = A.mult(B);
// // // }

// // // // BigInteger BigInteger::sub(const BigInteger& N) const {
// // // //     BigInteger Q;
// // // //     List A = digits, B = N.digits, C;

// // // //     if (signum == N.signum) {
// // // //         if (A.equals(B)) {
// // // //             Q.signum = 0;
// // // //             C.insertBefore(0);
// // // //             Q.digits = C;
// // // //             return Q;
// // // //         }
// // // //     }

// // // //     if (N.signum == -1 && signum == 1) {
// // // //         BigInteger NT = N;
// // // //         BigInteger TT = *this;
// // // //         NT.signum = 1;
// // // //         Q = TT.add(NT);
// // // //         return Q;
// // // //     }

// // // //     if (signum == -1 && N.signum == 1) {
// // // //         BigInteger NT = N;
// // // //         BigInteger TT = *this;
// // // //         NT.signum = -1;
// // // //         Q = TT.add(NT);
// // // //         return Q;
// // // //     }

// // // //     int tempsign = signum;

// // // //     if (A.length() >= B.length()) {
// // // //         B.moveBack();
// // // //         for (A.moveBack(); B.position() != 0;)
// // // //             C.insertAfter(A.movePrev() - B.movePrev());
// // // //         while (A.position())
// // // //             C.insertAfter(A.movePrev());
// // // //     } else {
// // // //         A.moveBack();
// // // //         for (B.moveBack(); A.position() != 0;)
// // // //             C.insertAfter(A.movePrev() - B.movePrev());
// // // //         while (B.position())
// // // //             C.insertAfter(-1 * B.movePrev());
// // // //     }

// // // //     // Normalize the result directly within the subtraction
// operation
// // // //     C.moveBack();
// // // //     long borrow = 0;

// // // //     while (C.position()) {
// // // //         long value = C.movePrev() - borrow;
// // // //         if (value < 0) {
// // // //             borrow = 1;
// // // //             value += base;
// // // //         } else {
// // // //             borrow = 0;
// // // //         }
// // // //         C.setAfter(value);
// // // //     }

// // // //     // Remove leading zeros
// // // //     while (!C.front() && C.length() > 1)
// // // //         C.eraseAfter();

// // // //     // Set the result List in the BigInteger
// // // //     Q.digits = C;

// // // //     // Determine the sign of the result
// // // //     for (Q.digits.moveBack(); Q.digits.position();) {
// // // //         long val = Q.digits.movePrev();
// // // //         if (val) {
// // // //             if (N.signum == signum)
// // // //                 Q.signum = N.signum;
// // // //             break;
// // // //         }
// // // //     }
// // // //     Q.signum = tempsign;
// // // //     return Q;
// // // // }

// // // // List ListAdd(const List& P, const List& Q) {
// // // //     List C;
// // // //     List A = P;
// // // //     List B = Q;
// // // //     if (A.length() >= B.length()) {
// // // //         B.moveBack();
// // // //         for (A.moveBack(); B.position() != 0;) {
// // // //             long A_val = A.movePrev();
// // // //             long B_val = B.movePrev();
// // // //             long C_val = A_val+B_val;
// // // //             C.insertAfter(C_val);
// // // //         }
// // // //         while (A.position()!= 0) {
// // // //             long A_val = A.movePrev();
// // // //             C.insertAfter(A_val);
// // // //         }
// // // //     }
// // // //     else {
// // // //         A.moveBack();
// // // //         for (B.moveBack(); A.position() != 0;) {
// // // //             long A_val = A.movePrev();
// // // //             long B_val = B.movePrev();
// // // //             long C_val = A_val+B_val;
// // // //             C.insertAfter(C_val);
// // // //         }
// // // //         while (B.position()!= 0) {
// // // //             long B_val = B.movePrev();
// // // //             C.insertAfter(B_val);
// // // //         }
// // // //     }
// // // //     return C;
// // // // }

// // // // BigInteger BigInteger::mult(const BigInteger& N) const {
// // // //     BigInteger Q;

// // // //     if (this->signum == 0 || N.signum == 0) {
// // // //         Q.digits.insertBefore(0);
// // // //         Q.signum = 0;
// // // //         return Q;
// // // //     }

// // // //     if (this->signum == N.signum) {
// // // //         Q.signum = 1;
// // // //     } else {
// // // //         Q.signum = -1;
// // // //     }

// // // //     List A, B, C;
// // // //     if (this->digits.length() > N.digits.length()) {
// // // //         A = this->digits;
// // // //         B = N.digits;
// // // //     } else {
// // // //         A = N.digits;
// // // //         B = this->digits;
// // // //     }

// // // //     C.insertBefore(0);
// // // //     B.moveBack();

// // // //     for (int shift = 0; B.position(); ++shift) {
// // // //         long multi = B.movePrev();
// // // //         if (multi != 0) {
// // // //             List D;
// // // //             A.moveBack();
// // // //             while (A.position()) {
// // // //                 long val = A.movePrev();
// // // //                 long new_val = multi * val;
// // // //                 D.insertAfter(new_val);
// // // //             }
// // // //             D = scalar(D, 1, shift);
// // // //             C = ListAdd(C, D);
// // // //         }
// // // //     }

// // // //     Q.digits = C;
// // // //     return Q;
// // // // }

// // // // void normalize(List& L) {
// // // //     L.moveBack();
// // // //     int max = base;
// // // //     if (L.length() == 1) {
// // // //         long value = L.movePrev();
// // // //         if (value >= max) {
// // // //             long adder = value/base;
// // // //             long new_val = value%base;
// // // //             L.setAfter(new_val);
// // // //             L.insertBefore(adder);
// // // //         }
// // // //         return;
// // // //     }
// // // //     while (L.position() > 2) {
// // // //         long value = L.movePrev();
// // // //         if (value >= base) {
// // // //             long adder = value/base;
// // // //             long new_val = value%base;
// // // //             L.setAfter(new_val);
// // // //             long before_val = L.peekPrev();
// // // //             L.setBefore(before_val + adder);
// // // //         }
// // // //         if (value < 0) {
// // // //             long new_val = value + base;
// // // //             L.setAfter(new_val);
// // // //             long before_val = L.peekPrev();
// // // //             L.setBefore(before_val -1);
// // // //         }
// // // //     }
// // // //     //position is now at 1
// // // //     long value = L.movePrev();
// // // //     while (L.position()) {
// // // //         if (value >= max) {
// // // //             long adder = value/base;
// // // //             long new_val = value%base;
// // // //             L.setAfter(new_val);
// // // //             long before_val = L.peekPrev();
// // // //             if (adder == 0) break;
// // // //             before_val = before_val + adder;
// // // //             //if (L.position() == 1)
// // // //             L.setBefore(before_val);
// // // //         }
// // // //         if (value < 0) {
// // // //             long new_val = value + base;
// // // //             L.setAfter(new_val);
// // // //             long before_val = L.peekPrev();
// // // //             L.setBefore(before_val-1);
// // // //         }
// // // //         value = L.movePrev();
// // // //     }
// // // //     if (value != 0 && value >= max) {
// // // //         long adder = value/base;
// // // //         long new_val = value%base;
// // // //         L.setAfter(new_val);
// // // //         L.insertBefore(adder);
// // // //     }
// // // // }

// // // // List ListAdd(const List& P, const List& Q) {
// // // //     List C;
// // // //     List A = P;
// // // //     List B = Q;
// // // //     if (A.length() >= B.length()) {
// // // //         B.moveBack();
// // // //         for (A.moveBack(); B.position() != 0;) {
// // // //             long A_val = A.movePrev();
// // // //             long B_val = B.movePrev();
// // // //             long C_val = A_val+B_val;
// // // //             C.insertAfter(C_val);
// // // //         }
// // // //         while (A.position()!= 0) {
// // // //             long A_val = A.movePrev();
// // // //             C.insertAfter(A_val);
// // // //         }
// // // //     }
// // // //     else {
// // // //         A.moveBack();
// // // //         for (B.moveBack(); A.position() != 0;) {
// // // //             long A_val = A.movePrev();
// // // //             long B_val = B.movePrev();
// // // //             long C_val = A_val+B_val;
// // // //             C.insertAfter(C_val);
// // // //         }
// // // //         while (B.position()!= 0) {
// // // //             long B_val = B.movePrev();
// // // //             C.insertAfter(B_val);
// // // //         }
// // // //     }
// // // //     return C;
// // // // }