// Name: Jaisuraj Kaleeswaran
// CruzID: jkaleesw
// Assignment Name: pa6

#include "List.h"
#include <iostream>
#include <stdexcept>
#include <string>

// Private Constructor --------------------------------------------------------

// Node constructor
List::Node::Node(ListElement x) {
  data = x;
  next = prev = nullptr;
}

// Class Constructors & Destructors -------------------------------------------

// Creates a new List in the empty state.
List::List() {
  frontDummy = new Node(0);
  backDummy = new Node(0);
  frontDummy->prev = backDummy->next = nullptr;
  backDummy->prev = beforeCursor = frontDummy;
  frontDummy->next = afterCursor = backDummy;
  num_elements = pos_cursor = 0;
}

// Copy Constructor.
List::List(const List &L) {
  frontDummy = new Node(0);
  backDummy = new Node(0);
  frontDummy->prev = backDummy->next = nullptr;
  backDummy->prev = beforeCursor = frontDummy;
  frontDummy->next = afterCursor = backDummy;
  num_elements = pos_cursor = 0;
  // load elements of Q into this List
  Node *N = L.frontDummy->next;
  while (N != L.backDummy) {
    insertBefore(N->data);
    N = N->next;
  }
}

// Destructor
List::~List() {
  clear();
  delete frontDummy, delete backDummy;
}

// Access functions -----------------------------------------------------------

// Front()
// Returns the value at the front of List.
// Pre: !isEmpty()
ListElement List::front() const {
  if (length())
    return frontDummy->next->data;
  throw std::length_error("List: front(): empty list");
}

ListElement List::back() const {
  if (length())
    return backDummy->prev->data;
  throw std::length_error("List: back(): empty List");
}

// getLength()
// Returns the length of List.
int List::length() const { return num_elements; }

int List::position() const { return pos_cursor; }

ListElement List::peekNext() const {
  Node *N = frontDummy;
  if (pos_cursor != length()) {
    for (int i = 0; i <= pos_cursor; i++)
      N = N->next;
    return N->data;
  }
  throw std::range_error("List: peekNext(): cursor at back");
}

ListElement List::peekPrev() const {
  Node *N = frontDummy;
  if (pos_cursor) {
    for (int i = 0; i < pos_cursor; i++)
      N = N->next;
    return N->data;
  }
  throw std::range_error("List: peekPrev(): cursor at front");
}

// Manipulation procedures ----------------------------------------------------

void List::clear() {
  if (length()) {
    Node *N = frontDummy->next;
    while (N != backDummy) {
      Node *aN = N;
      N = aN->next;
      delete aN;
    }
  }
  frontDummy->prev = backDummy->next = nullptr;
  frontDummy->next = afterCursor = backDummy;
  backDummy->prev = beforeCursor = frontDummy;
  num_elements = pos_cursor = 0;
}

void List::moveFront() { pos_cursor = 0; }

void List::moveBack() { pos_cursor = length(); }

ListElement List::moveNext() {
  if (pos_cursor != length()) {
    ListElement N = peekNext();
    pos_cursor++;
    return N;
  }
  throw std::range_error("List: moveNext(): cursor at back");
}

ListElement List::movePrev() {
  if (pos_cursor) {
    ListElement N = peekPrev();
    pos_cursor--;
    return N;
  }
  throw std::range_error("List: movePrev(): cursor at front");
}

void List::insertAfter(ListElement x) {
  Node *N = new Node(x), *NF = frontDummy, *NB = backDummy;
  if (length()) {
    for (int i = 0; i < pos_cursor; i++)
      NF = NF->next;
    NF->next->prev = N, N->next = NF->next, NF->next = N;
  } else {
    NF->next = N, NB->prev = N, N->next = NB;
  }
  N->prev = NF;
  num_elements++;
}

void List::insertBefore(ListElement x) {
  Node *N = new Node(x), *NF = frontDummy, *NB = backDummy;
  if (length()) {
    for (int i = 0; i <= pos_cursor; i++)
      NF = NF->next;
    NF->prev->next = N, N->prev = NF->prev;
    NF->prev = N, N->next = NF;
  } else {
    NF->next = NB->prev = N;
    N->next = NB, N->prev = NF;
  }
  num_elements++, pos_cursor++;
}

void List::setAfter(ListElement x) {
  Node *N = frontDummy->next;
  if (pos_cursor != length()) {
    for (int i = 0; i <= pos_cursor; i++)
      N = N->next;
    N->data = x;
    return;
  }
  throw std::range_error("List: setAfter(): cursor at back");
}

