#ifndef VECTOR_HPP
#define VECTOR_HPP

template <typename T>
class Vector {
public:
    Vector();                                     // construtor padrão
    Vector(int n, const T& value);               // construtor refinado (n cópias de value)
    Vector(unsigned int n);                             // **novo**: n cópias de T()
    Vector(const Vector& copy);                   // cópia
    Vector(Vector&& outro);                       // movimento
    ~Vector();                                    // destrutor

    unsigned int get_size() const;
    unsigned int get_capacity() const;
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
    unsigned int size;
    unsigned int capacity;
    T*     elements;
};

#include "../src/Vector.tpp"
#endif
