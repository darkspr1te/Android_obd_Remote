#include <Arduino.h>
#include <ESP32CAN.h> // https://github.com/miwagner/ESP32-Arduino-CAN
#include <CAN_config.h>

CAN_device_t CAN_cfg;                // CAN Config
const int rx_queue_size = 10;       // Receive Queue size
struct Canbus_previous_val {
    byte rpm[2]; // odometer info
    byte ext_temp,Doors,ver;
    uint16_t disc_btn;
    bool ign,ill,rear,brake; // I/O status
};

Canbus_previous_val CAN_OV;

CAN_frame_t canMsgRcv;
CAN_frame_t canMsgSend;

uint16_t SrollVal;
        
void Disc(int count) {
    if (count < 8 & count >0) {
        for (byte i = 1; i <= count; i++) {
           HU_button(SWC_KEY_MEMO_UP);  //remote->SendButtonCode(NextAlbum);
        }
    }
    count = -count;
    if (count < 8 & count >0) {
        for (byte i = 1; i <= count; i++) {
            HU_button(SWC_KEY_MEMO_DOWN); //remote->SendButtonCode(PreviousAlbum);
        }
    }
}
    

void CAN1_loop() {    
  if (xQueueReceive(CAN_cfg.rx_queue, &canMsgRcv, 3 * portTICK_PERIOD_MS) == pdTRUE) {
     switch (canMsgRcv.MsgID) {
        case 182: //0x0B6 rpm speed Fuel consumtion info (50ms)
          if (canMsgRcv.FIR.B.DLC == 8) {
            if (( canMsgRcv.data.u8[0] !=  CAN_OV.rpm[0] ) || (canMsgRcv.data.u8[1] != CAN_OV.rpm[1] )) {
              Car.I_rpm = canMsgRcv.data.u8[0] + canMsgRcv.data.u8[1] * 256; // to be verified
              if ( Car.I_rpm == 0 )  Car.I_Eng = false; else Car.I_Eng = true;
              Car.I_speed = canMsgRcv.data.u8[2] + canMsgRcv.data.u8[3] * 256; // to be verified divide by 10 for speed 
              CAN_OV.rpm[0]= canMsgRcv.data.u8[0];
              CAN_OV.rpm[1]= canMsgRcv.data.u8[1];
              Car.H_UPD_I= true;
            }
          }
        break;
        case 225: //0x0E1 parktronic
        break;
        case 246: //0x0F6 bsi TEMP COOLANT ?EXT? reverse gear ignition odometer
            if (canMsgRcv.data.u8[0] && 16) Car.ign = true; else Car.ign = false;
            if (canMsgRcv.data.u8[7] && 1) Car.rear = true; else Car.rear = false;
            if (canMsgRcv.data.u8[0] !=  CAN_OV.ext_temp) { 
                Car.exttemp = canMsgRcv.data.u8[6]; // divide by 2 and -39.5 for °c
                CAN_OV.ext_temp= canMsgRcv.data.u8[6];
                Car.H_UPD_I=true;
            }
            Car.Coolant = canMsgRcv.data.u8[1]; // add 39 for °c
        break;
        case 296: //0x128 Dashboard lights
        break;
        /* wheel position to the right ID 162 A2 XX 00 00 00 00 where XX varies 1A, 1B, 1C ..... FE, 00*/
        case 417: //0x1A1 Informational message
         Car.INF_MSG = canMsgRcv.data.u8[1]; //send as is to hu
         if (canMsgRcv.data.u8[2] == 1) Car.H_UPD_INF = 1; // ambigous
         if (canMsgRcv.data.u8[0] == 0x80) Car.H_UPD_INF = 1;  // ambigous
        break;
        case 464: //0x1D0 Climate control information
          if (canMsgRcv.FIR.B.DLC == 7 && Car.I_Eng) { // No fan activated if the engine is not ON on old models
            //if (canMsgRcv.data.u8[0]&&64)   //REAR_WINDSHIELD_HEAT
            AC_decode_2004();
          }
        break;
        case 493: //0x1ED Display conditioning commands
         if (canMsgRcv.data.u8[0] == 0x00) Car.AC_Mono =1; else Car.AC_Mono =0;
         if (canMsgRcv.data.u8[0] == 0x18) Car.AC_OFF =1; else Car.AC_OFF =0;
        break;
        case 543: //0x21F Radio remote control under the steering wheel
        /* if (canMsgRcv.data.u8[0]&&1) //remote->SendButtonCode(NextTrack); //forward
         if (canMsgRcv.data.u8[0]&&2) //remote->SendButtonCode(PreviousTrack);//backward
         if (canMsgRcv.data.u8[0]&&16) //remote->SendButtonCode(VolumeUp);//volume UP
         if (canMsgRcv.data.u8[0]&&32) //remote->SendButtonCode(VolumeDown);//volume DOWN
         if (canMsgRcv.data.u8[0]&&64) //remote->SendButtonCode(Source);//Source*/
            if (canMsgRcv.data.u8[0]&&1) HU_button(SWC_KEY_SEEK_UP); //forward
            if (canMsgRcv.data.u8[0]&&2) HU_button(SWC_KEY_SEEK_DOWN);//backward
            if (canMsgRcv.data.u8[0]&&16) HU_button(SWC_KEY_VOLUME_UP);//volume UP
            if (canMsgRcv.data.u8[0]&&32) HU_button(SWC_KEY_VOLUME_DOWN);//volume DOWN
            if (canMsgRcv.data.u8[0]&&64) HU_button(SWC_KEY_SRC);//Source
             
         SrollVal = canMsgRcv.data.u8[1]; // need to be tested 
            if (SrollVal <8 && CAN_OV.disc_btn > 200) SrollVal = SrollVal + 256; //prevent overflow change from 0 to 255 means -1
            if (SrollVal >200 && CAN_OV.disc_btn < 8) CAN_OV.disc_btn = CAN_OV.disc_btn + 256; //prevent overflow change from 255 to 0 mean +1           
            if (CAN_OV.disc_btn != SrollVal) { //scroll value from disk selection 
                Disc(SrollVal-CAN_OV.disc_btn); //change xtimes cd disc (max 7 pulses at time)
                CAN_OV.disc_btn = SrollVal;
            }
        break;
        case 544: //0x220 Door status
         if (canMsgRcv.data.u8[0] !=  CAN_OV.Doors) { 
          if (canMsgRcv.data.u8[0]&&1) Car.DOOR_FL = true; else Car.DOOR_FL = false;
          if (canMsgRcv.data.u8[0]&&2) Car.DOOR_FR = true; else Car.DOOR_FR = false;
          if (canMsgRcv.data.u8[0]&&4) Car.DOOR_BL = true; else Car.DOOR_BL = false;
          if (canMsgRcv.data.u8[0]&&8) Car.DOOR_BR = true; else Car.DOOR_BR = false;
          if (canMsgRcv.data.u8[0]&&16) Car.DOOR_TR = true; else Car.DOOR_TR = false;
          CAN_OV.Doors = canMsgRcv.data.u8[0];
          Car.H_UPD_DOOR=true;
         }  
        break;
        case 545: //0x221 Trip computer info instant
          if (canMsgRcv.FIR.B.DLC == 7) {
            if (canMsgRcv.data.u8[0]&&16) //trip command button
            if (canMsgRcv.data.u8[0]&&128) //voice command button
            Car.T_fuelC=canMsgRcv.data.u8[1]+canMsgRcv.data.u8[2]*256; //divide by 10 for L/100KM
            Car.T_fuelR=canMsgRcv.data.u8[3]+canMsgRcv.data.u8[4]*256;
            Car.H_UPD_Trip = true;  
          }
        break;
        case 609: //0x261 Trip computer info A
            Car.TA_Spd=canMsgRcv.data.u8[0];
            Car.TA_Mil=(canMsgRcv.data.u8[1]+canMsgRcv.data.u8[2]*256)/4;
            Car.TA_Fuelc=canMsgRcv.data.u8[3]; //divide by 10 for L/100KM
            Car.H_UPD_Trip = true;  
        break;
        case 673: //0x261 Trip computer info B
            Car.TB_Spd=canMsgRcv.data.u8[0];
            Car.TB_Mil=(canMsgRcv.data.u8[1]+canMsgRcv.data.u8[2]*256)/4;
            Car.TB_Fuelc=canMsgRcv.data.u8[3]; //divide by 10 for L/100KM
            Car.H_UPD_Trip = true;  
        break;             

      /*  case 848: //0x350
           // AC_decode_2010();
        break;*/
        }
    }
    if ( Car.C_UPD_AC) {
        // 1E3	7	1C 30 0D 0D 00 00 05 to be tested could be AC
        
        canMsgSend.FIR.B.FF = CAN_frame_std;
        canMsgSend.MsgID = 0x1E3;
        canMsgSend.FIR.B.DLC = 7;
        canMsgSend.data.u8[0] = 0x1C; //sniffed values
        canMsgSend.data.u8[1] = 0x30; //sniffed values
        canMsgSend.data.u8[2] = Car.AC_L;
        canMsgSend.data.u8[3] = Car.AC_R;
        canMsgSend.data.u8[4] = 0x00; //sniffed values
        canMsgSend.data.u8[5] = 0x00; //sniffed values
        canMsgSend.data.u8[6] = 0x05; //sniffed values
        ESP32Can.CANWriteFrame(&canMsgSend);
        Car.C_UPD_AC=false;
    }
}
/*
   // This is sent multiple times deliberately as long as the button is pressed
        // because sending the frame once isn't enough to trigger the reset
        // FIXME: we should test explicitely the tripModes because it will currently
        // reset the second memory if tripMode is equal to any value besides 1
        byte data[] = {
            tripMode == 1 ? 0x82 : 0x44, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00
        };
        CAN.sendMsgBuf(359, 0, 8, data);
        tripDidReset = true;
        
        */
    
    void  AC_decode_2004() { //see 12D too
      Car.AC_L = tamp_dec_2004(canMsgRcv.data.u8[5]);
      Car.AC_R = tamp_dec_2004(canMsgRcv.data.u8[6]); //12d 4 should be AC controller 64 for high
      Car.AC_FAN = canMsgRcv.data.u8[2]; //0f:off 00>07 1 >8  /12d 1 0>64 (64=100 dec) maybe %
      // Position Fan
      switch (canMsgRcv.data.u8[3]) {
      case 0x40:
        Car.AC_DOWN= false;Car.AC_UP= true;Car.AC_FRONT= false;
      break;
      case 0x30:
        Car.AC_DOWN= false;Car.AC_UP= false;Car.AC_FRONT= true;
      break;
      case 0x20:
        Car.AC_DOWN= true;Car.AC_UP= false;Car.AC_FRONT= false;
      break;
      case 0x70:
        Car.AC_DOWN= false;Car.AC_UP= true;Car.AC_FRONT= true;
      break;
      case 0x80:
        Car.AC_DOWN= true;Car.AC_UP= true;Car.AC_FRONT= true;
      break;
      case 0x50:
        Car.AC_DOWN= true;Car.AC_UP= false;Car.AC_FRONT= true;
      break;
      case 0x60:
        Car.AC_DOWN= true;Car.AC_UP= true;Car.AC_FRONT= false;
      break;
      default:
        Car.AC_DOWN= false;Car.AC_UP= false;Car.AC_FRONT= false;
      break;
      }
/*
    struct Car_data {
    uint16_t I_speed, I_rpm; // odometer info
    bool ign,ill,rear,brake,I_Eng; // I/O status
    byte AC_L,AC_R,AC_FAN,exttemp,Coolant,INF_MSG; // A/C values
    bool AC_DOWN,AC_UP,AC_FRONT,AC_Recycle,AC_WINDSH,AC_AUTO,AC_COMP,AC_OFF,AC_Mono; 
    bool DOOR_FL,DOOR_FR,DOOR_BL,DOOR_BR,DOOR_TR; //door status
    bool H_UPD_AC,H_UPD_I,H_UPD_IO,H_UPD_DOOR,H_UPD_INF,H_UPD_Trip; //update flag
    bool CH_UPD_AC; //update flag
};*/
      if (canMsgRcv.data.u8[4]&&4 ) Car.AC_Recycle=true; else Car.AC_Recycle=false;
      if (canMsgRcv.data.u8[4]&&8 ) Car.AC_WINDSH=true; else Car.AC_WINDSH=false;

      if (canMsgRcv.data.u8[0]&&32 ) Car.AC_AFAN=false; else Car.AC_AFAN=true; //autofan
      if (canMsgRcv.data.u8[0]&&2 ) Car.AC_AUTO=false; else Car.AC_AUTO=true; //auto A/C
      if (canMsgRcv.data.u8[0]&&1 ) Car.AC_WINDSH=false; else Car.AC_WINDSH=true; //windsh
      if (canMsgRcv.data.u8[0]&&128 ) Car.AC_SFAN=false; else Car.AC_SFAN=true; // Fan off

  }
