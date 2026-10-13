#include <exception>
#include <iostream>
#include <map>
#include <string>

#include "parser/ParserCartes.hpp"

int main(int argc, char* argv[]) {
    const std::string chemin = argc > 1 ? argv[1] : "resources/kld.json";

    try {
        const auto cartes = parser::chargerCartes(chemin);
        std::cout << cartes.size() << " cartes chargées depuis " << chemin << "\n\n";

        // nomType() et l'affichage dépendent de la classe réelle de chaque carte.
        std::map<std::string, int> nbParType;
        std::map<std::string, const Carte*> exemples;
        for (const auto& carte : cartes) {
            ++nbParType[carte->nomType()];
            exemples.emplace(carte->nomType(), carte.get());
        }

        for (const auto& [type, carte] : exemples) {
            std::cout << "--- " << type << " (" << nbParType[type] << " cartes) ---\n" << *carte << '\n';
        }
    } catch (const std::exception& erreur) {
        std::cerr << "Erreur : " << erreur.what() << '\n';
        return 1;
    }
    return 0;
}
