/**
 ******************************************************************************
 * @file           : room_control.c
 * @author         : Sam C
 * @brief          : Room control driver for STM32L476RGTx
 ******************************************************************************
 */
#include "room_control.h"

#include "gpio.h"    // Para controlar LEDs y leer el botón (aunque el botón es por EXTI)
#include "systick.h" // Para obtener ticks y manejar retardos/tiempos
#include "uart.h"    // Para enviar mensajes
#include "tim.h"     // Para controlar el PWM

static volatile uint32_t press_time = 0;
volatile uint32_t tup_time = 0;

void room_control_app_init(void)
{   
   
// Inicializar variables de estado si es necesario.
    // Por ejemplo, asegurar que los LEDs estén apagados al inicio

    // tim3_ch1_pwm_set_duty_cycle(50); // Establecer un duty cycle inicial para el PWM LED
}

void room_control_on_button_press(void)
{
    // TODO: Implementar anti-rebote
    if ((systick_get_tick() - press_time) < 200){return;}

    press_time = systick_get_tick();
    gpio_write_pin(EXTERNAL_LED_ONOFF_PORT,EXTERNAL_LED_PWM_PIN,GPIO_PIN_SET);
    tup_time = systick_get_tick();
    uart2_send_string("Boton B1: Presionado.\r\n");
    // TODO: Procesar la presion para realizar acciones
    }


void room_control_on_uart_receive(char received_char)
{
    // TODO: Procesar el carácter para realizar acciones
    // Ejemplo: si recibe 'h' o 'H', encender el LED PWM al 100%.
    //          si recibe 'l' o 'L', apagar el LED PWM (0%).
    //          si recibe 't', hacer toggle del LED ON/OFF.
}

void ON_OFF_led_toggle(void)
{ if ((systick_get_tick() - tup_time) > 3000) 
    {gpio_write_pin(EXTERNAL_LED_ONOFF_PORT,EXTERNAL_LED_ONOFF_PIN,GPIO_PIN_RESET);
    tup_time = systick_get_tick(); } 
}