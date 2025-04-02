#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include <string.h> //some functions need NULL to be defined
#include "inc/ssd1306.h"
#include "hardware/i2c.h"
#include "inc/ssd1306_i2c.c"


// SPI Defines
// We are going to use SPI 0, and allocate it to the following GPIO pins
// Pins can be changed, see the GPIO function select table in the datasheet for information on GPIO assignments
//#define SPI_PORT spi0
//#define RESET_PIN 20
/*
static const uint cs_pin = 17;
static const uint sck_pin = 18;
static const uint mosi_pin = 19;
static const uint miso_pin = 16;
*/
#include "mfrc522.c"

typedef struct {
    char nome[20];
    uint8_t ponto;
    uint8_t tag[4];
} meusAlunos;
meusAlunos alunos[25];

const uint I2C_SDA = 14;
const uint I2C_SCL = 15;

void ssd1306_turn_off(){
    uint8_t commands[] = {
    ssd1306_set_display | 0x00,
    };
    ssd1306_send_command_list(commands, count_of(commands));
}
    
void ssd1306_turn_on(){
    uint8_t commands[] = {
    ssd1306_set_display | 0x01,
    };
    ssd1306_send_command_list(commands, count_of(commands));
}

void exibe_tela(){
    
}

void main() {
    stdio_init_all();

     // Inicialização do i2c
    i2c_init(i2c1, ssd1306_i2c_clock * 1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);

    // Processo de inicialização completo do OLED SSD1306
    ssd1306_init();

    // Preparar área de renderização para o display (ssd1306_width pixels por ssd1306_n_pages páginas)
    struct render_area frame_area = {
        start_column : 0,
        end_column : ssd1306_width - 1,
        start_page : 0,
        end_page : ssd1306_n_pages - 1
    };

      calculate_render_area_buffer_length(&frame_area);

    for(int i=0; i<25; i++) {
        strcpy(alunos[i].nome, "\0");
        alunos[i].ponto = 0;
        for(int j=0; j<4; j++){
            alunos[i].tag[j] = 0xff;
        }
    }    

    //strcpy(alunos[23].nome, "Arthur Franco");
    alunos[23].nome[0] = 0x41;
    alunos[23].nome[1] = 0x72;
    alunos[23].nome[2] = 0x74;
    alunos[23].nome[3] = 0x68;
    alunos[23].nome[4] = 0x75;
    alunos[23].nome[5] = 0x72;
    alunos[23].nome[6] = 0x20;
    alunos[23].nome[7] = 0x46;
    alunos[23].nome[8] = 0x72;
    alunos[23].nome[9] = 0x61;
    alunos[23].ponto = 1;
    alunos[23].tag[0] = 0x35;
    alunos[23].tag[1] = 0x4e;
    alunos[23].tag[2] = 0x90;
    alunos[23].tag[3] = 0xbb;

    // zera o display inteiro
    uint8_t ssd[ssd1306_buffer_length];
    memset(ssd, 0, ssd1306_buffer_length);
    render_on_display(ssd, &frame_area);

/*    char *text_inicio[] = {
        "     Comedouro ",
        "        RFID   ",
        "  Arthur Franco",
        "  Embarcatech  "};

    int y = 0;
    for (uint i = 0; i < count_of(text_inicio); i++) {
        ssd1306_draw_string(ssd, 5, y, text_inicio[i]);
        y += 8;
    }
    render_on_display(ssd, &frame_area);

    sleep_ms(2000);
*/
  //  memset(ssd, 0, ssd1306_buffer_length);
  //  render_on_display(ssd, &frame_area);

    sleep_ms(2000);

    int y = 0;
    int x = 5;
  //  for (uint i = 0; i <count_of(alunos[23].nome); i++ ){
        //sd1306_draw_char(ssd, x, y, alunos[23].nome[i]);
        ssd1306_draw_string(ssd, 5, y, alunos[23].nome);
        render_on_display(ssd, &frame_area);
    //    x += 8;
    //    printf("%x", alunos[23].nome[i]);
    //}   

    //valores das TAGS RFID
    // Tag cachorro 1
    uint8_t tag1[] = {0x35, 0x4e, 0x90, 0xbb};

    // Tag cachorro 2
    //uint8_t tag2[] = {0x35, 0x4e, 0x90, 0xbb};
    // Tag Admin
    uint8_t tagAdm[4] = {0x00, 0x00, 0x00, 0x00};

    //Inicialização do Módulo RFID
    MFRC522Ptr_t mfrc = MFRC522_Init();
    PCD_Init(mfrc, spi0);

    sleep_ms(5000);

    // char test_result = PCD_SelfTest(mfrc);

    // if (test_result == 0) {
    //     printf("Self Test:  SUCCESS");
    // } else {
    //     printf("Self Test: FAILED");
    // }

    //Wait for new card

    // zera o display inteiro
    memset(ssd, 0, ssd1306_buffer_length);
    render_on_display(ssd, &frame_area);

    char *text_msg1[] = {
        "    Aproxime o ",
        " cartao \"ADM\"",
        "    no Leitor  "};

    y = 0;
        for (uint i = 0; i < count_of(text_msg1); i++) {
        ssd1306_draw_string(ssd, 5, y, text_msg1[i]);
        y += 8;
    }
    render_on_display(ssd, &frame_area);
    sleep_ms(2000);

    printf("Aproxime o cartao Admin no leitor \n\r");
    while(!PICC_IsNewCardPresent(mfrc));
    //Select the card

    // zera o display inteiro
    memset(ssd, 0, ssd1306_buffer_length);
    render_on_display(ssd, &frame_area);

    //Wait for new card
    char *text_msg2[] = {
        " Lendo Cartao! ",
        " Aguarde...    "};

    y = 0;
    for (uint i = 0; i < count_of(text_msg2); i++) {
        ssd1306_draw_string(ssd, 5, y, text_msg2[i]);
        y += 8;
    }
    render_on_display(ssd, &frame_area);
    sleep_ms(2000);

    printf("Lendo cartao...\n\r");
    PICC_ReadCardSerial(mfrc);

    printf("Salvando Tag Adm, Aguarde...\n\r");
    tagAdm[0] = mfrc-> uid.uidByte[0];
    tagAdm[1] = mfrc-> uid.uidByte[1];
    tagAdm[2] = mfrc-> uid.uidByte[2];
    tagAdm[3] = mfrc-> uid.uidByte[3];
    //memcpy(mfrc->uid.uidByte, tagAdm, 4);

    sleep_ms(2000);
    //Authorization with uid
    printf("Uid Adm is: ");
    for(int i = 0; i < 4; i++) {
        printf("%x ", tagAdm[i]);
    } printf("\n\r");

    // zera o display inteiro
    memset(ssd, 0, ssd1306_buffer_length);
    render_on_display(ssd, &frame_area);

   //Wait for new card
    char *text_msg3[] = {
        "  Aproxime o   ",
        " o cartao ADM  ",
        " novamente no  ",
        "    leitor     "};

    y = 0;
        for (uint i = 0; i < count_of(text_msg3); i++) {
        ssd1306_draw_string(ssd, 5, y, text_msg3[i]);
        y += 8;
    }
    render_on_display(ssd, &frame_area);
    sleep_ms(2000);

    printf("Aproxime novamente o cartao Admin para iniciar a congiguracao \n\r");
    while(!PICC_IsNewCardPresent(mfrc));
    //Select the card

    // zera o display inteiro
    memset(ssd, 0, ssd1306_buffer_length);
    render_on_display(ssd, &frame_area);

    y = 0;
        for (uint i = 0; i < count_of(text_msg2); i++) {
        ssd1306_draw_string(ssd, 5, y, text_msg2[i]);
        y += 8;
    }
    render_on_display(ssd, &frame_area);
    sleep_ms(2000);

    printf("Lendo cartao...\n\r");
    PICC_ReadCardSerial(mfrc);
    //Show UID on serial monitor
    //printf("PICC dump: \n\r");
    //PICC_DumpToSerial(mfrc, &(mfrc->uid));

    //Authorization with uid
    printf("Uid is: ");
    for(int i = 0; i < 4; i++) {
        printf("%x ", mfrc->uid.uidByte[i]);
    } printf("\n\r");

    // zera o display inteiro
    memset(ssd, 0, ssd1306_buffer_length);
    render_on_display(ssd, &frame_area);
   //Wait for new card
    char *text_msg4[] = {
        "  Sucesso na   ",
        " Autenticacao! ",
        "  Iniciando    ",
        "Configuracao..."};

   //Wait for new card
    char *text_msg5[] = {
        "  Falha na     ",
        " Autenticacao! ",
        "  Reiniciando  ",
        " processo...   "};

    for(int i=0; i<25; i++){
        if(memcmp(mfrc->uid.uidByte, alunos[i].tag, 4) == 0){
            printf("Aluno Encontrado %s \n\r", alunos[i].nome);
            printf("Aluno numero %d \n", i);
            break;  
        }
        else{
            printf("Aluno nao Encontrado \n\r");
            printf("Aluno numero %d \n", i);
        }
    }

    if(memcmp(mfrc->uid.uidByte, tagAdm, 4) == 0) {
        printf("Authentication Success\n\r");
        y = 0;
        for (uint i = 0; i < count_of(text_msg4); i++) {
            ssd1306_draw_string(ssd, 5, y, text_msg4[i]);
            y += 8;
        }
    } else {
        printf("Authentication Failed\n\r");
        y = 0;
        for (uint i = 0; i < count_of(text_msg5); i++) {
           ssd1306_draw_string(ssd, 5, y, text_msg5[i]);
            y += 8;
        }
    }
    render_on_display(ssd, &frame_area);
    sleep_ms(2000);

    while(1){
        tight_loop_contents();
    }
}



