#include "DHT.h" // Inclui a biblioteca DHT

#define LDR_pin 34 // Pino do sensor de luminosidade (LDR)

#define DHT_pin 4           // Pino do sensor DHT
#define DHT_type DHT22      // Tipo de sensor DHT (DHT22)
DHT dht(DHT_pin, DHT_type); // Criação de uma instância do objeto DHT

int luminosity = 0; // Variável para armazenar a luminosidade

int LDR_pin_led = 25;   // Pino do LED indicador de luminosidade
int DHT_error_led = 26; // Pino do LED indicador de erro do sensor DHT

void setup()
{
  Serial.begin(9600); // Inicia a comunicação serial

  dht.begin(); // Inicializa o sensor DHT

  pinMode(LDR_pin_led, OUTPUT);   // Define o pino do LED indicador de luminosidade como saída
  pinMode(DHT_error_led, OUTPUT); // Define o pino do LED indicador de erro do sensor DHT como saída
}

void loop()
{
  luminosity = analogRead(LDR_pin); // Lê o valor de luminosidade do sensor LDR

  if (luminosity < 100)
  {
    digitalWrite(LDR_pin_led, HIGH); // Acende o LED indicador de luminosidade
  }
  else
  {
    digitalWrite(LDR_pin_led, LOW); // Apaga o LED indicador de luminosidade
  }

  float temperature = dht.readTemperature(); // Lê a temperatura do sensor DHT
  float humidity = dht.readHumidity();       // Lê a umidade do sensor DHT

  if (isnan(temperature) || isnan(humidity))
  {                                                     // Veridica se temperature e humidity são números
    Serial.println("Falha na comunicação com o DHT22"); // Imprime mensagem indicando falha na comunicação com o sensor DHT

    digitalWrite(DHT_error_led, HIGH); // Acende o LED indicador de erro do sensor DHT
    delay(1000);

    digitalWrite(DHT_error_led, LOW); // Apaga o LED indicador de erro do sensor DHT
  }
  else
  {
    Serial.print("Luminosidade do ambiente: ");
    Serial.print(luminosity);
    Serial.print(" - Umidade: ");
    Serial.print(humidity);
    Serial.print("%");
    Serial.print(" - Temperatura: ");
    Serial.print(temperature);
    Serial.println(" *C");
  }

  delay(3000);
}
