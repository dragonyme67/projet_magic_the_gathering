
import { useEffect, useState } from 'react'

function App() {
  const [message, setMessage] = useState('Chargement...')

  useEffect(() => {
    fetch('/api/hello')
      .then((response) => {
        if (!response.ok) {
          throw new Error(`Erreur HTTP : ${response.status}`)
        }
        return response.json()
      })
      .then((data: { message: string }) => {
        setMessage(data.message)
      })
      .catch((error: Error) => {
        setMessage(`Erreur : ${error.message}`)
      })
  }, [])

  return (
    <main>
      <h1>Mon application</h1>
      <p>{message}</p>
    </main>
  )
}

export default App
