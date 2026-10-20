import { useEffect, useState } from 'react'
import type { Partie } from "../interfaces/Partie"
import ChampDeBatailleComponent from '../components/ChampDeBatailleComponent';

function Jeu() {

    const [partie, setPartie] = useState<Partie | null>(null)
    const [erreur, setErreur] = useState<string | null>(null)
    const [chargement, setChargement] = useState(true)

    useEffect(() => {
    let ignore = false

    fetch('/api/jeu', { method: 'POST' })
      .then((response) => {
        if (!response.ok) {
          throw new Error(`Erreur HTTP : ${response.status}`)
        }
        return response.json()
      })
      .then((data: Partie) => {
        if (!ignore) setPartie(data)
      })
      .catch((error: Error) => {
        if (!ignore) setErreur(error.message)
      })
      .finally(() => {
        if (!ignore) setChargement(false)
      })

    return () => {
      ignore = true
    }
    }, [])

    const [joueurBas, joueurHaut] = partie?.joueurs ?? []

    return (
    <main className="h-full flex flex-col overflow-hidden">

      {chargement && <p>Création de la partie...</p>}
      {erreur && <p>Erreur : {erreur}</p>}

      {joueurBas && joueurHaut && (
        <>
        <div className="flex-1" />
        {/* Main du joueur du haut */}
        <section
          aria-label={`Main de ${joueurHaut.nom}`}
          className="h-[25vh] shrink-0 overflow-hidden border-y border-gray-400 p-2"
        >
        <ChampDeBatailleComponent cartes={joueurHaut.main} />
        </section>
        <div className="h-[5vh]" />
        {/* Main du joueur du bas */}
        <section
          aria-label={`Main de ${joueurBas.nom}`}
          className="h-[25vh] shrink-0 overflow-hidden border-y border-gray-400 p-2"
        >
        <ChampDeBatailleComponent cartes={joueurBas.main} />
        </section>
        <div className="flex-1" />
        </>
      )}
    </main>
  )
}

export default Jeu
