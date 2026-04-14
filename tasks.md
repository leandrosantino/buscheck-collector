# Tarefas da Equipe - BusCheck Collector

## Distribuição de Responsabilidades

### Membro 1: Líder do Projeto / Coordenador

**Responsabilidades principais:**
- Coordenação geral da equipe
- Gestão do cronograma e acompanhamento de prazos
- Comunicação com stakeholders
- Garantia da qualidade do projeto

**Tarefas específicas:**
1. **Gestão do projeto (Semana 1-16)**
   - Criar e manter cronograma detalhado
   - Realizar reuniões semanais de acompanhamento
   - Distribuir e acompanhar tarefas da equipe
   - Gerenciar riscos e problemas

2. **Documentação (Semana 1-16)**
   - Elaborar relatório de extensão conforme estrutura definida
   - Documentar decisões técnicas importantes
   - Preparar apresentações para acompanhamento

3. **Integração (Semana 10-15)**
   - Coordenar testes de integração entre hardware, firmware e software
   - Validar funcionamento completo do sistema
   - Preparar demonstração final

### Membro 2: Especialista em Hardware/Eletrônica

**Responsabilidades principais:**
- Projeto e desenvolvimento do circuito eletrônico
- Seleção de componentes
- Montagem e testes do hardware

**Tarefas específicas:**
1. **Projeto de eletrônica (Semana 1-4)**
   - Desenvolver esquemático do circuito
   - Selecionar componentes (ESP8266, MFRC522, buzzer, resistores, capacitores)
   - Projetar layout da PCB (se aplicável)
   - Lista de materiais (BOM)

2. **Montagem do protótipo (Semana 5-6)**
   - Montar protótipo em breadboard
   - Testar conexões e funcionamento básico
   - Solucionar problemas de hardware

3. **Testes de hardware (Semana 7-8)**
   - Testar leitura RFID com diferentes tags
   - Validar comunicação Wi-Fi
   - Testar buzzer em diferentes frequências
   - Medir consumo de energia

### Membro 3: Desenvolvedor de Firmware

**Responsabilidades principais:**
- Desenvolvimento do firmware para ESP8266
- Integração com hardware (RFID, buzzer)
- Comunicação Wi-Fi e MQTT

**Tarefas específicas:**
1. **Configuração do ambiente (Semana 1-2)**
   - Configurar PlatformIO/Arduino IDE
   - Configurar bibliotecas (MFRC522, WiFi, MQTT)
   - Criar estrutura básica do projeto

2. **Desenvolvimento do firmware (Semana 3-8)**
   - Implementar leitura de tags RFID (RF01)
   - Desenvolver sistema de configuração via serial (RF06)
   - Implementar conexão Wi-Fi com reconexão automática (RF02)
   - Integrar com broker MQTT (RF03)
   - Implementar feedback sonoro (RF05)
   - Adicionar monitoramento de saúde (RF07)

3. **Testes do firmware (Semana 9-10)**
   - Testes unitários de cada funcionalidade
   - Testes de integração com hardware
   - Testes de estabilidade de conexão
   - Otimização de consumo de energia

### Membro 4: Desenvolvedor Backend (Python)

**Responsabilidades principais:**
- Desenvolvimento do worker Python
- Integração com MQTT e banco de dados
- Processamento de dados

**Tarefas específicas:**
1. **Configuração do ambiente (Semana 3-4)**
   - Configurar ambiente Python 3.x
   - Instalar dependências (paho-mqtt, psycopg2)
   - Configurar conexão com banco de dados

2. **Desenvolvimento do worker (Semana 5-10)**
   - Implementar assinatura MQTT (RF04)
   - Desenvolver parser e validador de mensagens
   - Implementar persistência no banco de dados
   - Adicionar sistema de logs
   - Criar scripts de monitoramento

3. **Testes e validação (Semana 11-12)**
   - Testar processamento de mensagens MQTT
   - Validar integridade dos dados no banco
   - Testar performance com múltiplas leituras
   - Implementar tratamento de erros

### Membro 5: Especialista em Infraestrutura/Testes

