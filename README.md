# HelloWorld
ECNG 3006 Group H Project Repository 2018/19
Code to interface SST39SF040 with PIC18F452 check defines in SST39S_v1.h for pin mappings. Data interfaced with 74LS299 I/0 parallel load, bidirectional, tristate shift register.
Addresses pins Interfaced with 3 74LS164 shift registers

DATA_OE is for 74LS299
S1 connects to S1 pin on 74LS299


flash RAM pins
OE
WE
CE

look for these functions in SST39S_v1.h

ram_init() - call this before anything else, readDATA(), writeDATA(), chipErase(), sectorErase() softwareID() softwareIDExit()

Have Fun.
