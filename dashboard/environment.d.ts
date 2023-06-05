declare namespace NodeJS {
  export interface ProcessEnv {
    readonly DATABASE_HOST: string;
    readonly DATABASE: string;
    readonly DATABASE_USER: string;
    readonly DATABASE_PASSWORD: string;
  }
}
