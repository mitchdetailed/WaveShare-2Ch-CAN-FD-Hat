//——————————————————————————————————————————————————————————————————————————————
//  ACAN2517FD Send Demo in NormalFD mode for Arduino Uno
//——————————————————————————————————————————————————————————————————————————————

#include "ACAN2517FD.h"
#include "SPI.h"

//——————————————————————————————————————————————————————————————————————————————
// Very very important: put a 10kΩ resistor between CS and VDD of MCP2517FD

static const byte MCP2517_CS  = 10 ; // CS input of MCP2517 
static const byte MCP2517_INT =  2 ; // INT output of MCP2517

//——————————————————————————————————————————————————————————————————————————————
//  ACAN2517FD Driver object
//——————————————————————————————————————————————————————————————————————————————

ACAN2517FD can (MCP2517_CS, SPI, MCP2517_INT) ;

//——————————————————————————————————————————————————————————————————————————————
//   SETUP
//——————————————————————————————————————————————————————————————————————————————

void setup () {
//--- Start serial
  Serial.begin (115200) ;
//--- Wait for serial (blink led at 10 Hz during waiting)
  while (!Serial) {
    delay (50) ;
  }
//----------------------------------- Begin SPI
  SPI.begin () ;
//--- Configure ACAN2517FD
  Serial.print ("sizeof (ACAN2517FDSettings): ") ;
  Serial.print (sizeof (ACAN2517FDSettings)) ;
  Serial.println (" bytes") ;
  Serial.println ("Configure ACAN2517FD") ;
  ACAN2517FDSettings settings (ACAN2517FDSettings::OSC_40MHz, 125UL * 1000UL, ACAN2517FDSettings::DATA_BITRATE_x1) ;
  settings.mRequestedMode = ACAN2517FDSettings::NormalFD ; // Select loopback mode
//--- Default values are too high for an Arduino Uno that contains 2048 bytes of RAM: reduce them
  settings.mDriverTransmitFIFOSize = 1 ;
  settings.mDriverReceiveFIFOSize = 1 ;
//--- RAM Usage
  Serial.print ("MCP2517FD RAM Usage: ") ;
  Serial.print (settings.ramUsage ()) ;
  Serial.println (" bytes") ;
//--- Begin
  const uint32_t errorCode = can.begin (settings, [] { can.isr () ; }) ;
  if (errorCode == 0) {
    Serial.print ("Bit Rate prescaler: ") ;
    Serial.println (settings.mBitRatePrescaler) ;
    Serial.print ("Arbitration Phase segment 1: ") ;
    Serial.println (settings.mArbitrationPhaseSegment1) ;
    Serial.print ("Arbitration Phase segment 2: ") ;
    Serial.println (settings.mArbitrationPhaseSegment2) ;
    Serial.print ("Arbitration SJW:") ;
    Serial.println (settings.mArbitrationSJW) ;
    Serial.print ("Actual Arbitration Bit Rate: ") ;
    Serial.print (settings.actualArbitrationBitRate ()) ;
    Serial.println (" bit/s") ;
    Serial.print ("Exact Arbitration Bit Rate ? ") ;
    Serial.println (settings.exactArbitrationBitRate () ? "yes" : "no") ;
    Serial.print ("Arbitration Sample point: ") ;
    Serial.print (settings.arbitrationSamplePointFromBitStart ()) ;
    Serial.println ("%") ;
  }else{
    Serial.print ("Configuration error 0x") ;
    Serial.println (errorCode, HEX) ;
  }
}

//——————————————————————————————————————————————————————————————————————————————
//   LOOP
//——————————————————————————————————————————————————————————————————————————————

static uint32_t gSendDate = 0 ;
static uint32_t gReceivedFrameCount = 0 ;
static uint32_t gSentFrameCount = 0 ;

//——————————————————————————————————————————————————————————————————————————————

void loop () {
  CANFDMessage frame ;
  frame.id=88;
  frame.len=20;
  frame.ext=true;
  for (uint8_t i=0 ; i<20 ; i++) {
      frame.data [i] = i ;
    }
  
  if (gSendDate < millis ()) {
    gSendDate += 2000 ;
    const bool ok = can.tryToSend (frame) ;
    if (ok) {
      gSentFrameCount += 1 ;
      Serial.print ("Sent: ") ;
      Serial.println (gSentFrameCount) ;
    }else{
      Serial.println ("Send failure") ;
    }
  }
}

//——————————————————————————————————————————————————————————————————————————————
