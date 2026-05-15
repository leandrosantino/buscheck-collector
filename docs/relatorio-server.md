# Servidor para Coleta de Dados (BusCheck Collector)

## 1. Objetivo Geral

O servidor implementa um subscriber MQTT que escuta mensagens enviadas por leitores RFID, processa os dados recebidos e os persiste em um banco Supabase. O sistema tem como finalidade registrar eventos de embarque ou presença em um contexto de transporte veicular, associando cada leitura de cartão a um veículo específico.

---

## 2. Bibliotecas Utilizadas

O código utiliza quatro bibliotecas: **paho.mqtt.client** para comunicação MQTT, **supabase** como cliente do banco de dados, **dotenv** para carregar variáveis de ambiente, e **os** para acessar configurações do ambiente.

---

## 3. Funcionamento do Sistema

### 3.1 Inicialização

Ao ser iniciado, o servidor carrega as variáveis de ambiente do arquivo `.env`, que contém as credenciais de acesso ao Supabase e as configurações do broker MQTT (endereço, porta e tópico). O tópico inscrito usa o padrão `sensor/+/rfid`, onde o curinga `+` captura qualquer identificador de dispositivo.

Em seguida, o cliente MQTT é configurado com as funções de callback para conexão e recebimento de mensagens, e então conecta-se ao broker.

### 3.2 Conexão MQTT

Quando a conexão com o broker é estabelecida, o callback `on_connect` é acionado. Em caso de falha, uma mensagem de erro é exibida. Em caso de sucesso, o servidor confirma a conexão e inscreve-se no tópico configurado, passando a escutar mensagens dos leitores RFID.

### 3.3 Recebimento e Processamento de Dados

Cada mensagem recebida no tópico MQTT contém o UID do cartão RFID lido por um dos módulos. O servidor extrai o identificador do dispositivo a partir do tópico (segundo segmento) e o UID do cartão a partir do payload da mensagem.

Com esses dados, o servidor insere um registro na tabela `check_in` do Supabase contendo:
- **codigo_veiculo**: identificador do dispositivo que fez a leitura
- **codigo_cartao**: UID do cartão RFID lido

### 3.4 Registro de Operação

O servidor mantém um log simplificado no terminal:
- Mensagens recebidas são exibidas com o dispositivo de origem e o conteúdo
- Inserções bem-sucedidas no banco são confirmadas
- Falhas de inserção são sinalizadas para alertar sobre problemas de conectividade ou banco

---

## 4. Fluxo de Dados

```
Leitor RFID → MQTT (sensor/UND001/rfid) → Servidor Python → Supabase (tabela check_in)
```
