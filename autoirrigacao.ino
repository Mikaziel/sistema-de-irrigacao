#include "dht.h" //Biblioteca para o sensor dht11 (umidade e temperatura do ambiente)
#include <SoftwareSerial.h> //Biblioteca para comunicação serial em pinos digitais (HC-05, entre outros)

//definir pinos do dht11
#define dht_apin A1 //Pino analógico 1
//definir pinos do sensor de umidade do solo
#define entradaDigital 13 //Pino digital 13
#define entradaAnalogica 0 //Pino analógico 0

int led1 = 6; //Pino do led vermelho
int led2 = 7; //Pino do led verde
int ledr = 3; // Pino do relé
int dSensor; //Variável que vai armazenar a leitura do sensor digital
int aSensor; //Variável que vai armazenar a leitura do sensor analógico
dht DHT; //Objeto DHT que vai permitir acessar funções da biblioteca

//Configuração do SoftwareSerial para o HC-05
SoftwareSerial BluetSerial(10, 11); //Pino RX no digital 10 e pino TX no digital 11

void setup() {
  Serial.begin(9600); //Iniciar a comunicação serial com a taxa de 9600 bps
  BluetSerial.begin(9600); //Iniciar a comunicação Bluetooth com a taxa de 9600 bps
  delay(500); //Aguarda 500 ms

  pinMode(entradaDigital, INPUT); //Define o pino digital do sensor de umidade do solo como entrada
  pinMode(ledr, OUTPUT); //Define o pino do relé como saída
  pinMode(led1, OUTPUT); //Define o pino do led vermelho como saída
  pinMode(led2, OUTPUT); //Define o pino do led verde como saída

  digitalWrite(ledr, HIGH); //Para que o relé comece desligado

//Mensagens de introdução e inicialização do sistema
//Envia para o terminal Bluetooth
  BluetSerial.println("Projeto automatico de irrigação de uma planta \n\n");
  BluetSerial.println("DHT11 e Sensor de Umidade do Solo \n\n");
//Envia para o monitor serial
  Serial.println("Projeto automatico de irrigação de uma planta \n\n");
  Serial.println("DHT11 e Sensor de Umidade do Solo \n\n");
  delay(1000);
}

void loop() {
   //Lê os valores dos pinos digital e analog do sensor de umidade do solo
  dSensor = digitalRead(entradaDigital);
  aSensor = analogRead(entradaAnalogica);
   //Lê os dados do sensor DHT11
  DHT.read11(dht_apin);

  //Exibir as leituras no monitor serial
  Serial.print("Umidade Atual: ");
  Serial.print(DHT.humidity); //Envia o valor da umidade do DHT
  Serial.print("%\n ");
  Serial.print("Temperatura: ");
  Serial.print(DHT.temperature); //Envia o valor da temperatura do DHT
  Serial.println("C  \n");

  //Enviar os dados via Bluetooth
  if (aSensor <= 600) { //Se a umidade do solo for menor ou igual a 600 (solo úmido)
    BluetSerial.print("Solo umido - Rele DESLIGADO\n"); //Envia o estado do solo e do relé para o terminal Bluetooth
  }
  else if (aSensor > 600) { //Se a umidade do solo for maior que 600 (solo praticamente seco)
    BluetSerial.print("Solo seco - Rele LIGADO\n");
  }
  //Enviar os dados do dht11 para o terminal Bluetooth
  BluetSerial.print("Umidade: ");
  BluetSerial.print(DHT.humidity);
  BluetSerial.print("%, Temperatura: ");
  BluetSerial.print(DHT.temperature);
  BluetSerial.println("C");
  delay(5000);

  //Controle dos leds e relé baseado na umidade do solo
  if (aSensor <= 600) {//Se o solo estiver úmido
    Serial.println("Solo umido - Rele DESLIGADO"); //Mensagem para o monitor serial
    digitalWrite(ledr, HIGH); //Desliga o relé
    digitalWrite(led1, HIGH); //Liga LED1 - VERMELHA(indica relé desligado)
    digitalWrite(led2, LOW); //Desligar LED2 - VERDE
  }
  else if (aSensor > 600) { //Se o solo estiver seco
    Serial.println("Solo seco - Rele LIGADO"); //Mensagem para o monitor serial
    digitalWrite(led2, HIGH); //Liga LED2 - VERDE (indica relé ligado)
    digitalWrite(led1, LOW); //Desliga LED1 - VERMELHA
    digitalWrite(ledr, LOW); //Liga o relé (por consequência, liga a bomba submersa)
    delay(5000); //Aguarda 5 segundos para a irrigação
    
    }
}
