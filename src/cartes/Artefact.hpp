#pragma once

#include <optional>

#include "cartes/Permanent.hpp"

// Artefact non-créature. Les véhicules ont une force/endurance sans être des créatures.
class Artefact : public Permanent {
public:
    explicit Artefact(DonneesCarte donnees, std::optional<ForceEndurance> forceEndurance = std::nullopt);

    std::string nomType() const override;

    bool estVehicule() const;
    const std::optional<ForceEndurance>& forceEndurance() const { return forceEndurance_; }

protected:
    void afficherDetails(std::ostream& os) const override;

private:
    std::optional<ForceEndurance> forceEndurance_;
};
