# MAX11203/MAX11213 Arduino Library
MAX11203 is a 16-Bit, Single-Channel, Ultra-Low-Power, Delta- Sigma ADCs with Programmable Gain (11213 only), and GPIO

In progress... 

## Notes:
 - focus on continous conversion (supposedly simpler but syncing might be an issue)
 - get basic functionality to access 16 bit
 
## TODO: 
 - test! See if this basic frame even works yet! 
 - verify continuous conversion check RDY/DOUT pin. Is there a datasheet error?? Note says conversion ready signaled by RDY/DOUT transition from 0 to 1, but normally RDY is true LOW...
 - implement calibration (at least the basic version)
 - implement single conversion
 - implement GPIO?? low priority
 - implement getRegister() (will be a good way to test if things are working properly)
