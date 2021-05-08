// MCU >> HU
	#define CH_CMD_BACKLIGHT_INFO 0x01;
	#define CH_CMD_STEERING_WHEEL_KEY 0x02
	#define CH_CMD_AIR_CONDITIONING_INFO 0x21
	#define CH_CMD_STEERING_WHEEL_ANGLE 0x29
	#define CH_CMD_FULL_RADAR_INFO 0x30
	#define CH_CMD_REVERSE_RADAR_INFO 0x32
	#define CH_CMD_COMPUTE_INFO_PAGE0 0x33
	#define CH_CMD_COMPUTE_INFO_PAGE1 0x34
	#define CH_CMD_COMPUTE_INFO_PAGE2 0x35
	#define CH_CMD_EXTERN_TEMP 0x36
	#define CH_CMD_ALARM_RECORD_INFO 0x37
	#define CH_CMD_VEHICLE_STATUS 0x38
	#define CH_CMD_VEHICLE_FUNC_INFO 0x39
	#define CH_CMD_VEHICLE_DIAGNOSTIC_INFO 0x3A
	#define CH_CMD_RECORD_SPEED_VALUE 0x3B
	#define CH_CMD_SPEED_INFO 0x3D
	#define CH_CMD_SPEED_ALARM_DIALOG 0x3F
	#define CH_CMD_PROTOCOL_VERSION_INFO 0x7F

// HU >> MCU
  #define HC_CMD_AC_CONTROL 0x8A
  #define HC_CMD_VEHICLE_PARAM_SETTING 0x80
	#define HC_CMD_COMPUTE_PARAM_SETTING 0x82
	#define HC_CMD_REQ_VEHICLE_ALARM_INFO 0x85
	#define HC_CMD_REQ_FUNCTION_STATUS_INFO 0x86
	#define HC_CMD_VEHICLE_DISGNOSTIC_INFO 0x87
	#define HC_CMD_SET_RECORD_SPEED 0x88
	#define HC_CMD_SET_SPEED_INFO 0x89
	#define HC_CMD_AC_CONTROL 0x8A
	#define HC_CMD_SCREEN_DISPLAY 0x8C
	#define HC_CMD_REQ_CONTROL_INFO 0x8F
	#define HC_CMD_SET_SPEEDOMETER 0x99
	#define HC_CMD_SET_TIME 0xA6
	
	#define SWC_KEY_NONE 0x00
	#define SWC_KEY_MENU 0x02
	#define SWC_KEY_MENU_UP 0x03
	#define SWC_KEY_MENU_DOWN 0x04
	#define SWC_KEY_OK 0x07 //OK
	#define SWC_KEY_ESC 0x08 //ESC
	#define SWC_KEY_MODE 0x10 //Mode
	#define SWC_KEY_SRC 0x11 //Source
	#define SWC_KEY_SEEK_DOWN 0x12 //Seek+
	#define SWC_KEY_SEEK_UP 0x13 //Seek-
	#define SWC_KEY_VOLUME_UP 0x14 //Volume+
	#define SWC_KEY_VOLUME_DOWN 0x15 //Volume-
	#define SWC_KEY_MUTE 0x16 //Mute
	#define SWC_KEY_MEMO_UP 0x17 //上一曲
	#define SWC_KEY_MEMO_DOWN 0x18 //下一曲
	#define SWC_KEY_PAGE_SW 0x20 //电脑信息页切换
	#define SWC_KEY_MENU4 0x21 //菜单
	#define SWC_KEY_MEMO 0x22 //记忆速度界面
	#define SWC_KEY_BT 0x23 //蓝牙按键
	#define SWC_KEY_PUSH_TO_TALK 0x29 //
	#define SWC_KEY_VEHICLE_SETTING 0x2A //车身设置
	#define SWC_KEY_VEHICLE_NAVI 0x2B 
	#define SWC_KEY_MUSIC 0x2C 
	#define SWC_KEY_BLUETOOTH 0x2D 
	#define SWC_KEY_APPS 0x2E 
	#define SWC_KEY_AIR_CONDTION_CONTROL 0x2F 
	#define SWC_KEY_PHONE_ACCEPT 0x30 //电话接听	
	#define SWC_KEY_PHONE_HANGUP 0x31 //电话挂断
	#define SWC_KEY_NAVI 0x32 //Navi
	#define SWC_KEY_RADIO 0x33 //收音
	#define SWC_KEY_SETUP 0x34 //设置
	#define SWC_KEY_ADDR 0x35 //Addr
	#define SWC_KEY_MEDIA 0x36
	#define SWC_KEY_TRAF 0x37
	#define SWC_KEY_UP 0x38
	#define SWC_KEY_DOWN 0x39
	#define SWC_KEY_LEFT  0x40
	#define SWC_KEY_RIGHT 0x41
	#define SWC_KEY_SCROLL_UP 0x42
	#define SWC_KEY_SCROLL_DOWN 0x43
	#define SWC_KEY_NUM1 0x44
	#define SWC_KEY_NUM2 0x45
	#define SWC_KEY_NUM3 0x46
	#define SWC_KEY_NUM4 0x47
	#define SWC_KEY_NUM5 0x48
	#define SWC_KEY_NUM6 0x49
	#define SWC_KEY_SRC2 0x4A
	#define SWC_KEY_BAND 0x50
	#define SWC_KEY_LIST 0x51
	#define SWC_KEY_SOUND 0x52
	#define SWC_KEY_TA 0x53
	#define SWC_KEY_DARK 0x54
	#define SWC_KEY_EJECT 0x55
	#define SWC_KEY_RIGHT2 0x56
	#define SWC_KEY_LEFT2 0x57
	#define SWC_KEY_UP2	0x58
	#define SWC_KEY_DOWN2 0x59
	#define SWC_KEY_MENU2 0x5A
	#define SWC_KEY_MENU3 0x5B
	#define SWC_KEY_OK2	0x5C
	#define SWC_KEY_MUTE2 0x5D
	#define SWC_KEY_BACK 0x5E
	#define SWC_KEY_CHECK 0x60
	#define SWC_KEY_POWER 0x80


	#define AC_LOWEST_TEMP_C 0.5f
	#define AC_HIGHEST_TEMP_C 127.0f
	
	#define AC_LOWEST_TEMP_F 1.0f
	#define AC_HIGHEST_TEMP_F 254.0f
	
	#define EXTERN_LOWEST_TEMP  -50.0f
	#define EXTERN_HIGHEST_TEMP 77.5f