#ifndef VECTOR_HPP
#define VECTOR_HPP

template <typename T>
class Vector {
public:
    Vector();                                     // construtor padrão
    Vector(int n, const T& value);               // construtor refinado (n cópias de value)
    Vector(size_t n);                             // **novo**: n cópias de T()
    Vector(const Vector& copy);                   // cópia
    Vector(Vector&& outro);                       // movimento
    ~Vector();                                    // destrutor

    size_t get_size() const;
    size_t get_capacity() const;
    bool is_empty() const;

    T& operator [](int index);
    const T& operator[](int index) const;

    Vector& operator =(const Vector&);            // atribuição por cópia
    Vector& operator=(Vector&& outro);            // atribuição por movimento

    void push_back(const T& obj);
    void pop_back();
    void erase(int index);
    void insert(int index, const T& value);
    void clear();

private:
    size_t size;
    size_t capacity;
    T*     elements;
};

#include "../src/Vector.tpp"
#endif
