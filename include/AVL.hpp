#ifndef AVL_HPP
#define AVL_HPP

#include <iostream> // Para std::cout, std::endl

// Definicao da funcao max simples (substituindo std::max)
template <typename T>
T my_max(T a, T b) {
    return (a > b) ? a : b;
}

template <typename TKey, typename TValue>
class AVL {
private:
    struct node {
        TKey key;
        TValue value;
        node* left;
        node* right;
        int height;

        node(const TKey& k, const TValue& v) : key(k), value(v), left(nullptr), right(nullptr), height(1) {}
    };

    node* root;

    int height(node* current);
    void updateHeight(node* current);
    int getBalance(node* current);

    node* rightRotation(node* current);
    node* leftRotation(node* current);

    node* insertUtility(node* current, const TKey& key, const TValue& value);
    node* removeUtility(node* current, const TKey& key);
    
    node* findMinNode(node* current);
    node* removeMinNode(node* current);
    node* findMaxNode(node* current); 

    // --- A DECLARACAO QUE ESTAVA FALTANDO! ---
    node* searchNode(node* current, const TKey& key); 

    void inOrderUtility(node* current) const;
    void preOrderUtility(node* current) const;
    void postOrderUtility(node* current) const;

    void destroyTree(node* current);

public:
    AVL();
    ~AVL();

    void insert(const TKey& key, const TValue& value);
    TValue* search(const TKey& key);
    void remove(const TKey& key);

    void inorder() const;
    void preOrder() const;
    void postOrder() const;
};

#include "AVL.tpp"
#endif // AVL_HPP