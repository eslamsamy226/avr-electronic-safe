/*
 * electronic_safe.c
 *
 * Created: 25/02/2024 16:21:02
 *  Author: Eslam
 */ 

#define F_CPU 8000000UL
#include <avr/io.h>
#include "LCD.h"
#include "LED.h"
#include "keypad.h"
#include "Buzzer.h"
#include <util/delay.h>

#include "std_macros.h"
char pass[8]={};
char locked=0;
signed char trials=3;



void set_password(){
		for (char i=0;i<8;i++)
		{
			pass[i]=0xff;
		}
	char count=0;
	while(1)
	{
		char x=keypad_u8check_press();
		if (x!=0xff)
		{
			if (x=='.' && count)
			{
				_delay_ms(40);
				break;
			}else if(count<7)
			{
				LCD_vSend_char(x);
				pass[count]=x;
				EEPROM_write(count,x);
				count++;
				LCD_movecursor(2,count);
				_delay_ms(750);
				LCD_vSend_char('*');
				
			}
		}
	}
}
char get_password(){
	char count=0;
	char new_pass[8]={};
			for (char i=0;i<8;i++)
			{
				new_pass[i]=0xff;
			}
	while(1)
	{
		char x=keypad_u8check_press();
		if (x!=0xff)
		{
			if (x=='.' && count)
			{
				_delay_ms(40);
				break;
			}else if(count<7)
			{
				LCD_vSend_char(x);
				new_pass[count]=x;
				EEPROM_write(count,x);
				count++;
				LCD_movecursor(2,count);
				_delay_ms(750);
				LCD_vSend_char('*');
				
			}
		}
	}
	char yes=1;
	for (char i=0;i<8;i++)
	{
		yes&=(new_pass[i]==pass[i]);
	}
	return yes;
}
void unlock(){
	LCD_clearscreen();
	servo_vmove_to(90.0);
	LCD_movecursor(1,1);
	LCD_vSend_string("UNLOCKED");
	locked=0;

	Buzzer_vOff('D',2);
	LED_vTurnOn('D',0);
	LED_vTurnOff('D',1);

}
void lock(){
	
	servo_vmove_to(0);
	

	LCD_movecursor(1,1);

	locked=1;
	
	LED_vTurnOff('D',0);
	LED_vTurnOn('D',1);
}
void clr_pass()
{
		for (char i=0;i<8;i++){
		EEPROM_write(i,0xff);
		pass[i]=0xff;
		}		
	
}
int main(void)
{
	

	LCD_vInit();
	keypad_vInit();
	LED_vInit('D',0); //green
	LED_vInit('D',1); //red
	Buzzer_vInit('D',2);

	for (char i=0;i<8;i++){
		pass[i]=EEPROM_read(i);
		}
			
    locked=(pass[0]!=0xFF);
	while(1)
    {
	

        if(pass[0]==0xff)
		{
			unlock();
			LCD_clearscreen();
			LCD_vSend_string("SET PASSWORD : ");
			LCD_movecursor(2,1);
			set_password();
			unlock();
			
			trials=3;
		}else if(locked==1){
			lock();
			LCD_clearscreen();
			LCD_vSend_string("ENTER PASSWORD : ");
			if (get_password())
			{
				unlock();
				trials=3;
			}else{
				LCD_clearscreen();
				LCD_vSend_string("WRONG PASSWORD!!");
				lock();
				trials--;
				if (trials<=0)
				{
					Buzzer_vOn('D',2);
				}else if(trials>0){
					
					LCD_movecursor(2,1);
					LCD_vSend_char('0'+trials); LCD_vSend_string(" TRIALS LEFT...");				
				
				}	
						_delay_ms(2000);			
					
			}
	
		}else{
			
			LCD_movecursor(2,1);
			LCD_vSend_string("C->LOCK ");
			LCD_vSend_string("R->RESET");
			
			char p=keypad_u8check_press();
			if(p=='.')
			{
				lock();
			}
			if(p=='+')
			{
				_delay_ms(100);
				clr_pass();
				continue;
			}
		}
							
    }
}

