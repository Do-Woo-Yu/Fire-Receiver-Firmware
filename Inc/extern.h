typedef unsigned char	byte;
typedef unsigned short	word;
typedef unsigned long	dword;
extern byte Tx_Data_List[50][50];
extern byte List_Cnt,Tx_Fg,Tx_Cnt,EE_Save_Cnt,Motion_test_Mode,Test_Mode_1_Cnt,Test_Mode_1_Cnt_BF;
extern byte TB1_Fire_Fg, TB2_Fire_Fg, TB3_Fire_Fg, TB4_Fire_Fg, TB5_Fire_Fg, TB6_Fire_Fg, TB7_Fire_Fg, TB8_Fire_Fg, TB9_Fire_Fg, TB10_Fire_Fg;
extern byte Main_bell_Key_Value, Main_bell_TOG, Guide_light_Key_Value, Guide_light_TOG, Fire_report_Key_Value, Fire_report_TOG, Earth_alarm_bell_Key_Value;
extern byte circulation_cnt, FIRE_NUM, MAIN_RY, FIRE_CNT;
extern void LCD_Tx(byte lcd_tx_num);
extern unsigned char Lcd_buffer[100];
extern byte EEPORM_Data_List[100];
extern uint8_t T_buffer[];
extern uint8_t R_buffer[];
extern uint16_t EEPROM_addr;
extern byte Data_sum(byte data[], byte no);
extern byte RTC_EN;
extern byte T2_500ms_flag;
extern byte MODE_SW, SET_SW, DOWN_SW, UP_SW;
extern byte MODE_BF_key, MODE_BFBF_key, MODE_Key_Value;
extern byte SET_BF_key, SET_BFBF_key, SET_Key_Value;
extern byte DOWN_BF_key, DOWN_BFBF_key, DOWN_Key_Value;
extern byte UP_BF_key, UP_BFBF_key, UP_Key_Value;
extern byte UP_DOWN_key_value_cnt, WHAT_MODE;
extern byte EEPROM_WTimer, EEPROM_RTimer;
extern uint16_t adcVal[14];
extern float BAT_AD, TB_IN_AD1, TB_IN_AD2, TB_IN_AD3, TB_IN_AD4, TB_IN_AD5, TB_IN_AD6,
TB_IN_AD7, TB_IN_AD8, TB_IN_AD9, TB_IN_AD10, CHK_26V, TB_SEND_AD, COIN_AD;
extern unsigned int RESTART_HOME;
extern byte Rx1_buf[100];
extern byte Rx1_index;
extern word CheckSum_data;
extern byte Rx1_step;
extern byte Rx1_CRC_H, Rx1_CRC_L;
extern byte CheckSum_ing;
extern byte CheckSum_EN;
extern word Fire_Detector_Data_crc, P_Type_Reciever_Data_crc;
extern byte success_cnt, fail_cnt, Fire_Schematic_Num;

extern word CRC16(byte *buf, word size);

extern unsigned char Date_Data[21]; // RTC 날짜 데이터를 LCD로 전송  2024.10.19
extern unsigned char Time_Data[19];// RTC 시간 데이터를 LCD로 전송  19:28:12

/** 화면 전환 명령어 **/
extern unsigned char SCREEN0_GO[9]; // SCREEN0으로 이동(홈 화면)
extern unsigned char SCREEN1_GO[9]; // SCREEN1으로 이동(MODE 화면)
extern unsigned char SCREEN2_GO[9]; // SCREEN2으로 이동(이벤트 보기 화면)
extern unsigned char SCREEN3_GO[9]; // SCREEN3으로 이동(USB 다운로드 화면)
extern unsigned char SCREEN4_GO[9]; // SCREEN4으로 이동(시간 조정 화면)

/** MODE에서 텍스트 색상 변경 명령어 **/
extern unsigned char MODE1_TEXT_RED[13]; // MODE 화면에서 이벤트 보기 텍스트 빨간색
extern unsigned char MODE1_TEXT_BLACK[13]; // MODE 화면에서 이벤트 보기 텍스트 검정색

extern unsigned char MODE2_TEXT_RED[13]; // MODE 화면에서 USB 다운로드 텍스트 빨간색
extern unsigned char MODE2_TEXT_BLACK[13]; // MODE 화면에서 USB 다운로드 텍스트 검정색

extern unsigned char MODE3_TEXT_RED[13]; // MODE 화면에서 시간 조정 텍스트 빨간색
extern unsigned char MODE3_TEXT_BLACK[13]; // MODE 화면에서 시간 조정 텍스트 검정색

/** 시간 조정 화면 에서 텍스트 색상 변경 명령어 **/
extern unsigned char DATE_YEAR_ADJ_TEXT_BLACK[13]; // 시간 조정 화면에서 달력의 년 텍스트 검정색
extern unsigned char DATE_YEAR_ADJ_TEXT_RED[13]; // 시간 조정 화면에서 달력의 년 텍스트 빨간색

extern unsigned char DATE_MONTH_ADJ_TEXT_BLACK[13]; // 시간 조정 화면에서 달력의 월 텍스트 검정색
extern unsigned char DATE_MONTH_ADJ_TEXT_RED[13]; // 시간 조정 화면에서 달력의 월 텍스트 빨간색

extern unsigned char DATE_DATE_ADJ_TEXT_BLACK[13]; // 시간 조정 화면에서 달력의 일 텍스트 검정색
extern unsigned char DATE_DATE_ADJ_TEXT_RED[13]; // 시간 조정 화면에서 달력의 일 텍스트 빨간색

extern unsigned char TIME_HOUR_ADJ_TEXT_BLACK[13]; // 시간 조정 화면에서 시간의 시 텍스트 검정색
extern unsigned char TIME_HOUR_ADJ_TEXT_RED[13]; // 시간 조정 화면에서 시간의 시 텍스트 빨간색

extern unsigned char TIME_MINUTE_ADJ_TEXT_BLACK[13]; // 시간 조정 화면에서 시간의 분 텍스트 검정색
extern unsigned char TIME_MINUTE_ADJ_TEXT_RED[13]; // 시간 조정 화면에서 시간의 분 텍스트 빨간색

extern unsigned char TIME_SEC_ADJ_TEXT_BLACK[13]; // 시간 조정 화면에서 시간의 초 텍스트 검정색
extern unsigned char TIME_SEC_ADJ_TEXT_RED[13]; // 시간 조정 화면에서 시간의 초 텍스트 빨간색
