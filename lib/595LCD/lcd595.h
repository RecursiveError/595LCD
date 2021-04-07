/*
Autor: RecursiveError
Programa: LCD595

Porijeto final do topico IO
Repositorio do projto:
https://github.com/RecursiveError/AVR

Objeto LCD para utilização com 74HC595 usando apenas 3 portas digitais
*/


#ifndef _LCD595_H
#define _LCD595_H
#include <string.h>

class LCD{    
public:
    LCD(char EN, char DATA, char DCLOCK);

    unsigned char getCurrentLine();
    unsigned char getCurrentCol();

    //inicia p display com a quantidade de Colonas e linhas
    void init(int col, int line);
    // envia um comando para o display
    void command(char cmd);
    //Envias os dados para o display
    //*NOTA floats ainda não são suportados
    void print(char msg);
    void print(String msg);
    void print(int num);
    //limpa o display
    void clear();

    //liga o cursor 
    void curso();
    //desliga 
    void noCurso();
    //liga o piscar o cursor
    void blink();
    //desliga o piscar do cursor
    void noblink();
    //move o cursor
    void setcurso(char line, char col);
private:

    //vARIAVEIS DE FUNCIONAMENTO
    unsigned char _DATA;
    unsigned char _CDATA;
    unsigned char _EN;

    //VARIAVEIS DE CONFIGURAÇÃO
    unsigned int  MAXLINE;
    unsigned int  MAXCOL;
    unsigned int  _current_line;
    unsigned int  _current_col;
    unsigned char _current_config = 0x0C;

    //Escreve diretamente no 74HC595
    void write(char data);

    //Gera clock no pino de data
    void pulse_data();

    //gera clock no pino EN
    void pulse_en();

    //Envia 4bits de informação e define o estado
    // std = false ENVIA UM COMANDO
    // std = true ENVIA UM SIMBOLO
    void push_nibble(char value, bool std);

    //Envia 8bits em 2 pacotes de 4bits e define o estado
    //std = false ENVIA UM COMANDO
    //std = true ENVIA UM SIMBOLO
    void push_byte(char value, bool std);

};

#endif