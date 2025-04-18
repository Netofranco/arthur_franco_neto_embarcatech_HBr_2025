/*Autor: Arthur Franco Neto
Data: 11/04/25

Faça um programa, em linguagem C, que implemente um contador decrescente controlado por interrupção, com o seguinte comportamento:

Toda vez que o Botão A (GPIO5) for pressionado:
O contador decrescente reinicia em 9 e o valor da contagem é mostrado no display OLED.
O sistema entra em modo de contagem regressiva ativa, decrementando o contador de 1 em 1 a cada segundo até chegar em zero.
Durante essa contagem (ou seja, de 9 até 0), o programa deve registrar quantas vezes o Botão B (GPIO6) foi pressionado. O valor deste registro de eventos de botão pressionado também deve ser mostrado no display OLED.
Quando o contador atingir zero, o sistema congela e ignora temporariamente os cliques no Botão B (eles não devem ser acumulados fora do intervalo ativo).
O sistema permanece parado após a contagem, exibindo:
O valor 0 no contador
A quantidade final de cliques no Botão B registrados durante o período de 9 segundo (contagem regressiva)
Somente ao pressionar novamente o Botão A, o processo todo se reinicia:
O contador volta para 9
O número de cliques do Botão B é zerado
A contagem recomeça do início
*/

#include <stdio.h>
#include "pico/stdlib.h"                    
#include "hardware/timer.h"
#include "inc/ssd1306.h"
#include "hardware/i2c.h"
#include "inc/ssd1306_i2c.c"

//Definicao dos GPIOs para Botao
#define BOTAO_A 5
#define BOTAO_B 6

#define I2C_SDA 14
#define I2C_SCL 15

//Declaracao de variaveis de controle para a frequencia que o led ira piscar e qual o tempo total que o led ficara piscando
uint8_t frequencia = 10; //Hz        Colocar a frequencia em Hz que corresponde a quanto vezes o led pisca por segundo
uint8_t tempo_on = 10; //segundos     Colocar a duracao total em que o led ficara piscando

// Variáveis globais voláteis para sincronização com interrupções
volatile int cont_botao_press = 0;      // variavel que conta o numero de vezes que o botao foi pressionado
volatile int count_dec = 0;               // variavel para calcular o periodo em que o led deve ficar aceso e apagado para cada vez que piscar
volatile int count_B = 0;

//Flags globais para identificacao e manipulacao do sistema
volatile bool count_active = false;       // Flag para indicar se o led esta aceso ou apagado (true - ligado / false - desligado)

uint8_t ssd[ssd1306_buffer_length]; //Definicao do tamanho do Buffer
//**********************************************************************************************/
   // Preparar área de renderização para o display (ssd1306_width pixels por ssd1306_n_pages páginas)
   struct render_area frame_area = {
    start_column : 0,
    end_column : ssd1306_width - 1,
    start_page : 0,
    end_page : ssd1306_n_pages - 1
};

//Funcoes para o Display SSD
void zerar_display(){
    // zera o display inteiro
    memset(ssd, 0, ssd1306_buffer_length);
    render_on_display(ssd, &frame_area);
}

int64_t ssd1306_turn_off(){
    uint8_t commands[] = {
    ssd1306_set_display | 0x00,
    };
    ssd1306_send_command_list(commands, count_of(commands));
    return 0;
}
    
void ssd1306_turn_on(){
    uint8_t commands[] = {
    ssd1306_set_display | 0x01,
    };
    ssd1306_send_command_list(commands, count_of(commands));
}

int64_t limpa_erro(){
    int y = 24;
    int x = 0;
    for (int i =0; i < 2; i++){
        for (int j = 0; j < 15; j++){
            ssd1306_draw_char(ssd, x, y, ' ');
            render_on_display(ssd, &frame_area);
            x = x+8;
        }
        x = 0;
        y = y+8;
    }
    return 0;
}

char *msg_inicial_contador[] = {
    "              ",
    "Aperte o BOTAO",
    "A para iniciar",
    " a contagem   "};   

char *msg_sistema_ativo[] = {
    "   Contagem   ",
    " Regressiva:  "};   

char *msg_sistema_pausado[] = {
        "   Contagem   ",
        "    Parada:   ",
        "Quantidade do ",
        "Botao B       "}; 


void Display_MSG_BOTAO_A(){
    //Criacao dos eixos x e y para o desenho do display
    int x = 0;      //Eixo horizontal do display
    int y = 0;      //Eixo Vertical do display
    //Exibe mensagem inicial para posicionar cartao do motorista
    for (uint i = 0; i < count_of(msg_sistema_ativo); i++) {
        ssd1306_draw_string(ssd, 5, y, msg_sistema_ativo[i]);
        y += 8;
    }
    render_on_display(ssd, &frame_area);
}

void Display_MSG_BOTAO_B(){
    //Criacao dos eixos x e y para o desenho do display
    int x = 0;      //Eixo horizontal do display
    int y = 0;      //Eixo Vertical do display
    //Exibe mensagem inicial para posicionar cartao do motorista
    for (uint i = 0; i < count_of(msg_sistema_pausado); i++) {
        ssd1306_draw_string(ssd, 5, y, msg_sistema_pausado[i]);
        y += 8;
    }
    render_on_display(ssd, &frame_area);
}

void Display_MSG_Inicial(){
    //Criacao dos eixos x e y para o desenho do display
    int x = 0;      //Eixo horizontal do display
    int y = 0;      //Eixo Vertical do display
    //Exibe mensagem inicial para posicionar cartao do motorista
    for (uint i = 0; i < count_of(msg_inicial_contador); i++) {
        ssd1306_draw_string(ssd, 5, y, msg_inicial_contador[i]);
        y += 8;
    }
    render_on_display(ssd, &frame_area);
}

