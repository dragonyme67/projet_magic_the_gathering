import type { Carte } from "../interfaces/Carte"
import ListeCarteComponent from '../components/ListeCarteComponent';
import BibliothequeComponent from '../components/BibliothequeComponent';
import PileFaceVisibleComponent from '../components/PileFaceVisibleComponent';

interface JoueurComponentProps {
    joueur: number
    actif?: boolean
    main: Carte[];
    bibliotheque: number;
    cimetiere: Carte[];
    exil: Carte[];
    champDeBataille: Carte[];
}


function JoueurComponent({ joueur, actif = false, main, bibliotheque, cimetiere, exil, champDeBataille }: JoueurComponentProps) {
    return (
        <>
            <div className={`flex flex-1 min-h-0 flex-col overflow-hidden gap-2 rounded-2xl bg-[#2a4d3f]/25 p-2 ${joueur == 1 ? 'flex-col-reverse' : 'flex-col'} ${actif ? 'ring-2 ring-[#d8c27c]' : 'ring-1 ring-[#cfe0d0]/20'}`}>
                {/* Champ de bataille du joueur */}
                <div className="relative flex flex-1 shrink-0 overflow-hidden rounded-xl border border-dashed border-[#cfe0d0]/35 bg-[#2a4d3f]/30 p-2">
                    {champDeBataille.length === 0 && (
                        <span className="pointer-events-none absolute inset-0 flex items-center justify-center font-serif text-sm uppercase tracking-[0.4em] text-[#f2efe4]/35">
                            Champ de bataille
                        </span>
                    )}
                    <ListeCarteComponent cartes={champDeBataille} />
                </div>
                <div className="flex flex-1 min-h-0 flex-col overflow-hidden">
                    <div className="flex h-full w-full justify-around gap-2">
                        <div className="flex flex-col w-1/10 items-center justify-center gap-2 h-full shrink-0 overflow-hidden rounded-xl border border-[#cfe0d0]/25 bg-[#2a4d3f]/50 p-2">
                            {/* Exil du joueur du haut */}
                            <div className="font-serif text-xs uppercase tracking-widest text-[#e3efe3]/90">
                                Exil
                            </div>
                            <PileFaceVisibleComponent cartes={exil} />
                        </div>

                        {/* Main du joueur du haut */}
                        <div className="h-full w-3/5 shrink-0 overflow-hidden rounded-xl border border-[#d8c27c]/35 bg-[#2a4d3f]/60 p-2">
                            <ListeCarteComponent cartes={main} />
                        </div>
                        <div className="flex flex-col w-1/10 items-center justify-center gap-2 h-full shrink-0 overflow-hidden rounded-xl border border-[#cfe0d0]/25 bg-[#2a4d3f]/50 p-2">
                            {/* Bibliothèque du joueur du haut */}
                            <div className="font-serif text-xs uppercase tracking-widest text-[#e3efe3]/90">
                                Bibliothèque
                            </div>
                            <BibliothequeComponent nb_cartes={bibliotheque} />
                        </div>
                        <div className="flex flex-col w-1/10 items-center justify-center gap-2 h-full shrink-0 overflow-hidden rounded-xl border border-[#cfe0d0]/25 bg-[#2a4d3f]/50 p-2">
                            {/* Cimetière du joueur du haut */}
                            <div className="font-serif text-xs uppercase tracking-widest text-[#e3efe3]/90">
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
