import type { Carte } from "../interfaces/Carte"
import CarteComponent from "./CarteComponent"

interface PileFaceVisibleComponentProps {
    cartes: Carte[];
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
                    <span className="pointer-events-none absolute inset-x-0 bottom-2 mx-auto w-fit min-w-8 rounded-full bg-[#2a4d3f]/90 px-2 py-0.5 text-center text-sm font-bold text-[#e4d194] ring-1 ring-[#d8c27c]/50">
                        {cartes.length}
                    </span>
                </>
            ) : (
                <div className="absolute inset-0 flex items-center justify-center rounded-xl border border-dashed border-[#cfe0d0]/35 text-xs uppercase tracking-widest text-[#f2efe4]/40">
                    vide
                </div>
            )}
        </div>
    )
}

export default PileFaceVisibleComponent
