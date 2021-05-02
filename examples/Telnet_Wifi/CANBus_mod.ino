#include <Arduino.h>
#include <ESP32CAN.h> // https://github.com/miwagner/ESP32-Arduino-CAN
#include <CAN_config.h>

CAN_device_t CAN_cfg;                // CAN Config
const int rx_queue_size = 10;       // Receive Queue size

struct Canbus_previous_val {
    byte rpm[2]; // odometer info
    byte ext_temp;
    byte doors;
    bool ign,ill,rear,brake; // I/O status
};

Canbus_previous_val CAN_OV;

CAN_frame_t canMsgRcv;

void CAN1_loop() {
  if (xQueueReceive(CAN_cfg.rx_queue, &rx_frame, 3 * portTICK_PERIOD_MS) == pdTRUE)
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
        case 417: //0x1A1 Informational message
         Car.INF_MSG = canMsgRcv.data.u8[1]; //send as is to hu
         if (canMsgRcv.data.u8[2] == 1) Car.H_UPD_INF = 1; // ambigous
         if (canMsgRcv.data.u8[0] == 0x80) Car.H_UPD_INF = 1;  // ambigous
        break;
        case 493: //0x1ED Display conditioning commands
         if (canMsgRcv.data.u8[0] == 0x00) Car.AC_Mono =1; else Car.AC_Mono =0;
         if (canMsgRcv.data.u8[0] == 0x18) Car.AC_OFF =1; else Car.AC_OFF =0;
        break;
        case 543: //0x21F Radio remote control under the steering wheel
         if (canMsgRcv.data.u8[0]&&1) //forward
         if (canMsgRcv.data.u8[0]&&2) //backward
         if (canMsgRcv.data.u8[0]&&16) //volume UP
         if (canMsgRcv.data.u8[0]&&32) //volume DOWN
         if (canMsgRcv.data.u8[0]&&64) //Source
         //Car.??? = canMsgRcv.data.u8[1] //scroll value from disk selection
           
        break;
        case 544: //0x220 Door status
         if (canMsgRcv.data.u8[0] !=  CAN_OV.Doors) { 
          if (canMsgRcv.data.u8[0]&&1) DOOR_FL = true; else DOOR_FL = false;
          if (canMsgRcv.data.u8[0]&&2) DOOR_FR = true; else DOOR_FR = false;
          if (canMsgRcv.data.u8[0]&&4) DOOR_BL = true; else DOOR_BL = false;
          if (canMsgRcv.data.u8[0]&&8) DOOR_BR = true; else DOOR_BR = false;
          if (canMsgRcv.data.u8[0]&&16) DOOR_TR = true; else DOOR_TR = false;
          CAN_OV.Doors = canMsgRcv.data.u8[0];
          Car.H_UPD_DOOR=true;
         }  
        break;
        case 545: //0x221 Trip computer info
          if (canMsgRcv.FIR.B.DLC == 7) {
            if (canMsgRcv.data.u8[0]&&16) //trip command button
            if (canMsgRcv.data.u8[0]&&128) //voice command button
            Car.T_fuelC=canMsgRcv.data.u8[1]+canMsgRcv.data.u8[2]*256; //divide by 10 for L/100KM
            Car.T_fuelR=canMsgRcv.data.u8[3]canMsgRcv.data.u8[4]*256;
            Car.H_UPD_Trip = true;  
          }
        break;
        case 609: //0x261 Trip computer info A
            Car.TA_Spd=canMsgRcv.data.u8[0];
            Car.TA_Mil=(canMsgRcv.data.u8[1]+canMsgRcv.data.u8[2]*256)/4;
            Car.TA_Fuelc=(canMsgRcv.data.u8[3]; //divide by 10 for L/100KM
            Car.H_UPD_Trip = true;  
        break;
        case 464: //0x1D0 Climate control information
          if (canMsgRcv.FIR.B.DLC == 7 && Car.I_Eng) { // No fan activated if the engine is not ON on old models
            if (canMsgRcv.data.u8[0]&&64)   //REAR_WINDSHIELD_HEAT
            AC_decode_2004();
          }
        break;
        case 848: //0x350
            AC_decode_2010();
        break;
        default:
      
        break;
}
    
    
    void  AC_decode_2004() {
      LeftTemp = canMsgRcv.data.u8[5];
      RightTemp = canMsgRcv.data.u8[6];
      if (LeftTemp == RightTemp) { // No other way to detect MONO mode
        Mono = true;
        LeftTemp = LeftTemp + 64;
      } else {
        Mono = false;
      }

      FanOff = false;
      // Fan Speed BSI_2010 = "41" (Off) > "49" (Full speed)
      tmpVal = canMsgRcv.data.u8[2];
      if (tmpVal == 15) {
        FanOff = true;
        FanSpeed = 0x41;
      } else {
        FanSpeed = (tmpVal + 66);
      }

      // Position Fan
      switch (canMsgRcv.data.u8[3]) {
      case 0x40:
        Car.AC_DOWN= false;Car.AC_UP= true;Car.AC_FRONT= false;
      break;
      case 0x30:
        Car.AC_DOWN= false;Car.AC_UP= false;Car.AC_FRONT= true;
      break;y
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

      tmpVal = canMsgRcv.data.u8[4];
      if (tmpVal == 0x10) {
        DeMist = true;
        AirRecycle = false;
      } else if (tmpVal == 0x30) {
        AirRecycle = true;
      } else {
        AirRecycle = false;
      }

      AutoFan = false;
      DeMist = false;

      tmpVal = canMsgRcv.data.u8[0];
      if (tmpVal == 0x11) {
        DeMist = true;
        AirConditioningON = true;
        FanOff = false;
      } else if (tmpVal == 0x12) {
        DeMist = true;
        AirConditioningON = false;
        FanOff = false;
      } else if (tmpVal == 0x21) {
        DeMist = true;
        AirConditioningON = true;
        FanOff = false;
      } else if (tmpVal == 0xA2) {
        FanOff = true;
        AirConditioningON = false;
      } else if (tmpVal == 0x22) {
        AirConditioningON = false;
      } else if (tmpVal == 0x20) {
        AirConditioningON = true;
      } else if (tmpVal == 0x02) {
        AirConditioningON = false;
        AutoFan = false;
      } else if (tmpVal == 0x00) {
        AirConditioningON = true;
        AutoFan = true;
      }

  }
/* 2011version  work in progess
              if (!FootAerator && !WindShieldAerator && CentralAerator) {
          FanPosition = 0x34;
        } else if (FootAerator && WindShieldAerator && CentralAerator) {
          FanPosition = 0x84;
        } else if (!FootAerator && WindShieldAerator && CentralAerator) {
          FanPosition = 0x74;
        } else if (FootAerator && !WindShieldAerator && CentralAerator) {
          FanPosition = 0x54;
        } else if (FootAerator && !WindShieldAerator && !CentralAerator) {
          FanPosition = 0x24;
        } else if (!FootAerator && WindShieldAerator && !CentralAerator) {
          FanPosition = 0x44;
        } else if (FootAerator && WindShieldAerator && !CentralAerator) {
          FanPosition = 0x64;
        } else {
          FanPosition = 0x04; // Nothing
        }

        if (DeMist) {
          FanSpeed = 0x10;
          FanPosition = FanPosition + 16;
        } else if (AutoFan) {
          FanSpeed = 0x10;
        }

        if (FanOff) {
          AirConditioningON = false;
          FanSpeed = 0x41;
          LeftTemp = 0x00;
          RightTemp = 0x00;
          FanPosition = 0x04;
        }

        if (AirConditioningON) {
          canMsgSnd.data.u8[0] = 0x01; // A/C ON - Auto Soft : "00" / Auto Normal "01" / Auto Fast "02"
        } else {
          canMsgSnd.data.u8[0] = 0x09; // A/C OFF - Auto Soft : "08" / Auto Normal "09" / Auto Fast "0A"
        }

        canMsgSnd.data.u8[1] = 0x00;
        canMsgSnd.data.u8[2] = 0x00;
        canMsgSnd.data.u8[3] = LeftTemp;
        canMsgSnd.data.u8[4] = RightTemp;
        canMsgSnd.data.u8[5] = FanSpeed;
        canMsgSnd.data.u8[6] = FanPosition;
        canMsgSnd.data.u8[7] = 0x00;
        canMsgSnd.can_id = 0x350;
        canMsgSnd.can_dlc = 8;
        CAN1.sendMessage( & canMsgSnd);
        if (Send_CAN2010_ForgedMessages) {
          CAN0.sendMessage( & canMsgSnd);
        }
    
  }

*/

void CAN1_setup() {
  CAN_cfg.speed = CAN_SPEED_125KBPS;
  CAN_cfg.tx_pin_id = GPIO_NUM_5;
  CAN_cfg.rx_pin_id = GPIO_NUM_4;
  CAN_cfg.rx_queue = xQueueCreate(rx_queue_size, sizeof(CAN_frame_t));
  // Init CAN Module
  ESP32Can.CANInit();
}
