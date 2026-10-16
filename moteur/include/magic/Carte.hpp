#pragma once

#include <string>
#include <vector>

namespace magic {

// Données imprimées, immuables. *Imprime(e) = français, le reste = anglais (oracle)
struct Carte {
    std::string id;  // colonne id de la table Cartes
    std::string nom;
    std::string ligneType;

    std::string oracleId;
    std::string nomImprime;
    std::string ligneTypeImprimee;
    std::string coutMana;
    std::string texteOracle;
    std::string texteImprime;
    std::string force;  // texte ("*" possible)
    std::string endurance;

    // français s'il existe, sinon anglais
    const std::string& nomAffiche() const { return nomImprime.empty() ? nom : nomImprime; }
    const std::string& typeAffiche() const { return ligneTypeImprimee.empty() ? ligneType : ligneTypeImprimee; }
    const std::string& texteAffiche() const { return texteImprime.empty() ? texteOracle : texteImprime; }
};

struct Deck {
    std::string joueur;
    std::vector<Carte> cartes;  // une entrée par exemplaire
};

}
