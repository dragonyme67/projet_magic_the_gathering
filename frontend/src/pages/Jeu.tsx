import { useEffect, useState } from 'react'
import type { Partie } from "../interfaces/Partie"
import JoueurComponent from '../components/JoueurComponent';
import InfoPartieComponent from '../components/InfoPartieComponent';

function Jeu() {

  const [partie, setPartie] = useState<Partie | null>(null)
  const [erreur, setErreur] = useState<string | null>(null)
  const [chargement, setChargement] = useState(true)

  useEffect(() => {
    let ignore = false

    fetch('/api/jeu', { method: 'POST' })
      .then((response) => {
        if (!response.ok) throw new Error(`Erreur HTTP : ${response.status}`)
        return response.json()
      })
      .then((data: Partie) => { if (!ignore) setPartie(data) })
      .catch((error: Error) => { if (!ignore) setErreur(error.message) })
      .finally(() => { if (!ignore) setChargement(false) })

    return () => { ignore = true }
  }, [])

  const haut = partie?.joueurs[0]
  const bas = partie?.joueurs[1]

  return (
    <main className="flex flex-1 min-h-0 text-[#f2efe4] bg-[radial-gradient(ellipse_at_center,#6b9b7c,#41705b)]">

      {chargement && <p className="m-4 font-titre text-lg">Création de la partie...</p>}
      {erreur && <p className="m-4 text-[#f3c5b8]">Erreur : {erreur}</p>}

      {partie && haut && bas && (
      <>
        <div className="flex h-full w-48 shrink-0 min-h-0 overflow-hidden gap-2 border-r border-[#cfe0d0]/25 bg-[#2a4d3f]/55">
          <InfoPartieComponent
            vie_joueur_haut={haut.vie}
            vie_joueur_bas={bas.vie}
            tour={partie.tour}
            nom_joueur_haut={haut.nom}
            nom_joueur_bas={bas.nom}
            joueur_actif={partie?.joueurs[partie.joueurActif].nom}
          />
        </div>
        <div className="flex flex-1 min-h-0 flex-col h-full w-full gap-2 p-2">
          <JoueurComponent
            actif={partie.joueurActif === 0}
            main={haut.main}
            bibliotheque={haut.bibliotheque}
            cimetiere={haut.cimetiere}
            exil={haut.exil}
            champDeBataille={haut.champ_de_bataille}
            joueur={1}
          />
          <JoueurComponent
            actif={partie.joueurActif === 1}
            main={bas.main}
            bibliotheque={bas.bibliotheque}
            cimetiere={bas.cimetiere}
            exil={bas.exil}
            champDeBataille={bas.champ_de_bataille}
            joueur={2}
          />
        </div>
      </>
      )}
    </main>
  )
}

export default Jeu
