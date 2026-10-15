import { Routes, Route, Link } from 'react-router'

import Accueil from './pages/Accueil'

function App() {
  return (
    <div className="min-h-screen p-6">
      <nav className="mb-6 flex gap-4">
        <Link to="/">Accueil</Link>
      </nav>

      <Routes>
        <Route path="/" element={<Accueil />} />
      </Routes>
    </div>
  )
}

export default App

