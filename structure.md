# Structure du projet

Plateforme web pour jouer à Magic, assistée par un moteur de règles CHR++ qui applique les effets des cartes et explique ses raisonnements.

## Flux

```
Scryfall (JSON) ─► parser ─► Carte (données imprimées)
                                 │
   action joueur ─► moteur (état de partie) ◄─► règles CHR++ ─► trace ─► explication
                                 │
                   API Drogon (REST / WebSocket) ◄─► front web
```

## Arborescence

```
Projet_Magic/
├── CMakeLists.txt
├── docs/              sujet, supports CHR
├── resources/         sets Scryfall (kld.json)
├── utils/             scripts annexes (card_import.py)
├── src/
│   ├── main.cpp       exécutable console (charge et affiche les cartes)
│   ├── cartes/        modèle : Carte et ses classes filles
│   ├── parser/        JSON Scryfall → Carte
│   ├── moteur/        joueurs, zones, pile, tours, phases            (à venir)
│   ├── regles/        fichiers .chrpp : règles du jeu + effets       (à venir)
│   ├── explication/   trace CHR → justification lisible              (à venir)
│   └── web/           serveur Drogon                                 (à venir)
├── front/             interface web                                  (à venir)
├── tests/                                                            (à venir)
└── docker/                                                           (à venir)
```

Dépendances : `cartes ← parser`, `cartes ← moteur ⇄ regles → explication`, `web → moteur, explication`.
Tout sauf `main.cpp` et `web/` va dans la bibliothèque `magic_core`, que partagent la console, les tests et le serveur.

## Hiérarchie des cartes

```
Carte (abstraite)
├── Permanent          reste sur le champ de bataille
│   ├── Creature       force / endurance
│   ├── Artefact       force / endurance optionnelles (véhicules)
│   ├── Enchantement   estAura()
│   ├── Terrain        mana produit
│   └── Planeswalker   loyauté
└── Sort               va au cimetière après résolution
    ├── Ephemere
    └── Rituel
```

- Le parser choisit la classe d'après le type principal, dans cet ordre de priorité : Créature > Planeswalker > Terrain > Artefact > Enchantement > Éphémère > Rituel.
- `Carte` garde tous les supertypes, types et sous-types : une « Artifact Creature » est une `Creature` avec `aType("Artifact") == true`.

## Principes

- **Carte = données imprimées, immuables.** L'état en jeu (zone, engagée, marqueurs, blessures, contrôleur) appartient au moteur, dans un objet qui *référence* une Carte. On peut ainsi avoir plusieurs exemplaires d'une même carte.
- **Événements → contraintes CHR.** Le moteur poste des faits (`entre_en_jeu(Id)`, `lance_sort(J, Id)`, `blessures(Src, Cible, N)`…). Les règles officielles et les effets des cartes sont des règles CHR qui produisent de nouveaux faits.
  Idée : `entre_en_jeu(C), nom(C, "Cogworker's Puzzleknot") ==> creer_jeton(C, servo)`
- **Explication** : s'appuie sur la trace CHR++ (`chrppc --trace`) ou sur une contrainte `cause(...)` posée par chaque règle.
- **Texte oracle (EN)** : référence pour écrire les règles. **Texte FR** : affichage.

## Choix techniques

C++17 · CMake · jsoncpp (déjà requis par Drogon) · CHR++ (`chrppc` lancé par CMake via `add_custom_command`) · Drogon · Docker

## Compiler

```
sudo apt install libjsoncpp-dev
cmake -S . -B build && cmake --build build
./build/magic [resources/kld.json]
```

## Étapes

1. Modèle de cartes + parser ← *en cours*
2. Moteur minimal : zones, pioche, jouer un terrain, lancer un sort
3. Premières règles CHR++ : règles de base + quelques cartes KLD
4. Explication des effets
5. API Drogon + front
6. Docker
