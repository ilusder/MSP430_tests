//******************************************************************************

//    _____ _               _____                        _     _       
//   |_   _| |             |  __ \                      | |   (_)      
//     | | | |_   _  __ _  | |  | | ___ _ __ _   _  __ _| |__  _ _ __  
//     | | | | | | |/ _` | | |  | |/ _ \ '__| | | |/ _` | '_ \| | '_ \ 
//    _| |_| | |_| | (_| | | |__| |  __/ |  | |_| | (_| | |_) | | | | |
//   |_____|_|\__, |\__,_| |_____/ \___|_|   \__, |\__,_|_.__/|_|_| |_|
//             __/ |                          __/ |                    
//            |___/                          |___/                     

//******************************************************************************

#include "msp430.h"
#include <driverlib.h>
#include "hal_LCD.h"
#include <stdlib.h>

#define STARTUP_MODE         0

volatile unsigned char mode = STARTUP_MODE;

volatile unsigned int counter = 0;

volatile unsigned int timer = 0;

void fill_zero_LCD(void)
{
  showChar('0', pos1);
  showChar('0', pos2);
  showChar('0', pos3);
  showChar('0', pos4);
  showChar('0', pos5);
  showChar('0', pos6);
}

void show_int_LCD(int num)
{
  char buffer[6];
  int i;
  for (i = 0; i < 6; i++)
  {
    buffer[i] = num % 10 + 0x30;
    num /= 10;
  }
  showChar( buffer[5],pos1);
  showChar( buffer[4],pos2);
  showChar( buffer[3],pos3);
  showChar( buffer[2],pos4);
  showChar( buffer[1],pos5);
  showChar( buffer[0],pos6);
}

int main(void)
{
  WDTCTL = WDTPW | WDTHOLD;		// Stop watchdog timer
  PMMCTL0 = PMMPW;		        // Open PMM Module
  PM5CTL0 &= ~LOCKLPM5;			// Clear locked IO Pins
    //set timer
  //BCSCTL1 |= DIVA_3;    // divide by 8
  //BCSCTL3 |= XCAP_3;
  TA1CCTL0 = CM_0 + CCIS_0 + OUTMOD_0 + CCIE;
  TA1CCR0 = 8096;
  TA1CTL = TASSEL_1 + ID_2 + MC_1;
  
  TA0R = 0x0000;  
 TA0CTL = TASSEL__ACLK + MC__UP    + TACLR         + ID__1                + TAIE;
 TA0CCR0 = 32768;
  //_BIS_SR (LPM3_bits + GIE); 

  
      // Configure button S1 (P1.1) interrupt
    GPIO_selectInterruptEdge(GPIO_PORT_P1, GPIO_PIN1, GPIO_HIGH_TO_LOW_TRANSITION);
    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P1, GPIO_PIN1);
    GPIO_clearInterrupt(GPIO_PORT_P1, GPIO_PIN1);
    GPIO_enableInterrupt(GPIO_PORT_P1, GPIO_PIN1);
    // Configure button S2 (P1.2) interrupt
    GPIO_selectInterruptEdge(GPIO_PORT_P1, GPIO_PIN2, GPIO_HIGH_TO_LOW_TRANSITION);
    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P1, GPIO_PIN2);
    GPIO_clearInterrupt(GPIO_PORT_P1, GPIO_PIN2);
    GPIO_enableInterrupt(GPIO_PORT_P1, GPIO_PIN2);
        // Configure in (P1.6) interrupt
    GPIO_selectInterruptEdge(GPIO_PORT_P1, GPIO_PIN6, GPIO_HIGH_TO_LOW_TRANSITION);
    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P1, GPIO_PIN6);
    GPIO_clearInterrupt(GPIO_PORT_P1, GPIO_PIN6);
    GPIO_enableInterrupt(GPIO_PORT_P1, GPIO_PIN6);
  Init_LCD();
  displayScrollText("WELCOME TO LED SWITCH");
  GPIO_clearInterrupt(GPIO_PORT_P1, GPIO_PIN1);
  GPIO_clearInterrupt(GPIO_PORT_P1, GPIO_PIN2);
  GPIO_clearInterrupt(GPIO_PORT_P1, GPIO_PIN6);
  
  
   __enable_interrupt();
  P1DIR |= BIT0;                        // Set P1.0 to output direction
  P9DIR |= BIT7;                        // Set P9.7 to output direction
  P9OUT |= BIT7;                      //
  P1OUT &= ~BIT0;
  fill_zero_LCD();
  show_int_LCD(32);
  for (;;)
  {
    volatile unsigned int i;            // volatile to prevent optimization
    P1OUT ^= BIT0;                      // Toggle P1.0 using exclusive-OR
    P9OUT ^= BIT7;                      // Toggle P9.7 using exclusive-OR
    i = 50000;                          // SW Delay
    do i--;
    while (i != 0);
  }
}

//interupt every 1000ms
#pragma vector=TIMER1_A0_VECTOR
__interrupt void TIMER1_A0_ISR(void)
{
  //timer++;
}

/*
 * PORT1 Interrupt Service Routine
 * Handles S1 and S2 button press interrupts
 */
#pragma vector = PORT1_VECTOR
__interrupt void PORT1_ISR(void)
{
  switch(__even_in_range(P1IV, P1IV_P1IFG7))
  {
    case P1IV_NONE : break;
    case P1IV_P1IFG0 : break;
    case P1IV_P1IFG1 :  counter = 0; break;// Button S1 pressed
    case P1IV_P1IFG3 : break;
    case P1IV_P1IFG4 : break;
    case P1IV_P1IFG5 : break;
    case P1IV_P1IFG6 :  counter++; break;
    case P1IV_P1IFG7 : break;
    case P1IV_P1IFG2 :  counter++; break;    // Button S2 pressed
  }
  show_int_LCD(counter);
}


#pragma vector=TIMER0_A1_VECTOR
__interrupt void Timer (void) {
	if (TA0IV == 0x0E) 
        {
            timer++;
            show_int_LCD(timer);
        }
        
}

