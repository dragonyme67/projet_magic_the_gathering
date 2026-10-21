interface BibliothequeComponent {
    nb_cartes: number;
}

function BibliothequeComponent({ nb_cartes }: BibliothequeComponent) {
  return (
    <div className="relative h-full w-full min-h-0 min-w-0 overflow-hidden">
      <img
        src="https://media.wizards.com/2022/images/daily/en_7C0yERSDK6.png"
        alt={`${nb_cartes} cartes restantes dans la pioche)`}
        className="block h-full w-full object-contain rounded-2xl"
      />
      <span className="pointer-events-none absolute inset-x-0 bottom-2 mx-auto w-fit min-w-8 rounded-full bg-[#2a4d3f]/90 px-2 py-0.5 text-center text-sm font-bold text-[#e4d194] ring-1 ring-[#d8c27c]/50">
        {nb_cartes}
      </span>
    </div>
  );
}

export default BibliothequeComponent
