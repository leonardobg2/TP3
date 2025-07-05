#include "Lista.hpp"

template <typename T>
Lista<T>::Lista() : inicio(nullptr), fim(nullptr), tamanho(0) {}

template <typename T>
Lista<T>::Lista(const Lista& other) : inicio(nullptr), fim(nullptr), tamanho(0) {
    Node* currentOther = other.inicio;
    while (currentOther) {
        push_back(currentOther->valor);
        currentOther = currentOther->prox;
    }
}

template <typename T>
Lista<T>::~Lista() { clear(); }

template <typename T>
Lista<T>& Lista<T>::operator=(const Lista& other) {
    if (this == &other) return *this;

    clear();
    Node* currentOther = other.inicio;
    while (currentOther) {
        push_back(currentOther->valor);
        currentOther = currentOther->prox;
    }
    return *this;
}

template <typename T>
void Lista<T>::push_front(const T& valor) {
    Node* novo = new Node(valor);
    novo->prox = inicio;
    inicio = novo;
    if (!fim) fim = novo; // Se a lista estava vazia
    tamanho++;
}

template <typename T>
void Lista<T>::push_back(const T& valor) {
    Node* novo = new Node(valor);
    if (!inicio) {
        inicio = fim = novo;
    } else {
        fim->prox = novo;
        fim = novo;
    }
    tamanho++;
}

template <typename T>
void Lista<T>::pop_front() {
    if (!inicio) return;
    Node* temp = inicio;
    inicio = inicio->prox;
    delete temp;
    tamanho--;
    if (!inicio) fim = nullptr; // Lista ficou vazia
}

template <typename T>
bool Lista<T>::empty() const { return tamanho == 0; }

template <typename T>
int Lista<T>::size() const { return tamanho; }

template <typename T>
void Lista<T>::clear() {
    while (!empty())
        pop_front();
}

template <typename T>
int Lista<T>::find(const T& valor) const {
    Node* atual = inicio;
    int idx = 0;
    while (atual) {
        if (atual->valor == valor)
            return idx;
        atual = atual->prox;
        idx++;
    }
    return -1;
}

template <typename T>
T& Lista<T>::operator[](int index) {
    if (index < 0 || index >= tamanho)
        throw "Erro: Indice fora dos limites da lista!";
    
    Node* atual = inicio;
    for (int i = 0; i < index; ++i)
        atual = atual->prox;
    return atual->valor;
}

template <typename T>
const T& Lista<T>::operator[](int index) const {
    if (index < 0 || index >= tamanho)
        throw "Erro: Indice fora dos limites da lista (const)!";
    
    Node* atual = inicio;
    for (int i = 0; i < index; ++i)
        atual = atual->prox;
    return atual->valor;
}
