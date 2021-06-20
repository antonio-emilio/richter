#include "SSD1306.h" //Responsável pela comunicação com o display

//Define os pinos que serão utilizados pelo display LoRa 
#define SCK     5   // GPIO5  -- SX127x's SCK
#define MISO    19  // GPIO19 -- SX127x's MISO
#define MOSI    27  // GPIO27 -- SX127x's MOSI
#define SS      18  // GPIO18 -- SX127x's CS
#define RST     14  // GPIO14 -- SX127x's RESET
#define DI00    26  // GPIO26 -- SX127x's IRQ(Interrupt Request)
SSD1306 display(0x3c, 4, 15); //Construtor do objeto do display LoRa


//Exibe mensagem recebida como parâmetro
void showDisplay(int line, String msg, bool clear)
{
  //Limpa o display
  if(clear)
  {
    display.clear();
    //Configura alinhamento de texto à esquerda
    display.setTextAlignment(TEXT_ALIGN_LEFT);
    //Configura fonte do texto
    display.setFont(ArialMT_Plain_10);
  }
  //Escreve na pos 0,25 a mensagem
  display.drawString(0 , line*10 , msg); 
  //Exibe no display
  display.display();  
}

//Configura Display LoRa
bool loraDisplayConfig()
{
  //Configura RST do oled como saida
  pinMode(16,OUTPUT);
  //Reseta o OLED
  digitalWrite(16, LOW);   
  //Aguarda 50ms
  delay(50); 
  //Enquanto o OLED estiver ligado, GPIO16 deve estar HIGH
  digitalWrite(16, HIGH);

  //Inicializa display
  if(!display.init())
    return false;

  //Inverte a tela verticalmente (de ponta cabeça)
  display.flipScreenVertically();  
  //Configura a fonte do display
  display.setFont(ArialMT_Plain_10);
  //Aguarda 200ms
  delay(200);
  //Limpa display
  display.clear();

  return true;
}
