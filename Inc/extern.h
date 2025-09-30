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

extern unsigned char Date_Data[21]; // RTC ��¥ �����͸� LCD�� ����  2024.10.19
extern unsigned char Time_Data[19];// RTC �ð� �����͸� LCD�� ����  19:28:12

/** ȭ�� ��ȯ ��ɾ� **/
extern unsigned char SCREEN0_GO[9]; // SCREEN0���� �̵�(Ȩ ȭ��)
extern unsigned char SCREEN1_GO[9]; // SCREEN1���� �̵�(MODE ȭ��)
extern unsigned char SCREEN2_GO[9]; // SCREEN2���� �̵�(�̺�Ʈ ���� ȭ��)
extern unsigned char SCREEN3_GO[9]; // SCREEN3���� �̵�(USB �ٿ�ε� ȭ��)
extern unsigned char SCREEN4_GO[9]; // SCREEN4���� �̵�(�ð� ���� ȭ��)

/** MODE���� �ؽ�Ʈ ���� ���� ��ɾ� **/
extern unsigned char MODE1_TEXT_RED[13]; // MODE ȭ�鿡�� �̺�Ʈ ���� �ؽ�Ʈ ������
extern unsigned char MODE1_TEXT_BLACK[13]; // MODE ȭ�鿡�� �̺�Ʈ ���� �ؽ�Ʈ ������

extern unsigned char MODE2_TEXT_RED[13]; // MODE ȭ�鿡�� USB �ٿ�ε� �ؽ�Ʈ ������
extern unsigned char MODE2_TEXT_BLACK[13]; // MODE ȭ�鿡�� USB �ٿ�ε� �ؽ�Ʈ ������

extern unsigned char MODE3_TEXT_RED[13]; // MODE ȭ�鿡�� �ð� ���� �ؽ�Ʈ ������
extern unsigned char MODE3_TEXT_BLACK[13]; // MODE ȭ�鿡�� �ð� ���� �ؽ�Ʈ ������

/** �ð� ���� ȭ�� ���� �ؽ�Ʈ ���� ���� ��ɾ� **/
extern unsigned char DATE_YEAR_ADJ_TEXT_BLACK[13]; // �ð� ���� ȭ�鿡�� �޷��� �� �ؽ�Ʈ ������
extern unsigned char DATE_YEAR_ADJ_TEXT_RED[13]; // �ð� ���� ȭ�鿡�� �޷��� �� �ؽ�Ʈ ������

extern unsigned char DATE_MONTH_ADJ_TEXT_BLACK[13]; // �ð� ���� ȭ�鿡�� �޷��� �� �ؽ�Ʈ ������
extern unsigned char DATE_MONTH_ADJ_TEXT_RED[13]; // �ð� ���� ȭ�鿡�� �޷��� �� �ؽ�Ʈ ������

extern unsigned char DATE_DATE_ADJ_TEXT_BLACK[13]; // �ð� ���� ȭ�鿡�� �޷��� �� �ؽ�Ʈ ������
extern unsigned char DATE_DATE_ADJ_TEXT_RED[13]; // �ð� ���� ȭ�鿡�� �޷��� �� �ؽ�Ʈ ������

extern unsigned char TIME_HOUR_ADJ_TEXT_BLACK[13]; // �ð� ���� ȭ�鿡�� �ð��� �� �ؽ�Ʈ ������
extern unsigned char TIME_HOUR_ADJ_TEXT_RED[13]; // �ð� ���� ȭ�鿡�� �ð��� �� �ؽ�Ʈ ������

extern unsigned char TIME_MINUTE_ADJ_TEXT_BLACK[13]; // �ð� ���� ȭ�鿡�� �ð��� �� �ؽ�Ʈ ������
extern unsigned char TIME_MINUTE_ADJ_TEXT_RED[13]; // �ð� ���� ȭ�鿡�� �ð��� �� �ؽ�Ʈ ������

extern unsigned char TIME_SEC_ADJ_TEXT_BLACK[13]; // �ð� ���� ȭ�鿡�� �ð��� �� �ؽ�Ʈ ������
extern unsigned char TIME_SEC_ADJ_TEXT_RED[13]; // �ð� ���� ȭ�鿡�� �ð��� �� �ؽ�Ʈ ������
