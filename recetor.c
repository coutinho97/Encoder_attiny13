
/* INCLUDES */
#include <avr/io.h>
#include <avr/interrupt.h>


/* DEFINES */
#define PWM_IN_PIN    			PB1
#define ENC_A_PIN      			PB0
#define ENC_B_PIN      			PB2

// 20%, 40%, 60%, 80%
#define DIV_STATE_0  			51
#define DIV_STATE_1  			102
#define DIV_STATE_2  			154
#define DIV_STATE_3  			205


/* MACROS */
#define READ_PINB(pin)      	(PINB & (1 << pin))
#define SET_PINB_HIGH(pin) 		(PORTB |= (1 << pin))
#define SET_PINB_LOW(pin)  		(PORTB &= ~(1 << pin))


/* FUNCTIONS */
void timer_init() 
{
    // Configura Timer0 em modo normal
    TCCR0A = 0;
    TCCR0B |= (1 << CS01);  // Prescaler 8
}

void pin_change_interrupt_init() 
{
    // Configura PWM_IN_PIN como entrada
    DDRB &= ~(1 << PWM_IN_PIN);

    // Configura pinos de saída
    DDRB |= (1 << ENC_A_PIN) | (1 << ENC_B_PIN);

    // Habilita interrupção de mudança de pino
    GIMSK |= (1 << PCIE);
    PCMSK |= (1 << PWM_IN_PIN);

    // Habilita interrupções globais
    sei();
}

ISR(PCINT0_vect) 
{
	static volatile uint8_t high_time = 0;
	
    if (READ_PINB(PWM_IN_PIN)) 
	{
        // Subida - iniciar medição do tempo
        TCNT0 = 0;
        TIFR0 |= (1 << TOV0); // Limpa flag de estouro
    } 
	else 
	{
        // Descida - parar medição e calcular tempo
        high_time = TCNT0;
        
        if (high_time <= DIV_STATE_0) 
		{
            SET_PINB_LOW(ENC_A_PIN);
			SET_PINB_LOW(ENC_B_PIN);
        } 
		else if (high_time <= DIV_STATE_1) 
		{
			SET_PINB_HIGH(ENC_A_PIN);
            SET_PINB_LOW(ENC_B_PIN);
        } 
		else if (high_time <= DIV_STATE_2) 
		{
			SET_PINB_LOW(ENC_A_PIN);
			SET_PINB_HIGH(ENC_B_PIN);
        } 
		else 
		{
            SET_PINB_HIGH(ENC_A_PIN);
			SET_PINB_HIGH(ENC_B_PIN);
        }
    }
}

int main(void) 
{
	timer_init();	
    pin_change_interrupt_init();
    
    while (1) 
	{
        // infinite loop
    }
}
