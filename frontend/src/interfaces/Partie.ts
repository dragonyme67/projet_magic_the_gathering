import type { Carte } from "./Carte"

export interface CarteEnJeu extends Carte {
  id: number
  engage: boolean
}

export interface JoueurPartie {
  nom: string
  vie: number
  bibliotheque: number
  main: CarteEnJeu[]
  champ_de_bataille: CarteEnJeu[]
  cimetiere: CarteEnJeu[]
  exil: CarteEnJeu[]
}

export interface Partie {
  graine: number
  tour: number
  joueurActif: number
  joueurs: JoueurPartie[]
}