void List::setBefore(ListElement x) {
  Node *N = frontDummy;
  if (pos_cursor) {
    for (int i = 0; i < pos_cursor; i++)
      N = N->next;
    N->data = x;
    return;
  }
  throw std::range_error("List: setBefore(): cursor at front");
}

void List::eraseAfter() {
  if (pos_cursor != num_elements) {
    Node *N = frontDummy;
    for (int i = 0; i <= pos_cursor; i++)
      N = N->next;
    Node *aN = N;
    N->next->prev = N->prev, N->prev->next = N->next;
    delete aN;
    num_elements--;
    return;
  }
  throw std::range_error("List: eraseAfter(): cursor at back");
}

void List::eraseBefore() {
  if (pos_cursor) {
    Node *N = frontDummy;
    for (int i = 0; i < pos_cursor; i++)
      N = N->next;
    Node *aN = N;
    N->next->prev = N->prev, N->prev->next = N->next;
    delete aN;
    num_elements--, pos_cursor--;
    return;
  }
  throw std::range_error("List: eraseBefore(): cursor at front");
}

// Other Functions ------------------------------------------------------------

int List::findNext(ListElement x) {
  Node *N = frontDummy;
  for (int i = 0; i <= pos_cursor; i++)
    N = N->next;
  while (N->data != x) {
    N = N->next;
    pos_cursor++;
    if (pos_cursor == length())
      return -1;
  }
  pos_cursor++;
  return position();
}

int List::findPrev(ListElement x) {
  Node *N = frontDummy;
  for (int i = 0; i < pos_cursor; i++)
    N = N->next;
  while (N->data != x) {
    N = N->prev;
    pos_cursor--;
    if (!pos_cursor)
      return -1;
  }
  pos_cursor--;
  return position();
}

void List::cleanup() {
  if (length() != 0 || length() != 1) {
    for (Node *N = frontDummy; N != backDummy; N = N->next) {
      int poscount = -1;
      for (Node *Nx = frontDummy->next; Nx != backDummy; Nx = Nx->next) {
        poscount++;
        if (N->data == Nx->data && N != Nx) {
          Node *aN = Nx;
          Nx->next->prev = Nx->prev, Nx->prev->next = Nx->next;
          Nx = Nx->prev;
          delete aN;
          num_elements--;
          if (poscount < pos_cursor)
            pos_cursor--, poscount--;
        }
      }
    }
  }
}

List List::concat(const List &L) const {
  List l;
  Node *N = frontDummy->next, *M = L.frontDummy->next;
  while (N != backDummy) {
    l.insertBefore(N->data);
    N = N->next;
  }
  while (M != L.backDummy) {
    l.insertBefore(M->data);
    M = M->next;
  }
  l.pos_cursor = 0;
  return l;
}

// to_string()
// Returns a string representation of List consisting of a space separated
// list of data values.
std::string List::to_string() const {
  std::string result = "(";
  Node *current = frontDummy->next;
  while (current != backDummy) {
    result += std::to_string(current->data);
    if (current->next != backDummy)
      result += ", ";
    current = current->next;
  }
  return result += ")";
}

// equals()
// Returns true if and only if this is the same sequence as Q.
bool List::equals(const List &L) const {
  bool eq = (num_elements == L.num_elements);
  //   Node *N = nullptr, *M = nullptr;

  Node *N = frontDummy->next, *M = L.frontDummy->next;
  while (eq && N != nullptr) {
    eq = (N->data == M->data);
    N = N->next, M = M->next;
  }
  return eq;
}

// Overloaded Operators --------------------------------------------------------

// operator<<()
// Inserts string representation of Q, as defined by member function
// to_string(), into stream.
std::ostream &operator<<(std::ostream &stream, const List &L) {
  return stream << L.to_string();
}

// operator==()
// Returns true if and only if List A equals List B, as defined by member
// member function equals().
bool operator==(const List &A, const List &B) { return A.equals(B); }

// operator=()
// Overwrites the state of this List with state of L, then returns a reference
// to this List.
List &List::operator=(const List &L) {
  if (this != &L) { // not self assignment
    // make a copy of Q
    List temp = L;

    // then swap the copy's fields with fields of this
    std::swap(frontDummy, temp.frontDummy);
    std::swap(backDummy, temp.backDummy);
    std::swap(beforeCursor, temp.beforeCursor);
    std::swap(afterCursor, temp.afterCursor);
    std::swap(num_elements, temp.num_elements);
    std::swap(pos_cursor, temp.pos_cursor);
  }

  // return this with the new data installed
  return *this;

  // the copy, if there is one, is deleted upon return
}
