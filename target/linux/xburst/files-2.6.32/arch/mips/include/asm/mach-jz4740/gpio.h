/*
 *  Copyright (C) 2009, Lars-Peter Clausen <lars@metafoo.de>
 *  	JZ7420/JZ4740 GPIO pin definitions
 *
 *  This program is free software; you can redistribute	 it and/or modify it
 *  under  the terms of	 the GNU General  Public License as published by the
 *  Free Software Foundation;  either version 2 of the	License, or (at your
 *  option) any later version.
 *
 *  You should have received a copy of the  GNU General Public License along
 *  with this program; if not, write  to the Free Software Foundation, Inc.,
 *  675 Mass Ave, Cambridge, MA 02139, USA.
 *
 */

#ifndef _JZ_GPIO_H
#define _JZ_GPIO_H

#include <linux/types.h>

enum jz_gpio_function {
    JZ_GPIO_FUNC_NONE,
    JZ_GPIO_FUNC1,
    JZ_GPIO_FUNC2,
    JZ_GPIO_FUNC3,
};


/*
 Usually a driver for a SoC component has to request several gpio pins and
 configure them as funcion pins.
 jz_gpio_bulk_request can be used to ease this process.
 Usually one would do something like:

 const static struct jz_gpio_bulk_request i2c_pins[] = {
	JZ_GPIO_BULK_PIN(I2C_SDA),
	JZ_GPIO_BULK_PIN(I2C_SCK),
 };

 inside the probe function:

    ret = jz_gpio_bulk_request(i2c_pins, ARRAY_SIZE(i2c_pins));
    if (ret) {
	...

 inside the remove function:

    jz_gpio_bulk_free(i2c_pins, ARRAY_SIZE(i2c_pins));


*/
struct jz_gpio_bulk_request {
	int gpio;
	const char *name;
	enum jz_gpio_function function;
};

#define JZ_GPIO_BULK_PIN(pin) { \
    .gpio = JZ_GPIO_ ## pin, \
    .name = #pin, \
    .function = JZ_GPIO_FUNC_ ## pin \
}

int jz_gpio_bulk_request(const struct jz_gpio_bulk_request *request, size_t num);
void jz_gpio_bulk_free(const struct jz_gpio_bulk_request *request, size_t num);
void jz_gpio_bulk_suspend(const struct jz_gpio_bulk_request *request, size_t num);
void jz_gpio_bulk_resume(const struct jz_gpio_bulk_request *request, size_t num);
void jz_gpio_enable_pullup(unsigned gpio);
void jz_gpio_disable_pullup(unsigned gpio);
int jz_gpio_set_function(int gpio, enum jz_gpio_function function);

int jz_gpio_port_direction_input(int port, uint32_t mask);
int jz_gpio_port_direction_output(int port, uint32_t mask);
void jz_gpio_port_set_value(int port, uint32_t value, uint32_t mask);
uint32_t jz_gpio_port_get_value(int port, uint32_t mask);

#include <asm/mach-generic/gpio.h>

#define JZ_GPIO_PORTA(x) (x + 32 * 0)
#define JZ_GPIO_PORTB(x) (x + 32 * 1)
#define JZ_GPIO_PORTC(x) (x + 32 * 2)
#define JZ_GPIO_PORTD(x) (x + 32 * 3)

