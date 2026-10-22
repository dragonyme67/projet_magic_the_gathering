import type { CarteEnJeu } from "../interfaces/Partie"
import CarteComponent from '../components/CarteComponent';

interface ListeCartesProps {
    cartes: CarteEnJeu[];
    joueur: number;
    onCarteClick?: (carte: CarteEnJeu) => void;
}


function ListeCartesProps({ cartes, onCarteClick }: ListeCartesProps) {
  return (
    <div className="grid min-h-0 h-full w-full grid-flow-col auto-cols-[minmax(0,1fr)] gap-1">        
        {cartes.map((carte) => (
            <CarteComponent
                key={carte.id}
                nom={carte.nom}
                image={carte.image}
                description={carte.description}
                onClick={onCarteClick ? () => onCarteClick(carte) : undefined}
                estEngage = {carte.engage}
            />
        ))}
    </div>
  );
}

export default ListeCartesProps