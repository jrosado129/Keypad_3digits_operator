#include <stdint.h>
#include "stm32lm053xx.h"

uint8_t digito1_display = 0xFF; // Primer dígito para mostrar
uint8_t digito2_display = 0xFF; // Segundo dígito para mostrar
uint8_t digito3_display = 0xFF; // Segundo dígito para mostrar
uint8_t digito4_display = 0xFF; // Segundo dígito para mostrar
uint8_t digito5_display = 0xFF;
uint8_t valor_almacenado = 0; // Variable para almacenar el valor mostrado en los displays
int valor_operacion = 0;

const uint8_t key_to_display[] = {
    uno, dos, tres, A, // Contenidos de la Columna 1
    cuatro, cinco, seis, B, // Contenidos de la Columna 2
    siete, ocho, nueve, C, // Contenidos de la Columna 3
    E, cero, F, D // Contenidos de la Columna 4
};

// Define una función para mapear un código de tecla a su número correspondiente.
uint8_t codigo_tecla(uint8_t key){
	switch (key){
		case 0: return 1; // Cuando la tecla es 0, retorna 1.
		case 1: return 2; // Cuando la tecla es 1, retorna 2.
		case 2: return 3; // Cuando la tecla es 2, retorna 3.
		case 4: return 4; // Cuando la tecla es 4, retorna 4.
		case 5: return 5; // Cuando la tecla es 5, retorna 5.
		case 6: return 6; // Cuando la tecla es 6, retorna 6.
		case 8: return 7; // Cuando la tecla es 8, retorna 7.
		case 9: return 8; // Cuando la tecla es 9, retorna 8.
		case 10: return 9; // Cuando la tecla es 10, retorna 9.
		case 13: return 0; // Cuando la tecla es 13, retorna 0.
		default: return 0; // En cualquier otro caso, retorna 0.
	}
}

// Define una función para obtener el código de tecla correspondiente a un número.
uint8_t numero(uint8_t number){
	switch (number){
		case 1: return 0; // Cuando el número es 1, retorna 0.
		case 2: return 1; // Cuando el número es 2, retorna 1.
		case 3: return 2; // Cuando el número es 3, retorna 2.
		case 4: return 4; // Cuando el número es 4, retorna 4.
		case 5: return 5; // Cuando el número es 5, retorna 5.
		case 6: return 6; // Cuando el número es 6, retorna 6.
		case 7: return 8; // Cuando el número es 7, retorna 8.
		case 8: return 9; // Cuando el número es 8, retorna 9.
		case 9: return 10; // Cuando el número es 9, retorna 10.
		case 0: return 13; // Cuando el número es 0, retorna 13.
		default: return 13; // En cualquier otro caso, retorna 13.
	}
}


