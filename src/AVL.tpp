#include "AVL.hpp"

// --- Construtor e Destrutor ---

template <typename TKey, typename TValue>
AVL<TKey, TValue>::AVL() : root(nullptr) {}

template <typename TKey, typename TValue>
AVL<TKey, TValue>::~AVL() {
    destroyTree(root);
}

// --- Funcoes Auxiliares Privadas ---

template <typename TKey, typename TValue>
void AVL<TKey, TValue>::destroyTree(node* current) {
    if (current != nullptr) {
        destroyTree(current->left);
        destroyTree(current->right);
        delete current;
    }
}

template <typename TKey, typename TValue>
int AVL<TKey, TValue>::height(node* current) {
    if (current == nullptr) return 0;
    return current->height;
}

template <typename TKey, typename TValue>
void AVL<TKey, TValue>::updateHeight(node* current) {
    if (current != nullptr) {
        current->height = 1 + my_max(height(current->left), height(current->right));
    }
}

template <typename TKey, typename TValue>
int AVL<TKey, TValue>::getBalance(node* current) {
    if (current == nullptr) return 0;
    return height(current->left) - height(current->right);
}

// --- Rotacoes ---

template <typename TKey, typename TValue>
typename AVL<TKey, TValue>::node* AVL<TKey, TValue>::rightRotation(node* y) {
    node* x = y->left;
    node* T2 = x->right;

    x->right = y;
    y->left = T2;

    updateHeight(y);
    updateHeight(x);

    return x;
}

template <typename TKey, typename TValue>
typename AVL<TKey, TValue>::node* AVL<TKey, TValue>::leftRotation(node* x) {
    node* y = x->right;
    node* T2 = y->left;

    y->left = x;
    x->right = T2;

    updateHeight(x);
    updateHeight(y);

    return y;
}

// --- Insercao ---

template <typename TKey, typename TValue>
typename AVL<TKey, TValue>::node* AVL<TKey, TValue>::insertUtility(node* current, const TKey& key, const TValue& value) {
    if (current == nullptr) {
        return new node(key, value);
    }

    if (key < current->key) {
        current->left = insertUtility(current->left, key, value);
    } else if (key > current->key) {
        current->right = insertUtility(current->right, key, value);
    } else {
        current->value = value;
        return current;
    }

    updateHeight(current);
    int balance = getBalance(current);

    if (balance > 1 && key < current->left->key) { // LL
        return rightRotation(current);
    }
    if (balance < -1 && key > current->right->key) { // RR
        return leftRotation(current);
    }
    if (balance > 1 && key > current->left->key) { // LR
        current->left = leftRotation(current->left);
        return rightRotation(current);
    }
    if (balance < -1 && key < current->right->key) { // RL
        current->right = rightRotation(current->right);
        return leftRotation(current);
    }
    return current;
}

// --- Remocao Auxiliares ---

template <typename TKey, typename TValue>
typename AVL<TKey, TValue>::node* AVL<TKey, TValue>::findMinNode(node* current) {
    if (current == nullptr || current->left == nullptr) return current;
    return findMinNode(current->left);
}

template <typename TKey, typename TValue>
typename AVL<TKey, TValue>::node* AVL<TKey, TValue>::findMaxNode(node* current) {
    if (current == nullptr || current->right == nullptr) return current;
    return findMaxNode(current->right);
}

template <typename TKey, typename TValue>
typename AVL<TKey, TValue>::node* AVL<TKey, TValue>::removeMinNode(node* current) {
    if (current->left == nullptr) {
        node* rightChild = current->right;
        delete current;
        return rightChild;
    }
    current->left = removeMinNode(current->left);
    updateHeight(current);
    int balance = getBalance(current);
    if (balance > 1) { // Left heavy
        if (getBalance(current->left) >= 0) return rightRotation(current); // LL
        else { // LR
            current->left = leftRotation(current->left);
            return rightRotation(current);
        }
    }
    if (balance < -1) { // Right heavy
        if (getBalance(current->right) <= 0) return leftRotation(current); // RR
        else { // RL
            current->right = rightRotation(current->right);
            return leftRotation(current);
        }
    }
    return current;
}

