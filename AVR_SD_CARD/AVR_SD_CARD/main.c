/*
 * AVR_SD_CARD.c
 *
 * Created: 19-09-2019 20:52:13
 * Author : jihas
 */ 
#define F_CPU 16000000UL
#define DRV_MMC 1
#include <avr/io.h>
#include <util/delay.h>
#include "ff.h"
#include "diskio.h"

FATFS fs;
FIL fil;
FRESULT fresult;
char buffer[256];
char buffer1[100];
UINT br,bw;	//file read/write count
//capacity related functions
FATFS *pfs;
DWORD fre_clust;
unsigned long total, free_space;


unsigned char temp;

void Serial_begin(uint32_t BAUDRATE){
	uint32_t BAUD_PRESCALLER;
	BAUD_PRESCALLER = (((F_CPU / (BAUDRATE * 16UL))) - 1);
	UBRR0H = (uint8_t)(BAUD_PRESCALLER>>8);
	UBRR0L = (uint8_t)(BAUD_PRESCALLER);
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);
	UCSR0C = (3<<UCSZ00);
}

unsigned char Serial_read(void){
	
	while(!(UCSR0A & (1<<RXC0)));
	return UDR0;
}

void Serial_write( unsigned char data){
	
	while(!(UCSR0A & (1<<UDRE0)));
	UDR0 = data;
}

void Serial_print(char* StringPtr){
	
	while(*StringPtr != 0x00){
		Serial_write(*StringPtr);
	StringPtr++;}
}

void Serial_println(char* StringPtr){
	while(*StringPtr != 0x00){
		Serial_write(*StringPtr);
	StringPtr++;}
	Serial_write(' ');
}

uint8_t Serial_available(){
	return ((UCSR0A & (1<<RXC0)) !=0);
}

int main(void)
{
    Serial_begin(9600);
	Serial_println("Staring...");
	_delay_ms(500);
	fresult=f_mount(&fs,"",0);
	if(fresult!=FR_OK)
	{
		Serial_println("error mounting sd card");
	}
	else
	{
		Serial_println("sd card mounted successfully");
		f_getfree("",&fre_clust,&pfs);
		total=(uint32_t)((pfs-> n_fatent-2) * pfs->csize * 0.5);
		sprintf(buffer,"Total:%lu KB\r\n",total);
		Serial_println(buffer);
		free_space=(uint32_t)(fre_clust * pfs->csize * 0.5);
		sprintf(buffer,"Free :%lu KB\r\n",free_space);
		Serial_println(buffer);
		_delay_ms(1000);
		if (f_open(&fil, "DATA.TXT", FA_WRITE | FA_OPEN_ALWAYS) == FR_OK) 
		{
			Serial_println("File created");
		}
		else
		{
			Serial_println("Error creating file");
		}
	}
    while (1) 
    {
    }
}

