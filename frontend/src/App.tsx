import { Routes, Route, Link } from 'react-router'

import Accueil from './pages/Accueil'
import Catalogue from './pages/Catalogue'
import Jeu from './pages/Jeu'

function App() {
  return (
    <div className="h-screen flex flex-col overflow-hidden bg-green-200">
      <nav className="flex shrink-0 gap-2 bg-green-300 p-2">
        <Link to="/" className='btn-primary'>Accueil</Link>
        <Link to="/catalogue" className='btn-primary'>Catalogue</Link>
        <Link to="/jeu" className='btn-primary'>Jeu</Link>
      </nav>

      <Routes>
        <Route path="/" element={<Accueil />} />
        <Route path="/catalogue" element={<Catalogue />} />
        <Route path="/jeu" element={<Jeu />} />
      </Routes>
    </div>
  )
}

export default App

