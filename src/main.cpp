#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include "../include/Lista.hpp"
#include "../include/MultiMapa.hpp"
#include "../include/HashFunctions.hpp"
#include "../include/Evento.hpp"

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

// Separa linha em palavras
static Vector<std::string> split(const std::string& s) {
    Vector<std::string> tokens;
    int i = 0, n = s.size();
    while (i < n) {
        while (i < n && s[i] == ' ') i++;
        int start = i;
        while (i < n && s[i] != ' ') i++;
        if (start < i) tokens.push_back(s.substr(start, i - start));
    }
    return tokens;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Uso: " << argv[0] << " arquivo.txt\n";
        return 1;
    }
    std::ifstream input(argv[1]);
    if (!input) {
        std::cerr << "Erro ao abrir: " << argv[1] << "\n";
        return 1;
    }

    // Índices
    MultiMapa<int, Evento*>      mapa_pacote(128, hash_int);
    MultiMapa<std::string, Evento*> mapa_cliente(128, hash_str);

    // Lista mestre de ponteiros
    Lista<Evento*> lista_eventos;

    std::string linha;
    while (std::getline(input, linha)) {
        auto tok = split(linha);
        int tempo = std::stoi(tok[0]);
        const std::string& op = tok[1];

        if (op == "EV") {
            Evento* ev = new Evento();
            ev->tempo     = tempo;
            ev->tipo      = Evento::to_str(tok[2]);
            ev->pacote_id = std::stoi(tok[3]);

            switch (ev->tipo) {
                case TipoEvento::RG:
                    ev->remetente    = tok[4];
                    ev->destinatario = tok[5];
                    ev->origem       = std::stoi(tok[6]);
                    ev->destino      = std::stoi(tok[7]);
                    break;
                case TipoEvento::TR:
                    ev->origem  = std::stoi(tok[4]);
                    ev->destino = std::stoi(tok[5]);
                    break;
                case TipoEvento::EN:
                    ev->destino = std::stoi(tok[4]);
                    break;
                case TipoEvento::AR:
                case TipoEvento::RM:
                case TipoEvento::UR:
                    ev->origem = std::stoi(tok[4]);
                    ev->secao  = std::stoi(tok[5]);
                    break;
            }

            lista_eventos.push_back(ev);
            mapa_pacote.inserir(ev->pacote_id,    ev);
            if (!ev->remetente.empty()){
                std::cout<<" o nome é: " << ev->remetente <<std::endl;
                mapa_cliente.inserir(ev->remetente,    ev);}
            if (!ev->destinatario.empty()){
                std::cout<<" o nome é: " << ev->destinatario <<std::endl;
                mapa_cliente.inserir(ev->destinatario, ev);}
        }
        else if (op == "PC") {
            std::cout << linha << "\n";
            int pid = std::stoi(tok[2]);
            auto hist = mapa_pacote.obter(pid);
            std::cout << hist.get_size() << "\n";
            for (size_t i = 0; i < hist.get_size(); ++i) {
                Evento* e = hist[i];
                std::cout
                    << std::setw(7) << std::setfill('0') << e->tempo
                    << " EV " << tipoParaString(e->tipo)
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
        }
        else if (op == "CL") {
            std::cout << linha << "\n";
            const std::string& cli = tok[2];
            auto evs = mapa_cliente.obter(cli);

            Vector<int> pacs;
            for (size_t i = 0; i < evs.get_size(); ++i) {
                int id = evs[i]->pacote_id;
                bool seen = false;
                for (size_t j = 0; j < pacs.get_size(); ++j)
                    if (pacs[j] == id) { seen = true; break; }
                if (!seen) pacs.push_back(id);
            }

            std::cout << pacs.get_size() << "\n";
            for (size_t pi = 0; pi < pacs.get_size(); ++pi) {
                int pid = pacs[pi];
                auto hist = mapa_pacote.obter(pid);
                for (size_t j = 0; j < hist.get_size(); ++j) {
                    Evento* e = hist[j];
                    std::cout
                        << std::setw(7) << std::setfill('0') << e->tempo
                        << " EV " << tipoParaString(e->tipo)
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
            }
        }
    }

    // libera eventos
    while (!lista_eventos.empty()) {
        delete lista_eventos[0];
        lista_eventos.pop_front();
    }

    return 0;
}
