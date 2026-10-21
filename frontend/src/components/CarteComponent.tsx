import type { Carte } from "../interfaces/Carte"
import { useState } from "react"

function CarteComponent({ nom, image, description }: Carte) {
  const [survolee, setSurvolee] = useState(false);

  return (
    <>
      <div
        className="flex h-full w-full min-h-0 min-w-0 overflow-hidden items-center justify-center">
        <img
          src={image}
          alt={nom}
          className="block h-full object-contain rounded-2xl cursor-pointer transition duration-150 hover:brightness-110 hover:saturate-125"
          onMouseEnter={() => setSurvolee(true)}
          onMouseLeave={() => setSurvolee(false)}
        />
      </div>
      {
        survolee && (
          <div className="fixed inset-0 z-50 flex
                                  items-center justify-center gap-4
                                  bg-[#10241c]/55 pointer-events-none">
            <img
              src={image}
              alt={nom}
              className="w-64 shadow-2xl rounded-xl ring-1 ring-white/20"
            />
            <div className="w-64 rounded-xl border border-[#d8c27c]/45 bg-[#2a4d3f]/95 p-4 text-sm leading-relaxed text-[#f2efe4] shadow-2xl">
              <p className="mb-2 font-serif text-base font-bold text-[#e4d194]">{nom}</p>
              <p className="whitespace-pre-line">{description}</p>
            </div>
          </div>
        )
      }
    </>
  );
}

export default CarteComponent