#include <iterator>
#include <exception>


#ifndef LINKED_LIST_HPP
#define LINKED_LIST_HPP


template<typename T> class list {
private:
  struct node;

  node* first;
  node* last;
  int length;

  void _clear() {
    node* p = first;
    node* nextp;
    for(int i = 0; i < length; ++i) {
      nextp = p->next;
      delete p;
      p = nextp;
    }
    first = nullptr;
    last = nullptr;
    length = 0;
  }

  node* _findNode(const int& index) {
    node* p = first;
    for(int i = 0; i < index; ++i) {
      p = p->next;
    }
    return p;
  }
  const node* _findNode(const int& index) const {
    node* p = first;
    for(int i = 0; i < index; ++i) {
      p = p->next;
    }
    return p;
  }

  void _resize(const int& size, const T* const value) {
    if(length > size) {
      if(size <= 0) {
        _clear();
      } else {
        node* p = first;
        for(int i = 0; i < size - 1; ++i) {
          p = p->next;
        }
        last = p;

        node* nextp;
        nextp = p->next;
        p->next = nullptr;
        for(int i = size; i < length; ++i) {
          p = nextp;
          nextp = p->next;
          delete p;
        }

        length = size;
      }
    } else if(length < size) {
      node* p = last;

      if(value == nullptr) {
        for(int i = length - 1; i < size - 1; ++i) {
          p->next = new node;
          p = p->next;
        }
      } else {
        for(int i = length - 1; i < size - 1; ++i) {
          p->next = new node(new T(*value));
          p = p->next;
        }
      }

      last = p;
      length = size;
    }
  }

  void _add(T* const value) {
    if(length == 0) {
      first = new node(value);
      last = first;
      length++;
    } else {
      last->next = new node(value);
      last = last->next;
      length++;
    }
  }

  void _insert(const int& index, T* const value) {
    if(index == length) {
      _add(value);
    } else if(index == 0) {
      first = new node(value, first);
      length++;
    } else {
      node* p = first;
      for(int i = 0; i < index - 1; ++i) {
        p = p->next;
      }
      p->next = new node(value, p->next);
      length++;
    }
  }

  void _pop() {
    if(length == 1) {
      // Delete the only element
      delete first;
      first = nullptr;
      last = nullptr;
      length = 0;
    } else if(length > 1) {
      // Delete the last element
      delete last;
      length--;

      // Find the new last element
      last = _findNode(length - 1);

      // Clear its now-deleted next node
      last->next = nullptr;
    }
  }

/*
  void _remove(const int& index) {
    if(index == length - 1) {
      _pop();
    } else if(index == 0) {
      node* nextp = first->next;
      delete first;
      first = nextp;
      length--;
    } else {
      node* p = first;
      for(int i = 0; i < index - 1; ++i) {
        p = p->next;
      }

      node* nextp = p->next->next;
      delete p->next;
      p->next = nextp;
      length--;
    }
  } */
  void _remove(node*& ptr) {
    if(ptr == last) {
      _pop();
      ptr = nullptr;
      return;
    }

    node* p = ptr->next;
    if(p == last) {
      last = ptr;
    }

    delete ptr->value;
    ptr->value = p->value;
    ptr->next = p->next;

    p->value = nullptr;
    delete p;
    length--;
  }

public:
  class iterator;
  class const_iterator;
  class OutOfRange;

  list<T>() : first(nullptr), last(nullptr), length(0) {}

  /* Deep Copy */
  list<T>(const list<T>& other) : length(other.length) {
    if(length != 0) {
      node* otherp = other.first;
      first = new node(new T(*otherp->value));
      node* thisp = first;
      otherp = otherp->next;
      while(otherp != nullptr) {
        thisp->next = new node(new T(*otherp->value));
        thisp = thisp->next;
        otherp = otherp->next;
      }
      last = &thisp;
    } else {
      first = nullptr;
    }
  }

  /* Deep Copy */
  list<T>& operator=(const list<T>& other) {
    if(this == &other) {
      return *this;
    }

    // Resize
    _resize(other.length, nullptr);

    // Assign values
    node* thisp = first;
    node* otherp = other.first;
    while(otherp != nullptr) {
      delete thisp->value;
      thisp->value = new T(*otherp->value);
      thisp = thisp->next;
      otherp = otherp->next;
    }
    
    return *this;
  }

  ~list() {
    node* p = first;
    node* nextp;
    while(p != nullptr) {
      nextp = p->next;
      delete p;
      p = nextp;
    }
  }

  iterator begin() { return iterator(first); }
  const_iterator begin() const { return const_iterator(first); }

  iterator end() { return nullptr; }
  const_iterator end() const { return nullptr; }

  constexpr int size() const { return length; }

  T& operator[](const int& index) { return *_findNode(index)->value; }
  const T& operator[](const int& index) const { return *_findNode(index)->value; }

  T& at(const int& index) {
    if(index < 0 || index >= length) {
      throw OutOfRange(length, index);
    }
    return operator[](index);
  }
  const T& at(const int& index) const {
    if(index < 0 || index >= length) {
      throw OutOfRange(length, index);
    }
    return operator[](index);
  }

  T& back() { return *last->value; }
  const T& back() const { return *last->value; }

  void clear() { _clear(); }

  void resize(const int& size) { _resize(size, nullptr); }
  void resize(const int& size, const T& value) { _resize(size, &value); }

  void add(const T& value) {
    _add(new T(value));
  }

  void insert(const int& index, const T& value) {
    if(index < 0 || index > length) {
      throw OutOfRange(length, index);
    }
    _insert(index, new T(value));
  }

  void pop() { _pop(); }

  void remove(const int& index) {
    if(index < 0 || index >= length) {
      throw OutOfRange(length, index);
    }
    node* p = _findNode(index);
    _remove(p);
  }
  void remove(iterator& it) {
    _remove(it.ptr);
  }

  void transferTo(list& other) {
    if(this == &other) {
      return;
    }

    other.clear();
    other.first = first;
    other.last = last;
    other.length = length;
    
    first = nullptr;
    last = nullptr;
    length = 0;
  }
};


