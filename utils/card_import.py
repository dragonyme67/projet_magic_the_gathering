
import requests
import json
import sys
import os

if len(sys.argv) != 2:
    print(f"Usage : python3 {sys.argv[0]} <code_set>")
    sys.exit(1)

set_code = sys.argv[1].lower()

url = "https://api.scryfall.com/cards/search"
params = {
    "q": f"e:{set_code} lang:fr",
    "unique": "prints",
    "order": "set"
}

headers = {
    "User-Agent": "ProjetMagic/1.0",
    "Accept": "application/json"
}

cards = []

while url:
    response = requests.get(url, params=params, headers=headers)
    response.raise_for_status()
    data = response.json()

    cards.extend(data["data"])
    url = data.get("next_page")
    params = None

os.makedirs("resources", exist_ok=True)

filename = f"resources/{set_code}.json"

with open(filename, "w", encoding="utf-8") as f:
    json.dump(cards, f, ensure_ascii=False, indent=2)

print(f"{len(cards)} cartes téléchargées en français dans {filename}")
