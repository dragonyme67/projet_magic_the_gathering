# Structure du projet

Jeu de Magic: The Gathering en ligne, limité au set Kaladesh (~264 cartes), dont l'arbitre est un moteur de règles CHR++ qui valide chaque action et explique chaque enchaînement d'effets. Sources : `docs/Cahier des charges — Magic: The Gathering CHRPP.pdf`, `docs/2026_2027___Sujet_Master_2___CHR_et_Magic_the_gathering.pdf`, `docs/Diagramme d'architecture.drawio.png`.

## Moteur et Drogon : un seul programme

Oui. Le backend est **un seul exécutable, un seul processus** (`backend/build/backend`). Le cahier des charges le dit (« Drogon appelle directement le moteur de jeu »), et le schéma d'architecture les place dans la même boîte « Backend C++ ».

- `chrppc` compile les fichiers `.chrpp` en C++ : les règles sont du code C++ lié à l'exécutable, sans interpréteur ni processus externe.
- Le moteur est une **bibliothèque statique `magic_core` qui ne dépend pas de Drogon**, dans le dossier `moteur/` (projet CMake indépendant, voir « Squelette actuel »). Drogon (contrôleurs, JSON, HTTP) vit à l'extérieur et appelle `magic_core` par de simples appels de fonction.
- Intérêt de cette séparation : on essaie le moteur sans serveur (`moteur/console`), et `backend/` n'a pas à changer tant que le moteur n'est pas branché.
- Conséquences : une partie vit en mémoire (perdue au redémarrage, acceptable pour la v1), et une exception non rattrapée dans le moteur fait tomber le serveur. Les contrôleurs rattrapent donc toutes les exceptions et répondent en 500.

## Vue d'ensemble

```
 ┌──────────────┐  HTTP + JSON, une route par action   ┌───────────────────────────────────────┐
 │  Front web   │ ───────────────────────────────────► │ PROCESSUS BACKEND (un seul exécutable)│
 │ React/TS/Vite│ ◄─────────────────────────────────── │                                       │
 │  Tailwind    │     { accepte, etat, explications }  │  ┌─────────────────────────────────┐  │
 └──────────────┘                                      │  │ Drogon : controllers/ + api/    │  │
                                                       │  │ JSON ⇄ Action, Etat ⇒ JSON      │  │
                                                       │  └───────────────┬─────────────────┘  │
                                                       │       appel de fonction (même thread) │
                                                       │  ┌───────────────▼─────────────────┐  │
                                                       │  │ magic_core (sans Drogon)        │  │
                                                       │  │  Partie ─► règles CHR++ ─► Trace│  │
                                                       │  │   │                      │      │  │
                                                       │  │  Etat   Catalogue   Explicateur │  │
                                                       │  └───────────────▲─────────────────┘  │
                                                       │   Plugin Drogon : charge au démarrage │
                                                       └───────────────────┼───────────────────┘
                                                                           │ lecture seule
                                                                    data/database.sqlite (SQLite)
```

## Données : de Scryfall au moteur

```
 hors ligne (une fois)                                         au démarrage du serveur
 ─────────────────────                                         ───────────────────────
 Scryfall API ─► utils/card_import.py ─► resources/kld.json
                                              │
                          utils/card_json_to_db (C++) ─► data/database.sqlite  MoteurPlugin::initAndStart()
                                                              │   SELECT * FROM Cartes
                                                              └──────────────► Catalogue (en mémoire,
                                                                               lecture seule ensuite)
 data/decks.json (config, à modifier) ──── relu à chaque création de partie ──► Deck par joueur
```

- `data/database.sqlite` est la base utilisée par Drogon (`backend/config.json`) et celle que remplit `card_json_to_db` (274 cartes de Kaladesh, 264 `oracle_id`). Elle est versionnée ; `data/app.db` (ignoré par git) est un ancien fichier vide, à supprimer.
- La table `Cartes` (voir `data/schema.sql`) contient une ligne par impression et par langue. Le `Catalogue` regroupe les lignes par `oracle_id` : le **texte oracle EN** sert aux règles, le **texte FR** à l'affichage.
- Après le chargement, la base n'est plus lue pendant les parties (cahier des charges : « lue au démarrage puis gardée en mémoire »).

