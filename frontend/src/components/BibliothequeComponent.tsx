interface BibliothequeComponent {
    nb_cartes: number;
}

function BibliothequeComponent({ nb_cartes }: BibliothequeComponent) {
  return (
    <div className="h-full w-full min-h-0 min-w-0 overflow-hidden">
      <img
        src="https://media.wizards.com/2022/images/daily/en_7C0yERSDK6.png"
        alt={`${nb_cartes} cartes restantes dans la pioche)`}
        className="block h-full w-full object-contain rounded-2xl"
      />
    </div>
  );
}

export default BibliothequeComponent