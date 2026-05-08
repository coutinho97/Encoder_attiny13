
/* INCLUDES */
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>

/* DEFINES */
#define PWM_IN_PIN    			PB1
#define ENC_A_PIN      			PB0
#define ENC_B_PIN      			PB2

/* 20%, 40%, 60%, 80% */
#define DIV_STATE_0  			51
#define DIV_STATE_1  			102
#define DIV_STATE_2  			154
#define DIV_STATE_3  			205

/* MACROS */
#define READ_PINB(pin)      	((PINB & (1 << pin)) != 0)
#define SET_PINB_HIGH(pin) 		(PORTB |= (1 << pin))
#define SET_PINB_LOW(pin)  		(PORTB &= ~(1 << pin))

/* */
#define SET_PIN_ENC_A_HIGH      SET_PINB_HIGH(ENC_A_PIN)
#define SET_PIN_ENC_A_LOW       SET_PINB_LOW(ENC_A_PIN)
#define SET_PIN_ENC_B_HIGH      SET_PINB_HIGH(ENC_B_PIN)
#define SET_PIN_ENC_B_LOW       SET_PINB_LOW(ENC_B_PIN)

/* FUNCTIONS */
void SetStatePins(uint8_t state)
{
    if (state & 0b01)   SET_PIN_ENC_A_HIGH;
	else                SET_PIN_ENC_A_LOW;

    if (state & 0b10)   SET_PIN_ENC_B_HIGH;
	else                SET_PIN_ENC_B_LOW;
}

void Timer0_Init()
{
    // Configura Timer0 em modo normal
    TCCR0A = 0;
    TCCR0B |= (1 << CS01);  // Prescaler 8
}

void PinChangeInterrupt_Init()
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
        // Subida, iniciar medição do tempo
        TCNT0 = 0;
        // Limpa flag interrupção
        TIFR0 |= (1 << TOV0);
    } 
	else 
	{
        // Descida - parar medição e calcular tempo
        high_time = TCNT0;
        
        if (high_time <= DIV_STATE_0) 
		{
            SetStatePins(0);
        } 
		else if (high_time <= DIV_STATE_1) 
		{
			SetStatePins(1);
        } 
		else if (high_time <= DIV_STATE_2) 
		{
			SetStatePins(2);
        } 
		else 
		{
            SetStatePins(3);
        }
    }
}

int main(void) 
{
	Timer0_Init();	
    PinChangeInterrupt_Init();

    while(1);
}
