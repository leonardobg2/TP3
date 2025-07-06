#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include "../include/Lista.hpp"
#include "../include/MultiMapa.hpp"
#include "../include/HashFunctions.hpp"
#include "../include/Evento.hpp"

// Separa linha em palavras (trata \r também)
static Vector<std::string> split(const std::string& s) {
    Vector<std::string> tokens;
    int i = 0, n = s.size();
    while (i < n) {
        while (i < n && (s[i] == ' ' || s[i] == '\r')) i++;
        int start = i;
        while (i < n && (s[i] != ' ' && s[i] != '\r')) i++;
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

    MultiMapa<int, Evento*> mapa_pacote(128, hash_int);
    MultiMapa<std::string, Evento*> mapa_cliente(128, hash_str);
    Lista<Evento> lista_eventos;

    std::string linha;
    while (std::getline(input, linha)) {
        auto tok = split(linha);
        if (tok.is_empty()) break;

        int tempo = std::stoi(tok[0]);
        const std::string& op = tok[1];

        if (op == "EV") {
            Evento ev;
            ev.tempo     = tempo;
            ev.tipo      = Evento::to_str(tok[2]);
            ev.pacote_id = std::stoi(tok[3]);

            switch (ev.tipo) {
                case TipoEvento::RG:
                    ev.remetente    = tok[4];
                    ev.destinatario = tok[5];
                    ev.origem       = std::stoi(tok[6]);
                    ev.destino      = std::stoi(tok[7]);
                    break;
                case TipoEvento::TR:
                    ev.origem  = std::stoi(tok[4]);
                    ev.destino = std::stoi(tok[5]);
                    break;
                case TipoEvento::EN:
                    ev.destino = std::stoi(tok[4]);
                    break;
                default:
                    ev.origem = std::stoi(tok[4]);
                    ev.secao  = std::stoi(tok[5]);
                    break;
            }

            lista_eventos.push_back(ev);
            Evento* pev = &lista_eventos[ lista_eventos.size() - 1 ];

            mapa_pacote.inserir(pev->pacote_id, pev);
            if (!pev->remetente.empty())
                mapa_cliente.inserir(pev->remetente, pev);
            if (!pev->destinatario.empty())
                mapa_cliente.inserir(pev->destinatario, pev);
        }

        else if (op == "PC") {
            std::cout << linha.substr(1) << "\n";
            int pid = std::stoi(tok[2]);
            auto hist = mapa_pacote.obter(pid);
            std::cout << hist.get_size() << "\n";
            for (unsigned i = 0; i < hist.get_size(); ++i)
                Evento::printEvento(hist[i]);
        }

        else if (op == "CL") {
            std::cout << linha.substr(1) << "\n";
            const std::string& cli = tok[2];
            auto evs = mapa_cliente.obter(cli);

            struct PacInfo { int id; Evento* first; };
            Vector<PacInfo> pacs;

            for (unsigned i = 0; i < evs.get_size(); ++i) {
                Evento* e = evs[i];
                int pid = e->pacote_id;
                bool seen = false;
                for (unsigned j = 0; j < pacs.get_size(); ++j) {
                    if (pacs[j].id == pid) { seen = true; break; }
                }
                if (!seen && e->tipo == TipoEvento::RG) {
                    pacs.push_back({ pid, e });
                }
            }

            std::cout << 2 * pacs.get_size() << "\n";

            for (unsigned i = 0; i < pacs.get_size(); ++i)
                Evento::printEvento(pacs[i].first);

            Vector<Evento*> lasts;
            for (unsigned i = 0; i < pacs.get_size(); ++i) {
                int pid = pacs[i].id;
                auto hist = mapa_pacote.obter(pid);
                lasts.push_back( hist[ hist.get_size() - 1 ] );
            }

            // Ordenação usando operator<
            for (unsigned a = 0; a + 1 < lasts.get_size(); ++a) {
                for (unsigned b = 0; b + 1 < lasts.get_size() - a; ++b) {
                    if (*lasts[b+1] < *lasts[b]) {
                        Evento* tmp = lasts[b];
                        lasts[b]    = lasts[b+1];
                        lasts[b+1]  = tmp;
                    }
                }
            }

            for (unsigned i = 0; i < lasts.get_size(); ++i)
                Evento::printEvento(lasts[i]);
        }
    }

    return 0;
}