/* Port A function pins */
#define JZ_GPIO_MEM_DATA0		JZ_GPIO_PORTA(0)
#define JZ_GPIO_MEM_DATA1		JZ_GPIO_PORTA(1)
#define JZ_GPIO_MEM_DATA2		JZ_GPIO_PORTA(2)
#define JZ_GPIO_MEM_DATA3		JZ_GPIO_PORTA(3)
#define JZ_GPIO_MEM_DATA4		JZ_GPIO_PORTA(4)
#define JZ_GPIO_MEM_DATA5		JZ_GPIO_PORTA(5)
#define JZ_GPIO_MEM_DATA6		JZ_GPIO_PORTA(6)
#define JZ_GPIO_MEM_DATA7		JZ_GPIO_PORTA(7)
#define JZ_GPIO_MEM_DATA8		JZ_GPIO_PORTA(8)
#define JZ_GPIO_MEM_DATA9		JZ_GPIO_PORTA(9)
#define JZ_GPIO_MEM_DATA10		JZ_GPIO_PORTA(10)
#define JZ_GPIO_MEM_DATA11		JZ_GPIO_PORTA(11)
#define JZ_GPIO_MEM_DATA12		JZ_GPIO_PORTA(12)
#define JZ_GPIO_MEM_DATA13		JZ_GPIO_PORTA(13)
#define JZ_GPIO_MEM_DATA14		JZ_GPIO_PORTA(14)
#define JZ_GPIO_MEM_DATA15		JZ_GPIO_PORTA(15)
#define JZ_GPIO_MEM_DATA16		JZ_GPIO_PORTA(16)
#define JZ_GPIO_MEM_DATA17		JZ_GPIO_PORTA(17)
#define JZ_GPIO_MEM_DATA18		JZ_GPIO_PORTA(18)
#define JZ_GPIO_MEM_DATA19		JZ_GPIO_PORTA(19)
#define JZ_GPIO_MEM_DATA20		JZ_GPIO_PORTA(20)
#define JZ_GPIO_MEM_DATA21		JZ_GPIO_PORTA(21)
#define JZ_GPIO_MEM_DATA22		JZ_GPIO_PORTA(22)
#define JZ_GPIO_MEM_DATA23		JZ_GPIO_PORTA(23)
#define JZ_GPIO_MEM_DATA24		JZ_GPIO_PORTA(24)
#define JZ_GPIO_MEM_DATA25		JZ_GPIO_PORTA(25)
#define JZ_GPIO_MEM_DATA26		JZ_GPIO_PORTA(26)
#define JZ_GPIO_MEM_DATA27		JZ_GPIO_PORTA(27)
#define JZ_GPIO_MEM_DATA28		JZ_GPIO_PORTA(28)
#define JZ_GPIO_MEM_DATA29		JZ_GPIO_PORTA(29)
#define JZ_GPIO_MEM_DATA30		JZ_GPIO_PORTA(30)
#define JZ_GPIO_MEM_DATA31		JZ_GPIO_PORTA(31)

