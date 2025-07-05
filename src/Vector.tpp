#include "../include/Vector.hpp"

const int DEFAULT = 10;

template <typename T>
Vector<T>::Vector() : size(0), capacity(DEFAULT), elements(new T[capacity]) {}

template <typename T>
Vector<T>::Vector(int n, const T& value)
  : size(n), capacity(n * 2), elements(new T[capacity])
{
    for (size_t i = 0; i < size; ++i)
        elements[i] = value;
}

template <typename T>
Vector<T>::Vector(size_t n)
  : size(n), capacity(n * 2), elements(new T[capacity])
{
    for (size_t i = 0; i < size; ++i)
        elements[i] = T();  // default-constructed
}

template <typename T>
Vector<T>::Vector(const Vector& copy)
  : size(copy.size), capacity(copy.capacity), elements(new T[capacity])
{
    for (size_t i = 0; i < size; ++i)
        elements[i] = copy.elements[i];
}

template <typename T>
Vector<T>::Vector(Vector&& outro)
  : size(outro.size), capacity(outro.capacity), elements(outro.elements)
{
    outro.size     = 0;
    outro.capacity = 0;
    outro.elements = nullptr;
}

template <typename T>
Vector<T>::~Vector() {
    delete[] elements;
}

template <typename T>
size_t Vector<T>::get_size() const {
    return size;
}

template <typename T>
size_t Vector<T>::get_capacity() const {
    return capacity;
}

template <typename T>
bool Vector<T>::is_empty() const {
    return size == 0;
}

template <typename T>
T& Vector<T>::operator[](int index) {
    if (index < 0 || static_cast<size_t>(index) >= size)
        throw std::out_of_range("Vector index out of range");
    return elements[index];
}

template <typename T>
const T& Vector<T>::operator[](int index) const {
    if (index < 0 || static_cast<size_t>(index) >= size)
        throw std::out_of_range("Vector index out of range");
    return elements[index];
}

template <typename T>
Vector<T>& Vector<T>::operator=(const Vector& rhs) {
    if (this != &rhs) {
        delete[] elements;
        size     = rhs.size;
        capacity = rhs.capacity;
        elements = new T[capacity];
        for (size_t i = 0; i < size; ++i)
            elements[i] = rhs.elements[i];
    }
    return *this;
}

template <typename T>
Vector<T>& Vector<T>::operator=(Vector&& outro) {
    if (this != &outro) {
        delete[] elements;
        elements = outro.elements;
        size     = outro.size;
        capacity = outro.capacity;
        outro.elements = nullptr;
        outro.size     = 0;
        outro.capacity = 0;
    }
    return *this;
}

template <typename T>
void Vector<T>::push_back(const T& obj) {
    if (size == capacity) {
        T* newarr = new T[capacity * 2];
        for (size_t i = 0; i < size; ++i)
            newarr[i] = elements[i];
        delete[] elements;
        elements = newarr;
        capacity *= 2;
    }
    elements[size++] = obj;
}

template <typename T>
void Vector<T>::pop_back() {
    if (size > 0) --size;
}

template <typename T>
void Vector<T>::erase(int index) {
    if (index < 0 || static_cast<size_t>(index) >= size)
        throw std::out_of_range("erase() index out of range");
    for (size_t i = index; i + 1 < size; ++i)
        elements[i] = elements[i + 1];
    --size;
}

template <typename T>
void Vector<T>::insert(int index, const T& value) {
    if (index < 0 || static_cast<size_t>(index) > size)
        throw std::out_of_range("insert() index out of range");
    if (size == capacity) {
        T* newarr = new T[capacity * 2];
        for (size_t i = 0; i < size; ++i)
            newarr[i] = elements[i];
        delete[] elements;
        elements = newarr;
        capacity *= 2;
    }
    for (size_t i = size; i > static_cast<size_t>(index); --i)
        elements[i] = elements[i - 1];
    elements[index] = value;
    ++size;
}

template <typename T>
void Vector<T>::clear() {
    size = 0;
}
