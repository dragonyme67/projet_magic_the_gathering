import { Routes, Route, Link } from 'react-router'

import Accueil from './pages/Accueil'
import Catalogue from './pages/Catalogue'

function App() {
  return (
    <div className="min-h-screen p-6 bg-green-200">
      <nav className="mb-6 flex gap-4 bg-red-4e00 p-4 rounded-lg">
        <Link to="/" className='btn-primary'>Accueil</Link>
        <Link to="/catalogue" className='btn-primary'>Catalogue</Link>
      </nav>

      <Routes>
        <Route path="/" element={<Accueil />} />
        <Route path="/catalogue" element={<Catalogue />} />
      </Routes>
    </div>
  )
}

export default App

