#ifndef EVENTO_HPP
#define EVENTO_HPP

#include <string>

enum class TipoEvento {
    RG = 1,
    AR = 2,
    RM = 3,
    UR = 4,
    TR = 5,
    EN = 6
}; // enum é usado porque obriga o valor a pertencer a um conjunto válido e por ser mais legivel 

struct Evento {
    int tempo;
    int pacote_id;
    std::string remetente;
    std::string destinatario;
    int origem;
    int destino;
    TipoEvento tipo;
    int secao;

     Evento() : tempo(-1), pacote_id(-1), remetente(""), destinatario(""), origem(-1), destino(-1), tipo(TipoEvento::RG), secao(-1) {}

   
    bool operator < (const Evento& other) const{
    if(tempo != other.tempo)    return tempo < other.tempo;
    if(pacote_id != other.pacote_id) return pacote_id < other.pacote_id;
    return tipo < other.tipo;
    } // compara os eventos por tempo, pacote_id e tipo, nessa ordem

    static TipoEvento to_str(const std::string& tipo){
        if      (tipo == "RG") return TipoEvento::RG;
        else if (tipo == "AR") return TipoEvento::AR;
        else if (tipo == "RM") return TipoEvento::RM;
        else if (tipo == "UR") return TipoEvento::UR;
        else if (tipo == "TR") return TipoEvento::TR;
        else if (tipo == "EN") return TipoEvento::EN;
        throw std::invalid_argument("Tipo inválido: " + tipo);

    }

   // Converte enum → string
    static std::string tipoParaString(TipoEvento t) {
    switch (t) {
        case TipoEvento::RG: return "RG";
        case TipoEvento::AR: return "AR";
        case TipoEvento::RM: return "RM";
        case TipoEvento::UR: return "UR";
        case TipoEvento::TR: return "TR";
        case TipoEvento::EN: return "EN";
    }
    return "";
    } 

    // imprime um Evento no formato pedido
static void printEvento(Evento* e) {
    std::cout
        << std::setw(7) << std::setfill('0') << e->tempo
        << " EV " << Evento::tipoParaString(e->tipo)
        << " " << std::setw(3) << std::setfill('0') << e->pacote_id;
    if (e->tipo == TipoEvento::RG) {
        std::cout << " " << e->remetente
                  << " " << e->destinatario
                  << " " << std::setw(3) << e->origem
                  << " " << std::setw(3) << e->destino;
    }
    else if (e->tipo == TipoEvento::TR) {
        std::cout << " " << std::setw(3) << e->origem
                  << " " << std::setw(3) << e->destino;
    }
    else if (e->tipo == TipoEvento::EN) {
        std::cout << " " << std::setw(3) << e->destino;
    }
    else {
        std::cout << " " << std::setw(3) << e->origem
                  << " " << std::setw(3) << e->secao;
    }
    std::cout << "\n";
}

};

 

#endif