int main(void)
{

	// Activación de los relojes para los puertos GPIOC y GPIOB para permitir su funcionamiento
	RCC->IOPENR |= (1 << 2) | (1 << 1) | (1 << 0);

	// Definir los pines específicos de GPIOC como salidas, configurándolos para controlar los displays
	uint32_t portC_masks = (0b01 << 8) | (0b01 << 10) | (0b01 << 12) | (0b01 << 14) | (0b01 << 16) | (0b01 << 18);
	GPIOC->MODER &= ~(0b11 << 8 | 0b11 << 10 | 0b11 << 12 | 0b11 << 14 | 0b11 << 16 | 0b11 << 18);
	GPIOC->MODER |= portC_masks;

	// Preparar pines de GPIOB para manejar los segmentos de los displays, estableciendo su dirección como salida
	GPIOB->MODER &= ~(0xFFFF); // Resetear la configuración de los pines para asegurar una base limpia
	uint32_t portB_display_masks = (0b01 << 0) | (0b01 << 2) | (0b01 << 4) | (0b01 << 6) |
	                               (0b01 << 8) | (0b01 << 10) | (0b01 << 12) | (0b01 << 14);
	GPIOB->MODER |= portB_display_masks; // Aplicar la máscara para configurar los pines como salidas

	// Establecer pines específicos de GPIOB para controlar las columnas de la matriz del teclado como salidas
	GPIOB->MODER &= ~((0b11 << 24) | (0b11 << 26) | (0b11 << 28) | (0b11 << 30)); // Resetear configuración actual
	GPIOB->MODER |= ((0b01 << 24) | (0b01 << 26) | (0b01 << 28) | (0b01 << 30)); // Configurar como salidas

	// Ajustar los pines de GPIOB para utilizar resistencias pull-up y mejorar la fiabilidad de la lectura de entradas
	GPIOB->PUPDR &= ~((0b11 << 16) | (0b11 << 18) | (0b11 << 20) | (0b11 << 22)); // Resetear configuraciones previas
	GPIOB->PUPDR |= ((0b01 << 16) | (0b01 << 18) | (0b01 << 20) | (0b01 << 22)); // Activar resistencias pull-up

	// Configurar pines seleccionados en GPIOB para actuar como entradas, destinadas a la lectura del teclado matricial
	GPIOB->MODER &= ~((0b11 << 16) | (0b11 << 18) | (0b11 << 20) | (0b11 << 22)); // Limpiar configuración para asegurar que son entradas


    while (1)
    {
        for (int col = 0; col < 4; col++)
        {
            GPIOB->ODR = (0xF << 12); // Desactiva todas las columnas
            GPIOB->ODR &= ~(1 << (12 + col)); // Activa solo la columna actual

            retraso(10); // Deja tiempo para la estabilización

            uint32_t rows = GPIOB->IDR & (0xF << 8); // Lee el estado de las filas

            for (int row = 0; row < 4; row++)
            {
                if (!(rows & (1 << (row + 8)))) // Detecta si alguna fila está activa (presionada)
                {
                    uint8_t key_number = col * 4 + row; // Ajusta el cálculo de key_number

                    switch (key_number)
                    {
                    case KEY_D_INDEX:
                        // Resetear el valor almacenado y los dígitos del display a sus estados iniciales
                        valor_almacenado = 0;
                        digito1_display = digito2_display = digito3_display = digito4_display = 0xFF;
                        break;

                    case KEY_A_INDEX:
                        // Sumar los valores de los dígitos multiplicando cada uno por su respectivo peso posicional
                        valor_almacenado += codigo_tecla(digito1_display) + codigo_tecla(digito2_display) * 10 + codigo_tecla(digito3_display) * 100;
                        // Convertir el valor almacenado a su representación en los dígitos del display
                        digito1_display = numero(valor_almacenado % 10);
                        digito2_display = numero((valor_almacenado / 10) % 10);
                        digito3_display = numero((valor_almacenado / 100) % 10);
                        digito4_display = 0xFF;  // Limpia el cuarto dígito
                        valor_almacenado = 0;    // Resetear el valor almacenado después de mostrarlo
                        break;

                    case KEY_C_INDEX:
                        // Multiplicar los valores representados en los tres dígitos
                        valor_operacion = codigo_tecla(digito1_display) * codigo_tecla(digito2_display) * codigo_tecla(digito3_display);
                        // Convertir el resultado de la operación a su representación en los dígitos del display
                        digito1_display = numero(valor_operacion % 10);
                        digito2_display = numero((valor_operacion / 10) % 10);
                        digito3_display = numero((valor_operacion / 100) % 10);
                        digito4_display = 0xFF; // Limpia el cuarto dígito
                        break;

                    case KEY_F_INDEX:
                        // Realizar una división, evitando la división por cero
                        if (codigo_tecla(digito1_display) != 0) {
                            valor_almacenado /= codigo_tecla(digito1_display);
                        }
                        // Convertir el valor almacenado a su representación en los dígitos del display
                        digito1_display = numero(valor_almacenado % 10);
                        digito2_display = numero((valor_almacenado / 10) % 10);
                        digito3_display = numero((valor_almacenado / 100) % 10);
                        digito4_display = 0xFF; // Limpia el cuarto dígito
                        valor_almacenado = 0;   // Resetear el valor almacenado después de mostrarlo
                        break;

                    case KEY_E_INDEX:
                        // Resetear el valor almacenado y los dígitos del display
                        valor_almacenado = 0;
                        digito1_display = digito2_display = digito3_display = digito4_display = 0xFF;
                        break;

                    default:
                        // Desplazar los dígitos y mostrar el nuevo número en el dígito más a la derecha
                        digito4_display = digito3_display;
                        digito3_display = digito2_display;
                        digito2_display = digito1_display;
                        // Convertir el número de la tecla presionada a su representación en el display
                        digito1_display = numero(codigo_tecla(key_number));
                        break;

                    }

                    while (!(GPIOB->IDR & (1 << (row + 8)))); // Espera a que la tecla se suelte
                    break; // Sale del bucle de filas para evitar múltiples lecturas
                }
            }
            GPIOB->ODR |= (1 << (12 + col)); // Desactiva la columna actual antes de continuar
        }

        // Llama a la función act_valores para actualizar los valores mostrados en los displays.
        act_valores();

        // Actualiza valor_almacenado para mantenerlo dentro del rango de 0 a 999.
        // El uso del operador módulo (%) asegura que valor_almacenado no exceda 999,
        // lo cual puede ser útil para evitar desbordamientos o mantener el número dentro de un rango visible en un display limitado.
        valor_almacenado = valor_almacenado % 1000;
    }


}



