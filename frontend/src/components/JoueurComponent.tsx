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
            <div className={`flex flex-1 min-h-0 flex-col overflow-hidden gap-2 rounded-2xl bg-black/20 p-2 transition-shadow duration-300 ${joueur == 1 ? 'flex-col-reverse' : 'flex-col'} ${actif ? 'ring-2 ring-amber-400/80 shadow-[0_0_35px_rgba(251,191,36,0.25)]' : 'ring-1 ring-white/10'}`}>
                {/* Champ de bataille du joueur */}
                <div className="relative flex flex-1 shrink-0 overflow-hidden rounded-xl border border-dashed border-emerald-200/25 bg-black/25 p-2">
                    {champDeBataille.length === 0 && (
                        <span className="pointer-events-none absolute inset-0 flex items-center justify-center font-serif text-sm uppercase tracking-[0.4em] text-emerald-100/25">
                            Champ de bataille
                        </span>
                    )}
                    <ListeCarteComponent cartes={champDeBataille} />
                </div>
                <div className="flex flex-1 min-h-0 flex-col overflow-hidden">
                    <div className="flex h-full w-full justify-around gap-2">
                        <div className="flex flex-col w-1/10 items-center justify-center gap-2 h-full shrink-0 overflow-hidden rounded-xl border border-white/10 bg-black/30 p-2">
                            {/* Exil du joueur du haut */}
                            <div className="font-serif text-xs uppercase tracking-widest text-emerald-100/80">
                                Exil
                            </div>
                            <PileFaceVisibleComponent cartes={exil} />
                        </div>

                        {/* Main du joueur du haut */}
                        <div className="h-full w-3/5 shrink-0 overflow-hidden rounded-xl border border-amber-200/20 bg-linear-to-b from-black/30 to-black/50 p-2">
                            <ListeCarteComponent cartes={main} />
                        </div>
                        <div className="flex flex-col w-1/10 items-center justify-center gap-2 h-full shrink-0 overflow-hidden rounded-xl border border-white/10 bg-black/30 p-2">
                            {/* Bibliothèque du joueur du haut */}
                            <div className="font-serif text-xs uppercase tracking-widest text-emerald-100/80">
                                Bibliothèque
                            </div>
                            <BibliothequeComponent nb_cartes={bibliotheque} />
                        </div>
                        <div className="flex flex-col w-1/10 items-center justify-center gap-2 h-full shrink-0 overflow-hidden rounded-xl border border-white/10 bg-black/30 p-2">
                            {/* Cimetière du joueur du haut */}
                            <div className="font-serif text-xs uppercase tracking-widest text-emerald-100/80">
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
