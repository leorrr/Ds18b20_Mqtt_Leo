/* 
* Adaptation of Paul Stoffregens One wire library to the ESP8266 and 
* Necromant�s Frankenstein firmware by Erland Lewin <erland@lewin.nu> 
* 
* Pauls original library site: 
*   http://www.pjrc.com/teensy/td_libs_OneWire.html 
* 
* See also http://playground.arduino.cc/Learning/OneWire 
* 
* Stripped down to bare minimum by Peter Scargill for single DS18B20 or DS18B20P integer read 
*/

static int gpioPin;
#include "driver/ds18b20.h"

void ICACHE_FLASH_ATTR ds_init(int gpio)
{
    //set gpio2 as gpio pin 
    PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO2_U, FUNC_GPIO2); 
    //disable pulldown 
    PIN_PULLDWN_DIS(PERIPHS_IO_MUX_GPIO2_U); 
    //enable pull up R 
    PIN_PULLUP_EN(PERIPHS_IO_MUX_GPIO2_U); 
    // Configure the GPIO with internal pull-up 
    // PIN_PULLUP_EN( gpio ); 
    GPIO_DIS_OUTPUT( gpio ); 
    gpioPin = gpio; 
}

// Perform the onewire reset function.  We will wait up to 250uS for 
// the bus to come high, if it doesnt then it is broken or shorted 
// and we return;

void ICACHE_FLASH_ATTR ds_reset(void) 
{ 
    //    IO_REG_TYPE mask = bitmask; 
    //    volatile IO_REG_TYPE *reg IO_REG_ASM = baseReg; 
    uint8_t retries = 125; 
    // noInterrupts(); 
    // DIRECT_MODE_INPUT(reg, mask); 
    GPIO_DIS_OUTPUT( gpioPin ); 
    // interrupts(); 
    // wait until the wire is high just in case 
    do { 
        if (retries == 0) return;
        os_delay_us(2); 
    } while ( !GPIO_INPUT_GET( gpioPin )); 
    // noInterrupts(); 
    GPIO_OUTPUT_SET( gpioPin, 0 ); 
    // DIRECT_WRITE_LOW(reg, mask); 
    // DIRECT_MODE_OUTPUT(reg, mask);    // drive output low 
    // interrupts(); 
    os_delay_us(480); 
    // noInterrupts(); 
    GPIO_DIS_OUTPUT( gpioPin ); 
    // DIRECT_MODE_INPUT(reg, mask);    // allow it to float 
    os_delay_us(70); 
    // r = !DIRECT_READ(reg, mask); 
    //r = !GPIO_INPUT_GET( gpioPin ); 
    // interrupts(); 
    os_delay_us(410); 
}

// 
// Write a bit. Port and bit is used to cut lookup time and provide 
// more certain timing. 
// 
static inline void write_bit( int v ) 
{ 
    // IO_REG_TYPE mask=bitmask; 
    //    volatile IO_REG_TYPE *reg IO_REG_ASM = baseReg; 
    GPIO_OUTPUT_SET( gpioPin, 0 ); 
    if( v ) { 
        // noInterrupts(); 
        //    DIRECT_WRITE_LOW(reg, mask); 
        //    DIRECT_MODE_OUTPUT(reg, mask);    // drive output low 
        os_delay_us(10); 
        GPIO_OUTPUT_SET( gpioPin, 1 ); 
        // DIRECT_WRITE_HIGH(reg, mask);    // drive output high 
        // interrupts(); 
        os_delay_us(55); 
    } else { 
        // noInterrupts(); 
        //    DIRECT_WRITE_LOW(reg, mask); 
        //    DIRECT_MODE_OUTPUT(reg, mask);    // drive output low 
        os_delay_us(65); 
        GPIO_OUTPUT_SET( gpioPin, 1 ); 
        //    DIRECT_WRITE_HIGH(reg, mask);    // drive output high 
        //        interrupts(); 
        os_delay_us(5); 
    } 
}

// 
// Read a bit. Port and bit is used to cut lookup time and provide 
// more certain timing. 
// 
static inline int read_bit(void) 
{ 
    //IO_REG_TYPE mask=bitmask; 
    //volatile IO_REG_TYPE *reg IO_REG_ASM = baseReg; 
    int r; 
    // noInterrupts(); 
    GPIO_OUTPUT_SET( gpioPin, 0 ); 
    // DIRECT_MODE_OUTPUT(reg, mask); 
    // DIRECT_WRITE_LOW(reg, mask); 
    os_delay_us(3); 
    GPIO_DIS_OUTPUT( gpioPin ); 
    // DIRECT_MODE_INPUT(reg, mask);    // let pin float, pull up will raise 
    os_delay_us(10); 
    // r = DIRECT_READ(reg, mask); 
    r = GPIO_INPUT_GET( gpioPin ); 
    // interrupts(); 
    os_delay_us(53); 
    return r; 
}

// 
// Write a byte. The writing code uses the active drivers to raise the 
// pin high, if you need power after the write (e.g. DS18S20 in 
// parasite power mode) then set power to 1, otherwise the pin will 
// go tri-state at the end of the write to avoid heating in a short or 
// other mishap. 
// 
void ICACHE_FLASH_ATTR  ds_write( uint8_t v, int power ) { 
    uint8_t bitMask; 
    for (bitMask = 0x01; bitMask; bitMask <<= 1) { 
        write_bit( (bitMask & v)?1:0); 
    } 
    if ( !power) { 
        // noInterrupts(); 
        GPIO_DIS_OUTPUT( gpioPin ); 
        GPIO_OUTPUT_SET( gpioPin, 0 ); 
        // DIRECT_MODE_INPUT(baseReg, bitmask); 
        // DIRECT_WRITE_LOW(baseReg, bitmask); 
        // interrupts(); 
    } 
}

// 
// Read a byte 
// 
uint8_t ICACHE_FLASH_ATTR ds_read() { 
    uint8_t bitMask; 
    uint8_t r = 0; 
    for (bitMask = 0x01; bitMask; bitMask <<= 1) { 
        if ( read_bit()) r |= bitMask; 
    } 
    return r; 
}
