#pragma once

#include <json/json.h>

#include <memory>
#include <mutex>
#include <optional>

#include "magic/Game.hpp"
#include "magic/io/DeckRepository.hpp"

class JeuService {
public:
    JeuService();
    
    Json::Value creer();

    // nullopt si aucune partie
    std::optional<Json::Value> etat();

private:
    std::mutex mutex_;
    magic::DeckRepository decks_;
    std::unique_ptr<magic::Game> jeu_;
};
