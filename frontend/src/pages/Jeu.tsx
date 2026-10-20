import { useEffect, useState } from 'react'
import type { Carte } from "../interfaces/Carte"
import JoueurComponent from '../components/JoueurComponent';

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
      .then((data: Carte[]) => {
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

      {chargement && <p>Chargement des cartes...</p>}
      {erreur && <p>Erreur : {erreur}</p>}

      {!chargement && !erreur && cartes.length === 0 && (
        <p>Aucune carte.</p>
      )}

      {!chargement && !erreur && cartes.length > 0 && (
        <>
          <div className="flex min-h-0 flex-col h-full w-full">
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
      )}
    </main>
  )
}

export default Jeu