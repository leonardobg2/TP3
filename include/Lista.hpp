#ifndef LISTA_HPP
#define LISTA_HPP

template <typename T>
class Lista {
private:
    struct Node {
        T valor;
        Node* prox;
        Node(const T& v) : valor(v), prox(nullptr) {}
    };

    Node* inicio;
    Node* fim;
    int tamanho;

public:
    Lista();
    Lista(const Lista& other); // Construtor de Copia
    ~Lista();
    
    Lista& operator=(const Lista& other); // Operador de Atribuicao de Copia

    void push_front(const T& valor);
    void push_back(const T& valor);
    void pop_front();
    bool empty() const;
    int size() const;
    void clear();
    int find(const T& valor) const;
    
    T& operator[](int index);
    const T& operator[](int index) const;
};

#include "../src/Lista.tpp"
#endif