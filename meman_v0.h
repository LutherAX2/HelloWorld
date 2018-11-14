#define WRT1 0b11111110 
#define WRT2 0b11111100
#define WRT3 0b11111000
#define WRT4 0b11110000
#define WRT5 0b11100000
#define WRT6 0b11000000
#define WRT7 0b10000000
#define WRT8 0b00000000

#define HR_TRACK_SECTOR  0x11 
#define HRV_TRACK_SECTOR 0x23 
#define TEMP_TRACK_SECTOR 0x35
#define GLUC_TRACK_SECTOR 0x47 

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


#define HR_STORAGE_SECTOR 0x00 
#define HRV_STORAGE_SECTOR 0x02 
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
extern volatile unsigned int HRtrkSTATE;
extern volatile unsigned int HRVtrkSTATE;
extern volatile unsigned int TEMPtrkSTATE;
extern volatile unsigned int GLUCtrkSTATE;

// record counts
extern volatile unsigned char HRreccnt;
extern volatile unsigned char HRVreccnt;
extern volatile unsigned char TEMPreccnt;
extern volatile unsigned char GLUCreccnt;

// Bit track count offsets
extern volatile unsigned char HRTrkcntOffset;
extern volatile unsigned char HRVTrkcntOffset;
extern volatile unsigned char TEMPTrkcntOffset;
extern volatile unsigned char GLUCTrkcntOffset;

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




//functions
//void RecordTrack(unsigned char SECTOR, unsigned int trackAddress, usnigned char* OFFSET, trkSTATE stateVAR)
void RecordTrack(unsigned char,unsigned int, volatile unsigned char*, volatile unsigned int*);

//void RecordTrackInit(unsigned char SECTOR, unsigned int ADDRESS1, unsigned char* offsetTRK, trkSTATE stateVAR, unsigned char *reccnt, 
//      unsigned char* offsetMax, unsigned int* writeAddress, unsigned char TypeSize)
void RecordTrackInit(unsigned char, unsigned int, volatile unsigned char*, volatile unsigned int*, volatile unsigned char*, volatile unsigned char*, volatile unsigned int*,unsigned char);

//void MaxFinder(unsigned char SECTOR, unsigned int ADDRESS1, unsigned char maxRecord,unsigned char* offeetMAX);
void MaxFinder(unsigned char, unsigned int, volatile unsigned char, volatile unsigned char* );


//void MakeRecordFloat(unsigned char storeSECTOR, unsigned int* writeAddress,unsigned char trackSECTOR, unsigned int trackAddress, unsigned char* offsetTRK, trkSTATE stateVAR, 
//  unsigned char *reccnt, union uFLOAT val)

void MakeRecordFloat(unsigned char , volatile unsigned int* , volatile unsigned int* , volatile unsigned char *, union uFLOAT);
void MakeRecordInt(unsigned char , volatile unsigned int* , volatile unsigned int* , volatile unsigned char *, union uINT);

