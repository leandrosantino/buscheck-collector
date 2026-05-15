# Relatório  — Firmware do Leitor RFID (BusCheck Collector)

## 1. Objetivo Geral

O firmware implementa um sistema de leitura de cartões RFID que captura o identificador único de cada cartão e o envia para um servidor central via MQTT. O sistema é controlado por um ESP8266 e tem como finalidade o registro de embarque ou presença em um contexto de transporte veicular.

---

## 2. Bibliotecas Utilizadas

O código utiliza quatro bibliotecas: **ESP8266WiFi.h** para conexão Wi-Fi, **PubSubClient.h** para comunicação MQTT, **SPI.h** para comunicação com o módulo RFID, e **MFRC522.h** como driver do leitor de cartões.

---

## 3. Funcionamento do Sistema

### 3.1 Inicialização

Ao ser ligado, o sistema primeiro se conecta à rede Wi-Fi. Durante esse processo, um LED indicador pisca para sinalizar que a conexão está sendo estabelecida. Quando conectado, o LED permanece aceso indicando que o sistema está operacional.

Em seguida, o sistema estabelece a conexão com o servidor MQTT, que é o destino dos dados coletados. Caso a conexão falhe, o sistema tenta novamente até obter sucesso.

Por fim, o leitor RFID é inicializado e o sistema entra em modo de espera, aguardando a aproximação de cartões.

### 3.2 Leitura de Cartões

Quando um cartão RFID é aproximado do leitor, o sistema detecta sua presença e faz a leitura do seu identificador único (UID). Nesse momento, dois sinais são acionados simultaneamente: um buzzer emite um som curto e um LED acende, ambos indicando visual e sonoramente que a leitura foi realizada com sucesso.

### 3.3 Envio dos Dados

Após a leitura, o UID do cartão é formatado como uma string hexadecimal e enviado imediatamente para o servidor MQTT no tópico `sensor/UND001/rfid`. O sistema então aguarda 2 segundos antes de estar pronto para uma nova leitura.

### 3.4 Indicadores de Estado

O sistema conta com dois LEDs para comunicação visual com o usuário:

- **LED de operação (D2)**: indica que o sistema está ligado e conectado à rede. Fica piscando durante a conexão Wi-Fi e aceso quando conectado.
- **LED de leitura (D3)**: acende momentaneamente sempre que um cartão é lido, dando retorno visual imediato ao usuário.

O buzzer complementa a sinalização emitindo um bipe curto no momento da leitura do cartão.

---

