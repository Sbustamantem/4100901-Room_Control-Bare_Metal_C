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
//Contros Variables 
static volatile uint32_t press_time = 0;
volatile uint32_t tup_time = 0;
volatile uint32_t duty_value = 0;

void room_control_app_init(void)
{   
   gpio_write_pin(EXTERNAL_LED_ONOFF_PORT,EXTERNAL_LED_ONOFF_PIN,GPIO_PIN_RESET);
// Ensures That External Led ON/OFF Is Set To OFF
}

void room_control_on_button_press(void) //Actions To Do After Button Press Interruption
{
    // Debounce
    if ((systick_get_tick() - press_time) < 200){return;}
    press_time = systick_get_tick();
    //Turns ON The ON/OFF Led And Gets The Time When Its Done
    gpio_write_pin(EXTERNAL_LED_ONOFF_PORT,EXTERNAL_LED_ONOFF_PIN,GPIO_PIN_SET);
    tup_time = systick_get_tick();
    uart2_send_string("Boton B1: Presionado.\r\n");
    
    }

//Uart Character Interpretation
void room_control_on_uart_receive(char received_char)
{ switch (received_char)
//Handles Duty Cycle, And Toggles ON/OFF LED Acording To Recived Chr
{
case 'D':
    duty_value+= 10;    
    break;
case 'd':
    if (duty_value < 10) {duty_value = 100;}
    else{duty_value-= 10;}
    break;
case 'R':
    duty_value = 0;
    uart2_send_string("PWM Duty Set To 0\r\n");
    break;
case 'S':
    duty_value = 100;
    uart2_send_string("PWM Duty Set To 100\r\n");
    break;
case 's':
    gpio_write_pin(EXTERNAL_LED_ONOFF_PORT,EXTERNAL_LED_ONOFF_PIN,GPIO_PIN_SET);
    uart2_send_string("Led ON/OFF Set To ON\r\n");
    break;
case 'r':
    gpio_write_pin(EXTERNAL_LED_ONOFF_PORT,EXTERNAL_LED_ONOFF_PIN,GPIO_PIN_RESET);
    uart2_send_string("Led ON/OFF Set To OFF\r\n");
    break;
    default:
    uart2_send_string("Comando Desconocido\r\n");
    break;
}
 if(duty_value > 100  ){duty_value=0;}

tim3_ch1_pwm_set_duty_cycle(duty_value);

 }

//3 Seconds After Last Press Check Using The Time Gotten From Button Press
void ON_OFF_led_toggle(void)
{ 
 if ((tup_time != 0 && (systick_get_tick() - tup_time) >= 3000) ) 
    {gpio_write_pin(EXTERNAL_LED_ONOFF_PORT,EXTERNAL_LED_ONOFF_PIN,GPIO_PIN_RESET);
    uart2_send_string("Ext Led ON/OFF Apagado\r\n");
    tup_time = 0;
     } 
}