# FatFS_AVR
Porting elm-chan's FatFS library for ATmega 2560

Coding Environment:- AtmelStudio 7.0

I have used the FatFS library in an STM32 project, It heped me lot. I found plenty of tutorials and video tutorials for implementing FatFS in STM32 with SPI inteface.

Then i got another project using Atmega2560 which also have SD card interface and file system requirement. I searched the internet for porting guide of FatFS for AVR. I found most of the implementations are outdated.

Then i decided to port the Library using My previous experience in FatFS with STM32.

I ported the code and build it with no errors. But it got some problems. Any helpis appreciated. 

# Steps I made

1.  Downloaded ffsample from ELM-CHAN's website http://elm-chan.org/
    ffsample link: http://elm-chan.org/fsw/ff/ffsample.zip
    The ffsample includes examples for different platforms including avr. But i don't know which programming environment is used. Because i     
    have started coding very recently and started avr coding in Atmel Studio.
2.  Created a new project in atmel studio and added necessary files from ffsample_avr.Here is the list of files i included
    ff.c 
    
    ff.h
    
    ffconf.h
    
    diskio.c
    
    diskio.h
    
    mmc_avr.h
    
    mmc_avr_spi.c
    
    ffunicode_avr.c
    
3.  In the main code included ff.h and diskio.h
4.  Included #define DRV_MMC 1  in code to interface SD card
5.  Switched to tiny buffer mode by Setting FF_FS_TINY value to 1 in ffconf.h

    #define FF_FS_TINY		1
6.  I'm not using any RTC and Time Stamp functions so i set FF_FS_NORTC value to 1 

    #define FF_FS_NORTC		1

7.  Created a timer interrupt for 100HZ timer opertion.
    ```
    void start_timer(void)
    {
	    /*Start 100Hz system timer with TC0 */
	    OCR0A = ((F_CPU / (2*1024) / 100) - 1;
	    TCCR0A = (1<<WGM01);//CTC mode
	    TCCR0B = 0b101; //Prescale = clock/1024
	    TIMSK0 = (1<<OCIE0A);//Timer/Counter0 Output Compare Match A Interrupt Enable
	    sei();
    }
    ```
    And the ISR
    ```
    ISR(TIMER0_COMPA_vect)
    {
    		//For Timing operations of the File system
		if(Timer1>0)
		{
			Timer1--;
		}
		if(Timer2>0)
		{
			Timer2--;
		}
    }
    ```
    Whenever Timer1 or Timer2 values have been changed, we need to decrement it at 100Hz rate

8.  adde
