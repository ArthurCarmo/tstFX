# Simulador para matrizes de NeoPixel

Simulador com o objetivo de facilitar a prototipação de efeitos e jogos para matrizes de NeoPixel, imitando a sintaxe utilizada pela biblioteca para arduino.

## Introdução

Para utilizar o simulador basta criar um arquivo exemplo.cpp com a cara:  
```
#include "fx.cpp"

void setup()
{

}


void loop()
{

}
```
E compilá-lo utilizando o script comp.sh  
```
./comp.sh exemplo.cpp exemplo
```

O simulador possui uma classe Adafruit_NeoPixel com a mesma sintaxe da biblioteca para arduino, além de algumas definições de funções padrão do arduino como pinMode, digitalWrite e digitalRead.  
Um exemplo de um programa para o simulador é:

```
#include "fx.cpp"

#define LED_PIN 6
#define LED_COUNT 144

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

void setup()
{
	strip.begin();
	strip.show();
}

void loop()
{
	strip.clear();
	for(int i = 0; i < LED_COUNT; i++)
	{
		strip.setPixelColor(i, 255, 0, 0);
		strip.show();
		delay(100);
	}
}

```

O tamanho da mesa é inferido a partir do LED_COUNT, e é considerado que os leds estam com a contagem em zig-zag, com o pixel 0 no canto superior esquerdo da mesa.  
No momento só é possível utilizar mesas retangulares, e as dimensões da matriz podem ser modificadas no arquivo grid_dim.config \(padrão 12x12\)

O simulador utiliza os seguintes arquivos:

* fx.cpp		- Gerencia o grid e a thread de efeitos
* comp.sh		- Script que facilita a compilação do código escrito para o simulador
* grid_dim.config - Especifica as dimensões da matriz de NeoPixel
* digital_input.config	- Permite abstrair caracteres do teclado como pinos de entrada do arduino

E depende da biblioteca 

* freeglut3-dev

## Uso


