# Sistema de Operaciones con Keypad 4x4 en STM32

Este proyecto implementa un sistema de entrada numérica y operaciones básicas utilizando un microcontrolador STM32 y un teclado matriz 4x4. Permite realizar operaciones aritméticas básicas y visualizar números de hasta tres dígitos en un display.

## Características

- Entrada de números mediante un keypad 4x4.
- Operaciones de suma, multiplicación y división básicas.
- Visualización de resultados en displays de 7 segmentos controlados por GPIO.
- Restricción de entrada a un máximo de tres dígitos.

## Requisitos de Hardware

- Microcontrolador STM32.
- Teclado matriz 4x4.
- Displays de 7 segmentos.
- Resistencias y conexiones básicas para configuración de GPIO.

## Configuración del Hardware

1. **GPIOC y GPIOB:** Configurar los pines de estos puertos para la interacción con el teclado y los displays.
2. **Keypad 4x4:** Conectar las filas a los pines de entrada de GPIOB y las columnas a los pines de salida.
3. **Displays de 7 segmentos:** Conectar cada display a los pines específicos en GPIOC y GPIOB para control de segmentos y selección de dígitos.

## Estructuras de Datos Principales

- `GPIO_RegDef_t`: Define los registros para configurar los puertos GPIO.
- `RCC_RegDef_t`: Define los registros para configurar el controlador de reloj (RCC).

## Funciones Principales

- `retraso(uint16_t n)`: Genera un retardo aproximado en milisegundos.
- `act_valores(void)`: Actualiza los valores mostrados en los displays.
- `decodificador(uint8_t value_to_decode)`: Decodifica un valor para ser mostrado en un display de 7 segmentos.
- `decimal_decodificador(uint8_t value)`: Gestiona la lógica de desplazamiento de dígitos y visualización en el display.

## Uso

1. Al iniciar el sistema, todos los dígitos en los displays están apagados.
2. Al presionar una tecla en el keypad, el valor correspondiente se muestra en los displays.
3. Se pueden realizar operaciones de suma (tecla A), multiplicación (tecla C) y división (tecla F) según se detalla en la documentación del código.
