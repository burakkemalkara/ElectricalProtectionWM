/* 
 * File:   W2
 * Author: Burak Kemal Kara
 * MCU: 12F508
 * Date 08_01_2018
 * 
 * Despcription: practice of protection for overvoltage and 
 * undervoltage conditions applied to Inverter welding machines.
 */
#ifndef _XTAL_FREQ
#define _XTAL_FREQ 4000000 //4Mhz FRC internal osc. Calibration by linker.
#endif
#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#pragma config OSC = IntRC      // Oscillator Selection bits (internal RC oscillator)
#pragma config WDT = OFF        // Watchdog Timer Enable bit (WDT disabled)
#pragma config CP = ON          // Code Protection bit (Code protection on)
#pragma config MCLRE = OFF      // GP3/MCLR Pin Function Select bit (GP3/MCLR pin function is digital input, MCLR internally tied to VDD)
#define cut_PWM GP5
#define diagnostics_LED GP4
#define status_low_low GP3
#define status_low_high GP2
#define status_high_low GP1
#define status_high_high GP0

int main(int argc, char** argv) {
    /* Configuration of working options (bit 0-7, value 0/1)
        7: Not enabled (1): el "Wake-up on change bit"  
        6: Not enabled (1): "Weak Pull-ups" 
        5: Enabled internal oscillator (0): as clock source (Fosc/4)
        4: Edge selection for TMR0 (0): (lo to hi)
        3: Prescaler assigned to (0) TMR0
        2,1,0: (010) Prescaler 2,56 ms for each decimal unit
    */
    OPTION = 0b11000010;       
    /*
     Pinout 12F508
        1: VDD (2 to 5.5 Vdc)
        2: GP5  output to cut PWM (suppose 1 is pwm on)
        3: GP4  output to diagnostics led
        4: GP3  input low-low
        5: GP2  input low-high
        6: GP1  input high-low
        7: GP0  input high-high (suppose that 1 is overvoltage)
        8: VSS (0 Vdc)
     */
   TRISGPIO = 0b00001111; /* (1 input, 0 output)*/
   cut_PWM = 0;
   diagnostics_LED = 0;
   int check=1;
 
 while (1) { 
    diagnostics_LED = 1;
    if(status_high_high){ /*Voltage is greater than 253, system should be stop to prevent and damage */
		cut_PWM=1;
		check=0;
	}else if (status_high_low){ /*Voltage is between 253 and 240, İt can be OK but, good to stop if system is already unstable */
		if(check==0){
			cut_PWM=1;
			check=0;
		}else{
			cut_PWM=0;
			check=1;
		}
	}else if (status_low_high){ /*Voltage is between 240 and 220, system is running perfectly */
		cut_PWM=0;
		check=1;
	}else if (status_low_low){ /*Voltage is between 220 and 207, İt can be OK but, good to stop if system is already unstable */
		if(check==0){
			cut_PWM=1;
			check=0;
		}else{
			cut_PWM=0;
			check=1;
		}
	}else{ /*Voltage smaller than 207, System STOPS */
		cut_PWM=1;
		check=0;
		}
  }
 return (EXIT_SUCCESS);
}


     
