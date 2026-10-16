import { useEffect, useState } from 'react'
import type { Carte } from "../interfaces/Carte"
import ChampDeBatailleComponent from '../components/ChampDeBatailleComponent';

function Catalogue() {

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
    <main className="h-full flex flex-col overflow-hidden">

      {chargement && <p>Chargement des cartes...</p>}
      {erreur && <p>Erreur : {erreur}</p>}

      {!chargement && !erreur && cartes.length === 0 && (
        <p>Aucune carte.</p>
      )}

      {!chargement && !erreur && cartes.length > 0 && (
        <>
        <div className="flex-1" />
        {/* Champ de bataille du joueur du haut */}
        <section className="h-[25vh] shrink-0 overflow-hidden border-y border-gray-400 p-2">
        <ChampDeBatailleComponent cartes={cartes} />
        </section>
        <div className="h-[5vh]" />
        {/* Champ de bataille du joueur du bas */}
        <section className="h-[25vh] shrink-0 overflow-hidden border-y border-gray-400 p-2">
        <ChampDeBatailleComponent cartes={cartes} />
        </section>
        <div className="flex-1" />
        </>
      )}
    </main>
  )
}

export default Catalogue