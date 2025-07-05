#include "../include/MultiMapa.hpp"

// Busca binária genérica para inserção ordenada (para valores que têm operator< definido)
template <typename Valor>
static unsigned int lowerBoundValor(const Vector<Valor>& vec, const Valor& v) {
    unsigned int l = 0, r = vec.get_size();
    while (l < r) {
        unsigned int m = (l + r) / 2;
        if (vec[m] < v) l = m + 1;
        else            r = m;
    }
    return l;
}

// Especialização para ponteiros: compara conteúdo (*ptr)
template <typename T>
static unsigned int lowerBoundValor(const Vector<T*>& vec, T* const& v) {
    unsigned int l = 0, r = vec.get_size();
    while (l < r) {
        unsigned int m = (l + r) / 2;
        if (*vec[m] < *v) l = m + 1;
        else              r = m;
    }
    return l;
}

// Calcula índice de balde
template <typename Chave, typename Valor>
unsigned int MultiMapa<Chave,Valor>::indicePara(const Chave& chave) const {
    return hashFn(chave) % numBaldes;
}

// Construtor: inicializa 'numBaldes' baldes vazios
template <typename Chave, typename Valor>
MultiMapa<Chave,Valor>::MultiMapa(unsigned int numBaldes,
                                   unsigned int (*hashFn)(const Chave&))
    : tabela(numBaldes), numBaldes(numBaldes), numPares(0), limiteCarga(0.75f), hashFn(hashFn) {
    // tabela já vem com "numBaldes" vetores vazios graças a Vector(size_t)
}

// Insere valor ordenado dentro do grupo da chave
template <typename Chave, typename Valor>
void MultiMapa<Chave,Valor>::inserir(const Chave& chave, const Valor& valor) {
    if ((numPares + 1.0f) / numBaldes > limiteCarga)
        rehash();

    unsigned int idx = indicePara(chave);
    auto& balde = tabela[idx];

    // procura grupo existente
    for (unsigned int g = 0; g < balde.get_size(); ++g) {
        if (balde[g].chave == chave) {
            unsigned int pos = lowerBoundValor(balde[g].valores, valor);
            balde[g].valores.insert(pos, valor);
            ++numPares;
            return;
        }
    }

    // não encontrou grupo: cria novo
    balde.push_back(GrupoChave<Chave,Valor>(chave));
    auto& novoGrupo = balde[balde.get_size() - 1];
    size_t pos = lowerBoundValor(novoGrupo.valores, valor);
    novoGrupo.valores.insert(pos, valor);
    ++numPares;
}

// Retorna vetor de valores para a chave (pode estar vazio)
template <typename Chave, typename Valor>
Vector<Valor> MultiMapa<Chave,Valor>::obter(const Chave& chave) const {
    const auto& balde = tabela[indicePara(chave)];
    for (size_t g = 0; g < balde.get_size(); ++g) {
        if (balde[g].chave == chave)
            return balde[g].valores;
    }
    return Vector<Valor>(0);
}

// Remove todo o grupo associado à chave e retorna quantos valores foram removidos
template <typename Chave, typename Valor>
unsigned int MultiMapa<Chave,Valor>::remover(const Chave& chave) {
    unsigned int idx = indicePara(chave);
    auto& balde = tabela[idx];
    for (unsigned int g = 0; g < balde.get_size(); ++g) {
        if (balde[g].chave == chave) {
            unsigned int cnt = balde[g].valores.get_size();
            numPares -= cnt;
            balde.erase(g);
            return cnt;
        }
    }
    return 0;
}

// Rehash: dobra número de baldes e reinsere grupos/valores
template <typename Chave, typename Valor>
void MultiMapa<Chave,Valor>::rehash() {
    unsigned int novaCap = numBaldes * 2;
    Vector< Vector< GrupoChave<Chave,Valor> > > novaTabela(novaCap);

    for (unsigned int b = 0; b < numBaldes; ++b) {
        auto& balde = tabela[b];
        for (unsigned int g = 0; g < balde.get_size(); ++g) {
            const auto& grupo = balde[g];
            unsigned int novoIdx = hashFn(grupo.chave) % novaCap;
            auto& destBalde = novaTabela[novoIdx];
            destBalde.push_back(GrupoChave<Chave,Valor>(grupo.chave));
            auto& destino = destBalde[destBalde.get_size() - 1];
            for (unsigned int i = 0; i < grupo.valores.get_size(); ++i)
                destino.valores.push_back(grupo.valores[i]);
        }
    }

    tabela = std::move(novaTabela);
    numBaldes = novaCap;
}
