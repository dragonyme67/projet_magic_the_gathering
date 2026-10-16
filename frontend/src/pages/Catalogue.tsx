import { useEffect, useState } from 'react'
import type { Carte } from "../interfaces/Carte"
import CarteComponent from '../components/CarteComponent';

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
    <main>
    <h1 className="text-4xl font-bold text-center my-8">Magic the Gathering - Catalogue</h1>

    {chargement && <p>Chargement des cartes...</p>}
    {erreur && <p>Erreur : {erreur}</p>}

    {!chargement && !erreur && cartes.length === 0 && (
        <p>Aucune carte.</p>
    )}

    <div className="grid grid-cols-4 gap-4">
        {cartes.map((carte) => (
            <CarteComponent
                key={carte.nom}
                nom={carte.nom}
                image={carte.image}
            />
        ))}
    </div>
    </main>
  )
}

export default Catalogue