## Squelette actuel du moteur (`moteur/`)

Étape en cours : un moteur à 2 joueurs **sans aucune règle** (tout est permis), séparé de Drogon, qu'on peut **jouer à la main dans la console**. Il ne sait que déplacer des cartes entre zones, engager, changer la vie et passer le tour.

```
moteur/
├── CMakeLists.txt             bibliothèque magic_core + console_moteur
├── include/magic/
│   ├── Carte.hpp              Carte (colonnes utiles de la table Cartes, FR + EN) et Deck
│   ├── Catalogue.hpp          Catalogue (id → Carte, rempli par l'appelant) et construireDeck(ids)
│   ├── Etat.hpp               Joueur, Objet, Zone, Etat + primitives (deplacer, piocher, engager…)
│   ├── Action.hpp             std::variant : Piocher, Deplacer, Engager, ChangerVie, FinDuTour
│   ├── Evenement.hpp          ligne du journal (id, cause, règle, texte) : base des explications
│   ├── Arbitre.hpp            interface valider(etat, joueur, action) + ArbitrePermissif
│   └── Partie.hpp             façade : Partie(decks, graine), appliquer(joueur, action) → Resultat
├── src/Catalogue.cpp, Etat.cpp, Partie.cpp
└── console/                   jeu en console (seule partie qui dépend de SQLite et jsoncpp)
    ├── main.cpp               boucle de commandes → Action → Partie::appliquer
    ├── Chargeurs.{hpp,cpp}    base SQLite → Catalogue ; fichier de decks JSON → Deck
    └── Affichage.{hpp,cpp}    rendu de l'état, d'une zone, d'une carte

data/decks.json                decks des 2 joueurs (ids de la table Cartes)
```

```
 Action ──► Partie::appliquer ──► Arbitre::valider ──refus──► Resultat{accepte=false, refus}
                                         │ ok
                                         ▼
                              Partie::executer(action)  ──► primitives de Etat (deplacer, piocher…)
                                         │
                                         └──► journal (Evenement) ──► Resultat{accepte=true, evenements}
```

Trois idées à garder quand on ajoutera les règles :

- **`Etat` est une valeur** (copiable, sans pointeur externe) : retour arrière et sérialisation JSON faciles. Ses méthodes sont les **primitives** que les règles appelleront ; elles ne vérifient que la cohérence (id valide).
- **`Arbitre` est le point d'insertion des règles** : aujourd'hui `ArbitrePermissif`, demain un arbitre CHR++. `Partie` ne change pas.
- **Une `Carte` est immuable, un `Objet` est un exemplaire en jeu** (zone, engagé…) qui la référence : plusieurs exemplaires d'une même carte sont possibles.

Ajouter une action : une struct dans `Action.hpp`, l'ajouter au `std::variant`, écrire la surcharge `Partie::executer` (sans elle, le code ne compile pas), puis la commande correspondante dans `console/main.cpp`. Ajouter une zone : une valeur dans `Zone` et son nom dans `Etat.cpp`.

### Fichier de decks (`data/decks.json`)

