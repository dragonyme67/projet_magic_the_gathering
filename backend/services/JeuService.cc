#include "JeuService.h"
#include <iostream>

#include <cstdlib>
#include <vector>

struct Participant {
    const char* nom;
    const char* deck;
};

static const Participant kParticipants[] = {{"Alice", "deck_1"}, {"Bob", "deck_2"}};

struct ZoneNommee {
    magic::Zone zone;
    const char* cle;
};

static const ZoneNommee kZones[] = {
    {magic::Zone::Bibliotheque, "bibliotheque"},
    {magic::Zone::Main, "main"},
    {magic::Zone::ChampDeBataille, "champ_de_bataille"},
    {magic::Zone::Cimetiere, "cimetiere"},
    {magic::Zone::Exil, "exil"},
};

static std::string env(const char* nom, const char* defaut) {
    const char* valeur = std::getenv(nom);
    return valeur ? valeur : defaut;
}

static Json::Value carteJson(const magic::Etat& etat, magic::ObjetId id) {
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

static Json::Value evenementJson(const magic::Evenement& evenement) {
    Json::Value json;
    json["id"] = evenement.id;
    json["cause"] = evenement.cause;
    json["regle"] = evenement.regle;
    json["texte"] = evenement.texte;
    return json;
}

static Json::Value partieJson(const magic::Game& jeu) {
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
        for (const ZoneNommee& nommee : kZones) {
            if (nommee.zone == magic::Zone::Bibliotheque) continue;
            Json::Value cartes(Json::arrayValue);
            for (magic::ObjetId id : joueur.zone(nommee.zone)) cartes.append(carteJson(etat, id));
            j[nommee.cle] = cartes;
        }
        json["joueurs"].append(j);
    }
    return json;
}

bool zoneDepuisCle(const std::string& cle, magic::Zone& zone) {
    for (const ZoneNommee& nommee : kZones) {
        if (cle == nommee.cle) {
            zone = nommee.zone;
            return true;
        }
    }
    return false;
}

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

bool JeuService::etat(Json::Value& resultat) {
    std::lock_guard<std::mutex> verrou(mutex_);
    if (!jeu_) return false;
    resultat = partieJson(*jeu_);
    return true;
}

bool JeuService::appliquer(magic::JoueurId joueur, const magic::Action& action, ReponseAction& reponse) {
    std::lock_guard<std::mutex> verrou(mutex_);
    if (!jeu_) return false;

    const magic::Resultat resultat = jeu_->appliquer(joueur, action);
    reponse.accepte = resultat.accepte;
    reponse.json = Json::Value();
    reponse.json["accepte"] = resultat.accepte;
    if (!resultat.accepte) {
        // TO DO : Gérer le JSON des evenements  
        reponse.json["refus"] = resultat.refus;
        std::cout << resultat.refus << std::endl;
        return true;
    }

    Json::Value evenements(Json::arrayValue);
    for (const magic::Evenement& evenement : resultat.evenements) evenements.append(evenementJson(evenement));
    reponse.json["evenements"] = evenements;
    reponse.json["partie"] = partieJson(*jeu_);
    return true;
}
