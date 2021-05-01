#include <SPI.h>
#include <mcp2515.h> // https://github.com/autowp/arduino-mcp2515

#define CS_PIN_CAN0 10

MCP2515 CAN0(CS_PIN_CAN0); // CAN-BUS Shield N°1



void CAN1_loop() {
  if (CAN0.readMessage( & canMsgRcv) == MCP2515::ERROR_OK) {
    uint16_t id = canMsgRcv.can_id;
    uint16_t len = canMsgRcv.can_dlc;
     switch (id) {
        case 182: //0B6 rpm speed Fuel consumtion info (50ms)
          if (len == 8) {
            uint16_t rpm = canMsgRcv.data[0] + canMsgRcv.data[1] * 256; // to be verified
            
            
            if (rpm > 0 ) { // Engine RPM, 0 when the engine is OFF
              EngineRunning = true;
            } else {
              EngineRunning = false;
            }
          }
        break;
        case 225: //0E1 parktronic
        break;
        case 246: //0F6 bsi TEMP COOLANT ?EXT? reverse gear ignition odometer
        break;
        case 296: //128 Dashboard lights
        break;
        case 417: //1A1 Informational message
         Car.INF_MSG = canMsgRcv.data[1]; //send as is to hu
         if (canMsgRcv.data[2] == 1) Car.H_UPD_INF = 1; // ambigous
         if (canMsgRcv.data[0] == 0x80) Car.H_UPD_INF = 1;  // ambigous
        break;
        case 493: //1ED Display conditioning commands
         if (canMsgRcv.data[0] == 0x00) Car.AC_Mono =1; else Car.AC_Mono =0;
         if (canMsgRcv.data[0] == 0x18) Car.AC_OFF =1; else Car.AC_OFF =0;
        break;
        case 543: //21F Radio remote control under the steering wheel
         if (canMsgRcv.data[0]&&1) //forward
         if (canMsgRcv.data[0]&&2) //backward
         if (canMsgRcv.data[0]&&8) //volume UP
         if (canMsgRcv.data[0]&&16) //volume DOWN
        break;
        case 544: //220 Door status
        break;
        case 545: //221 Trip computer info
        break;
        case 464: //1D0 Climate control information
          if (len == 7 && EngineRunning) { // No fan activated if the engine is not ON on old models
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
      LeftTemp = canMsgRcv.data[5];
      RightTemp = canMsgRcv.data[6];
      if (LeftTemp == RightTemp) { // No other way to detect MONO mode
        Mono = true;
        LeftTemp = LeftTemp + 64;
      } else {
        Mono = false;
      }

      FanOff = false;
      // Fan Speed BSI_2010 = "41" (Off) > "49" (Full speed)
      tmpVal = canMsgRcv.data[2];
      if (tmpVal == 15) {
        FanOff = true;
        FanSpeed = 0x41;
      } else {
        FanSpeed = (tmpVal + 66);
      }

      // Position Fan
      tmpVal = canMsgRcv.data[3];

      if (tmpVal == 0x40) {
        FootAerator = false;
        WindShieldAerator = true;
        CentralAerator = false;
      } else if (tmpVal == 0x30) {
        FootAerator = false;
        WindShieldAerator = false;
        CentralAerator = true;
      } else if (tmpVal == 0x20) {
        FootAerator = true;
        WindShieldAerator = false;
        CentralAerator = false;
      } else if (tmpVal == 0x70) {
        FootAerator = false;
        WindShieldAerator = true;
        CentralAerator = true;
      } else if (tmpVal == 0x80) {
        FootAerator = true;
        WindShieldAerator = true;
        CentralAerator = true;
      } else if (tmpVal == 0x50) {
        FootAerator = true;
        WindShieldAerator = false;
        CentralAerator = true;
      } else if (tmpVal == 0x10) {
        FootAerator = false;
        WindShieldAerator = false;
        CentralAerator = false;
      } else if (tmpVal == 0x60) {
        FootAerator = true;
        WindShieldAerator = true;
        CentralAerator = false;
      } else {
        FootAerator = false;
        WindShieldAerator = false;
        CentralAerator = false;
      }

      tmpVal = canMsgRcv.data[4];
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

      tmpVal = canMsgRcv.data[0];
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
          canMsgSnd.data[0] = 0x01; // A/C ON - Auto Soft : "00" / Auto Normal "01" / Auto Fast "02"
        } else {
          canMsgSnd.data[0] = 0x09; // A/C OFF - Auto Soft : "08" / Auto Normal "09" / Auto Fast "0A"
        }

        canMsgSnd.data[1] = 0x00;
        canMsgSnd.data[2] = 0x00;
        canMsgSnd.data[3] = LeftTemp;
        canMsgSnd.data[4] = RightTemp;
        canMsgSnd.data[5] = FanSpeed;
        canMsgSnd.data[6] = FanPosition;
        canMsgSnd.data[7] = 0x00;
        canMsgSnd.can_id = 0x350;
        canMsgSnd.can_dlc = 8;
        CAN1.sendMessage( & canMsgSnd);
        if (Send_CAN2010_ForgedMessages) {
          CAN0.sendMessage( & canMsgSnd);
        }
    
  }
  }
