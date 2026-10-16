#pragma once

#include <string>
#include <vector>

namespace magic {

// Données imprimées d'une carte : immuables, partagées par tous ses exemplaires.
// Reprend les colonnes utiles de la table Cartes. Les champs *Imprime(e) sont la version
// française (affichage) ; les autres sont la version anglaise (référence pour les règles).
struct Carte {
    std::string id;         // id de la table Cartes (id Scryfall de l'impression)
    std::string nom;
    std::string ligneType;  // ex. "Basic Land — Plains"

    std::string oracleId;
    std::string nomImprime;
    std::string ligneTypeImprimee;
    std::string coutMana;   // ex. "{2}{W}"
    std::string texteOracle;
    std::string texteImprime;
    std::string force;      // texte : "*" ou "1+*" sont possibles
    std::string endurance;

    // Version à afficher : le français s'il existe, sinon l'anglais.
    const std::string& nomAffiche() const { return nomImprime.empty() ? nom : nomImprime; }
    const std::string& typeAffiche() const { return ligneTypeImprimee.empty() ? ligneType : ligneTypeImprimee; }
    const std::string& texteAffiche() const { return texteImprime.empty() ? texteOracle : texteImprime; }
};

struct Deck {
    std::string joueur;         // nom du joueur, affiché dans le journal
    std::vector<Carte> cartes;  // une entrée par exemplaire
};

}  // namespace magic
