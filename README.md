# Simulador para matrizes de NeoPixel

Simulador com o objetivo de facilitar a prototipação de efeitos e jogos para matrizes de NeoPixel, imitando a sintaxe utilizada pela biblioteca para arduino.

## Introdução

Para utilizar o simulador basta criar um arquivo exemplo.cpp com a cara:  
```
\#include "fx.cpp"

void setup()
{

}


void loop()
{

}
```  
compilá-lo utilizando o script comp.sh  
```
./comp.sh exemplo.cpp exemplo
```
O simulador possui uma classe Adafruit_NeoPixel com a mesma sintaxe da biblioteca para arduino, além de algumas definições de funções padrão do arduino como pinMode, digitalWrite e digitalRead.  

O simulador utiliza os seguintes arquivos:

* fx.cpp		- Gerencia o grid e a thread de efeitos
* digital_input.config	- Permite abstrair caracteres do teclado como pinos de entrada do arduino
* comp.sh		- Script que facilita a compilação do código escrito para o simulador

E depende da biblioteca 

* freeglut3-dev

## Uso


