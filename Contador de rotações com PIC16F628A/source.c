sbit LCD_RS at RB2_Bit;                        //Pino RS do display no RB2 do PIC
sbit LCD_EN at RB3_Bit;                        //Pino EN do display no RB3 do PIC
sbit LCD_D4 at RB4_Bit;                        //Pino D4 do display no RB4 do PIC
sbit LCD_D5 at RB5_Bit;                        //Pino D5 do display no RB5 do PIC
sbit LCD_D6 at RB6_Bit;                        //Pino D6 do display no RB6 do PIC
sbit LCD_D7 at RB7_Bit;                        //Pino D7 do display no RB7 do PIC

sbit LCD_RS_Direction at TRISB2_Bit;           //Direciona TRISB2 para controle de RS
sbit LCD_EN_Direction at TRISB3_Bit;           //Direciona TRISB3 para controle de EN
sbit LCD_D4_Direction at TRISB4_Bit;           //Direciona TRISB4 para controle de D4
sbit LCD_D5_Direction at TRISB5_Bit;           //Direciona TRISB5 para controle de D5
sbit LCD_D6_Direction at TRISB6_Bit;           //Direciona TRISB6 para controle de D6
sbit LCD_D7_Direction at TRISB7_Bit;           //Direciona TRISB7 para controle de D7                                           

long freq = 0;                                 //Variável para leitura da frequência
long calc = 0;                                 //Variável para cálculo da ferquência
long freq2 = 0;
long freq3 = 0;

int cnt = 0;                                   //Variável auxiliar do timer0

char d1 = 0,                                   //Variável para dígito 1 do valor de leitura
     d2 = 0,                                   //Variável para dígito 2 do valor de leitura
     d3 = 0,                                   //Variável para dígito 3 do valor de leitura
     d4 = 0,                                   //Variável para dígito 4 do valor de leitura
     d5 = 0;                                   //Variável para dígito 5 do valor de leitura

void interrupt() {

     if(TMR0IF_Bit) {                          //Testa se houve estouro do timer0

        TMR0IF_Bit = 0;                        //Seta flag do timer0 em 0
        TMR0 = 55;                             //Carega valor 55 no registrador do timer0
        cnt++;                                 //Incrementa variável cnt

        if(cnt == 5000) {                      //Se variável cnc for igual a 5000

           cnt = 0;                            //Sera variável cnt
           freq = calc;                        //Variável freq é igual a variável calc
           calc = 0;                           //Zera variável calc
           TMR0 = 55;                          //Carrega valor 55 no registrador do timer0

        }
     }

     if(INTF_Bit) {                            //Testa de houve uma interrupção externa
        calc++;                                //Incrementa variável calc quando RB estiver em 1
        INTF_Bit = 0;                          //Seta RB0 em 0
     }

}                                              //Fim interrupt

void main() {

     TRISA = 0xFF;  //seta como saida
     VRCON = 0xE7;  //saida VREF analógica

     CMCON = 0x07;                             //Desabilita os comparadores internos

     OPTION_REG = 0b00000001;                  //Configura timer 0 para o prescale de 1:4
     GIE_Bit = 1;                              //Abilita a interrupção global
     PEIE_Bit = 0;                             //Desabilita a interrupção por reriféricos
     TMR0IE_Bit = 1;                           //Abilita a interrupção do timer0
     INTE_Bit = 1;                             //Abilita a interrupção externa


     TMR0 = 55;                                //Carrega valor 55 no registrador do timer 0

     TRISB0_Bit = 1;                           //Configura porta RB0 como entrada digital

     Lcd_Init();                               //Inicia o display
     Lcd_Cmd(_LCD_CLEAR);                      //Limpa o display
     Lcd_Cmd(_LCD_CURSOR_OFF);                 //Apaga o cursor do display
     Lcd_Out(1, 2, "Frequencimetro");          //Imprime mensagem na linha 1
     Lcd_Out(2, 1, "Freq:");                   //Imprime mensagem na linha 2

     Delay_ms(10);                            //Espera 200 milissegundos

     while(1) {

           d1 = freq3 / 10000;                  //Faz d1 igual a freq e divide por 10000
           d2 = (freq3 % 10000) / 1000;         //Faz d2 igual a freq e divide resto de 10000 por 1000
           d3 = (freq3 % 1000) / 100;           //Faz d3 igual a freq e divide resto de 1000 por 100
           d4 = (freq3 % 100) / 10;             //Faz d4 igual a freq e divide resto de 100 por 10
           d5 = freq3 % 10;                     //Faz d5 igual a freq e divide resto de 10 por 10
           
           Lcd_Chr(2, 7, d1 + '0');            //Impreme d1 no linha 2 caractere 7
           Lcd_Chr_Cp(d2 + '0');               //Impreime d2 no caractere 8
           Lcd_Chr_Cp('.');                    //Imprime um "PONTO" no caractere 9
           Lcd_Chr_Cp(d3 + '0');               //Imprime d3 no caractere 10
           Lcd_Chr_Cp(d4 + '0');               //Imprime d4 no caractere 11
           Lcd_Chr_Cp(d5 + '0');               //Imprime d5 no caractere 12


           if(freq > 999) {                   //Se o valor da frequência for maior que 999Hz

              Lcd_Out(2, 14, "KHz");          //Imprime "KHz" no lado do valor medido

              }
              else if(freq < 1000) {          //Caso contrário de o valor da frequência medido for menor que 1KHz
              Lcd_Out(2, 14, " Hz");          //Impremine "Hz" no lado do valor medido
              }

     freq3 = freq / 4;
     freq2 = freq3 * 10;

     if(freq2 >= 20){
     VRCON = 0xE1;
     }
     if(freq2 >= 40){
     VRCON = 0xE2;
     }
     if(freq2 >= 60){
     VRCON = 0xE3;
     }
     if(freq2 >= 80){
     VRCON = 0xE4;
     }
     if(freq2 >= 100){
     VRCON = 0xE5;
     }
     if(freq2 >= 120){
     VRCON = 0xE6;
     }
     if(freq2 >= 140){
     VRCON = 0xE7;
     }
     if(freq2 >= 160){
     VRCON = 0xE8;
     }     
     if(freq2 >= 180){
     VRCON = 0xE9;
     }
     delay_ms(100);
     
     }                                        //Fim loop

}                                             //Fim programa
