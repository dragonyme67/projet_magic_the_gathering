interface CarteComponentProps {
  nom: string;
  image: string;
}


function CarteComponent({ nom, image }: CarteComponentProps) {
  return (
    <div className="text-center">
      <img
        src={image}
        alt={nom}
        className="mx-auto w-full max-w-[200px]"
      />
      <p className="font-semibold">{nom}</p>
    </div>
  );
}

export default CarteComponent