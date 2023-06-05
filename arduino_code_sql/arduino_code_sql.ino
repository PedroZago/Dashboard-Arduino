#include "DHT.h"  // Inclui a biblioteca DHT
#include <WiFi.h> // Inclui a biblioteca WiFi

#define LDR_pin 34 // Pino do sensor de luminosidade (LDR)

#define DHT_pin 4           // Pino do sensor DHT
#define DHT_type DHT22      // Tipo de sensor DHT 22)
DHT dht(DHT_pin, DHT_type); // Criação de uma instância do objeto DHT

int luminosity = 0; // Variável para armazenar a luminosidade

int LDR_pin_led = 25;   // Pino do LED indicador de luminosidade
int DHT_error_led = 26; // Pino do LED indicador de erro do sensor DHT

const char *ssid = "";     // Nome da rede WiFi
const char *password = ""; // Senha da rede WiFi
const char *host = "";     // Endereço do host para conexão

void setup()
{
  Serial.begin(9600); // Inicia a comunicação serial

  dht.begin(); // Inicializa o sensor DHT

  pinMode(LDR_pin_led, OUTPUT);   // Define o pino do LED indicador de luminosidade como saída
  pinMode(DHT_error_led, OUTPUT); // Define o pino do LED indicador de erro do sensor DHT como saída

  Serial.println();
  Serial.println();
  Serial.print("Conectando à rede "); // Imprime mensagem indicando a tentativa de conexão à redeF
  Serial.println(ssid);               // Imprime o nome da rede WiFi

  WiFi.begin(ssid, password); // Inicia a conexão WiFi

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi conectado"); // Imprime mensagem indicando que a conexão WiFi foi estabelecida
  Serial.println("Endereço IP: ");
  Serial.println(WiFi.localIP()); // Imprime o endereço IP atribuído ao dispositivo
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

    Serial.print("Conectando a ");
    Serial.println(host); // Imprime o endereço do host para conexão

    WiFiClient client;
    const int httpPort = 80;
    if (!client.connect(host, httpPort))
    {                                     // Realiza a conexão com o host
      Serial.println("Falha na conexão"); // Imprime mensagem indicando falha na conexão com o host
      return;
    }

    client.print(String("GET http://localhost/dashboard_arduino/connect.php?") + ("&temperature=") + temperature + ("&humidity=") + humidity + ("&luminosity=") + luminosity + " HTTP/1.1\r\n" + "Host: " + host + "\r\n" + "Conexão: fechada\r\n\r\n");
    unsigned long timeout = millis();
    while (client.available() == 0)
    {
      if (millis() - timeout > 1000)
      {
        Serial.println(">>> Timeout no cliente !"); // Imprime mensagem indicando timeout no cliente
        client.stop();
        return;
      }
    }

    while (client.available())
    {
      String line = client.readStringUntil('\r');
      Serial.print(line);
    }

    Serial.println();
    Serial.println("Fechando conexão");
  }

  delay(3000);
}
