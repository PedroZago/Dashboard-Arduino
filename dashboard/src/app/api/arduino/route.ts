import { executeQuery } from "@src/lib/db";
import { NextApiRequest, NextApiResponse } from "next";
import { NextResponse } from "next/server";

export interface QueryProps {
  id: number;
  temperature: number;
  humidity: number;
  created_at: Date;
  luminosity: number;
}

export async function GET(_req: NextApiRequest, _res: NextApiResponse) {
  const query = "SELECT * FROM monitoring_data";

  const result = (await executeQuery({ query })) as QueryProps[];

  return NextResponse.json({ result });
}