template<typename T> struct list<T>::node {
  T* value;
  node* next;

  node() : value(nullptr), next(nullptr) {}
  node(T* const value) : value(value), next(nullptr) {}
  node(T* const value, node* const next) : value(value), next(next) {}

  node(const node& other) : next(nullptr) { // Deep Copy
    value = new T(*other.value);
  }
  node& operator=(const node& other) { // Deep Copy
    if(this == &other) {
      return *this;
    }

    delete value;
    value = new T(other.value);
    next = other.next;
    return *this;
  }

  ~node() {
    delete value;
  }
};


template<typename T> class list<T>::iterator {
  using iterator_category = std::forward_iterator_tag;
  friend class list<T>;

private:
  node* ptr;

public:
  iterator() : ptr(nullptr) {}
  iterator(std::nullptr_t) : ptr(nullptr) {}
  iterator(node& n) : ptr(&n) {}
  iterator(node* const n) : ptr(n) {}

  T& operator*() const { return *ptr->value; }
  T* operator->() const { return ptr->value; }
  T& value() const {
    if(ptr == nullptr) throw std::runtime_error("Trying to access null iterator.");
    if(ptr->value == nullptr) throw std::runtime_error("Trying to access null node value.");
    return *ptr->value;
  }

  iterator& operator++() {
    if(ptr) {
      ptr = ptr->next;
    }
    return *this;
  }
  iterator operator++(int) {
    iterator prev = *this;
    if(ptr) {
      ptr = ptr->next;
    }
    return prev;
  }
  iterator next() const { return iterator(ptr->next); }

  bool operator==(const iterator& other) const { return ptr == other.ptr; }
  bool operator<(const iterator& other) const { return ptr < other.ptr; }
  bool operator>(const iterator& other) const { return ptr > other.ptr; }

  operator bool() const { return ptr != nullptr; }
  bool isnull() const { return ptr == nullptr; }
  bool valid() const { return ptr != nullptr; }
  bool nextValid() const { return ptr->next != nullptr; }

  void clear() const { ptr = nullptr; }
};

template<typename T> class list<T>::const_iterator {
  using iterator_category = std::forward_iterator_tag;

private:
  const node* ptr;

public:
  const_iterator() : ptr(nullptr) {}
  const_iterator(std::nullptr_t) : ptr(nullptr) {}
  const_iterator(const node& n) : ptr(&n) {}
  const_iterator(const node* const n) : ptr(n) {}

  const T& operator*() const { return *ptr->value; }
  const T* operator->() const { return ptr->value; }

  const_iterator& operator++() {
    if(ptr) {
      ptr = ptr->next;
    }
    return *this;
  }
  const_iterator operator++(int) {
    const_iterator prev = *this;
    if(ptr) {
      ptr = ptr->next;
    }
    return prev;
  }
  const_iterator next() const { return const_iterator(ptr->next); }

  bool operator==(const const_iterator& other) const { return ptr == other.ptr; }
  bool operator<(const const_iterator& other) const { return ptr < other.ptr; }
  bool operator>(const const_iterator& other) const { return ptr > other.ptr; }

  operator bool() const { return ptr != nullptr; }
  bool isnull() const { return ptr == nullptr; }
  bool valid() const { return ptr != nullptr; }
  bool nextValid() const { return ptr->next != nullptr; }

  const T& node() const {
    if(ptr == nullptr) throw std::runtime_error("Trying to access null iterator.");
    if(ptr->value == nullptr) throw std::runtime_error("Trying to access null node value.");
    return *ptr->value;
  }
  
  void clear() const { ptr = nullptr; }
};


template<typename T>
class list<T>::OutOfRange : public std::exception
{
  std::string msg;

public:
  OutOfRange() : msg("Linked List index out of range.") {}
  OutOfRange(int containerSize, int requestedSize) : msg("Linked List index out of range.") {
    msg += " Container size: " + std::to_string(containerSize);
    msg += "; requested index: " + std::to_string(requestedSize);
  }

  virtual const char* what() const throw()
  {
    return msg.c_str();
  }
};


#endif // LINKED_LIST_HPP
