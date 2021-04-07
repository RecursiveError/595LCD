/*
Autor: RecursiveError
Programa: LCD595

Porijeto final do topico IO
Repositorio do projto:
https://github.com/RecursiveError/AVR

Objeto LCD para utilização com 74HC595 usando apenas 3 portas digitais
*/
#include <Arduino.h>
#include "lcd595.h"
#include "string.h"

LCD::LCD(char EN, char DATA, char CDATA){
    _EN = EN;
    _DATA = DATA;
    _CDATA = CDATA;
}

//gera clock no pino data
void LCD::pulse_data(){
    digitalWrite(_CDATA, HIGH);
    digitalWrite(_CDATA, LOW);
}

//gera clock no pino EN
void LCD::pulse_en(){
    digitalWrite(_EN, HIGH);
    digitalWrite(_EN, LOW);
}

//Escreve diretamente no 74HC595
void LCD::write(char value){
    for(char i = 0; i<8; i++){
        digitalWrite(_DATA, value & 0x80);
        pulse_data();
        value <<= 1;
    }
    pulse_en();
}

//Envia 4bits de informação e define o estado
// std = false ENVIA UM COMANDO
// std = true ENVIA UM SIMBOLO
void LCD::push_nibble(char value, bool std){
    unsigned char msg;
    if(std){
        msg  = value + 0x60;
    }else{
        msg =  value + 0x40;
    }
    write(msg);
    delayMicroseconds(5);
    write(msg - 0x40);
    delayMicroseconds(5);
    write(0x00);
    delayMicroseconds(5);
    //taçvez não seja a forma mais eficiente, 
    //porem foi a unica forma de limpar o 74hc595 sem gerar bugs 
}

//Envia 8bits em 2 pacotes de 4bits e define o estado
//std = false ENVIA UM COMANDO
//std = true ENVIA UM SIMBOLO
void LCD::push_byte(char value, bool std){
   unsigned char high_nibble = (value >> 4) & 0x0F;
   unsigned char low_nibble = value & 0x0F;
   push_nibble((high_nibble), std);
   push_nibble((low_nibble), std); 
    
}
    
// envia um comando para o display
void LCD::command(char comando){
    push_byte(comando, false);
    delay(2);
}

//move o cursor
void LCD::setcurso(char line, char col){
    _current_col = col;
    _current_line = line;
    if(line == 1) command((0x80+col));
    if(line == 2) command((0xC0+col));
}

//Envias os dados para o display
void LCD::print(char msg){
    push_byte(msg, true);
    delayMicroseconds(80);
    //almenta a coluna a cada letra
    //caso chegue ao limite do display passa para a proxima linha
    //caso chegue ao final do display volta para a primeira coluna/linha
    _current_col++;
    if(_current_col > MAXCOL){
        _current_line++; 
        setcurso(_current_line, 0);
    }
    if(_current_line > MAXLINE)setcurso(1,0);
    
    
}

//Envias os dados para o display
void LCD::print(String msg){
    unsigned int i = 0;
    while(msg[i] != '\0'){
        print(msg[i]);
        i++;
    }
}

//Envias os dados para o display
//*NOTA floats ainda não são suportados
void LCD::print(int num){
    unsigned int i = 10000;
    while(i>0){
        print((char)((num/i) % 10 + 48));
        i /= 10;
    }
}
// CONFIGURAÇÃO DO DISPLAY

//liga o cursor
void LCD::curso(){
_current_config |= (1<<1);
 command(_current_config);
}

//desliga o cursor
void LCD::noCurso(){
    _current_config &= ~(1<<1);
    command(_current_config);
}

//liga o piscar do cursor
void LCD::blink(){
    _current_config |= (1<<0);
    command(_current_config);
}

//desliga o piscar do cursor 
void LCD::noblink(){
    _current_config &= ~(1<<0);
    command(_current_config);
}

void LCD::clear(){
    command(0x01);
    delayMicroseconds(15);
}


//retorna a linha atual
unsigned char LCD::getCurrentLine(){
    return _current_line;
}

//retorna a coluna Atual
unsigned char LCD::getCurrentCol(){
    return _current_col;
}
void LCD::init(int col, int line){
    pinMode(_EN, OUTPUT);
    pinMode(_DATA, OUTPUT);
    pinMode(_CDATA, OUTPUT);
    MAXLINE = line;
    MAXCOL = col;
    _current_line = 1;
    _current_col = 0;
     
    delay(15);
    //ESTADO DA COMUNICAÇÃO É INCERTO
    push_nibble(0X3, false);
    delay(5);
    push_nibble(0X3, false);
    delayMicroseconds(160);
    push_nibble(0X3, false);//ESTADO DA COMUNICAÇÃO É 8BITS

    //MUDANDO PARA 4BITS
    delayMicroseconds(160);
    push_nibble(0x2, false);
    delay(10);
    //confg do display
    command(0x28);
    command(_current_config);
    command(0x01);
    delayMicroseconds(15);
}