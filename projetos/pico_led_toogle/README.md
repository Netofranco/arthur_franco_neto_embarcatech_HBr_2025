Unidade 1  - Led Pico Toogle

- 🎯 Objetivo do projeto

  Fazer o led da raspberry pi pico piscar indefinidamente com intervalo de 1s (500ms On/ 500ms Off). 

  Estruturar o código de maneira modular com pasta de drivers, hal, app e include

  

- 🔧 Componentes usados

  Placa BitDogLab

  

- 💾 Como compilar e executar o código

  Adicionar ao CMakeLists.txt no comando add executable os caminhos dos arquivos e subpastas utilizadas:

  add_executable(pico_led_toogle 
      src/app/main.c
      src/drivers/led_embutido.c
      src/hal/hal_led.c )

  

- ⚡ Pinagem dos dispositivos utilizados

  Pinagem interna da Raspberry Pi Pico W

  

- 🖼️ Imagens e/ou vídeos do projeto em funcionamento (se possível)

  

- 📈 Resultados esperados ou obtidos

  Resultado positivo, o projeto foi estruturado de forma modular e funcionou corretamente