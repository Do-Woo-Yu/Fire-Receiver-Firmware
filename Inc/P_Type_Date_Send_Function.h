#include "extern.h" 
#include "UART_LCD_SEND.h"
//LCD로 이벤트 전송하는 함수
void Send_EVENT_1(void);
void Send_EVENT_1(void);
void Send_EVENT_1(void);

extern word P_Type_Cnt;
extern byte P_Type_Receiver_Event[1001][64];
extern UART_HandleTypeDef huart3;

void Send_EVENT_1(void) // LCD로 첫번째 이벤트 보기 전송
{
  /** LCD로 첫번째 이벤트 데이터를 보냄 **/
  // Event_Date_Time_1 배열에 P_Type_Receiver_Event[2][5] ~ P_Type_Receiver_Event[2][24] 복사
  for (int i = 0; i < 20; i++) {
	Event_Date_Time_1[i] = P_Type_Receiver_Event[(Event_Page*3)-1][5 + i];
  }

  // Send_Event_Date_Time_1 배열에 번호를 매기고 명령어를 붙여 데이터를 재 가공
  Event_Date_Time_Length_1 = sprintf(&Send_Event_Date_Time_1[7], "%s", Event_Date_Time_1);
  Send_Event_Date_Time_1[7 + Event_Date_Time_Length_1] = 0xFF;
  Send_Event_Date_Time_1[8 + Event_Date_Time_Length_1] = 0xFC;
  Send_Event_Date_Time_1[9 + Event_Date_Time_Length_1] = 0xFF;
  Send_Event_Date_Time_1[10 + Event_Date_Time_Length_1] = 0xFF;


  // Event_Text_1 배열에 P_Type_Receiver_Event[2][27]부터 0x0D가 나오기 전까지 복사
  for (int i = 27; i < 100; i++) {	
	if (P_Type_Receiver_Event[(Event_Page*3)-1][i] == 0x0D) {
		break; // 0x0D가 나오면 중단
	}
	Event_Text_1[Text_Index_1++] = P_Type_Receiver_Event[(Event_Page*3)-1][i];
  }
  
  byte save_p_type_cnt = 0;
  
  if(P_Type_Cnt >= 1000) // 999개 저장 완료 표시 이때 부터는 저장하는 방식이 달라짐
  {
	save_p_type_cnt = P_Type_Cnt - 999;
  }
  
  // Send_Event_Text_1 배열에 번호를 매기고 명령어를 붙여 데이터를 재 가공
  if(save_p_type_cnt < 10) // 1의 자리
  {
	Event_Text_Length_1 = sprintf(&Send_Event_Text_1[7], "  %d. %s", (Event_Page*3)-2, Event_Text_1);
	Send_Event_Text_1[7 + Event_Text_Length_1] = 0xFF;
	Send_Event_Text_1[8 + Event_Text_Length_1] = 0xFC;
	Send_Event_Text_1[9 + Event_Text_Length_1] = 0xFF;
	Send_Event_Text_1[10 + Event_Text_Length_1] = 0xFF;
  }
  else if(save_p_type_cnt >= 10 && save_p_type_cnt < 100) // 10의 자리
  {
	Event_Text_Length_1 = sprintf(&Send_Event_Text_1[7], "  %d. %s", (Event_Page*3)-2, Event_Text_1);
	Send_Event_Text_1[7 + Event_Text_Length_1] = 0xFF;
	Send_Event_Text_1[8 + Event_Text_Length_1] = 0xFC;
	Send_Event_Text_1[9 + Event_Text_Length_1] = 0xFF;
	Send_Event_Text_1[10 + Event_Text_Length_1] = 0xFF;
  }
  else if(save_p_type_cnt >= 100 && save_p_type_cnt < 1000) // 100의 자리
  {
	Event_Text_Length_1 = sprintf(&Send_Event_Text_1[7], "  %d. %s", (Event_Page*3)-2, Event_Text_1);
	Send_Event_Text_1[7 + Event_Text_Length_1] = 0xFF;Send_Event_Text_1[8 + Event_Text_Length_1] = 0xFC;
	Send_Event_Text_1[9 + Event_Text_Length_1] = 0xFF;
	Send_Event_Text_1[10 + Event_Text_Length_1] = 0xFF;
  }

  // LCD로 번호. 첫번째 이벤트 종류 데이터를 전송
  HAL_UART_Transmit(&huart3, Send_Event_Text_1_Reset, 11, 10); // 이벤트 화면에서 첫번째 이벤트 종류 초기화 데이터 전송
  HAL_UART_Transmit(&huart3, Send_Event_Text_1, 11 + Event_Text_Length_1, 10); // 이벤트 화면에서 첫번째 이벤트 종류 데이터 전송

  // LCD로 번호. 첫번째 이벤트 날짜 및 시간 데이터를 전송
  HAL_UART_Transmit(&huart3, Send_Event_Date_Time_1_Reset, 11, 10); // 이벤트 화면에서 첫번째 이벤트 종류 초기화 데이터 전송
  HAL_UART_Transmit(&huart3, Send_Event_Date_Time_1, 11 + Event_Date_Time_Length_1, 10); // 이벤트 화면에서 첫번째 이벤트 종류 데이터 전송

  Text_Index_1 = 0;
  Event_Text_Length_1 = 0;
  Event_Date_Time_Length_1 = 0;

  for(int eve_txt_1_reset = 0; eve_txt_1_reset < 50; eve_txt_1_reset++)
  {
	Event_Text_1[eve_txt_1_reset] = '\0';
  }
  for(int eve_date_time_1_reset = 0; eve_date_time_1_reset < 50; eve_date_time_1_reset++)
  {
	Event_Date_Time_1[eve_date_time_1_reset] = '\0';
  }
}