#define JZ_GPIO_FUNC_MEM_DATA0		JZ_GPIO_FUNC1
#define JZ_GPIO_FUNC_MEM_DATA1		JZ_GPIO_FUNC1
#define JZ_GPIO_FUNC_MEM_DATA2		JZ_GPIO_FUNC1
#define JZ_GPIO_FUNC_MEM_DATA3		JZ_GPIO_FUNC1
#define JZ_GPIO_FUNC_MEM_DATA4		JZ_GPIO_FUNC1
#define JZ_GPIO_FUNC_MEM_DATA5		JZ_GPIO_FUNC1
#define JZ_GPIO_FUNC_MEM_DATA6		JZ_GPIO_FUNC1
#define JZ_GPIO_FUNC_MEM_DATA7		JZ_GPIO_FUNC1
#define JZ_GPIO_FUNC_MEM_DATA8		JZ_GPIO_FUNC1
#define JZ_GPIO_FUNC_MEM_DATA9		JZ_GPIO_FUNC1
#define JZ_GPIO_FUNC_MEM_DATA10		JZ_GPIO_FUNC1
#define JZ_GPIO_FUNC_MEM_DATA11		JZ_GPIO_FUNC1
#define JZ_GPIO_FUNC_MEM_DATA12		JZ_GPIO_FUNC1
#define JZ_GPIO_FUNC_MEM_DATA13		JZ_GPIO_FUNC1
#define JZ_GPIO_FUNC_MEM_DATA14		JZ_GPIO_FUNC1
#define JZ_GPIO_FUNC_MEM_DATA15		JZ_GPIO_FUNC1
#define JZ_GPIO_FUNC_MEM_DATA16		JZ_GPIO_FUNC1
#define JZ_GPIO_FUNC_MEM_DATA17		JZ_GPIO_FUNC1
#define JZ_GPIO_FUNC_MEM_DATA18		JZ_GPIO_FUNC1
#define JZ_GPIO_FUNC_MEM_DATA19		JZ_GPIO_FUNC1
#define JZ_GPIO_FUNC_MEM_DATA20		JZ_GPIO_FUNC1
#define JZ_GPIO_FUNC_MEM_DATA21		JZ_GPIO_FUNC1
#define JZ_GPIO_FUNC_MEM_DATA22		JZ_GPIO_FUNC1
#define JZ_GPIO_FUNC_MEM_DATA23		JZ_GPIO_FUNC1
#define JZ_GPIO_FUNC_MEM_DATA24		JZ_GPIO_FUNC1
#define JZ_GPIO_FUNC_MEM_DATA25		JZ_GPIO_FUNC1
#define JZ_GPIO_FUNC_MEM_DATA26		JZ_GPIO_FUNC1
#define JZ_GPIO_FUNC_MEM_DATA27		JZ_GPIO_FUNC1
#define JZ_GPIO_FUNC_MEM_DATA28		JZ_GPIO_FUNC1
#define JZ_GPIO_FUNC_MEM_DATA29		JZ_GPIO_FUNC1
#define JZ_GPIO_FUNC_MEM_DATA30		JZ_GPIO_FUNC1
#define JZ_GPIO_FUNC_MEM_DATA31		JZ_GPIO_FUNC1

/* Port B function pins */
#define JZ_GPIO_MEM_ADDR0		JZ_GPIO_PORTB(0)
#define JZ_GPIO_MEM_ADDR1		JZ_GPIO_PORTB(1)
#define JZ_GPIO_MEM_ADDR2		JZ_GPIO_PORTB(2)
#define JZ_GPIO_MEM_ADDR3		JZ_GPIO_PORTB(3)
#define JZ_GPIO_MEM_ADDR4		JZ_GPIO_PORTB(4)
#define JZ_GPIO_MEM_ADDR5		JZ_GPIO_PORTB(5)
#define JZ_GPIO_MEM_ADDR6		JZ_GPIO_PORTB(6)
#define JZ_GPIO_MEM_ADDR7		JZ_GPIO_PORTB(7)
#define JZ_GPIO_MEM_ADDR8		JZ_GPIO_PORTB(8)
#define JZ_GPIO_MEM_ADDR9		JZ_GPIO_PORTB(9)
#define JZ_GPIO_MEM_ADDR10		JZ_GPIO_PORTB(10)
#define JZ_GPIO_MEM_ADDR11		JZ_GPIO_PORTB(11)
#define JZ_GPIO_MEM_ADDR12		JZ_GPIO_PORTB(12)
#define JZ_GPIO_MEM_ADDR13		JZ_GPIO_PORTB(13)
#define JZ_GPIO_MEM_ADDR14		JZ_GPIO_PORTB(14)
#define JZ_GPIO_MEM_ADDR15		JZ_GPIO_PORTB(15)
#define JZ_GPIO_MEM_ADDR16		JZ_GPIO_PORTB(16)
#define JZ_GPIO_MEM_CLS			JZ_GPIO_PORTB(17)
#define JZ_GPIO_MEM_SPL			JZ_GPIO_PORTB(18)
#define JZ_GPIO_MEM_DCS			JZ_GPIO_PORTB(19)
#define JZ_GPIO_MEM_RAS			JZ_GPIO_PORTB(20)
#define JZ_GPIO_MEM_CAS			JZ_GPIO_PORTB(21)
#define JZ_GPIO_MEM_SDWE		JZ_GPIO_PORTB(22)
#define JZ_GPIO_MEM_CKE			JZ_GPIO_PORTB(23)
#define JZ_GPIO_MEM_CKO			JZ_GPIO_PORTB(24)
#define JZ_GPIO_MEM_CS0			JZ_GPIO_PORTB(25)
#define JZ_GPIO_MEM_CS1			JZ_GPIO_PORTB(26)
#define JZ_GPIO_MEM_CS2			JZ_GPIO_PORTB(27)
#define JZ_GPIO_MEM_CS3			JZ_GPIO_PORTB(28)
#define JZ_GPIO_MEM_RD			JZ_GPIO_PORTB(29)
#define JZ_GPIO_MEM_WR			JZ_GPIO_PORTB(30)
#define JZ_GPIO_MEM_WE0			JZ_GPIO_PORTB(31)

