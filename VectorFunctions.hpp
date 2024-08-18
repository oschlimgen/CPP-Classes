#include <vector>
#include <string>
#include <random>
#include <chrono>

#ifndef VECTORFUNCTIONS_H
#define VECTORFUNCTIONS_H

namespace VectorFunctions
{
  namespace
  {
    std::mt19937 generator(std::chrono::system_clock::now().time_since_epoch().count());
  }

  
  // Converts vector to a comma-seperated string
  template <typename T>
  std::string toString(const typename std::vector<T> &v)
  {
    std::string s = "";
    for (const T &i : v)
    {
      s += (std::string)i + ',';
    }
    return s;
  }

  template <typename T>
  typename std::vector<T>::const_iterator find(typename std::vector<T> &v, const T &element)
  {
    for (typename std::vector<T>::const_iterator it = v.begin(); it != v.end(); ++it)
    {
      if (*it == element)
      {
        return it;
      }
    }
    return v.end();
  }

  template <typename T>
  int count(typename std::vector<T> &v, const T &element)
  {
    int count = 0;
    for (typename std::vector<T>::iterator it = v.begin(); it != v.end(); ++it)
    {
      if (*it == element)
      {
        ++count;
      }
    }
    return count;
  }

  // Deletes the first instance of an element. Returns true if successful.
  template <typename T>
  bool remove(typename std::vector<T> &v, const T &element)
  {
    for (typename std::vector<T>::iterator it = v.begin(); it != v.end(); ++it)
    {
      if (*it == element)
      {
        v.erase(it);
        return true;
      }
    }
    return false;
  }

  // Randomly choose and return an element of a vector
  template <typename T>
  const T &choose(const typename std::vector<T> &v)
  {
    if (v.size() == 0)
      throw std::out_of_range("Can't return element. Vector size is 0.");
    static std::uniform_int_distribution<> dist(0, v.size() - 1);
    int i = std::uniform_int_distribution<>(0, v.size() - 1)(generator);
    return v.at(i);
  }

  // Randomly choose and remove an element of a vector. Return a copy.
  template <typename T>
  T chooseRemove(typename std::vector<T> &v)
  {
    if (v.size() == 0)
      throw std::out_of_range("Can't return element. Vector size is 0.");
    static std::uniform_int_distribution<> dist(0, v.size() - 1);
    int i = std::uniform_int_distribution<>(0, v.size() - 1)(generator);
    T element = v.at(i);
    v.erase(v.begin() + i);
    return element;
  }

  // Determines if two vectors contain all of the same elements
  template <typename T>
  bool isPermutation(const typename std::vector<T> &sortedReference, typename std::vector<T> v) {
    // Check if the vectors have the same size
    if (sortedReference.size() != v.size()) {
      return false;
    }

    // Sort the vector
    std::sort(v.begin(), v.end());

    // Compare the sorted vectors
    return sortedReference == v;
  }
}


#endif /* VECTORFUNCTIONS_H */
