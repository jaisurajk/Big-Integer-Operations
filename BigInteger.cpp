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
