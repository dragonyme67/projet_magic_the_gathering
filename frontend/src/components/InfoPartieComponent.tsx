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
                <div className={`w-full flex flex-col items-center justify-center gap-1 rounded-xl border bg-[#2a4d3f]/50 py-4 ${joueur_actif === nom_joueur_haut ? 'border-[#d8c27c] text-[#e4d194]' : 'border-[#cfe0d0]/25 text-[#f2efe4]/80'}`}>
                    <span className="font-serif text-lg tracking-wide">{nom_joueur_haut}</span>
                    <span className="text-5xl font-bold tabular-nums">{vie_joueur_haut}</span>
                    <span className="text-xs uppercase tracking-widest">Points de vie</span>
                </div>
                <div className="flex flex-col items-center gap-1 rounded-xl border border-[#d8c27c]/45 bg-[#2a4d3f]/60 px-4 py-3 text-center">
                    <span className="text-xs uppercase tracking-widest text-[#e3efe3]/80">Tour {tour}</span>
                    <span className="font-serif text-xl font-bold text-[#e4d194]">{joueur_actif}</span>
                    <span className="text-xs text-[#f2efe4]/65">joue</span>
                </div>
                <div className={`w-full flex flex-col items-center justify-center gap-1 rounded-xl border bg-[#2a4d3f]/50 py-4 ${joueur_actif === nom_joueur_bas ? 'border-[#d8c27c] text-[#e4d194]' : 'border-[#cfe0d0]/25 text-[#f2efe4]/80'}`}>
                    <span className="font-serif text-lg tracking-wide">{nom_joueur_bas}</span>
                    <span className="text-5xl font-bold tabular-nums">{vie_joueur_bas}</span>
                    <span className="text-xs uppercase tracking-widest">Points de vie</span>
                </div>
            </div>
        </>
    );
}

export default InfoPartieComponent