#define JZ_GPIO_FUNC_MEM_ADDR0		JZ_GPIO_FUNC1
#define JZ_GPIO_FUNC_MEM_ADDR1		JZ_GPIO_FUNC1
#define JZ_GPIO_FUNC_MEM_ADDR2		JZ_GPIO_FUNC1
#define JZ_GPIO_FUNC_MEM_ADDR3		JZ_GPIO_FUNC1
#define JZ_GPIO_FUNC_MEM_ADDR4		JZ_GPIO_FUNC1
#define JZ_GPIO_FUNC_MEM_ADDR5		JZ_GPIO_FUNC1
#define JZ_GPIO_FUNC_MEM_ADDR6		JZ_GPIO_FUNC1
#define JZ_GPIO_FUNC_MEM_ADDR7		JZ_GPIO_FUNC1
#define JZ_GPIO_FUNC_MEM_ADDR8		JZ_GPIO_FUNC1
#define JZ_GPIO_FUNC_MEM_ADDR9		JZ_GPIO_FUNC1
#define JZ_GPIO_FUNC_MEM_ADDR10		JZ_GPIO_FUNC1
#define JZ_GPIO_FUNC_MEM_ADDR11		JZ_GPIO_FUNC1
#define JZ_GPIO_FUNC_MEM_ADDR12		JZ_GPIO_FUNC1
#define JZ_GPIO_FUNC_MEM_ADDR13		JZ_GPIO_FUNC1
#define JZ_GPIO_FUNC_MEM_ADDR14		JZ_GPIO_FUNC1
#define JZ_GPIO_FUNC_MEM_ADDR15		JZ_GPIO_FUNC1
#define JZ_GPIO_FUNC_MEM_ADDR16		JZ_GPIO_FUNC1
#define JZ_GPIO_FUNC_MEM_CLS	        JZ_GPIO_FUNC1
#define JZ_GPIO_FUNC_MEM_SPL		JZ_GPIO_FUNC1
#define JZ_GPIO_FUNC_MEM_DCS		JZ_GPIO_FUNC1
#define JZ_GPIO_FUNC_MEM_RAS		JZ_GPIO_FUNC1
#define JZ_GPIO_FUNC_MEM_CAS		JZ_GPIO_FUNC1
#define JZ_GPIO_FUNC_MEM_SDWE		JZ_GPIO_FUNC1
#define JZ_GPIO_FUNC_MEM_CKE		JZ_GPIO_FUNC1
#define JZ_GPIO_FUNC_MEM_CKO		JZ_GPIO_FUNC1
#define JZ_GPIO_FUNC_MEM_CS0		JZ_GPIO_FUNC1
#define JZ_GPIO_FUNC_MEM_CS1		JZ_GPIO_FUNC1
#define JZ_GPIO_FUNC_MEM_CS2		JZ_GPIO_FUNC1
#define JZ_GPIO_FUNC_MEM_CS3		JZ_GPIO_FUNC1
#define JZ_GPIO_FUNC_MEM_RD		JZ_GPIO_FUNC1
#define JZ_GPIO_FUNC_MEM_WR		JZ_GPIO_FUNC1
#define JZ_GPIO_FUNC_MEM_WE0		JZ_GPIO_FUNC1


