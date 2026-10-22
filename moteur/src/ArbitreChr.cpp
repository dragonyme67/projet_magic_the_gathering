#include "magic/ArbitreChr.hpp"

#include "Tools.hpp"
#include "ReglesARBITRE.hh"

namespace magic
{

    namespace
    {

        std::string translate(const std::string &regle)
        {
            if (regle == "pas_son_tour")
                return "Ce n'est pas votre tour.";
            if (regle == "piocher_vide")
                return "La bibliothèque est vide.";
            if (regle == "deja_piocher")
                return "Vous avez déjà pioché ce tour-ci.";
            return "Action refusée (règle " + regle + ").";
        }

    }

    struct ArbitreChr::Impl
    {
        Verdict verdict;
        chr::Shared_obj<ARBITRE> espace;
    };

    ArbitreChr::ArbitreChr() : impl_(std::make_unique<Impl>()) {}
    ArbitreChr::~ArbitreChr() = default;

    std::string ArbitreChr::valider(const Etat &etat, JoueurId joueur, const Action &action) const
    {
        // 1. L'espace n'est créé QU'UNE SEULE FOIS (au premier coup de la partie) :
        if (!impl_->espace)
        {
            impl_->espace = ARBITRE::create(etat, impl_->verdict);
        }
        // 2. On réinitialise le verdict du coup précédent :
        impl_->verdict.regle.clear();
        // 3. On pose la contrainte dans le store EXISTANT :
        switch (action.type)
        {
            case TypeAction::Piocher:
                impl_->espace->demande_piocher(static_cast<int>(joueur));
                break;
            case TypeAction::Deplacer:
                impl_->espace->demande_deplacer(static_cast<int>(joueur), static_cast<int> (action.zone));
                break;
            case TypeAction::Engager:
                impl_->espace->demande_engager(static_cast<int>(joueur), action.objet);
                break;
            case TypeAction::Degager:
                impl_->espace->demande_degager(static_cast<int>(joueur), action.objet);
                break;
            case TypeAction::ChangerVie:
                impl_->espace->demande_changer_vie(static_cast<int>(joueur), action.delta);
                break;
            case TypeAction::FinDuTour:
                impl_->espace->demande_fin_tour(static_cast<int>(joueur));
                break;
        }
        if (impl_->verdict.regle.empty())
            return "";
        return translate(impl_->verdict.regle);
    }

}
