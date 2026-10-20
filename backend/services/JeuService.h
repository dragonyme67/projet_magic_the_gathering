#pragma once

#include <json/json.h>

#include <memory>
#include <mutex>
#include <string>

#include "magic/Game.hpp"
#include "magic/io/DeckRepository.hpp"

struct ReponseAction {
    bool accepte = false;
    Json::Value json;
};

// "main", "champ_de_bataille"... ; false si la clé est inconnue
bool zoneDepuisCle(const std::string& cle, magic::Zone& zone);

class JeuService {
public:
    JeuService();

    Json::Value creer();

    // false si aucune partie
    bool etat(Json::Value& resultat);

    // false si aucune partie ; refus du moteur : reponse.accepte == false
    bool appliquer(magic::JoueurId joueur, const magic::Action& action, ReponseAction& reponse);

private:
    std::mutex mutex_;
    magic::DeckRepository decks_;
    std::unique_ptr<magic::Game> jeu_;
};