// Función para crear un retardo de aproximadamente n milisegundos
void retraso(uint16_t n) {
    // Inicia un bucle que se repite n * 10 veces
    // El factor de 10 es un ajuste basado en la velocidad a la que se ejecuta el bucle
    // y la frecuencia de la CPU para aproximarse a un milisegundo por iteración de n
    for (uint32_t i = 0; i < n * 10; i++) {
        // La instrucción NOP (No OPeration) es una instrucción de ensamblador que no hace nada
        // Se utiliza aquí para consumir tiempo de CPU sin realizar ninguna operación
        __asm__("NOP");
    }
}



void act_valores(void) {
    // Itera a través de cada dígito posible (4 en total)
    for (int i = 0; i < 4; i++) {
        // Utiliza un switch para determinar cuál dígito se está procesando
        switch (i) {
            case 0: // Caso para el primer dígito
                // Verifica si el primer dígito debe mostrarse (no es 0xFF)
                if (digito1_display != 0xFF) {
                    // Apaga todos los dígitos antes de actualizar
                    GPIOC->BSRR = 1 << (5 + 16);
                    GPIOC->BSRR = 1 << (6 + 16);
                    GPIOC->BSRR = 1 << (8 + 16);
                    GPIOC->BSRR = 1 << (7 + 16);

                    // Enciende el primer dígito
                    GPIOC->BSRR = 1 << 5;
                    // Apaga todos los segmentos antes de mostrar el nuevo valor
                    GPIOB->BSRR = 0xFF << 16;
                    // Muestra el valor en el dígito actual
                    GPIOB->BSRR = decodificador(digito1_display);
                    // Retardo para permitir que el valor se muestre correctamente
                    retraso(5);
                }
                break;

            case 1: // Caso para el segundo dígito
                if (digito2_display != 0xFF) {
                    GPIOC->BSRR = 1 << (8 + 16);
                    GPIOC->BSRR = 1 << (5 + 16);
                    GPIOC->BSRR = 1 << 6;
                    GPIOB->BSRR = 0xFF << 16;
                    GPIOB->BSRR = decodificador(digito2_display);
                    retraso(5);
                }
                break;

            case 2: // Caso para el tercer dígito
                if (digito3_display != 0xFF) {
                    GPIOC->BSRR = 1 << (6 + 16);
                    GPIOC->BSRR = 1 << (5 + 16);
                    GPIOC->BSRR = 1 << 8;
                    GPIOB->BSRR = 0xFF << 16;
                    GPIOB->BSRR = decodificador(digito3_display);
                    retraso(5);
                }
                break;

            case 3: // Caso para el cuarto dígito
                if (digito4_display != 0xFF) {
                    GPIOC->BSRR = 1 << (5 + 16);
                    GPIOC->BSRR = 1 << (6 + 16);
                    GPIOC->BSRR = 1 << (8 + 16);
                    GPIOC->BSRR = 1 << 7;
                    GPIOB->BSRR = 0xFF << 16;
                    GPIOB->BSRR = decodificador(digito4_display);
                    retraso(5);
                }
                break;
        }
    }
}


// Función para decodificar un número en su representación para un display de 7 segmentos
uint8_t decodificador(uint8_t number) {
    // Inicializa la variable key que almacenará el valor decodificado
    uint8_t key = 0;

    // Usa un switch para mapear el número de entrada a su representación en el display de 7 segmentos
    switch (number) {
        case 0: key = 0; break; // Si el número es 0, asigna 0 a key
        case 1: key = 1; break; // Si el número es 1, asigna 1 a key
        case 2: key = 2; break; // Si el número es 2, asigna 2 a key
        case 4: key = 4; break; // Si el número es 4, asigna 4 a key
        case 5: key = 5; break; // Si el número es 5, asigna 5 a key
        case 6: key = 6; break; // Si el número es 6, asigna 6 a key
        case 8: key = 8; break; // Si el número es 8, asigna 8 a key
        case 9: key = 9; break; // Si el número es 9, asigna 9 a key
        case 10: key = 10; break; // Si el número es 10, asigna 10 a key
        case 13: key = 13; break; // Si el número es 13, asigna 13 a key
        default: return 13; // En cualquier otro caso, retorna 13 (probablemente un valor de error o indefinido)
    }

    // Retorna la representación del número para el display de 7 segmentos, usando el array key_to_display
    return key_to_display[key];
}




void decimal_decodificador(uint8_t value) {
    // Verifica si el valor almacenado es un dígito válido (excluyendo A, B, C, D, * y #)
    if (value < 10) {
        // Actualiza los displays con el valor almacenado
    	digito3_display = digito2_display;
        digito2_display = digito1_display; // Mueve el primer dígito al segundo dígito
        digito1_display = value; // Establece el primer dígito como el valor almacenado
    }
}




