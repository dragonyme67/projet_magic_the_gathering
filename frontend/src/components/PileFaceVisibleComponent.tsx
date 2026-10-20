import type { Carte } from "../interfaces/Carte"
import CarteComponent from "./CarteComponent"

interface PileFaceVisibleComponentProps {
    cartes: Carte[];
}

function PileFaceVisibleComponent({ cartes }: PileFaceVisibleComponentProps) {
    const carteDessus = cartes[cartes.length - 1]

    return (
        <div className="relative min-h-0 h-full w-full">
            {carteDessus && (
                <div className="absolute inset-0">
                    <CarteComponent
                        key={carteDessus.nom}
                        nom={carteDessus.nom}
                        image={carteDessus.image}
                    />
                </div>
            )}
        </div>
    )
}

export default PileFaceVisibleComponent