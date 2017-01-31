// 5MZ MAX SPI clock
// 4 GPIOs (not implemented)
// 1 differential ADC (AINN, AINP)
// optional buffer on input (on for high impedence transducers)



#ifndef _MAX11203_H
#define _MAX11203_H

#if (ARDUINO >= 100) 
# include <Arduino.h>
#else
# include <WProgram.h>
#endif

#include <SPI.h>

// conversion speeds ((Continuous) Samples Per Second)
//single:
#define MAX_CMD_CONV    0x80        // 0b10000000
#define MAX_SPS_1       0x00
#define MAX_SPS_2_5     0x01
#define MAX_SPS_5       0x02
#define MAX_SPS_10      0x03
#define MAX_SPS_15      0x04
#define MAX_SPS_30      0x05
#define MAX_SPS_60      0x06
#define MAX_SPS_120     0x07
// continuous:
#define MAX_CSPS_60     0x04
#define MAX_CSPS_120    0x05
#define MAX_CSPS_240    0x06
#define MAX_CSPS_480    0x07

// registers: 
#define MAX_CMD_REG     0xC0        // 0b11000000
// register addresses:
#define MAX_STAT1       0x00 << 1
#define MAX_CTRL1       0x01 << 1
#define MAX_CTRL2       0x02 << 1
#define MAX_CTRL3       0x03 << 1
#define MAX_DATA        0x04 << 1
#define MAX_SOC         0x05 << 1
#define MAX_SGC         0x06 << 1
#define MAX_SCOC        0x07 << 1
#define MAX_SCGC        0x08 << 1
#define MAX_READ        0x01
#define MAX_WRITE       0x00

// CTRL1 register bits:
//#define MAX_BIPOLAR
#define MAX_UNIPOLAR    0x40        // input range: defaults to bipolar (-AREF to +AREF). UNI = (0 to +AREF)
#define MAX_CONTCONV    0x02        // defaults to single-conversion. FYI the first 3 data from continuous are incorrect. 



class MAX11203 {
    private:
        // Private functions and variables here.  They can only be accessed
        // by functions within the class.
        uint8_t _cs;
        uint8_t spiTransfer(uint8_t);

    public:
        // Public functions and variables.  These can be accessed from
        // outside the class.
        MAX11203() : _cs(10) {}
        MAX11203(uint8_t cs) : _cs(cs) {}
    
        void begin(bool cont = true, uint8_t rate = MAX_CSPS_60);
        //uint16_t analogReadSingle(uint8_t);   // single conversion: takes MX_SPS defines as argument
        //bool checkConversion(void);           // in continuous conversion, checks for ready data + syncs read.
        uint16_t analogRead(void);              // 
        int16_t analogReadDif(void);
        //uint8_t setGain(uint8_t gain);        // MSP11213 only
        uint32_t getRegister(uint8_t addr);     // registers 0-4 are 8 bit, 5-9 are 24 bit
        uint8_t setBuffer(bool on);             // returns CTRL1 register
        bool calibrate(void);
};
#endif