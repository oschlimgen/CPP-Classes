#include <cstddef>


#ifndef HEAP_ARR_HPP
#define HEAP_ARR_HPP

template<typename T, std::size_t n>
struct heapArr {
private:
  T* arr;

public:
  heapArr() : arr(new T[n]) {}

  heapArr(const T& element) : arr(new T[n]) {
    for(std::size_t i = 0; i < n; ++i) {
      arr[i] = element;
    }
  }

  heapArr(const heapArr<T,n>& other) : arr(new T[n]) {
    for(std::size_t i = 0; i < n; ++i) {
      arr[i] = other.arr[i];
    }
  }
  heapArr(heapArr<T,n>&& other) : arr(other.arr) {
    other.arr = nullptr;
  }
  heapArr<T,n> operator=(const heapArr<T,n>& other) {
    if(this != &other) {
      for(std::size_t i = 0; i < n; ++i) {
        arr[i] = other.arr[i];
      }
    }
    return *this;
  }
  heapArr<T,n> operator=(heapArr<T,n>&& other) {
    if(this != &other) {
      delete arr;
      arr = other.arr;
      other.arr = nullptr;
    }
    return *this;
  }
  ~heapArr() {
    delete[] arr;
  }

  T& operator[](const std::size_t index) {
    return arr[index];
  }
  const T& operator[](const std::size_t index) const {
    return arr[index];
  }

  T& at(const std::size_t index) {
    if(index >= n) {
      throw std::out_of_range("Index out of range.");
    }
    return arr[index];
  }
  const T& at(const std::size_t index) const {
    if(index >= n) {
      throw std::out_of_range("Index out of range.");
    }
    return arr[index];
  }
};

#endif /* HEAP_ARR_HPP */