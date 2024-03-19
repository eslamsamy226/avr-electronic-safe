/*
 * EEPROM.h
 *
 * Created: 08/02/2024 16:35:06
 *  Author: Eslam
 */ 


#ifndef EEPROM_H_
#define EEPROM_H_
#include "ports.h"
/*
Function Name        : EEPROM_write
Function Returns     : void
Function Arguments   : unsigned short address, unsigned char data
Function Description :  write one byte to the given  address.
*/
void EEPROM_write(unsigned short address, unsigned char data );
/*
Function Name        : EEPROM_read
Function Returns     : unsigned char
Function Arguments   :  unsigned short address
Function Description :  read one byte from the given  address.
*/
unsigned char EEPROM_read( unsigned short address );




#endif /* EEPROM_H_ */