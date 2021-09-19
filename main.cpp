/*
	@File 		nokia5110.cpp
	@Author		JOSIMAR PEREIRA LEITE
	@country	Brazil
	@Date		07/06/2021
	

    Copyright (C) 2021  JOSIMAR PEREIRA LEITE

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
*/ 

#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>

#include "nokia5110.h"
#include "i2c.h"

NOKIA5110 nokia5110;

#define DS1621_SLA_W					0x90
#define DS1621_SLA_R					0x91

#define DS1621_READ_TEMP				0xAA
#define DS1621_ACCESS_TH				0xA1
#define DS1621_ACCESS_TL				0xA2
#define DS1621_ACCESS_CONFIG			0xAC
#define DS1621_READ_COUNTER				0xA8
#define DS1621_READ_SLOPE				0xA9
#define DS1621_START_CONVET				0xEE
#define DS1621_STOP_CONVET				0x22

void DS1621_start()
{
	i2c_start();
	i2c_write(DS1621_SLA_W);
	i2c_write(DS1621_START_CONVET);
	i2c_stop();
}

void DS1621_config(char data)
{
	i2c_start();
	i2c_write(DS1621_SLA_W);
	i2c_write(DS1621_ACCESS_CONFIG);
	i2c_write(data);
	i2c_stop();
}


char DS1621_read()
{
	DS1621_start();
	
	DS1621_config(0x00);
	
	i2c_start();
	i2c_write(DS1621_SLA_W);
	i2c_write(DS1621_READ_TEMP);
	i2c_start();
	i2c_write(DS1621_SLA_R);
	char temp = i2c_nack();
	i2c_stop();
	
	return temp;
}

int main()
{
	i2c_init( F_CPU );
	
	nokia5110.Init();
	
	while(1)
	{
		char read = DS1621_read();
		
		char c[7];
		sprintf(c,"Temp: %d",read);
		
		nokia5110.WriteString(c,0,0,Font_Arial_16x16);
		
		_delay_ms(100);
	}
}