#define JZ_GPIO_MEM_ADDR21		JZ_GPIO_PORTB(17)
#define JZ_GPIO_MEM_ADDR22		JZ_GPIO_PORTB(18)

#define JZ_GPIO_FUNC_MEM_ADDR21		JZ_GPIO_FUNC2
#define JZ_GPIO_FUNC_MEM_ADDR22		JZ_GPIO_FUNC2

/* Port C function pins */
#define JZ_GPIO_LCD_DATA0		JZ_GPIO_PORTC(0)
#define JZ_GPIO_LCD_DATA1		JZ_GPIO_PORTC(1)
#define JZ_GPIO_LCD_DATA2		JZ_GPIO_PORTC(2)
#define JZ_GPIO_LCD_DATA3		JZ_GPIO_PORTC(3)
#define JZ_GPIO_LCD_DATA4		JZ_GPIO_PORTC(4)
#define JZ_GPIO_LCD_DATA5		JZ_GPIO_PORTC(5)
#define JZ_GPIO_LCD_DATA6		JZ_GPIO_PORTC(6)
#define JZ_GPIO_LCD_DATA7		JZ_GPIO_PORTC(7)
#define JZ_GPIO_LCD_DATA8		JZ_GPIO_PORTC(8)
#define JZ_GPIO_LCD_DATA9		JZ_GPIO_PORTC(9)
#define JZ_GPIO_LCD_DATA10		JZ_GPIO_PORTC(10)
#define JZ_GPIO_LCD_DATA11		JZ_GPIO_PORTC(11)
#define JZ_GPIO_LCD_DATA12		JZ_GPIO_PORTC(12)
#define JZ_GPIO_LCD_DATA13		JZ_GPIO_PORTC(13)
#define JZ_GPIO_LCD_DATA14		JZ_GPIO_PORTC(14)
#define JZ_GPIO_LCD_DATA15		JZ_GPIO_PORTC(15)
#define JZ_GPIO_LCD_DATA16		JZ_GPIO_PORTC(16)
#define JZ_GPIO_LCD_DATA17		JZ_GPIO_PORTC(17)
#define JZ_GPIO_LCD_PCLK		JZ_GPIO_PORTC(18)
#define JZ_GPIO_LCD_HSYNC		JZ_GPIO_PORTC(19)
#define JZ_GPIO_LCD_VSYNC		JZ_GPIO_PORTC(20)
#define JZ_GPIO_LCD_DE			JZ_GPIO_PORTC(21)
#define JZ_GPIO_LCD_PS			JZ_GPIO_PORTC(22)
#define JZ_GPIO_LCD_REV			JZ_GPIO_PORTC(23)
#define JZ_GPIO_MEM_WE1			JZ_GPIO_PORTC(24)
#define JZ_GPIO_MEM_WE2			JZ_GPIO_PORTC(25)
#define JZ_GPIO_MEM_WE3			JZ_GPIO_PORTC(26)
#define JZ_GPIO_MEM_WAIT		JZ_GPIO_PORTC(27)
#define JZ_GPIO_MEM_FRE			JZ_GPIO_PORTC(28)
#define JZ_GPIO_MEM_FWE			JZ_GPIO_PORTC(29)

