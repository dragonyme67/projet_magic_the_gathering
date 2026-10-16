import type { Carte } from "../interfaces/Carte"
import CarteComponent from '../components/CarteComponent';

interface ChampDeBatailleComponentProps {
    cartes: Carte[]
}


function ChampDeBatailleComponentProps({ cartes }: ChampDeBatailleComponentProps) {
  return (
    <div className="grid h-full w-full grid-flow-col auto-cols-[minmax(0,1fr)] gap-2">        
        {cartes.map((carte) => (
            <CarteComponent
                key={carte.nom}
                nom={carte.nom}
                image={carte.image}
            />
        ))}
    </div>
  );
}

export default ChampDeBatailleComponentProps