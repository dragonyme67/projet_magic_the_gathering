import { useEffect, useState } from 'react'

interface Carte {
  nom: string
  rarete: string
}

function App() {
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
      <h1>Mon application</h1>
      {chargement && <p>Chargement...</p>}
      {erreur && <p>Erreur : {erreur}</p>}
      {!chargement && !erreur && cartes.length === 0 && <p>Aucune carte.</p>}
      <ul>
        {cartes.map((carte) => (
          <li key={carte.nom}>
            {carte.nom} ({carte.rarete})
          </li>
        ))}
      </ul>
    </main>
  )
}

export default App
