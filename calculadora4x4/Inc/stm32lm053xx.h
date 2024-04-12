/*
 * stm32lm053xx.h
 *
 *  Created on: Apr 6, 2024
 *      Author: Asus Zephyrus
 */

#ifndef STM32LM053XX_H_
#define STM32LM053XX_H_

// Direcciones base de los periféricos y puertos del microcontrolador
#define PERIPHERAL_BASE_ADDRESS     0x40000000U // Dirección base de los periféricos
#define AHB_BASE_ADDRESS            (PERIPHERAL_BASE_ADDRESS + 0x00020000U) // Dirección base del bus AHB
#define RCC_BASE_ADDRESS            (AHB_BASE_ADDRESS + 0x00001000U) // Dirección base del controlador de reloj (RCC)
#define RCC_IOPENR_ADDRESS          (RCC_BASE_ADDRESS + 0x0000002CU) // Dirección del registro IOPENR en RCC
#define IOPORT_ADDRESS              (PERIPHERAL_BASE_ADDRESS + 0x10000000U) // Dirección base de los puertos de entrada/salida
#define GPIOA_BASE_ADDRESS          (IOPORT_ADDRESS + 0x00000000U) // Dirección base del puerto A
#define GPIOB_BASE_ADDRESS          (IOPORT_ADDRESS + 0x00000400U) // Dirección base del puerto B
#define GPIOC_BASE_ADDRESS          (IOPORT_ADDRESS + 0x00000800U) // Dirección base del puerto C

// Definiciones para facilitar el acceso a los registros de los puertos
#define GPIOA ((GPIO_RegDef_t*)GPIOA_BASE_ADDRESS) // Puntero a la estructura de registros de GPIOA
#define GPIOB ((GPIO_RegDef_t*)GPIOB_BASE_ADDRESS) // Puntero a la estructura de registros de GPIOB
#define GPIOC ((GPIO_RegDef_t*)GPIOC_BASE_ADDRESS) // Puntero a la estructura de registros de GPIOC
#define RCC ((RCC_RegDef_t*)RCC_BASE_ADDRESS) // Puntero a la estructura de registros de RCC

// Variable para detectar si una tecla ha sido presionada
int8_t	key_pressed = 0x00;

// Estructura para definir los registros de control de GPIO
typedef struct {
    uint32_t MODER;   // Modo del puerto
    uint32_t OTYPER;  // Tipo de salida del puerto
    uint32_t OSPEEDR; // Velocidad de salida del puerto
    uint32_t PUPDR;   // Pull-up/pull-down del puerto
    uint32_t IDR;     // Registro de entrada del puerto
    uint32_t ODR;     // Registro de salida del puerto
    uint32_t BSRR;    // Registro de set/reset de bits del puerto
    uint32_t LCKR;    // Registro de bloqueo del puerto
    uint32_t AFR[2];  // Registro de función alternativa del puerto
    uint32_t BRR;     // Registro de reset de bits del puerto
} GPIO_RegDef_t;

// Estructura para definir los registros de control de RCC
typedef struct {
    uint32_t CR;        // Registro de control RCC
    uint32_t ICSCR;     // Registro de calibración del reloj interno
    uint32_t CRRCR;     // Registro de control de calibración de reloj
    uint32_t CFGR;      // Registro de configuración RCC
    uint32_t CIER;      // Registro de habilitación de interrupciones de RCC
    uint32_t CIFR;      // Registro de flags de interrupción de RCC
    uint32_t CICR;      // Registro de control de interrupción de RCC
    uint32_t IOPRSTR;   // Registro de reset de periféricos de entrada/salida
    uint32_t AHBRSTR;   // Registro de reset de bus de alta velocidad
    uint32_t APB2RSTR;  // Registro de reset de periféricos del bus APB2
    uint32_t APB1RSTR;  // Registro de reset de periféricos del bus APB1
    uint32_t IOPENR;    // Registro de habilitación de reloj de periféricos de entrada/salida
} RCC_RegDef_t;


// Definiciones para representar los dígitos y letras en el display
#define cero   0b00111111  // Patrón de segmentos para mostrar el dígito 0
#define uno    0b00000110  // Patrón de segmentos para mostrar el dígito 1
#define dos    0b01011011  // Patrón de segmentos para mostrar el dígito 2
#define tres   0b01001111  // Patrón de segmentos para mostrar el dígito 3
#define cuatro 0b01100110  // Patrón de segmentos para mostrar el dígito 4
#define cinco  0b01101101  // Patrón de segmentos para mostrar el dígito 5
#define seis   0b01111101  // Patrón de segmentos para mostrar el dígito 6
#define siete  0b00000111  // Patrón de segmentos para mostrar el dígito 7
#define ocho   0b01111111  // Patrón de segmentos para mostrar el dígito 8
#define nueve  0b01100111  // Patrón de segmentos para mostrar el dígito 9
#define A      0b01110111  // Patrón de segmentos para mostrar la letra A
#define B      0b01111100  // Patrón de segmentos para mostrar la letra B
#define C      0b00111001  // Patrón de segmentos para mostrar la letra C
#define D      0b01011110  // Patrón de segmentos para mostrar la letra D
#define E      0b01111001  // Patrón de segmentos para mostrar la letra E
#define F      0b01110001  // Patrón de segmentos para mostrar la letra F


// Índices de las teclas especiales
#define KEY_A_INDEX 3  // Índice para la tecla A
#define KEY_B_INDEX 7  // Índice para la tecla B
#define KEY_C_INDEX 11 // Índice para la tecla C
#define KEY_D_INDEX 15 // Índice para la tecla D
#define KEY_F_INDEX 14 // Índice para la tecla F
#define KEY_E_INDEX 12 // Índice para la tecla E

// Declaraciones de funciones para operaciones de delay y actualización del display
void retraso(uint16_t n);
void act_valores(void); // Actualiza los valores mostrados en el display
uint8_t decodificador(uint8_t value_to_decode); // Decodifica un valor para el display
void decimal_decodificador(uint8_t value); // Decodifica un valor decimal para el display



#endif /* STM32LM053XX_H_ */
