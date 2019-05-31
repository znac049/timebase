/*
  ATtiny85 timebase generator
  Version: 1.0
  Author: Bob Green (bob@wookey.org.uk)
  Created: 20190529
 
  Generate a clock with a period of 1.3333333s with an on time of ~200mS (TBC), accurate to within a few mS a day

  The output clock will drive an N channel FET (2N7000) which will switch a coil of approx 20R @ 1.5v = ~75mA
 
*/

#define F_CPU 32768 // 32.768 KHz clock

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

// ATtiny85 Pin map
//                        +-\/-+
// Reset/Ain0 (D 5) PB5  1|o   |8  Vcc
//  Xtal/Ain3 (D 3) PB3  2|    |7  PB2 (D 2) Ain1
//  Xtal/Ain2 (D 4) PB4  3|    |6  PB1 (D 1) pwm1
//                  GND  4|    |5  PB0 (D 0) pwm0 <-- connect to gate of FET
//                        +----+

#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif

#define coilPin PB0

uint8_t times[] = {170, 171, 171};
uint8_t timeInd = 0;

uint8_t endOfCycle = 0;

void init(void) {
  DDRB = (1<<coilPin); // Pin to drive the coil
  PORTB |= ((1<<PB2) | (1<<PB1)); // Turn on pull-up resistors on other ports to save power

  // Disable interrupts while we set things up
  cli();
	
  // Set prescaler to 256 
  sbi(TCCR1, CS13);
  sbi(TCCR1, CS10);
	
  // Enable timer 1 compare interrupts
  sbi(TIMSK, TOIE1); 

  // Enable interrupts
  sei();
}

int main(void) {
  init();

  while(1) {
    /*
     * SLEEP_MODE_IDLE is the deepest sleep level we can enter while still
     * being able to respond to timer interrupts
     */
    set_sleep_mode(SLEEP_MODE_IDLE);
    sleep_mode();

    if (endOfCycle) {
      endOfCycle = 0;

      
      timeInd++;
      if (timeInd >= sizeof(times)) {
	timeInd = 0;
      }
    }
  }
}

// Timer 1 compare interrupts

ISR(TIM1_COMPA_vect) {
  PORTB |= (1<<coilPin);
}

ISR(TIM1_COMPB_vect) {
  PORTB &= ~(1<<coilPin);
  endOfCycle = 1;
}
