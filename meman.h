#define WRT1 0b11111110 
#define WRT2 0b11111100
#define WRT3 0b11111000
#define WRT4 0b11110000
#define WRT5 0b11100000
#define WRT6 0b11000000
#define WRT7 0b10000000
#define WRT8 0b00000000

#define HR_TRACK_SECTOR  0x01 
#define HRV_TRACK_SECTOR 0x03 
#define TEMP_TRACK_SECTOR 0x05
#define GLUC_TRACK_SECTOR 0x07 

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
extern unsigned int HRtrkSTATE;
extern unsigned int HRVtrkSTATE;
extern unsigned int TEMPtrkSTATE;
extern unsigned int GLUCtrkSTATE;

// record counts
extern unsigned char HRreccnt;
extern unsigned char HRVreccnt;
extern unsigned char TEMPreccnt;
extern unsigned char GLUCreccnt;

// Bit track count offsets
extern unsigned char HRTrkcntOffset;
extern unsigned char HRVTrkcntOffset;
extern unsigned char TEMPTrkcntOffset;
extern unsigned char GLUCTrkcntOffset;

// Max allowable records offsets
extern unsigned char HRMaxoffset; 
extern unsigned char HRVMaxoffset; 
extern unsigned char TEMPMaxoffset;
extern unsigned char GLUCMaxoffset; 

// Max allowable records
extern unsigned char HRMaxRecords; 
extern unsigned char HRVMaxRecords; 
extern unsigned char TEMPMaxRecords;
extern unsigned char GLUCMaxRecords; 

// Unions for measured data
extern union uINT HR;
extern union uFLOAT HRV;
extern union uFLOAT TEMP;
extern union uFLOAT GLUC;

//write addresses
extern unsigned int HRwriteAdd;
extern unsigned int HRVwriteAdd;
extern unsigned int TEMPwriteAdd;
extern unsigned int GLUCwriteAdd;

//scrollRead limits
extern unsigned int HRreadAddX;
extern unsigned int HRVreadAddX;
extern unsigned int TEMPreadAddX;
extern unsigned int GLUCreadAddX;

//scrollRead Pointers
extern unsigned int HRreadAdd;
extern unsigned int HRVreadAdd;
extern unsigned int TEMPreadAdd;
extern unsigned int GLUCreadAdd;




//functions
//void RecordTrack(unsigned char SECTOR, unsigned int trackAddress, usnigned char* OFFSET, trkSTATE stateVAR)
void RecordTrack(unsigned char,unsigned int, unsigned char*, unsigned int*);

//void RecordTrackInit(unsigned char SECTOR, unsigned int ADDRESS1, unsigned char* offsetTRK, trkSTATE stateVAR, unsigned char *reccnt, 
//      unsigned char* offsetMax, unsigned int* writeAddress, unsigned char TypeSize)
void RecordTrackInit(unsigned char, unsigned int, unsigned char*, unsigned int*, unsigned char*, unsigned char*, unsigned int*,unsigned char);

//void MaxFinder(unsigned char SECTOR, unsigned int ADDRESS1, unsigned char maxRecord,unsigned char* offeetMAX);
void MaxFinder(unsigned char, unsigned int, unsigned char, unsigned char* );


//void MakeRecordFloat(unsigned char storeSECTOR, unsigned int* writeAddress,unsigned char trackSECTOR, unsigned int trackAddress, unsigned char* offsetTRK, trkSTATE stateVAR, 
//  unsigned char *reccnt, union uFLOAT val)

void MakeRecordFloat(unsigned char , unsigned int* ,unsigned int*,unsigned char, unsigned int, unsigned char* ,unsigned int*, unsigned char*, union uFLOAT);