// Função de callback do temporizador repetitivo que é chamada a cada 100 ms.
// Essa função verifica o estado do botão e gerencia o estado do LED.
bool repeating_timer_callback(struct repeating_timer *t) {
  static absolute_time_t last_press_time = 0; // Armazena o tempo da última pressão do botão.
  static bool botaoA_ult_estado = false;      // Armazena o estado anterior do botão (pressionado ou não).
  static bool botaoB_ult_estado = false;

  // Verifica o estado atual do botão (pressionado = LOW, liberado = HIGH).
  bool botaoA_pressionado = !gpio_get(BOTAO_A); // O botão pressionado gera um nível baixo (0) que eh negado para flag ficar como true.
  bool botaoB_pressionado = !gpio_get(BOTAO_B);  // O botão pressionado gera um nível baixo (0) que eh negado para flag ficar como true.

  // Verifica se o botao B foi pressionada para mudar a frequencia para 1 Mhz
  if(count_active == true){  //verifica apenas o botao B quando o contador estiver ativo
    if (botaoB_pressionado && !botaoB_ult_estado && absolute_time_diff_us(last_press_time, get_absolute_time()) > 200000) { // Verifica se 200 ms se passaram (debounce).
      last_press_time = get_absolute_time();      // Atualiza o tempo da última pressão do botão.
      botaoB_ult_estado = true;
      count_B++;
    }
    else if (!botaoB_pressionado) {
        botaoB_ult_estado = false;                  // Atualiza o estado do botão como liberado quando ele não está pressionado.
      }  
  }

  if(count_active == false){                     // verifica se o sistema esta ativo, para caso esteja nao contabilizar o botao pressionado
    // Verifica se o botão foi pressionado e realiza o debounce.
    if (botaoA_pressionado && !botaoA_ult_estado && absolute_time_diff_us(last_press_time, get_absolute_time()) > 200000) { // Verifica se 200 ms se passaram (debounce).
      last_press_time = get_absolute_time();      // Atualiza o tempo da última pressão do botão.
      botaoA_ult_estado = true;                   // Atualiza o estado do botão como pressionado.
      count_active = true;                         //
      count_dec = 9;
      printf("Iniciado contador regressivo \n");
      }
    }
    else if (!botaoA_pressionado) {
        botaoA_ult_estado = false;                  // Atualiza o estado do botão como liberado quando ele não está pressionado.
      }  
  return true;                                  // Retorna true para continuar o temporizador de repetição.
}

int main() {                                    //menu principal
  stdio_init_all();                           // Inicializa a comunicação serial para permitir o uso de printf para depuração.
  // Configura o pino do botão como entrada com resistor de pull-up interno.
  gpio_init(BOTAO_A);                         // inicia botao A
  gpio_set_dir(BOTAO_A, GPIO_IN);             // configura GPIO do botao como entrada
  gpio_pull_up(BOTAO_A);                      // Habilita o resistor pull-up interno para evitar leituras incorretas.

  // Configura o pino do botão como entrada com resistor de pull-up interno.
  gpio_init(BOTAO_B);                         // inicia botao B
  gpio_set_dir(BOTAO_B, GPIO_IN);             // configura GPIO do botao como entrada
  gpio_pull_up(BOTAO_B);                      // Habilita o resistor pull-up interno para evitar leituras incorretas.

     // Inicialização do i2c para Display OLED
     i2c_init(i2c1, ssd1306_i2c_clock * 1000);           //Inicia I2C 1
     gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);          //Configura GPIO como I2c
     gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);          //Configura GPIO como I2c
     gpio_pull_up(I2C_SDA);                              //Ativa resistor de Pull up
     gpio_pull_up(I2C_SCL);                              //Ativa resistor de Pull up
     ssd1306_init();                                     // Processo de inicialização completo do OLED SSD1306
     calculate_render_area_buffer_length(&frame_area);
     zerar_display();
   Display_MSG_Inicial();                     //Mensagem para aproximar cartao do motorista

  // Configura o temporizador repetitivo para verificar o estado do botão a cada 100 ms.
  struct repeating_timer timer;
  add_repeating_timer_ms(100, repeating_timer_callback, NULL, &timer);

  // O loop principal permanece vazio, pois o controle do LED e do botão é gerenciado pelo temporizador.
  while (true) {
    if (count_active == true){
        zerar_display();
        Display_MSG_BOTAO_A();
        for(count_dec; count_dec > -1; count_dec--){
            printf("Contador Regressivo %d \n", count_dec);
            int x = 65;                                              //eixo x do display
            int y = 24; 
            char show_number = count_dec + 0x30;                                            //eixo y do display
            ssd1306_draw_char(ssd, x, y, show_number);    //Imprime do Display o nome do Aluno
            render_on_display(ssd, &frame_area);                
            sleep_ms(1000);    
        }
        count_dec = 0;
        zerar_display();
        Display_MSG_BOTAO_B();
            int x = 90;                                              //eixo x do display
            int y = 12; 
            char show_number = count_dec + 0x30;                                            //eixo y do display
            ssd1306_draw_char(ssd, x, y, show_number);    //Imprime do Display o nome do Aluno
            render_on_display(ssd, &frame_area);                

            x = 70;                                              //eixo x do display
            y = 24; 
            show_number = count_B + 0x30;                                            //eixo y do display
            ssd1306_draw_char(ssd, x, y, show_number);    //Imprime do Display o nome do Aluno
            render_on_display(ssd, &frame_area);                

        count_active = false;
        printf("Contador B é igual a: %d", count_B);
        count_B = 0;
        //add_alarm_in_ms(periodo / 2, inverte_led, NULL, false); //periodo dividido por 2, pois metade do tempo fica aceso e metade apagado
    }
  }
}