#define JZ_GPIO_FUNC_LCD_DATA0		JZ_GPIO_FUNC1
#define JZ_GPIO_FUNC_LCD_DATA1		JZ_GPIO_FUNC1
#define JZ_GPIO_FUNC_LCD_DATA2		JZ_GPIO_FUNC1
#define JZ_GPIO_FUNC_LCD_DATA3		JZ_GPIO_FUNC1
#define JZ_GPIO_FUNC_LCD_DATA4		JZ_GPIO_FUNC1
#define JZ_GPIO_FUNC_LCD_DATA5		JZ_GPIO_FUNC1
#define JZ_GPIO_FUNC_LCD_DATA6		JZ_GPIO_FUNC1
#define JZ_GPIO_FUNC_LCD_DATA7		JZ_GPIO_FUNC1
#define JZ_GPIO_FUNC_LCD_DATA8		JZ_GPIO_FUNC1
#define JZ_GPIO_FUNC_LCD_DATA9		JZ_GPIO_FUNC1
#define JZ_GPIO_FUNC_LCD_DATA10		JZ_GPIO_FUNC1
#define JZ_GPIO_FUNC_LCD_DATA11		JZ_GPIO_FUNC1
#define JZ_GPIO_FUNC_LCD_DATA12		JZ_GPIO_FUNC1
#define JZ_GPIO_FUNC_LCD_DATA13		JZ_GPIO_FUNC1
#define JZ_GPIO_FUNC_LCD_DATA14		JZ_GPIO_FUNC1
#define JZ_GPIO_FUNC_LCD_DATA15		JZ_GPIO_FUNC1
#define JZ_GPIO_FUNC_LCD_DATA16		JZ_GPIO_FUNC1
#define JZ_GPIO_FUNC_LCD_DATA17		JZ_GPIO_FUNC1
#define JZ_GPIO_FUNC_LCD_PCLK		JZ_GPIO_FUNC1
#define JZ_GPIO_FUNC_LCD_VSYNC		JZ_GPIO_FUNC1
#define JZ_GPIO_FUNC_LCD_HSYNC		JZ_GPIO_FUNC1
#define JZ_GPIO_FUNC_LCD_DE		JZ_GPIO_FUNC1
#define JZ_GPIO_FUNC_LCD_PS		JZ_GPIO_FUNC1
#define JZ_GPIO_FUNC_LCD_REV		JZ_GPIO_FUNC1
#define JZ_GPIO_FUNC_MEM_WE1		JZ_GPIO_FUNC1
#define JZ_GPIO_FUNC_MEM_WE2		JZ_GPIO_FUNC1
#define JZ_GPIO_FUNC_MEM_WE3		JZ_GPIO_FUNC1
#define JZ_GPIO_FUNC_MEM_WAIT		JZ_GPIO_FUNC1
#define JZ_GPIO_FUNC_MEM_FRE		JZ_GPIO_FUNC1
#define JZ_GPIO_FUNC_MEM_FWE		JZ_GPIO_FUNC1


#define JZ_GPIO_MEM_ADDR19		JZ_GPIO_PORTB(22)
#define JZ_GPIO_MEM_ADDR20		JZ_GPIO_PORTB(23)

#define JZ_GPIO_FUNC_MEM_ADDR19		JZ_GPIO_FUNC2
#define JZ_GPIO_FUNC_MEM_ADDR20		JZ_GPIO_FUNC2