bool tamp_dec_2004(byte tin) {
        if (tin ==  0) return 0; //	LO
        if (tin >1 && tin <6) return ((tin + 13) * 10)  // 2>5	15>18°
        if (tin >5 && tin <17) return ((tin + 31) * 5) // 6…16	18.5…23.5° (0.5° step)
        if (tin >16 && tin <21) return ((tin + 7) * 10)  //17…20	24…27°
        if (tin >20) return 255; //	LO
}

void  AC_decode_2010() {
      Car.AC_L = canMsgRcv.data.u8[3];
      Car.AC_R = canMsgRcv.data.u8[4];
      Car.AC_FAN = canMsgRcv.data.u8[5];// Fan Speed BSI_2010 = "41" (Off) > "49" (Full speed)
      if (canMsgRcv.data.u8[0]&&8) Car.AC_AUTO=true; else Car.AC_AUTO=false;//AirConditioningON
      if (canMsgRcv.data.u8[0]&&1) Car.AC_AUTO=true; else Car.AC_AUTO=false; //Auto normal
      if (canMsgRcv.data.u8[0]&&2) Car.AC_AUTO=true; else Car.AC_AUTO=false;//Auto fast
      if (canMsgRcv.data.u8[0]&&3) Car.AC_AUTO=false; else Car.AC_AUTO=true;//Auto soft
      Car.AC_FAN = canMsgRcv.data.u8[2]; //fan values 
      if (canMsgRcv.data.u8[2] && 15)  Car.AC_SFAN=false; else Car.AC_SFAN=true;//fan off
      // Fan Speed BSI_2010 = "41" (Off) > "49" (Full speed)
      switch (canMsgRcv.data.u8[6]) {
      case 0x44:
        Car.AC_DOWN= false;Car.AC_UP= true;Car.AC_FRONT= false;
      break;
      case 0x34:
        Car.AC_DOWN= false;Car.AC_UP= false;Car.AC_FRONT= true;
      break;
      case 0x24:
        Car.AC_DOWN= true;Car.AC_UP= false;Car.AC_FRONT= false;
      break;
      case 0x74:
        Car.AC_DOWN= false;Car.AC_UP= true;Car.AC_FRONT= true;
      break;
      case 0x84:
        Car.AC_DOWN= true;Car.AC_UP= true;Car.AC_FRONT= true;
      break;
      case 0x54:
        Car.AC_DOWN= true;Car.AC_UP= false;Car.AC_FRONT= true;
      break;
      case 0x64:
        Car.AC_DOWN= true;Car.AC_UP= true;Car.AC_FRONT= false;
      break;
      default:
        Car.AC_DOWN= false;Car.AC_UP= false;Car.AC_FRONT= false;
      break;
     }
}

void CAN1_setup() {
  CAN_cfg.speed = CAN_SPEED_125KBPS;
  CAN_cfg.tx_pin_id = GPIO_NUM_5;
  CAN_cfg.rx_pin_id = GPIO_NUM_4;
  CAN_cfg.rx_queue = xQueueCreate(rx_queue_size, sizeof(CAN_frame_t));
  // Init CAN Module
  ESP32Can.CANInit();
}
