#pragma once

#include <ostream>
#include <string>
#include <vector>

// Données imprimées communes à toutes les cartes, remplies par le parser.
struct DonneesCarte {
    std::string id;                       // identifiant Scryfall
    std::string nom;                      // nom anglais (oracle)
    std::string nomFr;
    std::string coutMana;                 // ex. "{2}{W}"
    int valeurMana = 0;                   // coût converti (cmc)
    std::vector<std::string> couleurs;    // W, U, B, R, G
    std::vector<std::string> supertypes;  // Legendary, Basic...
    std::vector<std::string> types;       // Artifact, Creature...
    std::vector<std::string> sousTypes;   // Human, Aura, Vehicle...
    std::vector<std::string> motsCles;    // Flying, Fabricate...
    std::string ligneTypeFr;              // ex. "Créature : humain et artificier"
    std::string texte;                    // texte oracle (anglais), référence pour les règles
    std::string texteFr;
};

// Caractéristiques de combat (créatures, véhicules).
struct ForceEndurance {
    int force = 0;
    int endurance = 0;
};

// Classe abstraite : une carte est forcément un Permanent ou un Sort.
class Carte {
public:
    explicit Carte(DonneesCarte donnees);
    virtual ~Carte() = default;

    // Nom du type principal en français (ex. "Créature").
    virtual std::string nomType() const = 0;
    // Vrai si la carte reste sur le champ de bataille une fois résolue.
    virtual bool estPermanent() const = 0;

    // Affichage commun ; les classes filles le complètent via afficherDetails().
    void afficher(std::ostream& os) const;

    const std::string& id() const { return donnees_.id; }
    const std::string& nom() const { return donnees_.nom; }
    const std::string& nomFr() const { return donnees_.nomFr; }
    const std::string& coutMana() const { return donnees_.coutMana; }
    int valeurMana() const { return donnees_.valeurMana; }
    const std::vector<std::string>& couleurs() const { return donnees_.couleurs; }
    const std::vector<std::string>& supertypes() const { return donnees_.supertypes; }
    const std::vector<std::string>& types() const { return donnees_.types; }
    const std::vector<std::string>& sousTypes() const { return donnees_.sousTypes; }
    const std::vector<std::string>& motsCles() const { return donnees_.motsCles; }
    const std::string& ligneTypeFr() const { return donnees_.ligneTypeFr; }
    const std::string& texte() const { return donnees_.texte; }
    const std::string& texteFr() const { return donnees_.texteFr; }

    bool aSupertype(const std::string& supertype) const;
    bool aType(const std::string& type) const;
    bool aSousType(const std::string& sousType) const;
    bool estLegendaire() const;

protected:
    // Infos propres au type (force/endurance, loyauté...). Rien par défaut.
    virtual void afficherDetails(std::ostream& os) const;

private:
    DonneesCarte donnees_;
};

std::ostream& operator<<(std::ostream& os, const Carte& carte);