void Send_EVENT_2(void) // LCD로 두번째 이벤트 보기 전송
{
  /** LCD로 두번째 이벤트 데이터를 보냄 **/
  // Event_Date_Time_2 배열에 P_Type_Receiver_Event[2][5] ~ P_Type_Receiver_Event[2][24] 복사
  for (int i = 0; i < 20; i++) {
	Event_Date_Time_2[i] = P_Type_Receiver_Event[(Event_Page*3)][5 + i];
  }

  // Send_Event_Date_Time_2 배열에 번호를 매기고 명령어를 붙여 데이터를 재 가공
  Event_Date_Time_Length_2 = sprintf(&Send_Event_Date_Time_2[7], "%s", Event_Date_Time_2);
  Send_Event_Date_Time_2[7 + Event_Date_Time_Length_2] = 0xFF;
  Send_Event_Date_Time_2[8 + Event_Date_Time_Length_2] = 0xFC;
  Send_Event_Date_Time_2[9 + Event_Date_Time_Length_2] = 0xFF;
  Send_Event_Date_Time_2[10 + Event_Date_Time_Length_2] = 0xFF;

  // Event_Text_1 배열에 P_Type_Receiver_Event[2][27]부터 0x0D가 나오기 전까지 복사
  for (int i = 27; i < 100; i++) {	
	if (P_Type_Receiver_Event[(Event_Page*3)][i] == 0x0D) {
		break; // 0x0D가 나오면 중단
	}
	Event_Text_2[Text_Index_2++] = P_Type_Receiver_Event[(Event_Page*3)][i];
  }
  
  byte save_p_type_cnt = 0;
  
  if(P_Type_Cnt >= 1000) // 999개 저장 완료 표시 이때 부터는 저장하는 방식이 달라짐
  {
	save_p_type_cnt = P_Type_Cnt - 999;
  }
  
  // Send_Event_Text_1 배열에 번호를 매기고 명령어를 붙여 데이터를 재 가공
  if(save_p_type_cnt < 10) // 1의 자리
  {
	Event_Text_Length_2 = sprintf(&Send_Event_Text_2[7], "  %d. %s", (Event_Page*3)-1, Event_Text_2);
	Send_Event_Text_2[7 + Event_Text_Length_2] = 0xFF;
	Send_Event_Text_2[8 + Event_Text_Length_2] = 0xFC;
	Send_Event_Text_2[9 + Event_Text_Length_2] = 0xFF;
	Send_Event_Text_2[10 + Event_Text_Length_2] = 0xFF;
  }
  else if(save_p_type_cnt >= 10 && save_p_type_cnt < 100) // 10의 자리
  {
	Event_Text_Length_2 = sprintf(&Send_Event_Text_2[7], "  %d. %s", (Event_Page*3)-1, Event_Text_2);
	Send_Event_Text_2[7 + Event_Text_Length_2] = 0xFF;
	Send_Event_Text_2[8 + Event_Text_Length_2] = 0xFC;
	Send_Event_Text_2[9 + Event_Text_Length_2] = 0xFF;
	Send_Event_Text_2[10 + Event_Text_Length_2] = 0xFF;
  }
  else if(save_p_type_cnt >= 100 && save_p_type_cnt < 1000) // 100의 자리
  {
	Event_Text_Length_2 = sprintf(&Send_Event_Text_2[7], "  %d. %s", (Event_Page*3)-1, Event_Text_2);
	Send_Event_Text_2[7 + Event_Text_Length_2] = 0xFF;
	Send_Event_Text_2[8 + Event_Text_Length_2] = 0xFC;
	Send_Event_Text_2[9 + Event_Text_Length_2] = 0xFF;
	Send_Event_Text_2[10 + Event_Text_Length_2] = 0xFF;
  }

  // LCD로 번호. 첫번째 이벤트 종류 데이터를 전송
  HAL_UART_Transmit(&huart3, Send_Event_Text_2_Reset, 11, 10); // 이벤트 화면에서 첫번째 이벤트 종류 초기화 데이터 전송
  HAL_UART_Transmit(&huart3, Send_Event_Text_2, 11 + Event_Text_Length_2, 10); // 이벤트 화면에서 첫번째 이벤트 종류 데이터 전송

  // LCD로 번호. 첫번째 이벤트 날짜 및 시간 데이터를 전송
  HAL_UART_Transmit(&huart3, Send_Event_Date_Time_2_Reset, 11, 10); // 이벤트 화면에서 첫번째 이벤트 종류 초기화 데이터 전송
  HAL_UART_Transmit(&huart3, Send_Event_Date_Time_2, 11 + Event_Date_Time_Length_2, 10); // 이벤트 화면에서 첫번째 이벤트 종류 데이터 전송

  Text_Index_2 = 0;
  Event_Text_Length_2 = 0;
  Event_Date_Time_Length_2 = 0;

  for(int eve_txt_2_reset = 0; eve_txt_2_reset < 50; eve_txt_2_reset++)
  {
	Event_Text_2[eve_txt_2_reset] = '\0';
  }
  for(int eve_date_time_2_reset = 0; eve_date_time_2_reset < 50; eve_date_time_2_reset++)
  {
	Event_Date_Time_2[eve_date_time_2_reset] = '\0';
  }
}

