/*
    http://datasheets.maximintegrated.com/en/ds/MAX11203-MAX11213.pdf
    
    CMD: [START:1, MODE:1 (register access), 0, RS3, RS2, RS1, RS0, W:0/R:1]

    SCYCLE BIT = 0 (continuous conversion)
    SCYCLE BIT = 1 (single conversion)
    
    The library currently uses continuous conversion (see begin() , uint8_t ctrl)

*/

#include <MAX11203.h>

void MAX11203::begin(bool cont, uint8_t rate) {
    pinMode(_cs, OUTPUT);
    digitalWrite(_cs, HIGH);
    //SPI.begin();
    //SPI.setClockDivider(SPI_CLOCK_DIV4); // assumes 16Mhz clock to get 4Mhz on SPI
    SPI.beginTransaction(SPISettings(5000000, MSBFIRST, SPI_MODE0));

    if (cont) {
        // set up control reg(s):
        digitalWrite(_cs, LOW);
        uint8_t cmd = MAX_CMD_CONV | rate;
        (void) spiTransfer(cmd);
        cmd = 0;
        cmd = (MAX_CMD_REG | MAX_CTRL1) | MAX_WRITE;
        uint8_t ctrl = MAX_UNIPOLAR | MAX_CONTCONV;
        (void) spiTransfer(cmd);
        (void) spiTransfer(ctrl);
        //digitalWrite(_cs, HIGH);  // leave low for continuous conversion. 
    } else {

    }
}

uint8_t MAX11203::spiTransfer(uint8_t i) {
    uint8_t d;
    d = SPI.transfer(i);
    return d;
}

// data comes out MSB first (D15 is MSB, D0 is LSB)
// for continuous conversion, ??trigger this as an interrupt callback?? on RDY/DOUT LOW to HIGH transition <-- this might be an error?? RDY should be HIGH to LOW???
// for now, just check each time if RDY/MISO is HIGH (LOW??)
uint16_t MAX11203::analogRead(void) {  
    uint16_t analogValue; 
    if (digitalRead(MISO) == HIGH) {
        digitalWrite(_cs, LOW);                                 // should already be LOW
        uint8_t cmd = (MAX_CMD_REG | MAX_DATA) | MAX_READ;
        (void) spiTransfer(cmd);
        //uint16_t analogValue = spiTransfer16(0);
        //spiTransfer16(0);
        uint16_t b_msb = spiTransfer(0);
        uint16_t b_lsb = spiTransfer(0);
        analogValue = (b_msb <<8) | (b_lsb);
    } else {
        analogValue = 0;
    }
    return analogValue;

    
    

    // uint8_t addr = 0b01100000 | ((pin & 0b111) << 2);
    // digitalWrite(_cs, LOW);
    // (void) spiTransfer(addr);
    // uint8_t b1 = spiTransfer(0);
    // uint8_t b2 = spiTransfer(0);
    // digitalWrite(_cs, HIGH);
    //return (b1 << 4) | (b2 >> 4);
}

int16_t MAX11203::analogReadDif(void) {
    // uint8_t diff;
    // uint8_t b1, b2;
    // uint8_t addr = 0b01000000 | ((pin & 0b11) << 3);
    // digitalWrite(_cs, LOW);
    // (void) spiTransfer(addr);
    // b1 = spiTransfer(0);
    // b2 = spiTransfer(0);
    // digitalWrite(_cs, HIGH);

    // diff = (b1 << 4) | (b2 >> 4);
    // if (diff > 0) {
    //     return diff;
    // }
    // addr = 0b01000100 | ((pin & 0b11) << 3);
    // digitalWrite(_cs, LOW);
    // (void) spiTransfer(addr);
    // b1 = spiTransfer(0);
    // b2 = spiTransfer(0);
    // digitalWrite(_cs, HIGH);
    // diff = (b1 << 4) | (b2 >> 4);
    // return -diff;
    return 0;
}

/*
// MAX11213 only:
uint8_t MAX11203::setGain(uint8_t gain) {

    return gain;
}
*/

uint32_t MAX11203::getRegister(uint8_t addr) {
    uint32_t reg = 0;

    return reg;
}

// CTRL1 register bits REFBUF & SIGBUF
uint8_t MAX11203::setBuffer(bool on) { 

    digitalWrite(_cs, HIGH);
    digitalWrite(_cs, LOW);
    uint8_t cmd = (MAX_CMD_REG | MAX_CTRL1) | MAX_WRITE;

    //uint8_t CTRL1 = spiTransfer(0);
    uint8_t ctrl = 0;
    if (on) {
        ctrl |= 0b00011000;    // these are the buffer bits;
    } else {
        //ctrl |= 0b00011000;    // make sure they are on
        //ctrl ^= 0b00011000;    // invert just those bits

        ctrl &= ~0b00011000;
    }
    (void) spiTransfer(cmd);
    (void) spiTransfer(ctrl);
    digitalWrite(_cs, HIGH);

    return ctrl;
}

// self calibration: command byte: CAL1:0, CAL0:1 (200ms)
// set zero: CAL1:1, CAL0:0 and present zero-level input (100ms)
// set max: CAL1:1, CAL0:1 and present max-level input (100ms)
// calibration bits: CTRL3 register: NOSYSG, NOSYSO, NOSCG, NOSCO 
//default to disabled (1) on power up which equals utilization of only 60% of full scale digital range
bool MAX11203::calibrate(void) {

}