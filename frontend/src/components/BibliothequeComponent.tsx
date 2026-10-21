import type { Partie, ReponseAction } from "../interfaces/Partie"

interface BibliothequeComponentProps {
  nb_cartes: number;
  joueur: number;
  onPiocher?: (partie: Partie) => void;
}

function BibliothequeComponent({ nb_cartes, joueur, onPiocher }: BibliothequeComponentProps) {

  async function piocher() {
    try {
      const response = await fetch("/api/jeu/piocher", {
        method: "POST",
        headers: {
          "Content-Type": "application/json",
        },
        body: JSON.stringify({
          joueur: joueur,
        }),
      });

      if (!response.ok) {
        const errorData = await response.json().catch(() => null);
        throw new Error(errorData?.refus || errorData?.error || "Erreur lors de la pioche");
      }

      const data: ReponseAction = await response.json();
      if (data.partie && onPiocher) {
        onPiocher(data.partie);
      }
    } catch (error) {
      console.error(error);
    }
  }

  return (
    <div className="relative h-full w-full min-h-0 min-w-0 overflow-hidden">
      <img
        src="https://media.wizards.com/2022/images/daily/en_7C0yERSDK6.png"
        alt={`${nb_cartes} cartes restantes dans la pioche`}
        className="block h-full w-full object-contain rounded-2xl cursor-pointer"
        onClick={piocher}
      />
      <span className="pointer-events-none absolute inset-x-0 bottom-2 mx-auto w-fit min-w-8 rounded-full bg-[#2a4d3f]/90 px-2 py-0.5 text-center font-titre text-base font-bold text-[#e4d194] ring-1 ring-[#d8c27c]/50">
        {nb_cartes}
      </span>
    </div>
  );
}

export default BibliothequeComponent
