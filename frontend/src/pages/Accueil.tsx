import { useEffect, useState } from 'react'

interface Carte {
  nom: string
  image: string
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
      <button onClick={sayHello} disabled={chargement} className="btn-primary">
        {chargement ? 'Chargement...' : 'Contacter le serveur'}
      </button>
      {message && <p>Réponse du serveur : {message}</p>}
      {erreur && <p>Erreur : {erreur}</p>}
      {!chargement && !erreur && cartes.length === 0 && <p>Aucune carte.</p>}
      <div className="grid grid-cols-4 gap-4">
      {cartes.map((carte) => (
        <div key={carte.nom} className="text-center">
          <img
            src={carte.image}
            alt={carte.nom}
            className="mx-auto w-full max-w-[200px]"
          />
          <p className="font-semibold">{carte.nom}</p>
        </div>
      ))}
    </div>
    </main>
  )
}

export default App
