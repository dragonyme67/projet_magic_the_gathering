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
            <div className="w-64 rotate-1 rounded-[3px_8px_4px_9px] border-2 border-[#6b4b32] bg-[#eee2c3] p-4 text-sm leading-relaxed text-[#35281a] shadow-[5px_5px_0_rgba(0,0,0,0.45)]">
              <p className="mb-2 border-b-2 border-dashed border-[#6b4b32]/50 pb-1 font-titre text-lg font-bold text-[#4a2f18]">{nom}</p>
              <p className="whitespace-pre-line">{description}</p>
            </div>
          </div>
        )
      }
    </>
  );
}

export default CarteComponent