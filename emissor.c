
/* INCLUDES */
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <stdbool.h>

/* DEFINES */
#define PWM_OUT_PIN     		PB0
#define ENC_A_PIN   			PB1
#define ENC_B_PIN   			PB2

/* 20%, 40%, 60%, 80% */
#define DIV_STATE_0  			51
#define DIV_STATE_1  			102
#define DIV_STATE_2  			154
#define DIV_STATE_3  			205

/* MACROS */
#define READ_PINB(pin)      	(PINB & (1 << pin))
#define SET_DUTY_CYCLE(duty)	(OCR0A = duty)


/* global variable */
static const uint8_t g_LUT_DUTY_STATE[4] =  
{DIV_STATE_0, DIV_STATE_1, DIV_STATE_2, DIV_STATE_3};

static volatile uint8_t g_state = 0;

/* FUNCTIONS */
void Timer0PWM_Init()
{
    // Configura o pino PWM como saída
    DDRB |= (1 << PWM_OUT_PIN);
    
    // Configura o Timer0 para Fast PWM, modo não-invertido
    TCCR0A |= (1 << WGM00) | (1 << WGM01) | (1 << COM0A1);
    
    // Prescaler de 8 para aproximar 5kHz
    TCCR0B |= (1 << CS01);
	
	// Habilita interrupção por comparação de saída (OCR0A)
    TIMSK0 |= (1 << OCIE0A);
}

void PinChangeInterrupt_Init() 
{
    // Configura pinos do encoder como entrada
    DDRB &= ~((1 << ENC_A_PIN) | (1 << ENC_B_PIN));

    // Habilita interrupção de mudança de pino
    GIMSK |= (1 << PCIE);
    PCMSK |= (1 << ENC_A_PIN) | (1 << ENC_B_PIN);

    // Habilita interrupções globais
    sei();
}

ISR(PCINT0_vect) 
{
    bool enc_a = READ_PINB(ENC_A_PIN);
    bool enc_b = READ_PINB(ENC_B_PIN);
	g_state = (enc_a << 1) | enc_b;
}

ISR(TIM0_COMPA_vect)
{
	SET_DUTY_CYCLE(g_LUT_DUTY_STATE[g_state]);
}

int main(void) 
{
    Timer0PWM_Init();
    PinChangeInterrupt_Init();

    while(1);
}
