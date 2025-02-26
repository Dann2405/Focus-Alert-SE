
# Focus Alert SE

## Descrição do Projeto

O **Focus Alert SE** é um sistema embarcado inovador desenvolvido com a placa BitDogLab (baseada no Raspberry Pi Pico W) que atua como um alarme programável para lembretes de medicação e também como uma ferramenta de gerenciamento de tempod. Projetado para auxiliar idosos e pessoas com alzheimer que precisam manter uma rotina bem definida, garantindo que a administração de medicamentos e a gestão do tempo ocorram de forma simples e confiável. O Focus Alert SE oferece uma interface física intuitiva e limpa, utilizando um display OLED SSD1306 para exibição de informações e dois buzzers para emissão de alarmes sonoros intensos.

## Objetivos

- **Notificação de Medicação:**  
  Fornecer lembretes de medicação de forma confiável, utilizando buzzers que geram um som forte e claro, aliado a um display que exibe a configuração do alarme.

- **Gestão de Tempo e Produtividade:**  
  Permitir o uso do dispositivo como, por exemplo, um timer Pomodoro, auxiliando o usuário a organizar sua rotina e reduzir distrações, promovendo foco e eficiência.

- **Integração de Múltiplas Funcionalidades:**  
  Demonstrar o potencial do RP2040 integrando a leitura do ADC (para o joystick) e controle via PWM (para os buzzers), tudo em um sistema compacto e de baixo custo.

- **Facilidade de Uso:**  
  Interface simples e robusta, que permite a configuração e a visualização das informações sem complicações.

## Componentes Utilizados

- **Placa de Desenvolvimento:**  
  BitDogLab com Raspberry Pi Pico W

- **Display OLED:**  
  SSD1306 128x64, conectado via I2C (SDA: GPIO 14, SCL: GPIO 15)

- **Buzzers:**  
  Dois buzzers "80dB Externally Driven Magnetic 2.7kHz SMD, 8.5x8.5mm" – para emitir alarmes sonoros intensos

- **Joystick Analógico:**  
  Conectado aos pinos GPIO 26 (VRY) e GPIO 27 (VRX), utilizado para programar o alarme

- **Botões:**  
  Botão A (GPIO 5) e Botão B (GPIO 6) – para iniciar/cancelar o alarme

- **LED RGB:**  
  Utilizado para fornecer feedback visual durante a operação do alarme

## Estrutura do Projeto

O código-fonte está organizado em módulos para facilitar a manutenção e expansão do sistema:

- **config.h:**  
  Contém as configurações de inicialização e definições dos pinos.

- **alarm.h / alarm.c:**  
  Gerenciam o sistema de alarme, contagem regressiva e transição entre os estados (programação, contagem e alarme), além do tratamento dos sensores (ADC para o joystick) e dos botões com debouncing.

- **ssd1306.h, interface.h, interface.c:**  
  Implementam o driver e as funções de interface para o display OLED, exibindo textos e gráficos relevantes para a operação do sistema.

- **biblioteca.h:**  
  Responsável por incluir todas as bibliotecas necessárias para a integração dos módulos.

- **project.c:**  
  O arquivo principal que integra todos os módulos e gerencia o loop de execução e a lógica de estados.

## Instruções de Uso

### 1. Clonando o Repositório

Abra o terminal e execute:

```bash
git clone https://github.com/Dann2405/Focus-Alert-SE.git
cd Focus-Alert-SE
```

### 2. Configurando o Ambiente de Desenvolvimento

- **Microcontrolador:** Raspberry Pi Pico W (utilizado na BitDogLab)  
- **SDK do RP2040:** Certifique-se de que o Pico SDK esteja instalado e configurado corretamente no VS Code.  
- **Extensões para VS Code:**
  - C/C++ (Microsoft)
  - CMake Tools
  - [Raspberry Pi Pico SDK Extension](https://marketplace.visualstudio.com/items?itemName=raspberry-pi.raspberry-pi-pico)

### 3. Compilando o Projeto

Utilize a interface de compilação do SDK do Raspberry Pi integrado ao VS Code para compilar o projeto. O procedimento gerará o firmware (arquivo `.uf2`) para upload.

### 4. Upload e Teste no Hardware

- **Conecte a Placa:**  
  Coloque o Raspberry Pi Pico W no modo BOOTSEL e conecte via USB.
- **Upload do Firmware:**  
  Faça o upload do arquivo `.uf2` gerado para a placa utilizando o método de sua preferência (arrastando o arquivo para a unidade BOOTSEL ou usando o Picotool).
- **Operação:**  
  Configure o alarme por meio do joystick e dos botões. O display mostrará as configurações, e, na hora de disparar o alarme, os buzzers emitirão um som forte (aproximadamente 4000 Hz, conforme configurado) enquanto o LED indica que o alarme esta sendo acionado.

## Funcionamento do Projeto

1. **Configuração e Programação:**  
   O usuário define os horários/duração do alarme utilizando o joystick – o eixo X para selecionar entre horas, minutos e segundos e o eixo Y para ajustar os valores. O display OLED apresenta as configurações de forma clara, destacando o campo selecionado.

2. **Contagem Regressiva / Timer:**  
   Após a configuração, o sistema inicia a contagem regressiva, atualizando o display com o tempo restante e mantendo uma interface visual dinâmica.

3. **Ativação do Alarme:**  
   Quando o tempo se esgota, o estado ALARM é ativado. Os buzzers (controlados por PWM) geram um som forte e consistente (aproximadamente 4000 Hz), enquanto o LED fornecem feedback visual. O alarme pode ser interrompido pelo botão A.

4. **Versatilidade:**  
   O mesmo hardware e software permitem que o dispositivo seja utilizado tanto para lembretes de medicação quanto para gestão de tempo, demonstrando a flexibilidade do sistema para diversas aplicações práticas.

## Considerações Finais

O **Focus Alert SE** demonstra a capacidade dos sistemas embarcados modernos de integrar múltiplas funcionalidades – desde a leitura de sensores (ADC para o joystick) até o controle por PWM dos buzzers – em um único dispositivo de baixo custo e alta eficiência. Este projeto consolida os conhecimentos adquiridos na capacitação e apresenta uma solução inovadora e prática para gerenciamento de tempo e lembretes de medicação, com foco na simplicidade e robustez da interface.

## Vídeo de Demonstração

Click [AQUI](https://drive.google.com/file/d/1MmiYwVj4-TfJyEAQABbOM3DRPEnRp2ho/view?usp=sharing) para acessar o video de demonstração.

## Licença

Este projeto é licenciado sob os termos da [MIT License](https://opensource.org/license/mit).
