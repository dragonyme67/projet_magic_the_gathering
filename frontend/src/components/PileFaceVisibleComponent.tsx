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
                    <span className="pointer-events-none absolute inset-x-0 bottom-2 mx-auto w-fit min-w-8 rounded-full bg-black/80 px-2 py-0.5 text-center text-sm font-bold text-amber-300 ring-1 ring-amber-300/50">
                        {cartes.length}
                    </span>
                </>
            ) : (
                <div className="absolute inset-0 flex items-center justify-center rounded-xl border border-dashed border-white/20 text-xs uppercase tracking-widest text-white/30">
                    vide
                </div>
            )}
        </div>
    )
}

export default PileFaceVisibleComponent
