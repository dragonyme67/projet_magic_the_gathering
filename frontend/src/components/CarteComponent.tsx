import type { Carte } from "../interfaces/Carte"
import { useState } from "react"

function CarteComponent({ nom, image }: Carte) {
  const [survolee, setSurvolee] = useState(false);

  return (
    <>
      <div
        className="flex h-full w-full min-h-0 min-w-0 overflow-hidden items-center justify-center">
        <img
          src={image}
          alt={nom}
          className="block h-full object-contain rounded-2xl cursor-pointer"
          onMouseEnter={() => setSurvolee(true)}
          onMouseLeave={() => setSurvolee(false)}
        />
      </div>
      {
        survolee && (
          <div className="fixed inset-0 z-50 flex
                                  items-center justify-center
                                  pointer-events-none">
            <img
              src={image}
              alt={nom}
              className="w-80 shadow-2xl rounded-xl"
            />
          </div>
        )
      }
    </>
  );
}

export default CarteComponent