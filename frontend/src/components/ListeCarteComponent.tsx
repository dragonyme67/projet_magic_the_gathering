import type { Carte } from "../interfaces/Carte"
import CarteComponent from '../components/CarteComponent';

interface ListeCartesProps {
    cartes: Carte[];
}


function ListeCartesProps({ cartes }: ListeCartesProps) {
  return (
    <div className="grid min-h-0 h-full w-full grid-flow-col auto-cols-[minmax(0,1fr)] gap-1">        
        {cartes.map((carte) => (
            <CarteComponent
                key={carte.nom}
                nom={carte.nom}
                image={carte.image}
                description={carte.description}
            />
        ))}
    </div>
  );
}

export default ListeCartesProps