void Send_EVENT_3(void) // LCD로 세번째 이벤트 보기 전송
{
  /** LCD로 세번째 이벤트 데이터를 보냄 **/
  // Event_Date_Time_3 배열에 P_Type_Receiver_Event[2][5] ~ P_Type_Receiver_Event[2][24] 복사
  for (int i = 0; i < 20; i++) {
	Event_Date_Time_3[i] = P_Type_Receiver_Event[(Event_Page*3)+1][5 + i];
  }
  
  // Send_Event_Date_Time_1 배열에 번호를 매기고 명령어를 붙여 데이터를 재 가공
  Event_Date_Time_Length_3 = sprintf(&Send_Event_Date_Time_3[7], "%s", Event_Date_Time_3);
  Send_Event_Date_Time_3[7 + Event_Date_Time_Length_3] = 0xFF;
  Send_Event_Date_Time_3[8 + Event_Date_Time_Length_3] = 0xFC;
  Send_Event_Date_Time_3[9 + Event_Date_Time_Length_3] = 0xFF;
  Send_Event_Date_Time_3[10 + Event_Date_Time_Length_3] = 0xFF;

  // Event_Text_1 배열에 P_Type_Receiver_Event[2][27]부터 0x0D가 나오기 전까지 복사
  for (int i = 27; i < 100; i++) {
	if (P_Type_Receiver_Event[(Event_Page*3)+1][i] == 0x0D) {
		break; // 0x0D가 나오면 중단
	}
	Event_Text_3[Text_Index_3++] = P_Type_Receiver_Event[(Event_Page*3)+1][i];
  }
  
  byte save_p_type_cnt = 0;
  
  if(P_Type_Cnt >= 1000) // 999개 저장 완료 표시 이때 부터는 저장하는 방식이 달라짐
  {
	save_p_type_cnt = P_Type_Cnt - 999;
  }
  
  // Send_Event_Text_1 배열에 번호를 매기고 명령어를 붙여 데이터를 재 가공
  if(save_p_type_cnt < 10) // 1의 자리
  {
	Event_Text_Length_3 = sprintf(&Send_Event_Text_3[7], "  %d. %s", (Event_Page*3), Event_Text_3);
	Send_Event_Text_3[7 + Event_Text_Length_3] = 0xFF;
	Send_Event_Text_3[8 + Event_Text_Length_3] = 0xFC;
	Send_Event_Text_3[9 + Event_Text_Length_3] = 0xFF;
	Send_Event_Text_3[10 + Event_Text_Length_3] = 0xFF;
  }
  else if(save_p_type_cnt >= 10 && save_p_type_cnt < 100) // 10의 자리
  {
	Event_Text_Length_3 = sprintf(&Send_Event_Text_3[7], "  %d. %s", (Event_Page*3), Event_Text_3);
	Send_Event_Text_3[7 + Event_Text_Length_3] = 0xFF;
	Send_Event_Text_3[8 + Event_Text_Length_3] = 0xFC;
	Send_Event_Text_3[9 + Event_Text_Length_3] = 0xFF;
	Send_Event_Text_3[10 + Event_Text_Length_3] = 0xFF;
  }
  else if(save_p_type_cnt >= 100 && save_p_type_cnt < 1000) // 100의 자리
  {
	Event_Text_Length_3 = sprintf(&Send_Event_Text_3[7], "  %d. %s", (Event_Page*3), Event_Text_3);
	Send_Event_Text_3[7 + Event_Text_Length_3] = 0xFF;
	Send_Event_Text_3[8 + Event_Text_Length_3] = 0xFC;
	Send_Event_Text_3[9 + Event_Text_Length_3] = 0xFF;
	Send_Event_Text_3[10 + Event_Text_Length_3] = 0xFF;
  }

  // LCD로 번호. 첫번째 이벤트 종류 데이터를 전송
  HAL_UART_Transmit(&huart3, Send_Event_Text_3_Reset, 11, 10); // 이벤트 화면에서 첫번째 이벤트 종류 초기화 데이터 전송
  HAL_UART_Transmit(&huart3, Send_Event_Text_3, 11 + Event_Text_Length_3, 10); // 이벤트 화면에서 첫번째 이벤트 종류 데이터 전송

  // LCD로 번호. 첫번째 이벤트 날짜 및 시간 데이터를 전송
  HAL_UART_Transmit(&huart3, Send_Event_Date_Time_3_Reset, 11, 10); // 이벤트 화면에서 첫번째 이벤트 종류 초기화 데이터 전송
  HAL_UART_Transmit(&huart3, Send_Event_Date_Time_3, 11 + Event_Date_Time_Length_3, 10); // 이벤트 화면에서 첫번째 이벤트 종류 데이터 전송

  Text_Index_3 = 0;
  Event_Text_Length_3 = 0;
  Event_Date_Time_Length_3 = 0;

  for(int eve_txt_3_reset = 0; eve_txt_3_reset < 50; eve_txt_3_reset++)
  {
	Event_Text_3[eve_txt_3_reset] = '\0';
  }
  for(int eve_date_time_3_reset = 0; eve_date_time_3_reset < 50; eve_date_time_3_reset++)
  {
	Event_Date_Time_3[eve_date_time_3_reset] = '\0';
  }
}