/* Port D function pins */
#define JZ_GPIO_CIM_DATA0		JZ_GPIO_PORTD(0)
#define JZ_GPIO_CIM_DATA1		JZ_GPIO_PORTD(1)
#define JZ_GPIO_CIM_DATA2		JZ_GPIO_PORTD(2)
#define JZ_GPIO_CIM_DATA3		JZ_GPIO_PORTD(3)
#define JZ_GPIO_CIM_DATA4		JZ_GPIO_PORTD(4)
#define JZ_GPIO_CIM_DATA5		JZ_GPIO_PORTD(5)
#define JZ_GPIO_CIM_DATA6		JZ_GPIO_PORTD(6)
#define JZ_GPIO_CIM_DATA7		JZ_GPIO_PORTD(7)
#define JZ_GPIO_MSC_CMD			JZ_GPIO_PORTD(8)
#define JZ_GPIO_MSC_CLK			JZ_GPIO_PORTD(9)
#define JZ_GPIO_MSC_DATA0		JZ_GPIO_PORTD(10)
#define JZ_GPIO_MSC_DATA1		JZ_GPIO_PORTD(11)
#define JZ_GPIO_MSC_DATA2		JZ_GPIO_PORTD(12)
#define JZ_GPIO_MSC_DATA3		JZ_GPIO_PORTD(13)
#define JZ_GPIO_CIM_MCLK		JZ_GPIO_PORTD(14)
#define JZ_GPIO_CIM_PCLK		JZ_GPIO_PORTD(15)
#define JZ_GPIO_CIM_VSYNC		JZ_GPIO_PORTD(16)
#define JZ_GPIO_CIM_HSYNC		JZ_GPIO_PORTD(17)
#define JZ_GPIO_SPI_CLK			JZ_GPIO_PORTD(18)
#define JZ_GPIO_SPI_CE0			JZ_GPIO_PORTD(19)
#define JZ_GPIO_SPI_DT			JZ_GPIO_PORTD(20)
#define JZ_GPIO_SPI_DR			JZ_GPIO_PORTD(21)
#define JZ_GPIO_SPI_CE1			JZ_GPIO_PORTD(22)
#define JZ_GPIO_PWM0			JZ_GPIO_PORTD(23)
#define JZ_GPIO_PWM1			JZ_GPIO_PORTD(24)
#define JZ_GPIO_PWM2			JZ_GPIO_PORTD(25)
#define JZ_GPIO_PWM3			JZ_GPIO_PORTD(26)
#define JZ_GPIO_PWM4			JZ_GPIO_PORTD(27)
#define JZ_GPIO_PWM5			JZ_GPIO_PORTD(28)
#define JZ_GPIO_PWM6			JZ_GPIO_PORTD(30)
#define JZ_GPIO_PWM7			JZ_GPIO_PORTD(31)

#define JZ_GPIO_FUNC_CIM_DATA0		JZ_GPIO_FUNC1
#define JZ_GPIO_FUNC_CIM_DATA1		JZ_GPIO_FUNC1
#define JZ_GPIO_FUNC_CIM_DATA2		JZ_GPIO_FUNC1
#define JZ_GPIO_FUNC_CIM_DATA3		JZ_GPIO_FUNC1
#define JZ_GPIO_FUNC_CIM_DATA4		JZ_GPIO_FUNC1
#define JZ_GPIO_FUNC_CIM_DATA5		JZ_GPIO_FUNC1
#define JZ_GPIO_FUNC_CIM_DATA6		JZ_GPIO_FUNC1
#define JZ_GPIO_FUNC_CIM_DATA7		JZ_GPIO_FUNC1
#define JZ_GPIO_FUNC_MSC_CMD		JZ_GPIO_FUNC1
#define JZ_GPIO_FUNC_MSC_CLK		JZ_GPIO_FUNC1
#define JZ_GPIO_FUNC_MSC_DATA0		JZ_GPIO_FUNC1
#define JZ_GPIO_FUNC_MSC_DATA1		JZ_GPIO_FUNC1
#define JZ_GPIO_FUNC_MSC_DATA2		JZ_GPIO_FUNC1
#define JZ_GPIO_FUNC_MSC_DATA3		JZ_GPIO_FUNC1
#define JZ_GPIO_FUNC_CIM_MCLK		JZ_GPIO_FUNC1
#define JZ_GPIO_FUNC_CIM_PCLK		JZ_GPIO_FUNC1
#define JZ_GPIO_FUNC_CIM_VSYNC		JZ_GPIO_FUNC1
#define JZ_GPIO_FUNC_CIM_HSYNC		JZ_GPIO_FUNC1
#define JZ_GPIO_FUNC_SPI_CLK		JZ_GPIO_FUNC1
#define JZ_GPIO_FUNC_SPI_CE0		JZ_GPIO_FUNC1
#define JZ_GPIO_FUNC_SPI_DT		JZ_GPIO_FUNC1
#define JZ_GPIO_FUNC_SPI_DR		JZ_GPIO_FUNC1
#define JZ_GPIO_FUNC_SPI_CE1		JZ_GPIO_FUNC1
#define JZ_GPIO_FUNC_PWM0		JZ_GPIO_FUNC1
#define JZ_GPIO_FUNC_PWM1		JZ_GPIO_FUNC1
#define JZ_GPIO_FUNC_PWM2		JZ_GPIO_FUNC1
#define JZ_GPIO_FUNC_PWM3		JZ_GPIO_FUNC1
#define JZ_GPIO_FUNC_PWM4		JZ_GPIO_FUNC1
#define JZ_GPIO_FUNC_PWM5		JZ_GPIO_FUNC1
#define JZ_GPIO_FUNC_PWM6		JZ_GPIO_FUNC1

