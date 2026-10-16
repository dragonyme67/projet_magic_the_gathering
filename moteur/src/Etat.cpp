#include "magic/Etat.hpp"

#include <algorithm>
#include <utility>

namespace magic {

const char* nomZone(Zone zone) {
    switch (zone) {
        case Zone::Bibliotheque:    return "bibliothèque";
        case Zone::Main:            return "main";
        case Zone::ChampDeBataille: return "champ de bataille";
        case Zone::Cimetiere:       return "cimetière";
        case Zone::Exil:            return "exil";
        case Zone::Nombre:          break;
    }
    return "?";
}

bool Etat::joueurValide(JoueurId id) const {
    return id >= 0 && static_cast<std::size_t>(id) < joueurs.size();
}

bool Etat::objetValide(ObjetId id) const {
    return id >= 0 && static_cast<std::size_t>(id) < objets.size();
}

ObjetId Etat::creerObjet(std::shared_ptr<const Carte> carte, JoueurId proprietaire, Zone zone) {
    Joueur& j = joueurs.at(proprietaire);
    const ObjetId id = static_cast<ObjetId>(objets.size());
    objets.push_back(Objet{id, std::move(carte), proprietaire, zone, false});
    j.zone(zone).push_back(id);
    return id;
}

void Etat::deplacer(ObjetId id, Zone destination) {
    Objet& objet = objets.at(id);
    Joueur& proprietaire = joueurs.at(objet.proprietaire);

    auto& origine = proprietaire.zone(objet.zone);
    origine.erase(std::remove(origine.begin(), origine.end(), id), origine.end());

    proprietaire.zone(destination).push_back(id);
    objet.zone = destination;
    objet.engage = false;  // un objet qui change de zone est un nouvel objet
}

std::optional<ObjetId> Etat::piocher(JoueurId joueur) {
    auto& bibliotheque = joueurs.at(joueur).zone(Zone::Bibliotheque);
    if (bibliotheque.empty()) return std::nullopt;
    const ObjetId id = bibliotheque.back();
    deplacer(id, Zone::Main);
    return id;
}

void Etat::engager(ObjetId id, bool engage) {
    objets.at(id).engage = engage;
}

void Etat::changerVie(JoueurId joueur, int delta) {
    joueurs.at(joueur).vie += delta;
}

void Etat::finDuTour() {
    joueurActif = static_cast<JoueurId>((joueurActif + 1) % joueurs.size());
    ++tour;
}

}  // namespace magic
