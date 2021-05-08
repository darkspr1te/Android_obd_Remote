// canbus sniffer for esp32
// to Serial

#include <Arduino.h>
#include <ESP32CAN.h> // https://github.com/miwagner/ESP32-Arduino-CAN
#include <CAN_config.h>

CAN_device_t CAN_cfg;                // CAN Config
const long SERIAL_SPEED = 115200;

CAN_frame_t canMsgRcv;
CAN_frame_t canMsgSend;

void loop() {
  if (xQueueReceive(CAN_cfg.rx_queue, &canMsgRcv, 3 * portTICK_PERIOD_MS) == pdTRUE) {
    Serial.print("FRAME:ID=");
    Serial.println(canMsgRcv.MsgID);
    Serial.print(":LEN=");
    Serial.println(canMsgRcv.FIR.B.DLC);
    for (byte i = 0; i < canMsgRcv.FIR.B.DLC; i++) {
      Serial.print(':');
      Serial.println(canMsgRcv.data.u8[i], HEX);
    }
    Serial.print('\r\n');    
  }
}
         
void setup() {
  Serial.begin(SERIAL_SPEED);
  CAN_cfg.speed = CAN_SPEED_125KBPS;
  CAN_cfg.tx_pin_id = GPIO_NUM_5;
  CAN_cfg.rx_pin_id = GPIO_NUM_4;
  CAN_cfg.rx_queue = xQueueCreate(rx_queue_size, sizeof(CAN_frame_t));
  // Init CAN Module
  ESP32Can.CANInit();
  Serial.begin(
}
