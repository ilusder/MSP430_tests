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
int main(void)
{
  WDTCTL = WDTPW | WDTHOLD;		// Stop watchdog timer
  PMMCTL0 = PMMPW;		        // Open PMM Module
  PM5CTL0 &= ~LOCKLPM5;			// Clear locked IO Pins
  P1DIR |= 0x01;                        // Set P1.0 to output direction
  P9DIR |= 0x80;                        // Set P9.7 to output direction
  P9OUT ^= 0x80;                      // Toggle P9.7 using exclusive-OR
  for (;;)
  {
    volatile unsigned int i;            // volatile to prevent optimization

    P1OUT ^= 0x01;                      // Toggle P1.0 using exclusive-OR
    P9OUT ^= 0x80;                      // Toggle P9.7 using exclusive-OR
    i = 10000;                          // SW Delay
    do i--;
    while (i != 0);
  }
}