Exactement 2 joueurs ; chaque carte est désignée par son **`id` de la table `Cartes`** (id Scryfall de l'impression : `SELECT id, nom_imprime FROM Cartes`). `quantite` vaut 1 par défaut ; les autres clés (ici `nom`) sont ignorées et ne servent qu'à relire le fichier.

```json
{ "joueurs": [
    { "nom": "Alice", "cartes": [ { "id": "6d1a0776-859d-4dda-accf-53500fea19a8", "quantite": 9, "nom": "Plaine" } ] },
    { "nom": "Bob",   "cartes": [ { "id": "a5affb42-c7af-4216-a3c3-35d261d4b4ea", "quantite": 9, "nom": "Montagne" } ] }
] }
```

### Jouer en console

```
cmake -S moteur -B moteur/build && cmake --build moteur/build
./moteur/build/console_moteur [data/decks.json] [data/database.sqlite] [graine]     # depuis la racine
```

Les deux joueurs jouent sur le même clavier ; les commandes (`piocher`, `jouer <id>`, `deplacer <id> <zone>`, `engager`, `degager`, `vie <n> [joueur]`, `fin`, `carte <id>`, `voir <zone>`, `journal`, `aide`, `quitter`) s'appliquent au joueur dont c'est le tour. La main de l'adversaire et les bibliothèques sont masquées.

Volontairement absents pour l'instant : phases du tour, pile, contrôleur, mana, combat, conditions de victoire, mise en JSON.

## Arborescence du backend (cible)

`[existe]` = déjà dans le dépôt, `[à créer]` = cible. Le sous-dossier `core/` ci-dessous est la **découpe visée pour le moteur quand les règles et les explications arriveront** ; son contenu actuel est le squelette de `moteur/` ci-dessus.

```
backend/
├── main.cc                      [existe]  config, enregistrement du plugin, listener, run
├── CMakeLists.txt               [existe]  + add_subdirectory(core), lien avec magic_core
├── config.json                  [existe]  db_clients (sqlite ../data/database.sqlite), listeners
├── models/                      [existe]  ORM Drogon généré pour Cartes (optionnel)
│
├── controllers/                 [à créer] couche HTTP mince : parse, appelle, sérialise
│   ├── PartieCtrl.{h,cc}        routes de partie et d'actions
│   └── CartesCtrl.{h,cc}        GET /api/cartes, /api/cartes/{oracle_id}
├── plugins/
│   └── MoteurPlugin.{h,cc}      [à créer] possède Catalogue + GestionnaireParties
├── api/                         [à créer] contrat JSON (utilise jsoncpp, pas Drogon)
│   ├── schemas/*.json           un schéma de requête et de réponse par action
│   ├── ActionJson.{h,cc}        Json::Value → Action        (valide + convertit)
│   └── EtatJson.{h,cc}          Etat, Explication → Json::Value
│
├── core/                        [à créer] bibliothèque magic_core, AUCUNE inclusion de <drogon/...>
│   ├── CMakeLists.txt           add_library + add_custom_command(chrppc ...)
│   ├── cartes/
│   │   ├── Carte.{h,cc}         données imprimées, immuables
│   │   └── Catalogue.{h,cc}     index par oracle_id et par nom, rempli par le plugin
│   ├── decks/
│   │   └── Deck.{h,cc}          Deck, chargerDecks(fichier) → liste de cartes résolues
│   ├── moteur/
│   │   ├── Etat.{h,cc}          Joueur, Zone, ObjetDeJeu, Pile, phase, tour
│   │   ├── Action.h             variantes d'actions du joueur
│   │   ├── Partie.{h,cc}        façade : Partie::creer, Partie::appliquer
│   │   └── GestionnaireParties.{h,cc}  id → Partie (+ mutex), creer/trouver/supprimer
│   ├── regles/                  fichiers .chrpp
│   │   ├── base/                tours, phases, priorité, pile, combat, actions basées sur l'état
│   │   ├── mots_cles/           vol, célérité, piétinement… (réutilisés par plusieurs cartes)
│   │   ├── kaladesh/            énergie, véhicules/pilotage, fabrication, effets par carte
│   │   └── Registre.{h,cc}      oracle_id → règles de la carte ; signale « non implémentée »
│   └── explication/
│       ├── Journal.{h,cc}       Evenement + chaîne de causes
│       └── Explicateur.{h,cc}   règle + paramètres → phrase FR (gabarits)
│
└── test/                        [existe]  test_main.cc (Drogon)

data/schema.sql, data/database.sqlite, data/decks.json   [schema.sql et database.sqlite existent ; decks.json à créer]
utils/card_import.py, utils/card_json_to_db.cpp      [existent]
frontend/                                            [existe] React + TS + Vite (+ Tailwind à ajouter)
```

`controllers/`, `plugins/` et `models/` sont repris par `aux_source_directory` dans le `CMakeLists.txt` actuel : un nouveau fichier n'est pris en compte qu'après un nouveau `cmake` (le script Docker ne le relance que si le cache n'existe pas : lancer `cmake -S backend -B backend/build` à la main).

**Règle de dépendance** (à respecter à la revue de code) :

```
controllers ─► api ─► core/moteur ─► core/regles ─► core/explication
     │                     │              │
     ▼                     ▼              ▼
  plugins ───────────► core/cartes ◄──────┘          core/* n'inclut jamais drogon
```

## Fonctions et types clés de `magic_core`

```cpp
// cartes/Catalogue.h  — lecture seule après remplissage
class Catalogue {
public:
    void ajouter(Carte c);                               // appelé par MoteurPlugin
    const Carte* parOracleId(const std::string&) const;
    const Carte* parNom(const std::string&) const;       // nom EN, utilisé par decks.json
    std::vector<const Carte*> recherche(const Filtre&) const;
};

// decks/Deck.h
std::vector<Deck> chargerDecks(const std::string& fichier, const Catalogue&); // erreur si carte inconnue

// moteur/Action.h  — une alternative par route HTTP
using Action = std::variant<JouerTerrain, LancerSort, ActiverCapacite,
                            DeclarerAttaquants, DeclarerBloqueurs, Choisir, PasserPriorite>;

// moteur/Partie.h  — SEUL point d'entrée du moteur pour Drogon
struct Resultat {
    bool                     accepte;
    std::string              raison_refus;   // vide si accepté
    Etat                     etat;           // état après l'action (inchangé si refus)
    std::vector<Explication> explications;   // chaîne de causes produite par CETTE action
    std::optional<Choix>     choix_en_attente;
};
class Partie {
public:
    static Partie creer(const Catalogue&, const std::vector<Deck>&, unsigned graine);
    Resultat       appliquer(JoueurId, const Action&);
    const Etat&    etat() const;
    const Journal& journal() const;          // historique complet de la partie
};

// moteur/GestionnaireParties.h
class GestionnaireParties {
public:
    PartieId creer(const std::vector<Deck>&);
    template <class F> auto avec(PartieId, F&& f);   // verrouille la partie le temps de l'appel
};

// explication/Explicateur.h
std::string formuler(const Evenement&);       // gabarit de la règle + paramètres → texte FR
```

### Où vit l'état, et comment CHR y accède

Choix proposé : **`Etat` (C++) est la source de vérité**. Les règles CHR lisent l'état dans leurs gardes et le modifient dans leurs corps par un petit nombre de méthodes (`deplacer`, `infliger`, `engager`, `ajouterEnergie`…), qui postent chacune un fait `evenement(...)`. Avantages : sérialisation JSON directe, copie triviale pour le retour arrière, un seul endroit à relire.

Les contraintes CHR portent les **événements et les décisions** : `demande(...)`, `refus(...)`, `evenement(...)`, `cause(...)`.

> À valider par un petit prototype (étape 2) : comment `chrppc` expose des types C++ dans les gardes et corps de règles. Si l'accès à `Etat` y est pénible, l'alternative est de garder l'état dans le magasin de contraintes et d'écrire un `exporterEtat()`.

Faits postés par le moteur, exemples : `action(J, lancer_sort(Carte))`, `phase(principale1)`, `evenement(Id, entre_en_jeu(Objet))`, `blessures(Src, Cible, N)`.

## Algorithme d'une action

```
Résultat Partie::appliquer(joueur, action):

    copie ← etat                                   // retour arrière si une exception survient
    journal.ouvrir(action)

    ── 1. VALIDATION (règles CHR en lecture seule : n'écrivent pas dans Etat) ──
    poster  demande(joueur, action)
    exécuter les règles jusqu'au point fixe        // ex. :
        // demande(J, lancer_sort(C)), phase(P), P ∉ {principale1, principale2}
        //     ==> refus("un sort non éphémère se lance en phase principale")
        // demande(J, lancer_sort(C)), pas_assez_de_mana(J, C)   ==> refus("mana insuffisant")
    si un fait refus(raison) existe :
        journal.enregistrer_refus(raison, règle fautive)
        retourner { accepte=faux, raison, etat inchangé, explications }

    ── 2. EXÉCUTION (les règles peuvent maintenant modifier Etat) ──
    poster  execute(joueur, action)               // payer le coût, mettre le sort sur la pile…

    ── 3. RÉSOLUTION jusqu'au point fixe (CHR enchaîne seul) ──
    tant que des règles se déclenchent :           // ordre des règles = priorité
        a) actions basées sur l'état (endurance ≤ 0 → cimetière, 0 loyauté, pioche vide…)
        b) capacités déclenchées → posées sur la pile
        c) si tous les joueurs passent la priorité : résoudre le sommet de la pile
        d) si une règle exige un choix du joueur : poster choix_requis(...) et arrêter
    chaque règle qui s'applique pose  cause(regle, evenement, evenement_parent)

    ── 4. SORTIE ──
    explications ← formuler( journal.chaîne_de_causes(action) )
    retourner { accepte=vrai, etat, explications, choix_en_attente }

    en cas d'exception : etat ← copie ; journal.annuler() ; relancer
```

Points de conception :

- **Validation avant exécution** : un refus ne laisse aucune trace dans `Etat`, sans restauration compliquée.
- **Choix du joueur** (cible, mode, « peut ») : le plus possible **à l'annonce** (comme dans les règles réelles, on choisit les cibles en lançant le sort). Sinon la résolution s'arrête sur `choix_en_attente`, et l'action suivante `Choisir` la reprend.
- **Priorité** : en v1 un seul joueur agit ; l'adversaire est supposé passer. À préciser avec l'encadrant (adversaire passif, second joueur humain, IA minimale).
- **Déterminisme** : `graine` fixe le mélange des bibliothèques, pour reproduire une partie et la tester.

## Explication : de la règle au texte

```
règle CHR déclenchée ──► cause(regle_id, evt_id, parent_id) ──► Journal (arbre d'événements)
                                                                      │
                                      remonter les parents depuis l'événement final
                                                                      ▼
                          [ « Vous lancez … », « parce que la règle X permet… », … ]
                                                                      │
                          Explicateur::formuler : gabarit FR par regle_id  ◄── fichier de gabarits
```

- Un événement ⇒ une ligne du journal ⇒ une phrase. La **chaîne de causes** est le chemin du résultat jusqu'à l'action initiale du joueur.
- Chaque `regle_id` doit avoir un gabarit ; un test échoue sinon, pour ne pas laisser de règle muette.
- Option complémentaire : la trace de `chrppc --trace`, utile pour déboguer plus que pour l'affichage.

## Contrat HTTP : une route par action

| Méthode | Route | Rôle |
|---|---|---|
| GET | `/api/cartes`, `/api/cartes/{oracle_id}` | catalogue, recherche |
| GET | `/api/decks` | decks lus dans `data/decks.json` |
| POST | `/api/parties` | crée une partie (decks du fichier), renvoie l'état initial |
| GET | `/api/parties/{id}` | état courant + journal |
| POST | `/api/parties/{id}/actions/jouer-terrain` | `{ "joueur": 0, "objet": 12 }` |
| POST | `/api/parties/{id}/actions/lancer-sort` | `{ "joueur", "objet", "cibles": [...] }` |
| POST | `/api/parties/{id}/actions/activer-capacite` | capacités activées, dont fabrication et pilotage |
| POST | `/api/parties/{id}/actions/declarer-attaquants`, `…/declarer-bloqueurs` | combat |
| POST | `/api/parties/{id}/actions/choisir` | réponse à un `choix_en_attente` |
| POST | `/api/parties/{id}/actions/passer-priorite` | fait avancer pile et phases |

Réponse type de toute action :

```json
{
  "accepte": true,
  "etat": {
    "tour": 3, "phase": "principale1", "joueur_actif": 0,
    "joueurs": [
      { "vie": 20, "energie": 2,
        "main": [ { "objet": 12, "oracle_id": "…" } ],
        "champ": [ { "objet": 7, "oracle_id": "…", "engage": false, "marqueurs": {} } ],
        "cimetiere": [], "bibliotheque": 41 }
    ],
    "pile": []
  },
  "explications": [
    { "id": 31, "cause": null, "regle": "lancer_sort", "texte": "Vous lancez … en payant {1}{R}." },
    { "id": 32, "cause": 31,   "regle": "energie_gagnee", "texte": "… vous fait gagner {E}{E}." }
  ],
  "refus": null,
  "choix_en_attente": null
}
```

Un refus renvoie `200` avec `"accepte": false`, `"refus": "<raison>"` et l'état inchangé : c'est un résultat normal du jeu, pas une erreur HTTP. `400` est réservé aux JSON invalides, `404` aux parties inconnues, `500` aux exceptions.

## Parties simultanées et threads

Drogon traite les requêtes sur plusieurs threads. Règles à respecter :

- `Catalogue` : rempli au démarrage, **jamais modifié ensuite** ⇒ partageable sans verrou.
- `Partie` : **pas thread-safe**. `GestionnaireParties::avec(id, f)` prend un `std::mutex` propre à la partie pendant l'appel. Les parties distinctes restent parallèles.
- Le moteur est synchrone et court (aucune E/S) : pas besoin de coroutines ni de file de tâches.
- Les espaces CHR++ ne sont pas partagés : un par `Partie`.

## Ajouter une règle ou une carte

| Je veux ajouter… | Je touche | Je n'ai pas à toucher |
|---|---|---|
| une règle générale | `core/regles/base/*.chrpp` + gabarit d'explication + test | contrôleurs, front |
| un mot-clé | `core/regles/mots_cles/<mot>.chrpp` + gabarit + test | les cartes qui l'ont déjà (via `mots_cles` de la base) |
| une carte | `core/regles/kaladesh/<carte>.chrpp`, enregistrée par `oracle_id` dans `Registre` + test | `Catalogue`, base de données |
| un deck | `data/decks.json` | tout le reste (relu à la création de partie) |

Garde-fous : une carte absente de `Registre` est **signalée « non implémentée »** (refus explicite, pas d'effet silencieusement ignoré), `core/CMakeLists.txt` récupère les `.chrpp` par `file(GLOB … CONFIGURE_DEPENDS)`, et chaque règle ou carte vient avec un scénario « faits initiaux → faits attendus » dans un dossier de tests (à créer).

Question ouverte : décrire les effets simples par des **données** (briques génériques `piocher(N)`, `creer_jeton(X)`…, sans recompiler) ou écrire **un `.chrpp` par carte**. Le plan ci-dessus part sur un `.chrpp` par carte ; la voie « données » peut s'ajouter plus tard pour les cartes simples.

## Choix techniques

C++17/20 (celui détecté par le `CMakeLists.txt` généré par Drogon) · CMake · Drogon · jsoncpp · CHR++ (`chrppc` lancé par CMake via `add_custom_command`) · SQLite · React + TypeScript + Vite + Tailwind · Docker Compose (toolchain de l'hôte, voir `README.md`).

## Lancer

```
sudo apt install libjsoncpp-dev
python3 utils/card_import.py kld                      # → resources/kld.json
g++ utils/card_json_to_db.cpp -o utils/card_json_to_db $(pkg-config --cflags --libs sqlite3 jsoncpp)
./utils/card_json_to_db …                             # → data/database.sqlite  (chemin écrit en dur dans le fichier)
docker compose up -d                                   # front :5173, back :8080
```

## Étapes

1. Base de cartes : import Scryfall → SQLite ← *fait*
2. **Squelette du moteur à 2 joueurs, sans règle** (`moteur/`) ← *fait*
3. **Partie sans règles de bout en bout** : brancher `magic_core` dans Drogon (`add_subdirectory(../moteur)`), `EtatJson`/`ActionJson`, routes `/api/parties…`, `Catalogue` chargé depuis SQLite pour construire les decks, page de partie dans le front
4. Prototype CHR++ : un `ArbitreChr` (une règle de validation + une règle d'effet) derrière l'interface `Arbitre`
5. Règles de base (tours, phases, pile, terrains, sorts, actions basées sur l'état), puis journal et explications (`cause`, `Explicateur`)
6. Mécaniques Kaladesh : énergie, véhicules et pilotage, fabrication ; puis les cartes par lots
