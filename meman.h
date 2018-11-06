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

#define INTSIZE 0x04
#define FLOATSIZE 0x08

volatile union uFLOAT{
    float floatVAL;
    char floatCHAR[4];
};

volatile union uINT{
    int intVAL;
    char intCHAR[2];
};


// state variables
typedef enum trkSTATE {W0,W1,W2,W3,W4,W5,W6,W7}; 
extern trkSTATE HRtrkSTATE;
extern trkSTATE HRVtrkSTATE;
extern trkSTATE TEMPtrkSTATE;
extern trkSTATE GLUCtrkSTATE;

// record counts
extern unsigned int HRreccnt;
extern unsigned int HRVreccnt;
extern unsigned int TEMPreccnt;
extern unsigned int GLUCreccnt;

extern unsigned int HRTrkcntOffset;
extern unsigned int HRVTrkcntOffset;
extern unsigned int TEMPTrkcntOffset;
extern unsigned int GLUCTrkcntOffset;

extern unsigned int HRMaxoffset; 
extern unsigned int HRVMaxoffset; 
extern unsigned int TEMPMaxoffset;
extern unsigned int GLUCMaxoffset;  

extern union uINT HR;
extern union uFLOAT HRV;
extern union uFLOAT TEMP;
extern union uFLOAT GLUC;

//functions
//void RecordTrack(unsigned char SECTOR, usnigned char* OFFSET, trkSTATE stateVAR)
void RecordTrack(unsigned char, unsigned char*, trkSTATE);

//void RecordTrack(unsigned char SECTOR, unsigned char* OFFSET, trkSTATE stateVAR, unsigned int *reccnt)
void RecordTrackInit(unsigned char, unsigned int, unsigned char*, trkSTATE, unsigned int*, unsigned char*,unsigned char*);