#define JZ_GPIO_MEM_SCLK_RSTN		JZ_GPIO_PORTD(18)
#define JZ_GPIO_MEM_BCLK		JZ_GPIO_PORTD(19)
#define JZ_GPIO_MEM_SDATO		JZ_GPIO_PORTD(20)
#define JZ_GPIO_MEM_SDATI		JZ_GPIO_PORTD(21)
#define JZ_GPIO_MEM_SYNC		JZ_GPIO_PORTD(22)
#define JZ_GPIO_I2C_SDA			JZ_GPIO_PORTD(23)
#define JZ_GPIO_I2C_SCK			JZ_GPIO_PORTD(24)
#define JZ_GPIO_UART0_TXD		JZ_GPIO_PORTD(25)
#define JZ_GPIO_UART0_RXD		JZ_GPIO_PORTD(26)
#define JZ_GPIO_MEM_ADDR17		JZ_GPIO_PORTD(27)
#define JZ_GPIO_MEM_ADDR18		JZ_GPIO_PORTD(28)
#define JZ_GPIO_UART0_CTS		JZ_GPIO_PORTD(30)
#define JZ_GPIO_UART0_RTS		JZ_GPIO_PORTD(31)

#define JZ_GPIO_FUNC_MEM_SCLK_RSTN	JZ_GPIO_FUNC2
#define JZ_GPIO_FUNC_MEM_BCLK		JZ_GPIO_FUNC2
#define JZ_GPIO_FUNC_MEM_SDATO		JZ_GPIO_FUNC2
#define JZ_GPIO_FUNC_MEM_SDATI		JZ_GPIO_FUNC2
#define JZ_GPIO_FUNC_MEM_SYNC		JZ_GPIO_FUNC2
#define JZ_GPIO_FUNC_I2C_SDA		JZ_GPIO_FUNC2
#define JZ_GPIO_FUNC_I2C_SCK		JZ_GPIO_FUNC2
#define JZ_GPIO_FUNC_UART0_TXD		JZ_GPIO_FUNC2
#define JZ_GPIO_FUNC_UART0_RXD		JZ_GPIO_FUNC2
#define JZ_GPIO_FUNC_MEM_ADDR17		JZ_GPIO_FUNC2
#define JZ_GPIO_FUNC_MEM_ADDR18		JZ_GPIO_FUNC2
#define JZ_GPIO_FUNC_UART0_CTS		JZ_GPIO_FUNC2
#define JZ_GPIO_FUNC_UART0_RTS		JZ_GPIO_FUNC2

#define JZ_GPIO_UART1_RXD		JZ_GPIO_PORTD(30)
#define JZ_GPIO_UART1_TXD		JZ_GPIO_PORTD(31)

#define JZ_GPIO_FUNC_UART1_RXD		JZ_GPIO_FUNC3
#define JZ_GPIO_FUNC_UART1_TXD		JZ_GPIO_FUNC3

#endif
