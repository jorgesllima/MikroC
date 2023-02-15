int counter = 0x00;        //Variável de cintagem auxiliar

void interrupt()           //Vetor de interrupção, endereço 04h de memória, que é chamado sempre quando há estouro no contador
{
    counter++;             //incrementa o counter
    TMR0       = 0x06;     //Reinicia o registrador TMR0

    T0IF_bit   = 0x00;     //Limpa a flag para a próxima interrupção
}

void main() {

    CMCON = 7;

    OPTION_REG = 0x81;     //Desabilita os resistores de pull-up internos
                           //Configura o prescaler para 1:4
    GIE_bit    = 0x01;     //Habilita a interrupção Global
    PEIE_bit   = 0x01;     //Habilita a interrupção por periféricos
    T0IE_bit   = 0x01;     //Habilita a interrupção por estouro do TMR0
    TMR0       = 0x06;     //Inicia a contagem em em 6 dos 256 bits


    TRISA.RA0 = 1; //entrada chave
    TRISA.RA1 = 1; //retorno botao 1
    TRISA.RA2 = 0; //saida rele de acn                    // 0 saida
    TRISA.RA3 = 0; //saida led painel                     // 1 entrada
    TRISA.RA4 = 0; //Resete do contador
    //TRISIO5_bit = 0;

    while(1)
    {

     if (RA1_bit == 1 && RA0_bit == 1){
     RA3_bit = 0;
     RA2_bit = 1;
     }
     
     if (RA1_bit == 0 && RA0_bit == 0){
     RA3_bit = 0;
     RA2_bit = 0;
     }

     if (counter <= 4000 && RA1_bit == 0 && RA0_bit == 1){
     RA3_bit = 1;
     RA2_bit = 1;
     }

     if (counter >= 4000 && RA1_bit == 0 && RA0_bit == 1){   //Acada 1 mili segundos, se ele for igual a 15000, será 15 segundos
     RA3_bit = 1;
     RA2_bit = 0;
     }

     if (RA4_bit == 1){
     counter = 0x00;
     }


    } //end wilhe

} //end main
