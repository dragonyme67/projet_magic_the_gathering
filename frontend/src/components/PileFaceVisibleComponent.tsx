import type { CarteEnJeu } from "../interfaces/Partie"
import CarteComponent from "./CarteComponent"

interface PileFaceVisibleComponentProps {
    cartes: CarteEnJeu[];
    joueur: number;
}

function PileFaceVisibleComponent({ cartes }: PileFaceVisibleComponentProps) {
    const carteDessus = cartes[cartes.length - 1]

    return (
        <div className="relative min-h-0 h-full w-full">
            {carteDessus ? (
                <>
                    <div className="absolute inset-0">
                        <CarteComponent
                            key={carteDessus.nom}
                            nom={carteDessus.nom}
                            image={carteDessus.image}
                            description={carteDessus.description}
                        />
                    </div>
                    <span className="pointer-events-none absolute inset-x-0 bottom-2 mx-auto w-fit min-w-8 rounded-full bg-[#2a4d3f]/90 px-2 py-0.5 text-center font-titre text-base font-bold text-[#e4d194] ring-1 ring-[#d8c27c]/50">
                        {cartes.length}
                    </span>
                </>
            ) : (
                <div className="absolute inset-0 flex items-center justify-center rounded-xl border border-dashed border-[#cfe0d0]/35 font-titre text-base italic text-[#f2efe4]/45">
                    vide
                </div>
            )}
        </div>
    )
}

export default PileFaceVisibleComponent
