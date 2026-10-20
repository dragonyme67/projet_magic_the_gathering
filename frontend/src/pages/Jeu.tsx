import { useEffect, useState } from 'react'
import type { Carte } from "../interfaces/Carte"
import JoueurComponent from '../components/JoueurComponent';
import InfoPartieComponent from '../components/InfoPartieComponent';

function Jeu() {

  const [cartes, setCartes] = useState<Carte[]>([])
  const [erreur, setErreur] = useState<string | null>(null)
  const [chargement, setChargement] = useState(true)

  useEffect(() => {
    fetch('/api/cartes/test')
      .then((response) => {
        if (!response.ok) {
          throw new Error(`Erreur HTTP : ${response.status}`)
        }
        return response.json()
      })
      .then((data) => {
        setCartes(data)
      })
      .catch((error: Error) => {
        setErreur(error.message)
      })
      .finally(() => {
        setChargement(false)
      })
  }, [])

  return (
    <main className="flex flex-1 min-h-0">

      {chargement && <p>Création de la partie...</p>}
      {erreur && <p>Erreur : {erreur}</p>}

      <>
          <div className="flex h-full w-[8vw] min-h-0 overflow-hidden gap-2 bg-purple-600">
          <InfoPartieComponent
            vie_joueur_haut={20}
            vie_joueur_bas={20}
            tour={1}
            nom_joueur_haut={"Alice"}
            nom_joueur_bas={"Bob"}
            joueur_actif={"Alice"}
          />
          </div>
          <div className="flex flex-1 min-h-0 flex-col h-full w-full">
            <JoueurComponent
              main={cartes}
              bibliotheque={cartes}
              cimetiere={cartes}
              exil={cartes}
              champDeBataille={cartes}
              joueur={1}
            />
            <JoueurComponent
              main={cartes}
              bibliotheque={cartes}
              cimetiere={cartes}
              exil={cartes}
              champDeBataille={cartes}
              joueur={2}
            />
          </div>
      </>
    </main>
  )
}

export default Jeu
