import type { CarteEnJeu, Partie } from "../interfaces/Partie"
import ListeCarteComponent from '../components/ListeCarteComponent';
import BibliothequeComponent from '../components/BibliothequeComponent';
import PileFaceVisibleComponent from '../components/PileFaceVisibleComponent';

interface JoueurComponentProps {
    joueur: number
    actif?: boolean
    main: CarteEnJeu[];
    bibliotheque: number;
    cimetiere: CarteEnJeu[];
    exil: CarteEnJeu[];
    champDeBataille: CarteEnJeu[];
    onPartieChange?: (partie: Partie) => void;
}


function JoueurComponent({ joueur, main, bibliotheque, cimetiere, exil, champDeBataille, onPartieChange }: JoueurComponentProps) {
    return (
        <>
            <div className={`flex flex-1 min-h-0 flex-col overflow-hidden gap-2 ${joueur === 0 ? 'flex-col-reverse' : 'flex-col'}`}>
                {/* Champ de bataille du joueur */}
                <div className="flex flex-1 shrink-0 overflow-hidden border-y border-gray-400 p-2 bg-gray-500">
                    <ListeCarteComponent cartes={champDeBataille} joueur={joueur}/>
                </div>
                <div className="flex flex-1 min-h-0 flex-col overflow-hidden">
                    <div className="flex h-full w-full justify-around gap-2">
                        <div className="flex flex-col w-1/10 items-center justify-center gap-2 h-full shrink-0 overflow-hidden p-2 bg-gray-500">
                            {/* Exil du joueur */}
                            <div>
                                Exil
                            </div>
                            <PileFaceVisibleComponent cartes={exil} joueur={joueur}/>
                        </div>

                        {/* Main du joueur */}
                        <div className="h-full w-3/5 shrink-0 overflow-hidden border-y border-gray-400 p-2 bg-red-200">
                            <ListeCarteComponent cartes={main} joueur={joueur}/>
                        </div>
                        <div className="flex flex-col w-1/10 items-center justify-center gap-2 h-full shrink-0 overflow-hidden p-2 bg-gray-500">
                            {/* Bibliothèque du joueur */}
                            <div>
                                Bibliothèque
                            </div>
                            <BibliothequeComponent nb_cartes={bibliotheque} joueur={joueur} onPiocher={onPartieChange}/>
                        </div>
                        <div className="flex flex-col w-1/10 items-center justify-center gap-2 h-full shrink-0 overflow-hidden p-2 bg-gray-500">
                            {/* Cimetière du joueur */}
                            <div>
                                Cimetière
                            </div>
                            <PileFaceVisibleComponent cartes={cimetiere} joueur={joueur}/>
                        </div>
                    </div>
                </div>
            </div>
        </>
    );
}

export default JoueurComponent
