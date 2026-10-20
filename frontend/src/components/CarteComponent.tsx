interface CarteComponentProps {
  nom: string;
  image: string;
}


function CarteComponent({ nom, image }: CarteComponentProps) {
  return (
    <div className="h-full w-full min-h-0 min-w-0 overflow-hidden">
      <img
        src={image}
        alt={nom}
        className="block h-full w-full object-contain rounded-2xl"
      />
    </div>
  );
}

export default CarteComponent