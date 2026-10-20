#include "JeuService.h"

#include <cstdlib>
#include <vector>

namespace {

struct Participant {
    const char* nom;
    const char* deck;
};

const Participant kParticipants[] = {{"Alice", "deck_1"}, {"Bob", "deck_2"}};

struct ZoneVisible {
    magic::Zone zone;
    const char* cle;
};

const ZoneVisible kZonesVisibles[] = {
    {magic::Zone::Main, "main"},
    {magic::Zone::ChampDeBataille, "champ_de_bataille"},
    {magic::Zone::Cimetiere, "cimetiere"},
    {magic::Zone::Exil, "exil"},
};

std::string env(const char* nom, const char* defaut) {
    const char* valeur = std::getenv(nom);
    return valeur ? valeur : defaut;
}

Json::Value carteJson(const magic::Etat& etat, magic::ObjetId id) {
    const magic::Objet& objet = etat.objets[id];
    Json::Value json;
    json["id"] = id;
    json["nom"] = objet.carte->nomAffiche();
    json["image"] = objet.carte->image;
    json["mana"] = objet.carte->coutMana;
    json["description"] = objet.carte->texteAffiche();
    json["engage"] = objet.engage;
    return json;
}

Json::Value partieJson(const magic::Game& jeu) {
    const magic::Etat& etat = jeu.etat();
    Json::Value json;
    json["graine"] = jeu.graine();
    json["tour"] = etat.tour;
    json["joueurActif"] = etat.joueurActif;
    json["joueurs"] = Json::arrayValue;
    for (const magic::Joueur& joueur : etat.joueurs) {
        Json::Value j;
        j["nom"] = joueur.nom;
        j["vie"] = joueur.vie;
        j["bibliotheque"] = static_cast<int>(joueur.zone(magic::Zone::Bibliotheque).size());
        for (const ZoneVisible& visible : kZonesVisibles) {
            Json::Value cartes(Json::arrayValue);
            for (magic::ObjetId id : joueur.zone(visible.zone)) cartes.append(carteJson(etat, id));
            j[visible.cle] = cartes;
        }
        json["joueurs"].append(j);
    }
    return json;
}

}

// Aucune lecture ici : les fichiers ne sont ouverts qu'à creer()
JeuService::JeuService()
    : decks_(env("DECKS_DIR", "../resources"), env("CARTES_DB", "../data/database.sqlite")) {}

Json::Value JeuService::creer() {
    std::lock_guard<std::mutex> verrou(mutex_);

    std::vector<magic::Deck> decks;
    for (const Participant& participant : kParticipants) {
        decks.push_back(decks_.charger(participant.deck, participant.nom));
    }

    auto jeu = std::make_unique<magic::Game>(decks);
    jeu->lancer();
    jeu_ = std::move(jeu);
    return partieJson(*jeu_);
}

std::optional<Json::Value> JeuService::etat() {
    std::lock_guard<std::mutex> verrou(mutex_);
    if (!jeu_) return std::nullopt;
    return partieJson(*jeu_);
}
