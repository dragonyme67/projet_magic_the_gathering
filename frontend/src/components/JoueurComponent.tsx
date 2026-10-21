import type { Carte } from "../interfaces/Carte"
import ListeCarteComponent from '../components/ListeCarteComponent';
import BibliothequeComponent from '../components/BibliothequeComponent';
import PileFaceVisibleComponent from '../components/PileFaceVisibleComponent';

interface JoueurComponentProps {
    joueur: number
    main: Carte[];
    bibliotheque: number;
    cimetiere: Carte[];
    exil: Carte[];
    champDeBataille: Carte[];
}


function JoueurComponent({ joueur, main, bibliotheque, cimetiere, exil, champDeBataille }: JoueurComponentProps) {
    return (
        <>
            <div className={`flex flex-1 min-h-0 flex-col overflow-hidden gap-2 ${joueur == 1 ? 'flex-col-reverse' : 'flex-col'}`}>
                {/* Champ de bataille du joueur */}
                <div className="flex flex-1 shrink-0 overflow-hidden border-y border-gray-400 p-2 bg-gray-500">
                    <ListeCarteComponent cartes={champDeBataille} />
                </div>
                <div className="flex flex-1 min-h-0 flex-col overflow-hidden">
                    <div className="flex h-full w-full items-center justify-center gap-2">
                        <div className="flex flex-col w-1/10 items-center justify-center gap-2 h-full shrink-0 overflow-hidden p-2 bg-gray-500">
                            {/* Exil du joueur du haut */}
                            <div>
                                Exil
                            </div>
                            <PileFaceVisibleComponent cartes={exil} />
                        </div>

                        {/* Main du joueur du haut */}
                        <div className="h-full w-3/5 shrink-0 overflow-hidden border-y border-gray-400 p-2 bg-red-200">
                            <ListeCarteComponent cartes={main} />
                        </div>
                        <div className="flex flex-col w-1/10 items-center justify-center gap-2 h-full shrink-0 overflow-hidden p-2 bg-gray-500">
                            {/* Bibliothèque du joueur du haut */}
                            <div>
                                Bibliothèque
                            </div>
                            <BibliothequeComponent nb_cartes={bibliotheque} />
                        </div>
                        <div className="flex flex-col w-1/10 items-center justify-center gap-2 h-full shrink-0 overflow-hidden p-2 bg-gray-500">
                            {/* Cimetière du joueur du haut */}
                            <div>
                                Cimetière
                            </div>
                            <PileFaceVisibleComponent cartes={cimetiere} />
                        </div>
                    </div>
                </div>
            </div>
        </>
    );
}

export default JoueurComponent