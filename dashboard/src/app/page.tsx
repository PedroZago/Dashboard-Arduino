"use client";

import { api } from "@src/services/api";
import { useEffect, useMemo, useState } from "react";
import { QueryProps } from "./api/arduino/route";
import Header from "@src/components/Header";
import Cards from "@src/components/Cards";
import Loader from "@src/components/Loader";

import { format } from "date-fns";

import ptBR from "date-fns/locale/pt-BR";
import ChartLine from "@src/components/ChartLine";

interface ArduinoDataResponse {
  result: QueryProps[];
}

export default function Home() {
  const [data, setData] = useState<QueryProps[]>([]);
  const [lastRegisters, setLastRegisters] = useState<QueryProps>(
    {} as QueryProps
  );
  const [loading, setLoading] = useState(false);

  useEffect(() => {
    const queryData = async () => {
      try {
        const response = await api.get<ArduinoDataResponse>("/arduino");

        setData(response.data.result);
        setLastRegisters(response.data.result[response.data.result.length - 1]);

        setLoading(true);
      } catch (error) {
        console.log(error);
      }
    };

    const interval = setInterval(queryData, 10000);

    return () => {
      clearInterval(interval);
    };
  }, []);

  const usedRegisters = 30;

  const lastLuminosityRegister = useMemo(() => {
    const luminosityPercent = (100 * lastRegisters.luminosity ?? 0) / 4095;
    return `${luminosityPercent?.toFixed(2)}%`;
  }, [lastRegisters.luminosity]);

  const lastTemperatureRegister = useMemo(() => {
    return `${lastRegisters.temperature?.toFixed(2)}°C`;
  }, [lastRegisters.temperature]);

  const lastHumidityRegister = useMemo(() => {
    return `${lastRegisters.humidity?.toFixed(2)}%`;
  }, [lastRegisters.humidity]);

  const chartValues = useMemo(() => {
    const lastTenRegisters = data.slice(usedRegisters * -1);

    const created_at = lastTenRegisters.map((item) =>
      format(new Date(item.created_at), "dd/MM/yyyy HH:mm", {
        locale: ptBR,
      })
    );
    const humidity = lastTenRegisters.map((item) => item.humidity);
    const luminosity = lastTenRegisters.map((item) => {
      return (100 * item.luminosity ?? 0) / 4095;
    });
    const temperature = lastTenRegisters.map((item) => item.temperature);

    return { created_at, humidity, luminosity, temperature };
  }, [data]);

  return (
    <div className="min-h-full">
      <Header />

      <main>
        <div className="mx-auto max-w-7xl py-6 sm:px-6 lg:px-8">
          {loading ? (
            <>
              <div className="mx-auto mt-10 grid max-w-2xl grid-cols-1 gap-x-8 gap-y-16 pt-10 sm:mt-16 sm:pt-16 lg:mx-0 lg:max-w-none lg:grid-cols-3">
                <Cards label="Luminosidade" value={lastLuminosityRegister} />

                <Cards label="Temperatura" value={lastTemperatureRegister} />

                <Cards label="Humidade do ar" value={lastHumidityRegister} />

                <ChartLine
                  data={chartValues.luminosity}
                  labels={chartValues.created_at}
                  title="Luminosidade"
                />

                <ChartLine
                  data={chartValues.temperature}
                  labels={chartValues.created_at}
                  title="Temperatura"
                />

                <ChartLine
                  data={chartValues.humidity}
                  labels={chartValues.created_at}
                  title="Humidade do ar"
                />
              </div>

              <div className="mx-auto mt-10">
                <p className="mt-5 line-clamp-3 text-lg leading-6 text-gray-600 text-center">
                  Atualizado em:{" "}
                  {format(new Date(), "dd/MM/yyyy HH:mm", {
                    locale: ptBR,
                  })}{" "}
                  - Últimos {usedRegisters} registros
                </p>
              </div>
            </>
          ) : (
            <div className="flex justify-center content-center">
              <Loader />
            </div>
          )}
        </div>
      </main>
    </div>
  );
}
