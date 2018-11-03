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
  Init_LCD();
  displayScrollText("WELCOME TO LED SWITCH");
  //rtc_init()
  P1DIR |= 0x01;                        // Set P1.0 to output direction
  P9DIR |= 0x80;                        // Set P9.7 to output direction
  P9OUT ^= 0x80;                      // 
  fill_zero_LCD();
  show_int_LCD(32);
  for (;;)
  {
    volatile unsigned int i;            // volatile to prevent optimization

    P1OUT ^= 0x01;                      // Toggle P1.0 using exclusive-OR
    i = 50000;                          // SW Delay
    do i--;
    while (i != 0);
    P9OUT ^= 0x80;                      // Toggle P9.7 using exclusive-OR
  }
}


