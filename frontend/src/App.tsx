import { Routes, Route, Link, useLocation } from 'react-router'

import Accueil from './pages/Accueil'
import Catalogue from './pages/Catalogue'
import Jeu from './pages/Jeu'

function App() {
  const classeLien = 'rounded-full border border-[#cfe0d0]/30 bg-[#2a4d3f]/60 px-5 py-2 font-titre text-lg font-bold text-[#f2efe4] hover:bg-[#2a4d3f]/85'

  return (
    <div className="h-screen flex flex-col overflow-hidden bg-green-200">
      <nav className="flex shrink-0 gap-2 p-2 bg-[#41705b]">
        <Link to="/" className={classeLien}>Accueil</Link>
        <Link to="/catalogue" className={classeLien}>Catalogue</Link>
        <Link to="/jeu" className={classeLien}>Jeu</Link>
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

