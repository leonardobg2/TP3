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

};

 

#endif