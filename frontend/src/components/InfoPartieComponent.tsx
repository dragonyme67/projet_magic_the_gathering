interface InfoPartieComponentProps {
    vie_joueur_haut: number;
    vie_joueur_bas: number;
    tour: number;
    nom_joueur_haut: string;
    nom_joueur_bas: string;
    joueur_actif: string;
}


function InfoPartieComponent({ vie_joueur_haut, vie_joueur_bas, tour, nom_joueur_haut, nom_joueur_bas, joueur_actif }: InfoPartieComponentProps) {
    return (
        <>
            <div className="flex flex-col flex-1 min-h-0 overflow-hidden gap-2 justify-between items-center p-3">
                <div className={`w-full flex flex-col items-center justify-center gap-1 rounded-xl border bg-black/30 py-4 ${joueur_actif === nom_joueur_haut ? 'border-amber-400/70 text-amber-300' : 'border-white/10 text-white/70'}`}>
                    <span className="font-serif text-lg tracking-wide">{nom_joueur_haut}</span>
                    <span className="text-5xl font-extrabold tabular-nums">{vie_joueur_haut}</span>
                    <span className="text-xs uppercase tracking-widest">Points de vie</span>
                </div>
                <div className="flex flex-col items-center gap-1 rounded-xl border border-amber-300/40 bg-black/40 px-4 py-3 text-center">
                    <span className="text-xs uppercase tracking-widest text-amber-200/80">Tour {tour}</span>
                    <span className="font-serif text-xl font-bold text-amber-300">{joueur_actif}</span>
                    <span className="text-xs text-white/60">joue</span>
                </div>
                <div className={`w-full flex flex-col items-center justify-center gap-1 rounded-xl border bg-black/30 py-4 ${joueur_actif === nom_joueur_bas ? 'border-amber-400/70 text-amber-300' : 'border-white/10 text-white/70'}`}>
                    <span className="font-serif text-lg tracking-wide">{nom_joueur_bas}</span>
                    <span className="text-5xl font-extrabold tabular-nums">{vie_joueur_bas}</span>
                    <span className="text-xs uppercase tracking-widest">Points de vie</span>
                </div>
            </div>
        </>
    );
}

export default InfoPartieComponent
