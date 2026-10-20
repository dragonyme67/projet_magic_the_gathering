import type { CarteEnJeu } from "../interfaces/Partie"
import CarteComponent from '../components/CarteComponent';

interface ChampDeBatailleComponentProps {
    cartes: CarteEnJeu[]
}


function ChampDeBatailleComponentProps({ cartes }: ChampDeBatailleComponentProps) {
  return (
    <div className="grid h-full w-full grid-flow-col auto-cols-[minmax(0,1fr)] gap-2">        
        {cartes.map((carte) => (
            <CarteComponent
                key={carte.id}
                nom={carte.nom}
                mana={carte.mana}
                description={carte.description}
                image={carte.image}
            />
        ))}
    </div>
  );
}

export default ChampDeBatailleComponentProps