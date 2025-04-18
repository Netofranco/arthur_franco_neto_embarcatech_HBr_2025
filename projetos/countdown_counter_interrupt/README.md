Unidade 1  - Contador regressivo

- 🎯 Objetivo do projeto

  Fazer um contador regressivo de 10 segundos acionado pelo Botão A, durante a contagem regressiva o programa deve contabilizar a quantidade de vezes que o Botão B é pressionado. A contagem regressiva e a quantidade de acionamentos do Botão B deve ser exibidos no display oled.

  

- 🔧 Componentes usados

  2 Botões (A e B)

  Display SSD 1306

  

- 💾 Como compilar e executar o código

  

  

- ⚡ Pinagem dos dispositivos utilizados

  GPIO 5 - Botão A

  GPIO 6 - Botão B

  GPIO 14 - SDA I2C Display Oled

  GPIO 15 - SCL I2C Display Oled

  

- 🖼️ Imagens e/ou vídeos do projeto em funcionamento (se possível)

  

- 📈 Resultados esperados ou obtidos

  Resultado positivo, o sistema inicia ao detectar o Botão A pressionado e faz a contagem decrescente de 9 a 0 enquanto monitora através da interrupção o número de vezes que o Botão B foi pressionado. O resultado é exibido no Display.