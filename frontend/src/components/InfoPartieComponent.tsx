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
            <div className="flex flex-col flex-1 min-h-0 overflow-hidden gap-2 justify-between items-center">
                <div className="h-1/10 w-full flex flex-col items-center justify-center">
                    <span className="font-bold">{vie_joueur_haut} PV</span>
                    <span className="font-bold">Joueur : {nom_joueur_haut}</span>
                </div>
                <div className="font-bold">
                    Tour ({tour}) de : {joueur_actif}
                </div>
                <div className="h-1/10 w-full flex flex-col items-center justify-center">
                    <span className="font-bold">{vie_joueur_bas} PV</span>
                    <span className="font-bold">Joueur : {nom_joueur_bas}</span>
                </div>
            </div>
        </>
    );
}

export default InfoPartieComponent