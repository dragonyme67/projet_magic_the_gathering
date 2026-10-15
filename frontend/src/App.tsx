import { useEffect, useState } from 'react'

interface Carte {
  nom: string
  rarete: string
}

interface message {
  message: string
}

function App() {
  const [message, setMessage] = useState<string>("")
  const [cartes, setCartes] = useState<Carte[]>([])
  const [erreur, setErreur] = useState<string | null>(null)
  const [chargement, setChargement] = useState(true)

  async function sayHello() {
    setChargement(true)
    setErreur(null)
    try {
      const response = await fetch('/api/hello')

      if (!response.ok) {
        throw new Error(`Erreur HTTP : ${response.status}`)
      }

      const data: message = await response.json()
      setMessage(data.message)

    } catch (error) {
      setErreur(
        error instanceof Error ? error.message : 'Erreur inconnue'
      )
    } finally {
      setChargement(false)
    }
  }

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
      <button onClick={sayHello} disabled={chargement}>
        {chargement ? 'Chargement...' : 'Contacter le serveur'}
      </button>
      {message && <p>Réponse du serveur : {message}</p>}
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
