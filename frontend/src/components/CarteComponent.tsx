import type { Carte } from "../interfaces/Carte"


function CarteComponent({ nom, image, mana, description }: Carte) {
  return (
    <div className="h-full w-full min-h-0 min-w-0 overflow-hidden">
      <img
        src={image}
        alt={nom}
        className="block h-full w-full object-contain rounded-2xl"
      />''
      {/* <p className="max-w-full truncate text-center text-sm font-semibold">
        {nom}
      </p> */}
    </div>
  );
}

export default CarteComponent