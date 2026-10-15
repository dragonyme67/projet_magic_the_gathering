CREATE TABLE IF NOT EXISTS Cartes (
    id                TEXT PRIMARY KEY,            -- Scryfall id (UUID)
    oracle_id         TEXT NOT NULL,
    multiverse_id     INTEGER,
    nom               TEXT NOT NULL,               -- name (anglais)
    nom_imprime       TEXT,                        -- printed_name
    langue            TEXT NOT NULL DEFAULT 'en',  -- lang
    date_sortie       TEXT,                        -- released_at (YYYY-MM-DD)
    layout            TEXT NOT NULL DEFAULT 'normal',
    cout_mana         TEXT,                        -- mana_cost, ex. "{2}{W}"
    cmc               REAL NOT NULL DEFAULT 0,
    ligne_type        TEXT NOT NULL,               -- type_line
    ligne_type_imprimee TEXT,                      -- printed_type_line
    texte_oracle      TEXT,                        -- oracle_text
    texte_imprime     TEXT,                        -- printed_text
    texte_ambiance    TEXT,                        -- flavor_text
    force             TEXT,                        -- power ("*", "1+*" possibles)
    endurance         TEXT,                        -- toughness
    loyaute           TEXT,                        -- loyalty
    couleurs          TEXT NOT NULL DEFAULT '[]',  -- JSON array: ["W"]
    identite_couleur  TEXT NOT NULL DEFAULT '[]',  -- JSON array
    mots_cles         TEXT NOT NULL DEFAULT '[]',  -- keywords, JSON array
    legalites         TEXT NOT NULL DEFAULT '{}',  -- JSON object
    code_set          TEXT NOT NULL,               -- set ("kld")
    nom_set           TEXT,                        -- set_name
    numero_collection TEXT NOT NULL,               -- collector_number
    rarete            TEXT NOT NULL,               -- rarity
    artiste           TEXT,
    image_small       TEXT,
    image_normal      TEXT,
    image_large       TEXT,
    image_png         TEXT,
    image_art_crop    TEXT,
    uri_scryfall      TEXT,
    uri_rulings       TEXT,
    edhrec_rank       INTEGER,
    UNIQUE (code_set, numero_collection, langue)
);

CREATE INDEX IF NOT EXISTS idx_cartes_oracle_id ON Cartes(oracle_id);
CREATE INDEX IF NOT EXISTS idx_cartes_nom       ON Cartes(nom);
CREATE INDEX IF NOT EXISTS idx_cartes_set       ON Cartes(code_set);