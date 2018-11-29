#define WRT1 0b11111110 
#define WRT2 0b11111100
#define WRT3 0b11111000
#define WRT4 0b11110000
#define WRT5 0b11100000
#define WRT6 0b11000000
#define WRT7 0b10000000
#define WRT8 0b00000000


#define SCROLL_UP  0xAA
#define SCROLL_DOWN 0x55

#define HR_TRACK_STATUS 0x02
#define HR_MAX_STORE 0x04
#define HR_RCCNT 0x06

#define HRV_TRACK_STATUS 0x08
#define HRV_MAX_STORE 0x0A
#define HRV_RCCNT 0x0C

#define TEMP_TRACK_STATUS 0x0E
#define TEMP_MAX_STORE 0x10
#define TEMP_RCCNT 0x12

#define GLUC_TRACK_STATUS 0x14
#define GLUC_MAX_STORE 0x16
#define GLUC_RCCNT 0x18

#define TRACKING 0x0F
#define NOT_TRACKING 0xF0


#define W0 0
#define W1 1
#define W2 2
#define W3 3
#define W4 4
#define W5 5
#define W6 6
#define W7 7

#define HR_TRACK_ADDR 0x000 
#define HRV_TRACK_ADDR 0x000 
#define TEMP_TRACK_ADDR 0x000
#define GLUC_TRACK_ADDR 0x000

#define HR_MAX_ADDR 0x030 
#define HRV_MAX_ADDR 0x030  
#define TEMP_MAX_ADDR 0x030 
#define GLUC_MAX_ADDR 0x030  


#define HR_STORAGE_SECTOR 0x05
#define HRV_STORAGE_SECTOR 0x01 
#define TEMP_STORAGE_SECTOR 0x04
#define GLUC_STORAGE_SECTOR 0x06

#define INTSIZE 0x02
#define FLOATSIZE 0x04
volatile union uFLOAT{
    float floatVAL;
    char floatCHAR[4];
};

volatile union uINT{
    int intVAL;
    char intCHAR[2];
};


// state variables
//enum trkSTATE {W0,W1,W2,W3,W4,W5,W6,W7}; 
extern volatile char HRScrollSTATE;
extern volatile char HRVScrollSTATE;
extern volatile char TEMPScrollSTATE;
extern volatile char GLUCScrollSTATE;

// record counts
extern volatile unsigned char HRreccnt;
extern volatile unsigned char HRVreccnt;
extern volatile unsigned char TEMPreccnt;
extern volatile unsigned char GLUCreccnt;

// Bit track count offsets


// Max allowable records offsets
extern volatile unsigned char HRMaxoffset; 
extern volatile unsigned char HRVMaxoffset; 
extern volatile unsigned char TEMPMaxoffset;
extern volatile unsigned char GLUCMaxoffset; 

// Max allowable records
extern volatile unsigned char HRMaxRecords; 
extern volatile unsigned char HRVMaxRecords; 
extern volatile unsigned char TEMPMaxRecords;
extern volatile unsigned char GLUCMaxRecords; 

// Unions for measured data
extern union uINT HR;
extern union uFLOAT HRV;
extern union uFLOAT TEMP;
extern union uFLOAT GLUC;

//write addresses
extern volatile unsigned int HRwriteAdd;
extern volatile unsigned int HRVwriteAdd;
extern volatile unsigned int TEMPwriteAdd;
extern volatile unsigned int GLUCwriteAdd;

//scrollRead limits
extern volatile unsigned int HRreadAddX;
extern volatile unsigned int HRVreadAddX;
extern volatile unsigned int TEMPreadAddX;
extern volatile unsigned int GLUCreadAddX;

//scrollRead Pointers
extern volatile unsigned int HRreadAdd;
extern volatile unsigned int HRVreadAdd;
extern volatile unsigned int TEMPreadAdd;
extern volatile unsigned int GLUCreadAdd;

extern volatile unsigned char HRscrollCnt;
extern volatile unsigned char HRVscrollCnt;
extern volatile unsigned char TEMPscrollCnt;
extern volatile unsigned char GLUCscrollCnt;

extern float tempDispL;
extern float glucDispL;
extern float hrvDispL;
extern unsigned int hrDispL;


void meman_init(void);
void eeprom_init(void);
char readEEPROM(unsigned char);
void writeEEPROM(unsigned char, unsigned char);
void RecordTrackInit(unsigned char, unsigned char, unsigned char, unsigned char, volatile unsigned char*, volatile unsigned char* , volatile unsigned int* , volatile unsigned int*);
void MakeRecordHR(unsigned char,unsigned char,union uINT);
void MakeRecordHRV(unsigned char,unsigned char,union uFLOAT);
void MakeRecordTEMP(unsigned char,unsigned char,union uFLOAT);
void MakeRecordGLUC(unsigned char,unsigned char,union uFLOAT);



void openRecordsINTLite(unsigned char, unsigned int*, volatile unsigned int*, volatile unsigned char*, volatile unsigned char*, volatile char*);
void openRecordsFLOATLite(unsigned char, float*, volatile unsigned int*, volatile unsigned char*, volatile unsigned char*, volatile char*);
void scrollRecordsHRLite(char, char, volatile char*, unsigned char); /* message this will be replaced with void*/
void scrollRecordsHRVLite(char, char, volatile char*, unsigned char); /* message this will be replaced with void*/
void scrollRecordsTEMPLite(char, char, volatile char*, unsigned char); /* message this will be replaced with void*/
void scrollRecordsGLUCLite(char, char,volatile char*, unsigned char); /* message this will be replaced with void*/
void openRecordsLite(void);
void pageScrollLite(char scrollmsg);