// --- Remocao Principal ---

template <typename TKey, typename TValue>
typename AVL<TKey, TValue>::node* AVL<TKey, TValue>::removeUtility(node* current, const TKey& key) {
    if (current == nullptr) return nullptr;

    if (key < current->key) {
        current->left = removeUtility(current->left, key);
    } else if (key > current->key) {
        current->right = removeUtility(current->right, key);
    } else { // Chave encontrada
        if (current->left == nullptr || current->right == nullptr) { // 0 ou 1 filho
            node* temp = current->left ? current->left : current->right;
            delete current;
            return temp;
        } else { // 2 filhos
            node* minNodeInRightSubtree = findMinNode(current->right); // Sucessor in-order
            current->key = minNodeInRightSubtree->key;
            current->value = minNodeInRightSubtree->value;
            current->right = removeUtility(current->right, minNodeInRightSubtree->key);
        }
    }

    if (current == nullptr) return nullptr;

    updateHeight(current);
    int balance = getBalance(current);

    if (balance > 1) { // Left heavy
        if (getBalance(current->left) >= 0) return rightRotation(current);
        else {
            current->left = leftRotation(current->left);
            return rightRotation(current);
        }
    } else if (balance < -1) { // Right heavy
        if (getBalance(current->right) <= 0) return leftRotation(current);
        else {
            current->right = rightRotation(current->right);
            return leftRotation(current);
        }
    }
    return current;
}


// --- Busca ---
template <typename TKey, typename TValue>
typename AVL<TKey, TValue>::node* AVL<TKey, TValue>::searchNode(node* current, const TKey& key) {
    if (current == nullptr || current->key == key) return current;
    if (key < current->key) return searchNode(current->left, key);
    return searchNode(current->right, key);
}

template <typename TKey, typename TValue>
TValue* AVL<TKey, TValue>::search(const TKey& key) {
    node* resultNode = searchNode(root, key);
    if (resultNode != nullptr) return &(resultNode->value);
    return nullptr;
}


// --- Funcoes Publicas ---

template <typename TKey, typename TValue>
void AVL<TKey, TValue>::insert(const TKey& key, const TValue& value) {
    root = insertUtility(root, key, value);
}

template <typename TKey, typename TValue>
void AVL<TKey, TValue>::remove(const TKey& key) {
    root = removeUtility(root, key);
}

template <typename TKey, typename TValue>
void AVL<TKey, TValue>::inorder() const {
    inOrderUtility(root);
    std::cout << std::endl;
}

template <typename TKey, typename TValue>
void AVL<TKey, TValue>::preOrder() const {
    preOrderUtility(root);
    std::cout << std::endl;
}

template <typename TKey, typename TValue>
void AVL<TKey, TValue>::postOrder() const {
    postOrderUtility(root);
    std::cout << std::endl;
}

// --- Travessias ---
template <typename TKey, typename TValue>
void AVL<TKey, TValue>::inOrderUtility(node* current) const {
    if (current == nullptr) return;
    inOrderUtility(current->left);
    std::cout << current->key << " ";
    inOrderUtility(current->right);
}

template <typename TKey, typename TValue>
void AVL<TKey, TValue>::preOrderUtility(node* current) const {
    if (current == nullptr) return;
    std::cout << current->key << " ";
    preOrderUtility(current->left);
    preOrderUtility(current->right);
}

template <typename TKey, typename TValue>
void AVL<TKey, TValue>::postOrderUtility(node* current) const {
    if (current == nullptr) return;
    postOrderUtility(current->left);
    postOrderUtility(current->right);
    std::cout << current->key << " ";
}
