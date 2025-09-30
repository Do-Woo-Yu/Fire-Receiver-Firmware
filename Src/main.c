/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "fatfs.h"
#include "usb_host.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "extern.h"
#include "user_define.h"
#include "LCD_DispKind.h"
#include "stdio.h"
#include "P_Type_Receiver_Event.h"
#include "P_Type_Date_Send_Function.h"
#include "EEPROM.c"
#include "SW_LED.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
byte RTC_EN = 0;
unsigned int min_cnt=0, hour_cnt=0;
byte TIME[6];
byte DATE[6];
byte set_rtc_hours = 0, set_rtc_minutes = 0, set_rtc_seconds = 0;
byte set_rtc_years = 0, set_rtc_months = 0, set_rtc_dates = 0;
byte success_cnt = 0, fail_cnt = 0, Fire_Schematic_Num = 0;
extern byte TB_IN_AD_FG;
/**USB 다운로드 변수**/
FATFS mynewdiskFatFs; /* File system object for User logical drive */
FIL MyFile; /* File object */
char mynewdiskPath[4]; /* User logical drive path */
uint8_t rwflag = 1;

uint32_t wbytes; /* File write counts */
uint8_t wtext[64]; /* File write buffer */ // UTF-8 P형 수신기 이벤트 텍스트

uint8_t P_Type_Data[64]; /* File write buffer */ // UTF-8 P형 수신기 이벤트 텍스트 + 날짜 + 시간 텍스트 
uint8_t Date_Time_Data[50];
uint8_t Data_Boundary[60]  = "---------------------------------------------------\r\n";

char USB_Down_Name_Data[30]; // USB로 저장할 파일 이름( 날짜_시간 )

byte test_p_event_cnt = 0;
byte save_event = 0;

byte EEPROM_WTimer, EEPROM_RTimer = 0, event_number_fg = 0;

#pragma location = 0x20000000//자동으로 pragma 바로 아래에 있는 변수 P_Type_Cnt의 주소를 0x2000 0000으로 할당
 __no_init word P_Type_Cnt;

#pragma location = 0x20000010//자동으로 pragma 바로 아래에 있는 변수 P_Type_Receiver_Event의 주소를 0x2100 0000으로 할당
// P형 수신기 이벤트 저장
__no_init  byte P_Type_Receiver_Event[1001][64];
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc2;
DMA_HandleTypeDef hdma_adc2;

RTC_HandleTypeDef hrtc;

SPI_HandleTypeDef hspi1;

TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim3;

UART_HandleTypeDef huart1;
UART_HandleTypeDef huart3;

/* USER CODE BEGIN PV */
RTC_TimeTypeDef sTime;
RTC_DateTypeDef sDate;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_RTC_Init(void);
static void MX_TIM2_Init(void);
static void MX_USART3_UART_Init(void);
static void MX_SPI1_Init(void);
static void MX_TIM3_Init(void);
static void MX_ADC2_Init(void);
static void MX_USART1_UART_Init(void);
void MX_USB_HOST_Process(void);

/* USER CODE BEGIN PFP */
void USB_DOWNLOAD(void);
void EVENT_CREATE(word event_cnt);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void USB_DOWNLOAD(void)
{
    word save_cnt = 0;
  
	if(FATFS_LinkDriver(&USBH_Driver, mynewdiskPath) == 0)
  	{
		if(f_mount(&mynewdiskFatFs, (TCHAR const*)mynewdiskPath, 0) == FR_OK)
		{
			memcpy(Tx_Data_List[List_Cnt], Download_Text_Appear, 27); // USB 다운로드 화면에서 3번 텍스트(다운로드 중) 출력
			List_Cnt++;
			// 저장할 파일 이름 (TV_날짜_시간.TXT)
			sprintf(USB_Down_Name_Data, "TV_20%d%d%d%d%d%d_%d%d%d%d%d%d.TXT",
			DATE[0], DATE[1], DATE[2], DATE[3], DATE[4], DATE[5],
			TIME[0], TIME[1], TIME[2], TIME[3], TIME[4], TIME[5]);
			sprintf(Date_Time_Data, "Date : 20%d%d-%d%d-%d%d  %d%d:%d%d:%d%d\r\n",
			DATE[0], DATE[1], DATE[2], DATE[3], DATE[4], DATE[5],
			TIME[0], TIME[1], TIME[2], TIME[3], TIME[4], TIME[5]);

			memcpy(P_Type_Receiver_Event[0], Date_Time_Data, sizeof(Date_Time_Data));
			memcpy(P_Type_Receiver_Event[1], Data_Boundary, sizeof(Data_Boundary));
			if(f_open(&MyFile, USB_Down_Name_Data, FA_CREATE_ALWAYS | FA_WRITE) == FR_OK)
			{
			  if(P_Type_Cnt >= 999)
			  {
				save_cnt = 1001;
			  }
			  else
			  {
				save_cnt = P_Type_Cnt+2;
			  }
				// P_Type_Receiver_Event에 저장된 데이터를 USB로 쓰기
				for (int i = 0; i < save_cnt; i++) {
					f_write(&MyFile, P_Type_Receiver_Event[i], strlen(P_Type_Receiver_Event[i]), (void *)&wbytes);
				}
				f_close(&MyFile);
				memcpy(Tx_Data_List[List_Cnt], Down_Success_Text_Green, 13); // USB 다운로드 화면에서 3번 텍스트(다운로드 완료) 색상 초록색으로 변경
				List_Cnt++;
				memcpy(Tx_Data_List[List_Cnt], Down_Success_Text_Appear, 30); // USB 다운로드 화면에서 3번 텍스트(다운로드 완료) 출력
				List_Cnt++;
			}
			else
			{
				memcpy(Tx_Data_List[List_Cnt], Down_Fail_Text_Red, 13); // USB 다운로드 화면에서 3번 텍스트(다운로드 실패) 색상 빨간색으로 변경
				List_Cnt++;
				memcpy(Tx_Data_List[List_Cnt], Down_Fail_Text_Appear, 30); // USB 다운로드 화면에서 3번 텍스트(다운로드 실패) 출력
				List_Cnt++;
			}
		}
		else
		{
			memcpy(Tx_Data_List[List_Cnt], Down_Fail_Text_Red, 13); // USB 다운로드 화면에서 3번 텍스트(다운로드 실패) 색상 빨간색으로 변경
			List_Cnt++;
			memcpy(Tx_Data_List[List_Cnt], Down_Fail_Text_Appear, 30); // USB 다운로드 화면에서 3번 텍스트(다운로드 실패) 출력
			List_Cnt++;
		}
	}
    FATFS_UnLinkDriver(mynewdiskPath);
}

void EVENT_CREATE(word event_cnt) // event_cnt는 해당 숫자의 배열(LCD_DispKind)에 저장되어있는 문자열을 가져와서 P_Type_Receiver_Event에 저장함
{
  // 기존 데이터 한 칸씩 아래로 밀기 (내림차순 복사)
  for (int i = 1000; i > 2; i--) 
  {
	// 초기화
	memset(P_Type_Receiver_Event[i], '\0', 64); // 전체 배열 초기화
	
    // 데이터 복사
    memcpy(P_Type_Receiver_Event[i] + 5, P_Type_Receiver_Event[i - 1] + 5, sizeof(P_Type_Receiver_Event[i - 1])-5);
  }
	
  if (++P_Type_Cnt > 1998) // 1999개가 넘어가면 다시 1000부터 카운트 시작
  {
	P_Type_Cnt = 1000;
  }
  
  // 초기화
  memset(P_Type_Data, '\0', 64); // 전체 배열 초기화
  
  sprintf(P_Type_Data, "20%d%d-%d%d-%d%d  %d%d:%d%d:%d%d  %s\r\n",
				DATE[0], DATE[1], DATE[2], DATE[3], DATE[4], DATE[5],
				TIME[0], TIME[1], TIME[2], TIME[3], TIME[4], TIME[5], LCD_DispKind[event_cnt]);
	
  // 새로운 포맷된 이벤트 데이터를 P_Type_Receiver_Event[2][5]부터 복사
  memcpy(P_Type_Receiver_Event[2] + 5, P_Type_Data, sizeof(P_Type_Data));
  
  // 기존 데이터 한 칸씩 아래로 밀기 (내림차순을 위해)
  for (int i = 1000; i > 1; i--)
  {
	// 데이터 이동과 함께 번호도 갱신하여 내림차순으로 번호 설정
	if (i <= 10) // 1의 자리
	{
	  sprintf(P_Type_Data, "  %d. ", i-1);
	}
	else if (i > 10 && i <= 100) // 10의 자리
	{
	  sprintf(P_Type_Data, " %d. ", i-1);
	}
	else if (i > 100) // 100의 자리
	{
	  sprintf(P_Type_Data, "%d. ", i-1);
	}
	memcpy(P_Type_Receiver_Event[i], P_Type_Data, 5);
  }
  
  EEPROM_WriteByte(0x00000000, (byte *)(0x20000000), 3);	// P_Type_Cnt 변수 데이터 EEPROM에 저장

  EEPROM_WTimer = 1;
}

uint16_t adcVal[14];
float BAT_AD, TB_IN_AD1, TB_IN_AD2, TB_IN_AD3, TB_IN_AD4, TB_IN_AD5, TB_IN_AD6,
TB_IN_AD7, TB_IN_AD8, TB_IN_AD9, TB_IN_AD10, CHK_26V, TB_SEND_AD, COIN_AD;                          

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */

int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_RTC_Init();
  MX_TIM2_Init();
  MX_FATFS_Init();
  MX_USB_HOST_Init();
  MX_USART3_UART_Init();
  MX_SPI1_Init();
  MX_TIM3_Init();
  MX_ADC2_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
  //MountUSB();
  USART1->CR1 |= USART_CR1_RXNEIE;		//USART1 수신인터럽트 허용
  USART3->CR1 |= USART_CR1_RXNEIE; 		//USART3 수신인터럽트 허용
  HAL_Delay(1000);
  EN_H_24V; // 24V_EN_H
  
  USB_DOWNLOAD();
  
  HAL_TIM_Base_Start_IT(&htim2);
  
  HAL_TIM_Base_Start_IT(&htim3);
  
  RTC_TimeTypeDef sTime = {0};
  RTC_DateTypeDef sDate = {0};
  
  // P_Type_Cnt,P_Type_Receiver_Event 메모리를 0으로 초기화
  P_Type_Cnt = 0;
  memset(P_Type_Receiver_Event, 0, sizeof(P_Type_Receiver_Event));
  
  EEPROM_ReadByte(0x00000000,(byte*)0x20000000,3); // EEPROM에 저장된 P_Type_Cnt 변수 데이터를 불러와서 CPU P_Type_Cnt 주소에 저장
  EEPROM_RTimer = 1;
  
  // ADC 시작(총 14개)
  HAL_ADC_Start_DMA(&hadc2, (uint32_t *)adcVal, 14); // 1 / (84MHZ * 480 Cycles * 14) = 0.5us 
  
  //W25Q80DV_ReadID();
  
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {  	  	  
	  
	static byte capture_test_fg = 0;
	if(capture_test_fg)
	{
		capture_test_fg = 0;
		TIM1->EGR |= TIM_EGR_CC2G;  // 강제 인터럽트 트리거
	}
	
	if(List_Cnt >= 50) // 데이터 전송 리스트가 100개가 넘어가면 0으로 초기화
	{
		List_Cnt = 0;
	}
	
	if(EE_Save_Cnt >= 100) // EEPROM에 저장할 데이터 리스트가 100개가 넘어가면 0으로 초기화
	{
		EE_Save_Cnt = 0;
	}
	  
	static unsigned int Scan_cnt = 0;
	
	Scan_cnt++;
	
    /* USER CODE END WHILE */
    MX_USB_HOST_Process();

    /* USER CODE BEGIN 3 */
	
	//memcpy(wtext, LCD_DispKind[0], sizeof(LCD_DispKind[0]));
	
	static byte save_event_cnt = 0;
	/** 이벤트 저장하는 부분 **/
	if(save_event) // P_Type_Cnt 변수 저장, P_Type_Receiver_Event 이중 배열 저장
	{
	  save_event = 0;
	  if(++save_event_cnt >= 74)
	  {
		save_event_cnt = 0;
	  }
	  EVENT_CREATE(save_event_cnt);
	  
	  EEPROM_WriteByte(0x00000000, (byte *)(0x20000000), 3);	// P_Type_Cnt 변수 데이터 EEPROM에 저장
	  
	  EEPROM_WTimer = 1;
	}
	
	/** EEPROM 테스트용 **/
	static byte eeprom_save = 0;
	
	if(eeprom_save == 1) // EEPROM 데이터 저장
	{
	  eeprom_save = 0;
	}
	else if(eeprom_save == 2) // EEPROM 데이터 읽기
	{
	  eeprom_save = 0;
	}
	else if(eeprom_save == 3) // EEPROM 데이터 초기화
	{
	  eeprom_save = 0;
	  
	  EEPROM_RESET();
	}
	
	if(RTC_EN)
	{
		RTC_EN = 0;
		
		sTime.Hours = set_rtc_hours;
		sTime.Minutes = set_rtc_minutes;
		sTime.Seconds = set_rtc_seconds;
		sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
		sTime.StoreOperation = RTC_STOREOPERATION_RESET;
		if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN) != HAL_OK)
		{
			Error_Handler();
		}
		sDate.WeekDay = RTC_WEEKDAY_MONDAY;
		sDate.Year = set_rtc_years;
		sDate.Month = set_rtc_months;
		sDate.Date = set_rtc_dates;
		if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BIN) != HAL_OK)
		{
			Error_Handler();
		}
   }

	HAL_RTC_GetTime(&hrtc, &sTime, FORMAT_BIN); //RTC로부터 시간을 받아옴(시,분,초)
	HAL_RTC_GetDate(&hrtc, &sDate, FORMAT_BIN); //RTC로부터 날짜를 받아옴(년,월,일,요일)
	
	TIME[0] = (sTime.Hours) / 10 + 0;
	TIME[1] = (sTime.Hours) % 10 + 0;
	
	TIME[2] = (sTime.Minutes) / 10 + 0;
	TIME[3] = (sTime.Minutes) % 10 + 0;
	
	TIME[4] = (sTime.Seconds) / 10 + 0;
	TIME[5] = (sTime.Seconds) % 10 + 0;
	
	DATE[0] = (sDate.Year) / 10 + 0;
	DATE[1] = (sDate.Year) % 10 + 0;
	
	DATE[2] = (sDate.Month) / 10 + 0;
	DATE[3] = (sDate.Month) % 10 + 0;
	
	DATE[4] = (sDate.Date) / 10 + 0;
	DATE[5] = (sDate.Date) % 10 + 0;
	
	/** MODE 스위치 동작 - MODE 화면으로 이동 **/
	if(MODE_Key_Value && (WHAT_MODE <= 1))// MODE 버튼이 눌렸을때 LCD에게 보낼 명령어
	{
	  MODE_Key_Value = 0;
	  
	  if(++MODE_key_value_cnt >= 2)
	  {
		MODE_key_value_cnt = 0;
	  }
	  
	  if((MODE_key_value_cnt % 2) == 0) // 홀수면 홈화면으로 이동
	  {
		WHAT_MODE = 0;
		memcpy(Tx_Data_List[List_Cnt], SCREEN0_GO, 9); // 홈 화면으로 이동
		List_Cnt++;
		RESTART_HOME = 0; // 10초 동안 아무런 움직임이 없으면 다시 홈화면으로 이동한다.
		UP_DOWN_key_value_cnt = 1;
		
		if(UP_DOWN_key_value_cnt == 1)
		{
		  memcpy(Tx_Data_List[List_Cnt], MODE1_TEXT_RED, 13); // 이벤트 보기 텍스트 빨간색
		  List_Cnt++;
		  memcpy(Tx_Data_List[List_Cnt], MODE2_TEXT_BLACK, 13); // USB 다운로드 텍스트 검정색
		  List_Cnt++;
		  memcpy(Tx_Data_List[List_Cnt], MODE3_TEXT_BLACK, 13); // 시간 조정 검정색
		  List_Cnt++;
		}
	  }
	  else if((MODE_key_value_cnt % 2) == 1) // 짝수면 화면으로 이동
	  {
		WHAT_MODE = 1;
		memcpy(Tx_Data_List[List_Cnt], SCREEN1_GO, 9); // 메뉴 화면으로 이동
		List_Cnt++;
		RESTART_HOME = 100000; // 10초 동안 아무런 움직임이 없으면 다시 홈화면으로 이동한다.
	  } 
	}
	
	if(MODE_Key_Value && (WHAT_MODE >= 2 && WHAT_MODE <= 4))
	{
	  MODE_Key_Value = 0;
	  WHAT_MODE = 1;
	  DATE_TIME_ADJ = 0;
	  USB_Dis_Cnt = 0;
	  Event_Page = 1;
	  memcpy(Tx_Data_List[List_Cnt], DATE_MONTH_ADJ_TEXT_BLACK, 13); // 10초 후 다시 홈화면으로 가면 시간 조정화면에서 날짜(월) 텍스트 검정색으로 초기화
	  List_Cnt++;
	  memcpy(Tx_Data_List[List_Cnt], DATE_DATE_ADJ_TEXT_BLACK, 13); // 10초 후 다시 홈화면으로 가면 시간 조정화면에서 날짜(일) 텍스트 검정색으로 초기화
	  List_Cnt++;
	  memcpy(Tx_Data_List[List_Cnt], TIME_HOUR_ADJ_TEXT_BLACK, 13); // 10초 후 다시 홈화면으로 가면 시간 조정화면에서 시간(시) 텍스트 검정색으로 초기화
	  List_Cnt++;
	  memcpy(Tx_Data_List[List_Cnt], TIME_MINUTE_ADJ_TEXT_BLACK, 13); // 10초 후 다시 홈화면으로 가면 시간 조정화면에서 시간(분) 텍스트 검정색으로 초기화
	  List_Cnt++;
	  memcpy(Tx_Data_List[List_Cnt], TIME_SEC_ADJ_TEXT_BLACK, 13); // 10초 후 다시 홈화면으로 가면 시간 조정화면에서 시간(초) 텍스트 검정색으로 초기화
	  List_Cnt++;
	  memcpy(Tx_Data_List[List_Cnt], DATE_YEAR_ADJ_TEXT_RED, 13); // 10초 후 다시 홈화면으로 가면 시간 조정화면에서 날짜(년) 텍스트 빨간색으로 초기화
	  List_Cnt++;
	  memcpy(Tx_Data_List[List_Cnt], SCREEN1_GO, 9); // 10초 후 다시 홈화면으로 이동
	  List_Cnt++;
	  RESTART_HOME = 100000; // 10초 동안 아무런 움직임이 없으면 다시 홈화면으로 이동한다.
	}
	
	/** SET 스위치 동작 - MODE 화면에서 1번(이벤트 보기),2번(USB 다운로드),3번(시간 조정)으로 이동 **/
	if(SET_Key_Value && WHAT_MODE == 1)// MODE 화면에서 SET 버튼이 눌렸을때 LCD에게 보낼 명령어
	{
	  SET_Key_Value = 0;
	  RESTART_HOME = 100000; // 10초 동안 아무런 움직임이 없으면 다시 홈화면으로 이동한다.
	  
	  if(UP_DOWN_key_value_cnt == 1) // 이벤트 보기 화면으로 이동
	  {
		WHAT_MODE = 2;
		Event_Page = 1;
		memcpy(Tx_Data_List[List_Cnt], SCREEN2_GO, 9); // 이벤트 보기 화면으로 이동
		List_Cnt++;
		Send_Page[8] = (Event_Page / 100) + 0x30;
		Send_Page[9] = ((Event_Page % 100) / 10) + 0x30;
		Send_Page[10] = ((Event_Page % 100) % 10) + 0x30;
		memcpy(Tx_Data_List[List_Cnt], Send_Page, 20); // 페이지 수 텍스트
		List_Cnt++;
		if(P_Type_Receiver_Event[(Event_Page*3)-1][5] != 0x00)
		{
		  Send_EVENT_1();
		}
		if(P_Type_Receiver_Event[(Event_Page*3)][5] != 0x00)
		{
		  Send_EVENT_2();
		}
		if(P_Type_Receiver_Event[(Event_Page*3)+1][5] != 0x00)
		{
		  Send_EVENT_3();
		}
	  }
	  else if(UP_DOWN_key_value_cnt == 2) // USB 다운로드 화면으로 이동
	  {
		WHAT_MODE = 3;
		memcpy(Tx_Data_List[List_Cnt], Execution_Text_Red, 13); // 실행 텍스트 빨간색 
		List_Cnt++;
		memcpy(Tx_Data_List[List_Cnt], Cancel_Text_Black, 13); // 취소 텍스트 검정색
		List_Cnt++;
		memcpy(Tx_Data_List[List_Cnt], Execution_Text_Appear, 20); // 실행 텍스트 나타나게 하기
		List_Cnt++;
		memcpy(Tx_Data_List[List_Cnt], Cancel_Text_Appear, 20); // 취소 텍스트 나타나게 하기
		List_Cnt++;
		memcpy(Tx_Data_List[List_Cnt], Downloaddis3_Text_DisAppear, 11); // 다운로드 텍스트 사라짐
		List_Cnt++;
		memcpy(Tx_Data_List[List_Cnt], SCREEN3_GO, 9); // 다운로드 화면으로 이동
		List_Cnt++;
		USB_Dis_Cnt = 1;
	  }
	  else if(UP_DOWN_key_value_cnt == 3) // 시간 조정 화면으로 이동
	  {
		WHAT_MODE = 4;
		
		/** 날짜 데이터 처리 **/
		
		ADJ_YEAR = sDate.Year;
		
		YEAR_Data[7] = 2 + 0x30;
		YEAR_Data[8] = 0 + 0x30;
		YEAR_Data[9] = (sDate.Year / 10) + 0x30;
		YEAR_Data[10] = (sDate.Year % 10) + 0x30;
		
		memcpy(Tx_Data_List[List_Cnt], YEAR_Data, 15); // 시간 조정 화면에서 년 데이터 전송
		List_Cnt++;
		
		ADJ_MONTH = sDate.Month;
		
		MONTH_Data[7] = (sDate.Month / 10) + 0x30;
		MONTH_Data[8] = (sDate.Month % 10) + 0x30;
		
		memcpy(Tx_Data_List[List_Cnt], MONTH_Data, 13); // 시간 조정 화면에서 월 데이터 전송
		List_Cnt++;
		
		ADJ_DATE = sDate.Date;
		
		DATE_Data[7] = (sDate.Date / 10) + 0x30;
		DATE_Data[8] = (sDate.Date % 10) + 0x30;	 
		
		memcpy(Tx_Data_List[List_Cnt], DATE_Data, 13); // 시간 조정 화면에서 일 데이터 전송
		List_Cnt++;
		
		/** 시간 데이터 처리 **/
		  
		ADJ_HOUR = sTime.Hours;
		
		HOUR_Data[7] = (sTime.Hours / 10) + 0x30;
		HOUR_Data[8] = (sTime.Hours % 10) + 0x30;
		
		memcpy(Tx_Data_List[List_Cnt], HOUR_Data, 13); // 시간 조정 화면에서 시 데이터 전송
		List_Cnt++;
		
		ADJ_MINUTE = sTime.Minutes;
		
		MINUTE_Data[7] = (sTime.Minutes / 10) + 0x30;
		MINUTE_Data[8] = (sTime.Minutes % 10) + 0x30;
		
		memcpy(Tx_Data_List[List_Cnt], MINUTE_Data, 13); // 시간 조정 화면에서 분 데이터 전송
		List_Cnt++;
		
		ADJ_SEC = sTime.Seconds;
		
		SEC_Data[7] = (sTime.Seconds / 10) + 0x30;
		SEC_Data[8] = (sTime.Seconds % 10) + 0x30;
		
		memcpy(Tx_Data_List[List_Cnt], SEC_Data, 13); // 시간 조정 화면에서 초 데이터 전송
		List_Cnt++;
		
		memcpy(Tx_Data_List[List_Cnt], SCREEN4_GO, 9); // 시간 조정 화면으로 이동
		List_Cnt++;
	  }
	}
	
	if(SET_Key_Value && WHAT_MODE == 2)// 이벤트 보기 화면에서 SET 버튼이 눌렸을때 LCD에게 보낼 명령어
	{
	  SET_Key_Value = 0;
	  RESTART_HOME = 100000; // 10초 동안 아무런 움직임이 없으면 다시 홈화면으로 이동한다.
	}
	
	if(SET_Key_Value && WHAT_MODE == 3)// USB 다운로드 화면에서 실행 텍스트 커서에 있을때 SET 버튼이 눌렸을때 LCD에게 보낼 명령어
	{
	  SET_Key_Value = 0;
	  if(USB_Dis_Cnt == 1)
	  {
		RESTART_HOME = 600000; // USB 다운로드 화면에서는 1분 동안 아무런 움직임이 없으면 다시 홈화면으로 이동한다.
		memcpy(Tx_Data_List[List_Cnt], Execution_Text_DisAppear, 11); // 1. 실행 텍스트 삭제
		List_Cnt++;
		
		memcpy(Tx_Data_List[List_Cnt], Cancel_Text_DisAppear, 11); // 2. 취소 텍스트 삭제
		List_Cnt++;
		
		memcpy(Tx_Data_List[List_Cnt], Downloaddis3_Text_Black, 13); // USB 다운로드 화면에서 3번 텍스트(초기화, 다운로드중) 색상을 검정색으로 변경
		List_Cnt++;
		
		memcpy(Tx_Data_List[List_Cnt], Reset_Text_Appear, 20); // USB 다운로드 화면에서 3번 텍스트(초기화) 출력
		List_Cnt++;
		/** USB 다운로드 **/
		USB_DOWNLOAD();
		/**USB 다운로드**/
	  }
	  else if(USB_Dis_Cnt == 2)// USB 다운로드 화면에서 취소 텍스트 커서에 있을때 SET 버튼이 눌렸을때 LCD에게 보낼 명령어
	  {
		RESTART_HOME = 100000; // 10초 동안 아무런 움직임이 없으면 다시 홈화면으로 이동한다.
		WHAT_MODE = 1;
		USB_Dis_Cnt = 0;

		memcpy(Tx_Data_List[List_Cnt], SCREEN1_GO, 9); // 메뉴 화면으로 이동
		List_Cnt++;
	  }
	}
	
	if(SET_Key_Value && WHAT_MODE == 4)// 시간 조정 MODE에서 SET 버튼이 눌렸을때 LCD에게 보낼 명령어
	{
	  SET_Key_Value = 0;
	  RESTART_HOME = 100000; // 10초 동안 아무런 움직임이 없으면 다시 홈화면으로 이동한다.
	  
	  DATE_TIME_ADJ++;
	  
	  switch(DATE_TIME_ADJ)
	  {
	   case 1:
		memcpy(Tx_Data_List[List_Cnt], DATE_YEAR_ADJ_TEXT_BLACK, 13); // 시간 조정 화면에서 날짜(년) 텍스트 검정색
		List_Cnt++;
		
		memcpy(Tx_Data_List[List_Cnt], DATE_MONTH_ADJ_TEXT_RED, 13); // 시간 조정 화면에서 날짜(월) 텍스트 빨간색
		List_Cnt++;
		break;
		
	   case 2:
		memcpy(Tx_Data_List[List_Cnt], DATE_MONTH_ADJ_TEXT_BLACK, 13); // 시간 조정 화면에서 날짜(월) 텍스트 검정색
		List_Cnt++;

		memcpy(Tx_Data_List[List_Cnt], DATE_DATE_ADJ_TEXT_RED, 13); // 시간 조정 화면에서 날짜(일) 텍스트 빨간색
		List_Cnt++;
		break;
		
	   case 3:
		memcpy(Tx_Data_List[List_Cnt], DATE_DATE_ADJ_TEXT_BLACK, 13); // 시간 조정 화면에서 날짜(일) 텍스트 검정색
		List_Cnt++;

		memcpy(Tx_Data_List[List_Cnt], TIME_HOUR_ADJ_TEXT_RED, 13); // 시간 조정 화면에서 시간(시) 텍스트 빨간색
		List_Cnt++;
		break;
		
	   case 4:
		memcpy(Tx_Data_List[List_Cnt], TIME_HOUR_ADJ_TEXT_BLACK, 13); // 시간 조정 화면에서 시간(시) 텍스트 검정색
		List_Cnt++;

		memcpy(Tx_Data_List[List_Cnt], TIME_MINUTE_ADJ_TEXT_RED, 13); // 시간 조정 화면에서 시간(분) 텍스트 빨간색
		List_Cnt++;
		break;
		
	   case 5:
		memcpy(Tx_Data_List[List_Cnt], TIME_MINUTE_ADJ_TEXT_BLACK, 13); // 시간 조정 화면에서 시간(분) 텍스트 검정색
		List_Cnt++;

		memcpy(Tx_Data_List[List_Cnt], TIME_SEC_ADJ_TEXT_RED, 13); // 시간 조정 화면에서 시간(초) 텍스트 빨간색
		List_Cnt++;
		break;
		
	   case 6:
		DATE_TIME_ADJ = 0;
		WHAT_MODE = 0;
		
		/** 날짜 및 시간 저장 **/
		set_rtc_years = ADJ_YEAR;
		set_rtc_months = ADJ_MONTH;
		set_rtc_dates = ADJ_DATE;
		set_rtc_hours = ADJ_HOUR;
		set_rtc_minutes = ADJ_MINUTE;
		set_rtc_seconds = ADJ_SEC;
		RTC_EN = 1;
		
		memcpy(Tx_Data_List[List_Cnt], TIME_SEC_ADJ_TEXT_BLACK, 13); // 시간 조정 화면에서 시간(초) 텍스트 검정색
		List_Cnt++;

		memcpy(Tx_Data_List[List_Cnt], DATE_YEAR_ADJ_TEXT_RED, 13); // 시간 조정 화면에서 날짜(년) 텍스트 빨간색
		List_Cnt++;

		memcpy(Tx_Data_List[List_Cnt], SCREEN0_GO, 9); // 시간 저장후에 홈화면으로 이동
		List_Cnt++;
		break;
		
	   default:
		break;
	  }
	}
	
	/** ▼ 스위치 동작 - 위로 이동 **/
	if(DOWN_Key_Value && WHAT_MODE == 1)// ▼ 버튼이 눌렸을때 MODE 화면에 보낼 명령어
	{
	  DOWN_Key_Value = 0;
	  RESTART_HOME = 100000; // 10초 동안 아무런 움직임이 없으면 다시 홈화면으로 이동한다.
	  if(--UP_DOWN_key_value_cnt <= 1)  
	  {
		UP_DOWN_key_value_cnt = 1;
	  }  
	  if(UP_DOWN_key_value_cnt == 1)
	  {
		memcpy(Tx_Data_List[List_Cnt], MODE1_TEXT_RED, 13); // 메뉴 화면에서 이벤트 보기 텍스트 빨간색
		List_Cnt++;

		memcpy(Tx_Data_List[List_Cnt], MODE2_TEXT_BLACK, 13); // 메뉴 화면에서 USB 다운로드 텍스트 검정색
		List_Cnt++;

		memcpy(Tx_Data_List[List_Cnt], MODE3_TEXT_BLACK, 13); // 메뉴 화면에서 시간 조정 텍스트 검정색
		List_Cnt++;
	  }
	  else if(UP_DOWN_key_value_cnt == 2)
	  {
		memcpy(Tx_Data_List[List_Cnt], MODE1_TEXT_BLACK, 13); // 메뉴 화면에서 이벤트 보기 텍스트 검정색
		List_Cnt++;

		memcpy(Tx_Data_List[List_Cnt], MODE2_TEXT_RED, 13); // 메뉴 화면에서 USB 다운로드 텍스트 빨간색
		List_Cnt++;

		memcpy(Tx_Data_List[List_Cnt], MODE3_TEXT_BLACK, 13); // 메뉴 화면에서 시간 조정 텍스트 검정색
		List_Cnt++;
	  }
	}
	else if(DOWN_Key_Value && WHAT_MODE == 2)// ▼ 버튼이 눌렸을때 이벤트 보기 화면에 보낼 명령어
	{
	  DOWN_Key_Value = 0;
	  RESTART_HOME = 100000; // 10초 동안 아무런 움직임이 없으면 다시 홈화면으로 이동한다.
	  
	  BF_EVENT_Page = Event_Page; // 과거 이벤트 페이지 값을 저장
	
	  if(--Event_Page < 1)
	  {
		Event_Page = 333;
	  }
	  
	  if(P_Type_Receiver_Event[(Event_Page*3)-1][5] == 0x00)
	  {
		Event_Page = BF_EVENT_Page;
	  }
	  
	  Send_Page[8] = (Event_Page / 100) + 0x30;
	  Send_Page[9] = ((Event_Page % 100) / 10) + 0x30;
	  Send_Page[10] = ((Event_Page % 100) % 10) + 0x30;
	  memcpy(Tx_Data_List[List_Cnt], Send_Page, 20); // 이벤트 보기 페이지 텍스트
	  List_Cnt++;
	  if(P_Type_Receiver_Event[(Event_Page*3)-1][5] != 0x00)
	  {
		Send_EVENT_1();
	  }
	  else
	  {
		memcpy(Tx_Data_List[List_Cnt], Send_Event_Text_1_Reset, 11); // 페이지 변경시 이벤트가 없다면 이벤트 텍스트 초기화
		List_Cnt++;

		memcpy(Tx_Data_List[List_Cnt], Send_Event_Date_Time_1_Reset, 11); // 페이지 변경시 이벤트가 없다면 시간 텍스트 초기화
		List_Cnt++;
	  }
	  if(P_Type_Receiver_Event[(Event_Page*3)][5] != 0x00)
	  {
		Send_EVENT_2();
	  }
	  else
	  {
		memcpy(Tx_Data_List[List_Cnt], Send_Event_Text_2_Reset, 11); // 페이지 변경시 첫번째 이벤트가 없다면 이벤트 텍스트 초기화
		List_Cnt++;
		
		memcpy(Tx_Data_List[List_Cnt], Send_Event_Date_Time_2_Reset, 11); // 페이지 변경시 두번째 이벤트가 없다면 시간 텍스트 초기화
		List_Cnt++;
	  }
	  if(P_Type_Receiver_Event[(Event_Page*3)+1][5] != 0x00)
	  {
		Send_EVENT_3();
	  }
	  else
	  {
		memcpy(Tx_Data_List[List_Cnt], Send_Event_Text_3_Reset, 11); // 페이지 변경시 세번째 이벤트가 없다면 이벤트 텍스트 초기화
		List_Cnt++;

		memcpy(Tx_Data_List[List_Cnt], Send_Event_Date_Time_3_Reset, 11); // 페이지 변경시 세번째 이벤트가 없다면 이벤트 텍스트 초기화
		List_Cnt++;
	  }
	}
	else if(DOWN_Key_Value && WHAT_MODE == 3)// ▼ 버튼이 눌렸을때 USB 다운로드 화면에 보낼 명령어
	{
	  DOWN_Key_Value = 0;
	  RESTART_HOME = 100000; // 10초 동안 아무런 움직임이 없으면 다시 홈화면으로 이동한다.
	  if(--USB_Dis_Cnt <= 1)
	  {
		USB_Dis_Cnt = 1;
	  }
	  if(USB_Dis_Cnt == 1)
	  {
		memcpy(Tx_Data_List[List_Cnt], Execution_Text_Red, 13); // 실행 텍스트 빨간색
		List_Cnt++;
		
		memcpy(Tx_Data_List[List_Cnt], Cancel_Text_Black, 13); // 취소 텍스트 검정색
		List_Cnt++;
	  }
	}
	
	if(DOWN_Key_Value && WHAT_MODE == 4)// 시간 조정 MODE에서 ▼ 버튼이 눌렸을때 LCD에게 보낼 명령어
	{
	  DOWN_Key_Value = 0;
	  RESTART_HOME = 100000; // 10초 동안 아무런 움직임이 없으면 다시 홈화면으로 이동한다.
	  switch(DATE_TIME_ADJ)
	  {
	   case 0 : // 년도 조정 UP
		if(++ADJ_YEAR >= 99)
		{
		  ADJ_YEAR = 24;
		}
		
		YEAR_Data[7] = 2 + 0x30;
		YEAR_Data[8] = 0 + 0x30;
		YEAR_Data[9] = (ADJ_YEAR / 10) + 0x30;
		YEAR_Data[10] = (ADJ_YEAR % 10) + 0x30;
		
		memcpy(Tx_Data_List[List_Cnt], YEAR_Data, 15); // 시간 조정 화면에서 년도 조정
		List_Cnt++;
		break;
		
	   case 1 : // 날짜의 달 조정 UP
		if(++ADJ_MONTH > 12)
		{
		  ADJ_MONTH = 1;
		}
		
		MONTH_Data[7] = (ADJ_MONTH / 10) + 0x30;
		MONTH_Data[8] = (ADJ_MONTH % 10) + 0x30;
		
		memcpy(Tx_Data_List[List_Cnt], MONTH_Data, 13); // 시간 조정 화면에서 월 조정
		List_Cnt++;
		break;
		
	   case 2 : // 날짜의 일수 조정 UP
		if(++ADJ_DATE > 31)
		{
		  ADJ_DATE = 1;
		}
		
		DATE_Data[7] = (ADJ_DATE / 10) + 0x30;
		DATE_Data[8] = (ADJ_DATE % 10) + 0x30;	 
		
		memcpy(Tx_Data_List[List_Cnt], DATE_Data, 13); // 시간 조정 화면에서 일 조정
		List_Cnt++;
		break;
		
	   case 3: // 시간의 시 조정 UP
		if(++ADJ_HOUR >= 24)
		{
		  ADJ_HOUR = 0;
		}
		
		HOUR_Data[7] = (ADJ_HOUR / 10) + 0x30;
		HOUR_Data[8] = (ADJ_HOUR % 10) + 0x30;
		
		memcpy(Tx_Data_List[List_Cnt], HOUR_Data, 13); // 시간 조정 화면에서 시 조정
		List_Cnt++;
		break;
		
	   case 4: // 시간의 분 조정 UP
		if(++ADJ_MINUTE >= 60)
		{
		  ADJ_MINUTE = 0;
		}
		
		MINUTE_Data[7] = (ADJ_MINUTE / 10) + 0x30;
		MINUTE_Data[8] = (ADJ_MINUTE % 10) + 0x30;
		
		memcpy(Tx_Data_List[List_Cnt], MINUTE_Data, 13); // 시간 조정 화면에서 분 조정
		List_Cnt++;
		break;
		
	   case 5: // 시간의 초 조정 UP
		if(++ADJ_SEC >= 60)
		{
		  ADJ_SEC = 0;
		}
		
		SEC_Data[7] = (ADJ_SEC / 10) + 0x30;
		SEC_Data[8] = (ADJ_SEC % 10) + 0x30;
		
		memcpy(Tx_Data_List[List_Cnt], SEC_Data, 13); // 시간 조정 화면에서 초 조정
		List_Cnt++;
		break;		
		
	   default :
		break;
	  }
	}
	
	/** ▲ 스위치 동작 - 아래로 이동 **/
	if(UP_Key_Value && WHAT_MODE == 1)// ▲ 버튼이 눌렸을때 LCD에게 보낼 명령어
	{
	  UP_Key_Value = 0;
	  RESTART_HOME = 100000; // 10초 동안 아무런 움직임이 없으면 다시 홈화면으로 이동한다.
	  if(++UP_DOWN_key_value_cnt >= 3)
	  {
		UP_DOWN_key_value_cnt = 3;
	  }
	  
	  if(UP_DOWN_key_value_cnt == 2)
	  {
		memcpy(Tx_Data_List[List_Cnt], MODE1_TEXT_BLACK, 13); // 이벤트 보기 텍스트 검정색
		List_Cnt++;
	
		memcpy(Tx_Data_List[List_Cnt], MODE2_TEXT_RED, 13); // USB 다운로드 텍스트 빨간색
		List_Cnt++;
		
		memcpy(Tx_Data_List[List_Cnt], MODE3_TEXT_BLACK, 13); // 시간 조정 텍스트 검정색
		List_Cnt++;
	  }
	  else if(UP_DOWN_key_value_cnt == 3)
	  {
		memcpy(Tx_Data_List[List_Cnt], MODE1_TEXT_BLACK, 13); // 이벤트 보기 텍스트 검정색
		List_Cnt++;
		
		memcpy(Tx_Data_List[List_Cnt], MODE2_TEXT_BLACK, 13); // USB 다운로드 텍스트 검정색
		List_Cnt++;
	
		memcpy(Tx_Data_List[List_Cnt], MODE3_TEXT_RED, 13); // 시간 조정 텍스트 빨간색
		List_Cnt++;
	  }
	}
	else if(UP_Key_Value && WHAT_MODE == 2)// ▼ 버튼이 눌렸을때 이벤트 보기 화면에 보낼 명령어
	{
	  UP_Key_Value = 0;
	  RESTART_HOME = 100000; // 10초 동안 아무런 움직임이 없으면 다시 홈화면으로 이동한다.
	  
	  BF_EVENT_Page = Event_Page; // 과거 이벤트 페이지 값을 저장
	  
	  if(++Event_Page > 333)
	  {
		Event_Page = 1;
	  }
	  
	  if(P_Type_Receiver_Event[(Event_Page*3)-1][5] == 0x00)
	  {
		Event_Page = BF_EVENT_Page;
	  }

	  Send_Page[8] = (Event_Page / 100) + 0x30;
	  Send_Page[9] = ((Event_Page % 100) / 10) + 0x30;
	  Send_Page[10] = ((Event_Page % 100) % 10) + 0x30;
	  memcpy(Tx_Data_List[List_Cnt], Send_Page, 20); // 페이지 수 텍스트
	  List_Cnt++;
	  if(P_Type_Receiver_Event[(Event_Page*3)-1][5] != 0x00)
	  {
		Send_EVENT_1();
	  }
	  else
	  {
		memcpy(Tx_Data_List[List_Cnt], Send_Event_Text_1_Reset, 11); // 페이지 이동시 첫번째 이벤트가 없다면 이벤트 텍스트 삭제
		List_Cnt++;
		
		memcpy(Tx_Data_List[List_Cnt], Send_Event_Date_Time_1_Reset, 11); // 페이지 이동시 첫번째 이벤트가 없다면 시간 텍스트 삭제
		List_Cnt++; 
	  }
	  if(P_Type_Receiver_Event[(Event_Page*3)][5] != 0x00)
	  {
		Send_EVENT_2();
	  }
	  else
	  {
		memcpy(Tx_Data_List[List_Cnt], Send_Event_Text_2_Reset, 11); // 페이지 이동시 두번째 이벤트가 없다면 이벤트 텍스트 삭제
		List_Cnt++;
		
		memcpy(Tx_Data_List[List_Cnt], Send_Event_Date_Time_2_Reset, 11); // 페이지 이동시 두번째 이벤트가 없다면 시간 텍스트 삭제
		List_Cnt++;
	  }
	  if(P_Type_Receiver_Event[(Event_Page*3)+1][5] != 0x00)
	  {
		Send_EVENT_3();
	  }
	  else
	  {
		memcpy(Tx_Data_List[List_Cnt], Send_Event_Text_3_Reset, 11); // 페이지 이동시 세번째 이벤트가 없다면 이벤트 텍스트 삭제
		List_Cnt++;

		memcpy(Tx_Data_List[List_Cnt], Send_Event_Date_Time_3_Reset, 11); // 페이지 이동시 세번째 이벤트가 없다면 시간 텍스트 삭제
		List_Cnt++;
	  }
	}
	else if(UP_Key_Value && WHAT_MODE == 3)// ▼ 버튼이 눌렸을때 USB 다운로드 화면에 보낼 명령어
	{
	  UP_Key_Value = 0;
	  RESTART_HOME = 100000; // 10초 동안 아무런 움직임이 없으면 다시 홈화면으로 이동한다.
	  if(++USB_Dis_Cnt >= 2)
	  {
		USB_Dis_Cnt = 2;
	  }
	  if(USB_Dis_Cnt == 2)
	  {
		memcpy(Tx_Data_List[List_Cnt], Execution_Text_Black, 13); // USB 다운로드 화면에서 실행 텍스트 검정색
		List_Cnt++;

		memcpy(Tx_Data_List[List_Cnt], Cancel_Text_Red, 13); // USB 다운로드 화면에서 취소 텍스트 빨간색
		List_Cnt++;
	  }
	}
	
	if(UP_Key_Value && WHAT_MODE == 4)// 시간 조정 MODE에서 ▲ 버튼이 눌렸을때 LCD에게 보낼 명령어
	{
	  UP_Key_Value = 0;
	  RESTART_HOME = 100000; // 10초 동안 아무런 움직임이 없으면 다시 홈화면으로 이동한다.
	  switch(DATE_TIME_ADJ)
	  {
	   case 0 : // 년도 조정 DOWN
		if(--ADJ_YEAR < 24)
		{
		  ADJ_YEAR = 24;
		}
		
		YEAR_Data[7] = 2 + 0x30;
		YEAR_Data[8] = 0 + 0x30;
		YEAR_Data[9] = (ADJ_YEAR / 10) + 0x30;
		YEAR_Data[10] = (ADJ_YEAR % 10) + 0x30;
		
		memcpy(Tx_Data_List[List_Cnt], YEAR_Data, 15); // 시간 조정 화면에서 년도 조정 텍스트
		List_Cnt++;
		break;
		
	   case 1 : // 날짜의 달 조정 UP
		if(--ADJ_MONTH < 1)
		{
		  ADJ_MONTH = 12;
		}
		
		MONTH_Data[7] = (ADJ_MONTH / 10) + 0x30;
		MONTH_Data[8] = (ADJ_MONTH % 10) + 0x30;
		
		memcpy(Tx_Data_List[List_Cnt], MONTH_Data, 13); // 시간 조정 화면에서 월 조정 텍스트
		List_Cnt++;
		break;
		
	   case 2 : // 날짜의 일수 조정 UP
		if(--ADJ_DATE < 1)
		{
		  ADJ_DATE = 31;
		}
		
		DATE_Data[7] = (ADJ_DATE / 10) + 0x30;
		DATE_Data[8] = (ADJ_DATE % 10) + 0x30;	 
		
		memcpy(Tx_Data_List[List_Cnt], DATE_Data, 13); // 시간 조정 화면에서 일 조정 텍스트
		List_Cnt++;
		break;
		
	   case 3: // 시간의 시 조정 UP
		--ADJ_HOUR;
		if(ADJ_HOUR == 0xFF)
		{
		  ADJ_HOUR = 23;
		}
		
		HOUR_Data[7] = (ADJ_HOUR / 10) + 0x30;
		HOUR_Data[8] = (ADJ_HOUR % 10) + 0x30;
		
		memcpy(Tx_Data_List[List_Cnt], HOUR_Data, 13); // 시간 조정 화면에서 시 조정 텍스트
		List_Cnt++;
		break;
		
	   case 4: // 시간의 분 조정 UP
		--ADJ_MINUTE;
		if(ADJ_MINUTE == 0xFF)
		{
		  ADJ_MINUTE = 59;
		}
		
		MINUTE_Data[7] = (ADJ_MINUTE / 10) + 0x30;
		MINUTE_Data[8] = (ADJ_MINUTE % 10) + 0x30;
		
		memcpy(Tx_Data_List[List_Cnt], MINUTE_Data, 13); // 시간 조정 화면에서 분 조정 텍스트
		List_Cnt++;
		break;
		
	   case 5: // 시간의 초 조정 UP
		--ADJ_SEC;
		if(ADJ_SEC == 0xFF)
		{
		  ADJ_SEC = 59;
		}
		
		SEC_Data[7] = (ADJ_SEC / 10) + 0x30;
		SEC_Data[8] = (ADJ_SEC % 10) + 0x30;
		
		memcpy(Tx_Data_List[List_Cnt], SEC_Data, 13); // 시간 조정 화면에서 초 조정 텍스트
		List_Cnt++;
		break;		
		
	   default :
		break;
	  }
	  
	}
  
	  /** 날짜 데이터 처리 **/

	  Date_Data[7] = 2 + 0x30;
	  Date_Data[8] = 0 + 0x30;
	  Date_Data[9] = (sDate.Year / 10) + 0x30;
	  Date_Data[10] = (sDate.Year % 10) + 0x30;
	  
	  if(Date_Data[9] == '0' && Date_Data[10] == '0')
	  {
		sDate.Year = 24;
	  }
	  
	  Date_Data[12] = (sDate.Month / 10) + 0x30;
	  Date_Data[13] = (sDate.Month % 10) + 0x30;
	  Date_Data[15] = (sDate.Date / 10) + 0x30;  
	  Date_Data[16] = (sDate.Date % 10) + 0x30;	  
	  
	  /** 시간 데이터 처리 **/
	  
	  Time_Data[7] = (sTime.Hours / 10) + 0x30;
	  Time_Data[8] = (sTime.Hours % 10) + 0x30;
	  
	  Time_Data[10] = (sTime.Minutes / 10) + 0x30; 
	  Time_Data[11] = (sTime.Minutes % 10) + 0x30;
	  
	  Time_Data[13] = (sTime.Seconds / 10) + 0x30; 
	  Time_Data[14] = (sTime.Seconds % 10) + 0x30;
	  
	  SYSTEM_POWER_Vol = CHK_26V * 10; 
	  
	  // 시스템 전압 십의 자리, 일의 자리, 소수점 1자리 추출
	  SYSTEM_POWER_Vol_10 = SYSTEM_POWER_Vol / 100;         // 십의 자리 (예: 267 -> 2)
	  SYSTEM_POWER_Vol_1 = (SYSTEM_POWER_Vol / 10) % 10;   // 일의 자리 (예: 267 -> 6)
	  SYSTEM_POWER_Vol_01 = SYSTEM_POWER_Vol % 10;       // 소수점 첫째 자리 (예: 267 -> 7)

	  STANDBY_POWER_Vol = BAT_AD * 10; 

	  // 예비전원 전압 십의 자리, 일의 자리, 소수점 1자리 추출
	  STANDBY_POWER_10 = STANDBY_POWER_Vol / 100;         // 십의 자리 (예: 267 -> 2)
	  STANDBY_POWER_1 = (STANDBY_POWER_Vol / 10) % 10;   // 일의 자리 (예: 267 -> 6)
	  STANDBY_POWER_01 = STANDBY_POWER_Vol % 10;       // 소수점 첫째 자리 (예: 267 -> 7)

	  CLOCK_POWER_Vol = COIN_AD * 10; 

	  // 시계전원 전압 일의 자리, 소수점 1자리 추출
	  CLOCK_POWER_10 = CLOCK_POWER_Vol / 10;   // 십의 자리 (예: 26 -> 2)
	  CLOCK_POWER_1 =  CLOCK_POWER_Vol % 10;   // 일의 자리 (예: 26 -> 6)
	  
	  if(WHAT_MODE == 0 && TB_IN_AD_FG == 1)
	  {
		if(SYSTEM_POWER_Vol_10 == 0)
		{
		  SYSTEM_Vol_0[7] = 0x20;
		}
		else
		{
		  SYSTEM_Vol_0[7] = SYSTEM_POWER_Vol_10 + 0x30;
		}
		SYSTEM_Vol_0[8] = SYSTEM_POWER_Vol_1 + 0x30;
		SYSTEM_Vol_0[10] = SYSTEM_POWER_Vol_01 + 0x30;
	  
		if(STANDBY_POWER_10 == 0)
		{
		  STANDBY_POWER_Vol_0[7] = 0x20;
		}
		else 
		{
		  if(STANDBY_POWER_10 != 3) 
		  {
		  	STANDBY_POWER_Vol_0[7] = STANDBY_POWER_10 + 0x30;
		  }
		  else // 30V 이상은 배터리 전압 표시 X
		  {
		   STANDBY_POWER_Vol_0[7] = 0x20;
		  }
		}
		STANDBY_POWER_Vol_0[8] = STANDBY_POWER_1 + 0x30;
		STANDBY_POWER_Vol_0[10] = STANDBY_POWER_01 + 0x30;
	  
		CLOCK_POWER_Vol_0[7] = CLOCK_POWER_10 + 0x30;
		CLOCK_POWER_Vol_0[9] = CLOCK_POWER_1 + 0x30;
	  }
	  else if(WHAT_MODE == 1 && TB_IN_AD_FG == 1)
	  {
		if(SYSTEM_POWER_Vol_10 == 0)
		{
		  SYSTEM_Vol_1[7] = 0x20;
		}
		else
		{
		  SYSTEM_Vol_1[7] = SYSTEM_POWER_Vol_10 + 0x30;
		}
		SYSTEM_Vol_1[8] = SYSTEM_POWER_Vol_1 + 0x30;
		SYSTEM_Vol_1[10] = SYSTEM_POWER_Vol_01 + 0x30;
	  
		if(STANDBY_POWER_10 == 0)
		{
		  STANDBY_POWER_Vol_1[7] = 0x20;
		}
		else
		{
		  STANDBY_POWER_Vol_1[7] = STANDBY_POWER_10 + 0x30;
		}
		STANDBY_POWER_Vol_1[8] = STANDBY_POWER_1 + 0x30;
		STANDBY_POWER_Vol_1[10] = STANDBY_POWER_01 + 0x30;
	  
		CLOCK_POWER_Vol_1[7] = CLOCK_POWER_10 + 0x30;
		CLOCK_POWER_Vol_1[9] = CLOCK_POWER_1 + 0x30;
	  }
	  
	  if(Motion_test_Mode == 1) // 동작시험 스위치를 짧게 누르면 동작시험 모드 1번째( 위 아래 버튼으로 각각의 회로 화재 발생 )
	  {
		// 홈 화면에서 동작시험 모드가 1일때 최초 위버튼이나 아래 버튼 둘중 아무거나를 누르면 회로 1번부터 화재 감지 테스트를 진행
		if(Test_Mode_1_Cnt == 1 && (Test_Mode_1_Cnt != Test_Mode_1_Cnt_BF)) // 회로 1 화재 감지 테스트
		{
			// 홈화면에서 P형 수신기, 테바테크 글자 지우고 중간에 화재 빨간색 글씨로 출력
			memcpy(Tx_Data_List[List_Cnt], P_TYPE_RECIEVER_TEXT_Reset, 11); // P형 수신기 텍스트 초기화
			List_Cnt++;
		  
			memcpy(Tx_Data_List[List_Cnt], TEVA_TECH_TEXT_Reset, 11); // 테바테크 텍스트 초기화
			List_Cnt++;
		  
			memcpy(Tx_Data_List[List_Cnt], FIRE_TEXT_Send, 17); // 화재 텍스트 출력
			List_Cnt++;
		  
			MOD_LED8_ON; // 화재 경보등1 LED 출력
			MOD_LED9_ON; // 화재 경보등2 LED 출력
			
			if(!TB1_Fire_Fg) // 회로 1이 화재가 아니라면
			{
				FIRE_CNT = 1;
				if(circulation_cnt == 0)
				{
					circulation_cnt++;
					FIRE_NUM++;
				}
				EEPORM_Data_List[EE_Save_Cnt] = 13; // P형 수신기 이벤트에 회로 1 화재 이벤트 추가 및 EEPROM에 저장
				EE_Save_Cnt++;
				IN_LED1_ON; // 회로 1 LED 출력
			}
			
			if(MAIN_RY == 0) // 주경종 OFF 즉 화재 중이 아닐때 동작시험 모드 1이라면
			{
				if(Main_bell_Key_Value == 1) // 주경종 SW ON상태일때인데 회로 화재라면
				{
				  Main_bell_Key_Value = 0;
				  Main_bell_TOG = 0;
				  SW_LED4_OFF;
				}
				// 화재시 유도등 LED OFF후 유도등 출력
				SW_LED2_OFF;
				Guide_light_Key_Value = 0;
				Guide_light_TOG = 0;
				
				// 화재시 이보 LED OFF후 이보 출력
				SW_LED1_OFF;
				Fire_report_Key_Value = 0;
				Fire_report_TOG = 0;
				
				MAIN_RY = 1; // MAIN_RY가 1이면 주경종 출력 중				
				EEPORM_Data_List[EE_Save_Cnt] = 37; // P형 수신기 이벤트에 주경종 출력 ON 이벤트 추가 및 EEPROM에 저장
				EE_Save_Cnt++;
				MAIN_RY_H; // 주경종 출력
				if(Earth_alarm_bell_Key_Value != 1) // 지구경종 SW OFF 일때만 지구경종 출력 및 이벤트 추가
				{
					EEPORM_Data_List[EE_Save_Cnt] = 35; // P형 수신기 이벤트에 지구경종 출력 ON 이벤트 추가 및 EEPROM에 저장
					EE_Save_Cnt++;
					LOCAL_RY_H; // 지구경종 출력
				}
				EEPORM_Data_List[EE_Save_Cnt] = 9; // P형 수신기 이벤트에 유도등 출력 ON 이벤트 추가 및 EEPROM에 저장
				EE_Save_Cnt++;
				GUID_RY_H; // 유도등 출력
				EEPORM_Data_List[EE_Save_Cnt] = 33; // P형 수신기 이벤트에 이보 출력 ON 이벤트 추가 및 EEPROM에 저장
				EE_Save_Cnt++;
				FIRE_RY_H; // 이보 출력
			  }
		  }
		  if(Test_Mode_1_Cnt == 2 && (Test_Mode_1_Cnt != Test_Mode_1_Cnt_BF)) // 회로 2 화재 감지 테스트
		  {
			if(!TB2_Fire_Fg) // 회로 2이 화재가 아니라면
			{
				FIRE_CNT = 1;
				if(circulation_cnt == 0)
				{
					circulation_cnt++;
					FIRE_NUM++;
				}
				EEPORM_Data_List[EE_Save_Cnt] = 15; // P형 수신기 이벤트에 회로 2 화재 이벤트 추가 및 EEPROM에 저장
				EE_Save_Cnt++;
				IN_LED2_ON; // 회로 2 LED 출력
			}
		  }
		  if(Test_Mode_1_Cnt == 3 && (Test_Mode_1_Cnt != Test_Mode_1_Cnt_BF)) // 회로 3 화재 감지 테스트
		  {
			if(!TB3_Fire_Fg) // 회로 3이 화재가 아니라면
			{
				FIRE_CNT = 1;
				if(circulation_cnt == 0)
				{
					circulation_cnt++;
					FIRE_NUM++;
				}
				EEPORM_Data_List[EE_Save_Cnt] = 17; // P형 수신기 이벤트에 회로 3 화재 이벤트 추가 및 EEPROM에 저장
				EE_Save_Cnt++;
				IN_LED3_ON; // 회로 3 LED 출력
			}
		  }
		  if(Test_Mode_1_Cnt == 4 && (Test_Mode_1_Cnt != Test_Mode_1_Cnt_BF)) // 회로 4 화재 감지 테스트
		  {
			if(!TB4_Fire_Fg) // 회로 4이 화재가 아니라면
			{
				FIRE_CNT = 1;
				if(circulation_cnt == 0)
				{
					circulation_cnt++;
					FIRE_NUM++;
				}
				EEPORM_Data_List[EE_Save_Cnt] = 19; // P형 수신기 이벤트에 회로 4 화재 이벤트 추가 및 EEPROM에 저장
				EE_Save_Cnt++;
				IN_LED4_ON; // 회로 4 LED 출력
			}
		  }
		  if(Test_Mode_1_Cnt == 5 && (Test_Mode_1_Cnt != Test_Mode_1_Cnt_BF)) // 회로 5 화재 감지 테스트
		  {
			if(!TB5_Fire_Fg) // 회로 5이 화재가 아니라면
			{
				FIRE_CNT = 1;
				if(circulation_cnt == 0)
				{
					circulation_cnt++;
					FIRE_NUM++;
				}
				EEPORM_Data_List[EE_Save_Cnt] = 21; // P형 수신기 이벤트에 회로 5 화재 이벤트 추가 및 EEPROM에 저장
				EE_Save_Cnt++;
				IN_LED5_ON; // 회로 5 LED 출력
			}
		  }
		  /*if(Test_Mode_1_Cnt == 6 && (Test_Mode_1_Cnt != Test_Mode_1_Cnt_BF)) // 회로 6 화재 감지 테스트
		  {
			if(!TB6_Fire_Fg) // 회로 6이 화재가 아니라면
			{
				FIRE_CNT = 1;
				if(circulation_cnt == 0)
				{
					circulation_cnt++;
					FIRE_NUM++;
				}
				EEPORM_Data_List[EE_Save_Cnt] = 23; // P형 수신기 이벤트에 회로 6 화재 이벤트 추가 및 EEPROM에 저장
				EE_Save_Cnt++;
				IN_LED6_ON; // 회로 6 LED 출력
			} 
		  }
		  if(Test_Mode_1_Cnt == 7 && (Test_Mode_1_Cnt != Test_Mode_1_Cnt_BF)) // 회로 7 화재 감지 테스트
		  {
			if(!TB7_Fire_Fg) // 회로 7이 화재가 아니라면
			{
				FIRE_CNT = 1;
				if(circulation_cnt == 0)
				{
					circulation_cnt++;
					FIRE_NUM++;
				}
				EEPORM_Data_List[EE_Save_Cnt] = 25; // P형 수신기 이벤트에 회로 7 화재 이벤트 추가 및 EEPROM에 저장
				EE_Save_Cnt++;
				IN_LED7_ON; // 회로 7 LED 출력
			}
		  }
		  if(Test_Mode_1_Cnt == 8 && (Test_Mode_1_Cnt != Test_Mode_1_Cnt_BF)) // 회로 8 화재 감지 테스트
		  {
			if(!T81_Fire_Fg) // 회로 8이 화재가 아니라면
			{
				FIRE_CNT = 1;
				if(circulation_cnt == 0)
				{
					circulation_cnt++;
					FIRE_NUM++;
				}
				EEPORM_Data_List[EE_Save_Cnt] = 27; // P형 수신기 이벤트에 회로 8 화재 이벤트 추가 및 EEPROM에 저장
				EE_Save_Cnt++;
				IN_LED8_ON; // 회로 8 LED 출력
			}
		  }
		  if(Test_Mode_1_Cnt == 9 && (Test_Mode_1_Cnt != Test_Mode_1_Cnt_BF)) // 회로 9 화재 감지 테스트
		  {
			if(!TB9_Fire_Fg) // 회로 9이 화재가 아니라면
			{
				FIRE_CNT = 1;
				if(circulation_cnt == 0)
				{
					circulation_cnt++;
					FIRE_NUM++;
				}
				EEPORM_Data_List[EE_Save_Cnt] = 29; // P형 수신기 이벤트에 회로 9 화재 이벤트 추가 및 EEPROM에 저장
				EE_Save_Cnt++;
				IN_LED9_ON; // 회로 9 LED 출력
			}
		  }
		  if(Test_Mode_1_Cnt == 10 && (Test_Mode_1_Cnt != Test_Mode_1_Cnt_BF)) // 회로 10 화재 감지 테스트
		  {
			if(!TB10_Fire_Fg) // 회로 10이 화재가 아니라면
			{
				FIRE_CNT = 1;
				if(circulation_cnt == 0)
				{
					circulation_cnt++;
					FIRE_NUM++;
				}
				EEPORM_Data_List[EE_Save_Cnt] = 31; // P형 수신기 이벤트에 회로 10 화재 이벤트 추가 및 EEPROM에 저장
				EE_Save_Cnt++;
				IN_LED10_ON; // 회로 10 LED 출력
			}
		  }*/
		  Test_Mode_1_Cnt_BF = Test_Mode_1_Cnt;
		}
	  
	  
	  /** PLC로 데이터 통신 후 감지기 몇번이 화재가 났는지를 수신기의 LCD에 츨력함 **/
		if(CheckSum_EN)
		{
			static byte check_cnt = 0;
			check_cnt++;
			
			static byte cal_number;
			CheckSum_EN = 0;
			P_Type_Reciever_Data_crc = 0;
			Fire_Detector_Data_crc = 0;
			if(Rx1_buf[1] == 0x04)
			{
				cal_number = 5;
			}
			P_Type_Reciever_Data_crc = CRC16(Rx1_buf, cal_number);//수신한 데이터의 CheckSum계산	
			CheckSum_ing = 0;
			Fire_Detector_Data_crc = (Rx1_CRC_H<<8) | Rx1_CRC_L;	
			
			static byte schematic_num = 0, Equipment_num = 0;
			if(P_Type_Reciever_Data_crc == Fire_Detector_Data_crc)//CheckSum OK!
			{
				success_cnt++;
				
				EEPORM_Data_List[EE_Save_Cnt] = 80 + (int)Rx1_buf[3]; // P형 수신기 이벤트에 회로? ?번 감지기 화재 이벤트 추가
				EE_Save_Cnt++;		
			}
		}
  }  
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE|RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief ADC2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC2_Init(void)
{

  /* USER CODE BEGIN ADC2_Init 0 */

  /* USER CODE END ADC2_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC2_Init 1 */

  /* USER CODE END ADC2_Init 1 */

  /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
  */
  hadc2.Instance = ADC2;
  hadc2.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV6;
  hadc2.Init.Resolution = ADC_RESOLUTION_12B;
  hadc2.Init.ScanConvMode = ENABLE;
  hadc2.Init.ContinuousConvMode = DISABLE;
  hadc2.Init.DiscontinuousConvMode = DISABLE;
  hadc2.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_RISING;
  hadc2.Init.ExternalTrigConv = ADC_EXTERNALTRIGCONV_T3_TRGO;
  hadc2.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc2.Init.NbrOfConversion = 14;
  hadc2.Init.DMAContinuousRequests = ENABLE;
  hadc2.Init.EOCSelection = ADC_EOC_SEQ_CONV;
  if (HAL_ADC_Init(&hadc2) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_0;
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_480CYCLES;
  if (HAL_ADC_ConfigChannel(&hadc2, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_1;
  sConfig.Rank = 2;
  if (HAL_ADC_ConfigChannel(&hadc2, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_2;
  sConfig.Rank = 3;
  if (HAL_ADC_ConfigChannel(&hadc2, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_3;
  sConfig.Rank = 4;
  if (HAL_ADC_ConfigChannel(&hadc2, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_4;
  sConfig.Rank = 5;
  if (HAL_ADC_ConfigChannel(&hadc2, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_5;
  sConfig.Rank = 6;
  if (HAL_ADC_ConfigChannel(&hadc2, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_6;
  sConfig.Rank = 7;
  if (HAL_ADC_ConfigChannel(&hadc2, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_7;
  sConfig.Rank = 8;
  if (HAL_ADC_ConfigChannel(&hadc2, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_8;
  sConfig.Rank = 9;
  if (HAL_ADC_ConfigChannel(&hadc2, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_9;
  sConfig.Rank = 10;
  if (HAL_ADC_ConfigChannel(&hadc2, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_10;
  sConfig.Rank = 11;
  if (HAL_ADC_ConfigChannel(&hadc2, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_11;
  sConfig.Rank = 12;
  if (HAL_ADC_ConfigChannel(&hadc2, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_12;
  sConfig.Rank = 13;
  if (HAL_ADC_ConfigChannel(&hadc2, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_14;
  sConfig.Rank = 14;
  if (HAL_ADC_ConfigChannel(&hadc2, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC2_Init 2 */

  /* USER CODE END ADC2_Init 2 */

}

/**
  * @brief RTC Initialization Function
  * @param None
  * @retval None
  */
static void MX_RTC_Init(void)
{

  /* USER CODE BEGIN RTC_Init 0 */

  /* USER CODE END RTC_Init 0 */

  RTC_TimeTypeDef sTime = {0};
  RTC_DateTypeDef sDate = {0};

  /* USER CODE BEGIN RTC_Init 1 */

  /* USER CODE END RTC_Init 1 */

  /** Initialize RTC Only
  */
  hrtc.Instance = RTC;
  hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
  hrtc.Init.AsynchPrediv = 127;
  hrtc.Init.SynchPrediv = 255;
  hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
  hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
  hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
  if (HAL_RTC_Init(&hrtc) != HAL_OK)
  {
    Error_Handler();
  }
  
  // RTC 캘리브레이션 적용 (RTC가 1초에 2ms 빠른 경우)
  HAL_RTCEx_SetSmoothCalib(&hrtc, RTC_SMOOTHCALIB_PERIOD_32SEC, RTC_CALIBSIGN_POSITIVE, 100);
  //HAL_RTCEx_SetSmoothCalib(&hrtc, RTC_SMOOTHCALIB_PERIOD_32SEC, RTC_CALIBSIGN_POSITIVE, 511); // 1초에 2ms 빠르다면 위의 코드를 주석 이 코드를 주석 해제
  
  /* USER CODE BEGIN Check_RTC_BKUP */

  /* USER CODE END Check_RTC_BKUP */

  /** Initialize RTC and set the Time and Date
  */
  /*sTime.Hours = 0x0;
  sTime.Minutes = 0x0;
  sTime.Seconds = 0x0;
  sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
  sTime.StoreOperation = RTC_STOREOPERATION_RESET;
  if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BCD) != HAL_OK)
  {
    Error_Handler();
  }
  sDate.WeekDay = RTC_WEEKDAY_MONDAY;
  sDate.Month = RTC_MONTH_JANUARY;
  sDate.Date = 0x1;
  sDate.Year = 0x24;

  if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BCD) != HAL_OK)
  {
    Error_Handler();
  }*/

  /** Enable the WakeUp
  */
  if (HAL_RTCEx_SetWakeUpTimer_IT(&hrtc, 0, RTC_WAKEUPCLOCK_CK_SPRE_16BITS) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN RTC_Init 2 */

  /* USER CODE END RTC_Init 2 */

}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_16;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 84-1;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 100-1;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */

}

/**
  * @brief TIM3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM3_Init(void)
{

  /* USER CODE BEGIN TIM3_Init 0 */

  /* USER CODE END TIM3_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 84-1;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 1000-1;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM3_Init 2 */

  /* USER CODE END TIM3_Init 2 */

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 9600;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief USART3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART3_UART_Init(void)
{

  /* USER CODE BEGIN USART3_Init 0 */

  /* USER CODE END USART3_Init 0 */

  /* USER CODE BEGIN USART3_Init 1 */

  /* USER CODE END USART3_Init 1 */
  huart3.Instance = USART3;
  huart3.Init.BaudRate = 115200;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART3_Init 2 */

  /* USER CODE END USART3_Init 2 */

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA2_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA2_Stream3_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA2_Stream3_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream3_IRQn);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOE, EN_H_24V_Pin|BAT_V_H_Pin|FIRE_RY_Pin|LOCAL_RY_Pin
                          |MAIN_RY_Pin|CHG_H_Pin|GUID_RY_Pin|IN_LED1_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, BUZ_OUT_Pin|MOD_LED1_Pin|SW_LED10_Pin|SW_LED9_Pin
                          |SW_LED8_Pin|SW_LED4_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, MOD_LED9_Pin|MOD_LED8_Pin|MOD_LED7_Pin|MOD_LED6_Pin
                          |MOD_LED5_Pin|MOD_LED4_Pin|MOD_LED3_Pin|MOD_LED2_Pin
                          |SW_LED3_Pin|SW_LED2_Pin|SW_LED1_Pin|IN_LED10_Pin
                          |IN_LED9_Pin|IN_LED8_Pin|IN_LED7_Pin|IN_LED6_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(W_CS1_GPIO_Port, W_CS1_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, IN_LED5_Pin|IN_LED4_Pin|IN_LED3_Pin|IN_LED2_Pin, GPIO_PIN_RESET);
  
  /*Configure GPIO pin : PA8_Pin */
  GPIO_InitStruct.Pin = GPIO_PIN_8;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  
  /*Configure GPIO pin : PA8_Pin */
  GPIO_InitStruct.Pin = GPIO_PIN_1;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pins : EN_H_24V_Pin BAT_V_H_Pin FIRE_RY_Pin LOCAL_RY_Pin
                           MAIN_RY_Pin CHG_H_Pin GUID_RY_Pin IN_LED1_Pin */
  GPIO_InitStruct.Pin = EN_H_24V_Pin|BAT_V_H_Pin|FIRE_RY_Pin|LOCAL_RY_Pin
                          |MAIN_RY_Pin|CHG_H_Pin|GUID_RY_Pin|IN_LED1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pin : KEY_SW14_Pin */
  GPIO_InitStruct.Pin = KEY_SW14_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(KEY_SW14_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : PC0 PC1 PC2 PC4 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_4;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : BUZ_OUT_Pin MOD_LED1_Pin SW_LED10_Pin SW_LED9_Pin
                           SW_LED8_Pin SW_LED4_Pin */
  GPIO_InitStruct.Pin = BUZ_OUT_Pin|MOD_LED1_Pin|SW_LED10_Pin|SW_LED9_Pin
                          |SW_LED8_Pin|SW_LED4_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : PA0 PA1 PA2 PA3
                           PA4 PA5 PA6 PA7 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3
                          |GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PB0 PB1 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : KEY_SW13_Pin KEY_SW12_Pin KEY_SW10_Pin KEY_SW9_Pin
                           KEY_SW8_Pin KEY_SW7_Pin */
  GPIO_InitStruct.Pin = KEY_SW13_Pin|KEY_SW12_Pin|KEY_SW10_Pin|KEY_SW9_Pin
                          |KEY_SW8_Pin|KEY_SW7_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pins : KEY_SW6_Pin KEY_SW5_Pin KEY_SW4_Pin KEY_SW3_Pin
                           KEY_SW2_Pin KEY_SW1_Pin */
  GPIO_InitStruct.Pin = KEY_SW6_Pin|KEY_SW5_Pin|KEY_SW4_Pin|KEY_SW3_Pin
                          |KEY_SW2_Pin|KEY_SW1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : MOD_LED9_Pin MOD_LED8_Pin MOD_LED7_Pin MOD_LED6_Pin
                           MOD_LED5_Pin MOD_LED4_Pin MOD_LED3_Pin MOD_LED2_Pin
                           SW_LED3_Pin SW_LED2_Pin SW_LED1_Pin IN_LED10_Pin
                           IN_LED9_Pin IN_LED8_Pin IN_LED7_Pin IN_LED6_Pin */
  GPIO_InitStruct.Pin = MOD_LED9_Pin|MOD_LED8_Pin|MOD_LED7_Pin|MOD_LED6_Pin
                          |MOD_LED5_Pin|MOD_LED4_Pin|MOD_LED3_Pin|MOD_LED2_Pin
                          |SW_LED3_Pin|SW_LED2_Pin|SW_LED1_Pin|IN_LED10_Pin
                          |IN_LED9_Pin|IN_LED8_Pin|IN_LED7_Pin|IN_LED6_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pin : W_CS1_Pin */
  GPIO_InitStruct.Pin = W_CS1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(W_CS1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : IN_LED5_Pin IN_LED4_Pin IN_LED3_Pin IN_LED2_Pin */
  GPIO_InitStruct.Pin = IN_LED5_Pin|IN_LED4_Pin|IN_LED3_Pin|IN_LED2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
