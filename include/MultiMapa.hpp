// ===== MultiMapa.hpp =====
#ifndef MULTIMAPA_HPP
#define MULTIMAPA_HPP

#include "Vector.hpp"

template <typename Chave, typename Valor>
struct GrupoChave {
    Chave chave;
    Vector<Valor> valores;


    GrupoChave() : chave(), valores()           {}
    GrupoChave(const Chave& c) : chave(c), valores() {}
};

template <typename Chave, typename Valor>
class MultiMapa {
public:
    MultiMapa(size_t numBaldes, size_t (*hashFn)(const Chave&));
    ~MultiMapa() = default;

    // Insere valor mantendo ordem no grupo da chave
    void inserir(const Chave& chave, const Valor& valor);

    // Retorna valores para a chave, na ordem correta
    Vector<Valor> obter(const Chave& chave) const;

    // Remove todos valores da chave; retorna quantos foram removidos
    size_t remover(const Chave& chave);

    unsigned int tamanho() const { return numPares; }
    bool vazio() const { return numPares == 0; }

private:
    // Cada balde: Vector de grupos por chave
    Vector< Vector< GrupoChave<Chave,Valor> > > tabela;
    size_t numBaldes;
    size_t numPares;
    float  limiteCarga;
    size_t (*hashFn)(const Chave&);

    size_t indicePara(const Chave& chave) const;
    void rehash();
};

#include "../src/MultiMapa.tpp"
#endif // MULTIMAPA_HPP