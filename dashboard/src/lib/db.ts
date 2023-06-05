import mysql from "serverless-mysql";

interface QueryProps {
  query: string;
  values?: string[];
}

const db = mysql({
  config: {
    host: process.env.DATABASE_HOST,
    database: process.env.DATABASE,
    user: process.env.DATABASE_USER,
    password: process.env.DATABASE_PASSWORD,
  },
});

export async function executeQuery({ query, values }: QueryProps) {
  try {
    const results = await db.query(query, values);
    await db.end();
    return results;
  } catch (error) {
    return { error };
  }
}
