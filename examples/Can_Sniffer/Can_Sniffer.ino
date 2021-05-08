#include <Arduino.h>
#include <ESP32CAN.h> // https://github.com/miwagner/ESP32-Arduino-CAN
#include <CAN_config.h>

CAN_device_t CAN_cfg;                // CAN Config


CAN_frame_t canMsgRcv;
CAN_frame_t canMsgSend;

void loop() {
  if (xQueueReceive(CAN_cfg.rx_queue, &canMsgRcv, 3 * portTICK_PERIOD_MS) == pdTRUE) {
     switch (canMsgRcv.MsgID) {
             if (canMsgRcv.FIR.B.DLC == 8) {
          canMsgRcv.data.u8[0]
}
         
void CAN1_setup() {
  CAN_cfg.speed = CAN_SPEED_125KBPS;
  CAN_cfg.tx_pin_id = GPIO_NUM_5;
  CAN_cfg.rx_pin_id = GPIO_NUM_4;
  CAN_cfg.rx_queue = xQueueCreate(rx_queue_size, sizeof(CAN_frame_t));
  // Init CAN Module
  ESP32Can.CANInit();
}