**Responsabilidades principais:**
- Configuração do servidor MQTT
- Testes de integração
- Documentação técnica

**Tarefas específicas:**
1. **Configuração do servidor MQTT (Semana 3-5)**
   - Instalar e configurar broker Mosquitto
   - Configurar segurança (usuários, senhas, ACL)
   - Definir estrutura de tópicos
   - Configurar persistência de mensagens

2. **Testes de integração (Semana 9-13)**
   - Testar comunicação completa (hardware → MQTT → worker → BD)
   - Testar cenários de falha (Wi-Fi, MQTT, banco)
   - Validar QoS e entrega de mensagens
   - Testar com múltiplos dispositivos simulados

3. **Documentação técnica (Semana 14-15)**
   - Documentar configuração do servidor MQTT
   - Criar manual de instalação e configuração
   - Documentar procedimentos de troubleshooting
   - Preparar evidências para relatório

## Cronograma Semanal

### Semanas 1-4: Planejamento e Início
- **Todos**: Reunião de kickoff, definição de escopo
- **M2**: Projeto de eletrônica
- **M3**: Configuração ambiente firmware
- **M4**: Configuração ambiente Python
- **M5**: Configuração servidor MQTT

### Semanas 5-8: Desenvolvimento Principal
- **M2**: Montagem protótipo
- **M3**: Desenvolvimento firmware principal
- **M4**: Desenvolvimento worker principal
- **M5**: Testes iniciais de infraestrutura

### Semanas 9-12: Integração e Testes
- **Todos**: Testes de integração
- **M3**: Testes e otimização firmware
- **M4**: Testes e validação worker
- **M5**: Testes de sistema completo

### Semanas 13-16: Finalização e Documentação
- **M1**: Elaboração relatório final
- **Todos**: Preparação demonstração
- **M5**: Documentação técnica completa
- **Todos**: Revisão final e entrega

## Critérios de Aceitação por Tarefa

### Hardware (Membro 2)
- [ ] Circuito funciona com todos os componentes conectados
- [ ] Leitura RFID funciona com 95%+ de acurácia
- [ ] Buzzer emite sons distintos para sucesso/falha
- [ ] Consumo de energia dentro dos limites especificados

### Firmware (Membro 3)
- [ ] Conecta-se a Wi-Fi configurada
- [ ] Publica leituras RFID no tópico correto
- [ ] Reconecta automaticamente em caso de falha
- [ ] Sistema de configuração via serial funcional

### Backend (Membro 4)
- [ ] Worker assina tópicos MQTT corretamente
- [ ] Processa e valida 100% das mensagens recebidas
- [ ] Persiste dados no banco com integridade
- [ ] Sistema de logs registra todas as operações

### Infraestrutura (Membro 5)
- [ ] Servidor MQTT configurado e seguro
- [ ] Sistema suporta 100+ mensagens por minuto
- [ ] Documentação de configuração completa
- [ ] Procedimentos de backup e recuperação

### Coordenação (Membro 1)
- [ ] Cronograma atualizado semanalmente
- [ ] Todas as reuniões documentadas
- [ ] Relatório de extensão completo
- [ ] Demonstração final preparada

## Reuniões e Comunicação

- **Reunião diária rápida**: 15 minutos, status de cada membro
- **Reunião semanal**: 1 hora, revisão de progresso, planejamento da semana
- **Reunião de revisão**: A cada 2 semanas, com demonstração de funcionalidades
- **Canal de comunicação**: Discord/WhatsApp para comunicação rápida
- **Repositório**: GitHub para versionamento e issues

## Riscos e Mitigação

1. **Atraso no hardware**: Membro 2 tem componentes sobressalentes
2. **Problemas de conexão Wi-Fi**: Membro 3 implementa fallback para AP mode
3. **Falhas no servidor MQTT**: Membro 5 configura monitoramento e alertas
4. **Integração complexa**: Testes incrementais desde a semana 9
5. **Problemas no banco de dados**: Membro 4 implementa retry logic e queue

*Documento atualizado em: 13/04/2026*