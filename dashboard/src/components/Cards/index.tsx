"use client";

interface CardsProps {
  label: string;
  value: string;
}

export default function Cards({ label, value }: CardsProps) {
  return (
    <article className="flex max-w-xl flex-col items-center justify-between">
      <div className="group relative">
        <h3 className="mt-3 text-2xl font-semibold leading-6 text-gray-900 group-hover:text-gray-600 text-center">
          {label}
        </h3>
        <p className="mt-5 line-clamp-3 text-xl leading-6 text-gray-600 text-center">
          {value}
        </p>
      </div>
    </article>
  );
}
