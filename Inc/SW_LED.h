/** SW_LED (주경종,지구경종,유도등..10개 스위치 누르면 LED 켜짐)**/
//화재 이보
#define SW_LED1_ON GPIOD-> ODR |= GPIO_PIN_2 //PD2---->1 
#define SW_LED1_OFF GPIOD-> ODR &= ~GPIO_PIN_2 //PD2---->0
#define SW_LED1_TOG GPIOD-> ODR ^= GPIO_PIN_2 //PD2---->Toggle

//유도등
#define SW_LED2_ON GPIOD-> ODR |= GPIO_PIN_1 //PD1---->1
#define SW_LED2_OFF GPIOD-> ODR &= ~GPIO_PIN_1 //PD1---->0
#define SW_LED2_TOG GPIOD-> ODR ^= GPIO_PIN_1 //PD1---->Toggle

//지구경종
#define SW_LED3_ON GPIOD-> ODR |= GPIO_PIN_0 //PD0---->1
#define SW_LED3_OFF GPIOD-> ODR &= ~GPIO_PIN_0 //PD0---->0
#define SW_LED3_TOG GPIOD-> ODR ^= GPIO_PIN_0 //PD0---->Toggle

//주경종
#define SW_LED4_ON GPIOC-> ODR |= GPIO_PIN_12 //PC12---->1
#define SW_LED4_OFF GPIOC-> ODR &= ~GPIO_PIN_12 //PD2---->0
#define SW_LED4_TOG GPIOC-> ODR ^= GPIO_PIN_12 //PC12---->Toggle

//복구 -> 26V이상으로 변경
//#define SW_LED5_ON GPIOE-> ODR |= GPIO_PIN_1 //PE1---->1
//#define SW_LED5_OFF GPIOE-> ODR &= ~GPIO_PIN_1 //PE1---->0

//자동 복구
#define SW_LED6_ON GPIOA-> ODR |= GPIO_PIN_8 //PA8---->1
#define SW_LED6_OFF GPIOA-> ODR &= ~GPIO_PIN_8 //PA8---->0

//예비전원
#define SW_LED7_ON GPIOC-> ODR |= GPIO_PIN_9 //PC9---->1
#define SW_LED7_OFF GPIOC-> ODR &= ~GPIO_PIN_9 //PC9---->0

//축적
#define SW_LED8_ON GPIOC-> ODR |= GPIO_PIN_8 //PC8---->1
#define SW_LED8_OFF GPIOC-> ODR &= ~GPIO_PIN_8 //PC8---->0

//도통부저
#define SW_LED9_ON GPIOC-> ODR |= GPIO_PIN_7 //PC7---->0
#define SW_LED9_OFF GPIOC-> ODR &= ~GPIO_PIN_7 //PC7---->0
#define SW_LED9_TOG GPIOC-> ODR ^= GPIO_PIN_7 //PC7---->Toggle

//동작시험 == MOD_LED1
#define SW_LED10_ON GPIOC-> ODR |= GPIO_PIN_6 //PC6---->1//GPIOC-> ODR |= GPIO_PIN_7 //PC7---->1
#define SW_LED10_OFF GPIOC-> ODR &= ~GPIO_PIN_6 //PC6---->0//GPIOC-> ODR &= ~GPIO_PIN_7 //PC7---->0

/** MOD_LED(20V이하, 전원정상, 26V이상, 교류전원, 예비전원, 발신기, 축적, 화재경보등x2 LED) **/
//26V 이상 == SW_LED5 PIN
#define MOD_LED1_ON GPIOE-> ODR |= GPIO_PIN_1 //PE1---->1
#define MOD_LED1_OFF GPIOE-> ODR &= ~GPIO_PIN_1 //PE1---->0

//전원정상
#define MOD_LED2_ON GPIOD-> ODR |= GPIO_PIN_15 //PD15---->1
#define MOD_LED2_OFF GPIOD-> ODR &= ~GPIO_PIN_15 //PD15---->0

//20V 이하
#define MOD_LED3_ON GPIOD-> ODR |= GPIO_PIN_14 //PD14---->1
#define MOD_LED3_OFF GPIOD-> ODR &= ~GPIO_PIN_14 //PD14---->0

//축적
#define MOD_LED4_ON GPIOD-> ODR |= GPIO_PIN_13 //PD13---->1
#define MOD_LED4_OFF GPIOD-> ODR &= ~GPIO_PIN_13 //PD13---->0
#define MOD_LED4_TOG GPIOD-> ODR ^= GPIO_PIN_13 //PD13---->Toggle

//발신기
#define MOD_LED5_ON GPIOD-> ODR |= GPIO_PIN_12 //PD12---->1
#define MOD_LED5_OFF GPIOD-> ODR &= ~GPIO_PIN_12 //PD12---->0

//예비전원
#define MOD_LED6_ON GPIOD-> ODR |= GPIO_PIN_11 //PD11---->1
#define MOD_LED6_OFF GPIOD-> ODR &= ~GPIO_PIN_11 //PD11---->0

//교류전원
#define MOD_LED7_ON GPIOD-> ODR |= GPIO_PIN_10 //PD10---->1
#define MOD_LED7_OFF GPIOD-> ODR &= ~GPIO_PIN_10 //PD10---->0

//화재경보등1
#define MOD_LED8_ON GPIOD-> ODR |= GPIO_PIN_9 //PD9---->1
#define MOD_LED8_OFF GPIOD-> ODR &= ~GPIO_PIN_9 //PD9---->0

//화재경보등2
#define MOD_LED9_ON GPIOD-> ODR |= GPIO_PIN_8 //PD8---->1
#define MOD_LED9_OFF GPIOD-> ODR &= ~GPIO_PIN_8 //PD8---->0

/** IN_LED(회로1~10 화재시 LED) **/
//회로1
#define IN_LED1_ON GPIOE-> ODR |= GPIO_PIN_0 //PE0---->1
#define IN_LED1_OFF GPIOE-> ODR &= ~GPIO_PIN_0 //PE0---->0
#define IN_LED1_TOG GPIOE-> ODR ^= GPIO_PIN_0 //PE0---->Toggle

//회로2
#define IN_LED2_ON GPIOB-> ODR |= GPIO_PIN_9 //PB9---->1
#define IN_LED2_OFF GPIOB-> ODR &= ~GPIO_PIN_9 //PB9---->0
#define IN_LED2_TOG GPIOB-> ODR ^= GPIO_PIN_9 //PB9---->Toggle

//회로3
#define IN_LED3_ON GPIOB-> ODR |= GPIO_PIN_8 //PB8---->1
#define IN_LED3_OFF GPIOB-> ODR &= ~GPIO_PIN_8 //PB8---->0
#define IN_LED3_TOG GPIOB-> ODR ^= GPIO_PIN_8 //PB8---->Toggle

//회로4
#define IN_LED4_ON GPIOB-> ODR |= GPIO_PIN_7 //PB7---->1
#define IN_LED4_OFF GPIOB-> ODR &= ~GPIO_PIN_7 //PB7---->0
#define IN_LED4_TOG GPIOB-> ODR ^= GPIO_PIN_7 //PB7---->Toggle

//회로5
#define IN_LED5_ON GPIOB-> ODR |= GPIO_PIN_6 //PB6---->1
#define IN_LED5_OFF GPIOB-> ODR &= ~GPIO_PIN_6 //PB6---->0
#define IN_LED5_TOG GPIOB-> ODR ^= GPIO_PIN_6 //PB6---->Toggle

//회로6
#define IN_LED6_ON GPIOD-> ODR |= GPIO_PIN_7 //PD7---->1
#define IN_LED6_OFF GPIOD-> ODR &= ~GPIO_PIN_7 //PD7---->0
#define IN_LED6_TOG GPIOD-> ODR ^= GPIO_PIN_7 //PD7---->Toggle

//회로7
#define IN_LED7_ON GPIOD-> ODR |= GPIO_PIN_6 //PD6---->1
#define IN_LED7_OFF GPIOD-> ODR &= ~GPIO_PIN_6 //PD6---->0
#define IN_LED7_TOG GPIOD-> ODR ^= GPIO_PIN_6 //PD6---->Toggle

//회로8
#define IN_LED8_ON GPIOD-> ODR |= GPIO_PIN_5 //PD5---->1
#define IN_LED8_OFF GPIOD-> ODR &= ~GPIO_PIN_5 //PD5---->0
#define IN_LED8_TOG GPIOD-> ODR ^= GPIO_PIN_5 //PD5---->Toggle

//회로9
#define IN_LED9_ON GPIOD-> ODR |= GPIO_PIN_4 //PD4---->1
#define IN_LED9_OFF GPIOD-> ODR &= ~GPIO_PIN_4 //PD4---->0
#define IN_LED9_TOG GPIOD-> ODR ^= GPIO_PIN_4 //PD4---->Toggle

//회로10
#define IN_LED10_ON GPIOD-> ODR |= GPIO_PIN_3 //PD3---->1
#define IN_LED10_OFF GPIOD-> ODR &= ~GPIO_PIN_3 //PD3---->0
#define IN_LED10_TOG GPIOD-> ODR ^= GPIO_PIN_3 //PD3---->Toggle

/** ENABLE PIN **/
//24V_EN
#define EN_H_24V GPIOE-> ODR |= GPIO_PIN_2 //PE2---->1 
#define EN_L_24V GPIOE-> ODR &= ~GPIO_PIN_2 //PE2---->0

//BAT_V_EN
#define BATV_H GPIOE-> ODR |= GPIO_PIN_3 //PE3---->1
#define BATV_L GPIOE-> ODR &= ~GPIO_PIN_3 //PE3---->0

//FIRE_RY_EN( 화재 이보 RY )
#define FIRE_RY_H GPIOE-> ODR |= GPIO_PIN_4 //PE4---->1
#define FIRE_RY_L GPIOE-> ODR &= ~GPIO_PIN_4 //PE4---->0

//LOCAL_RY_EN( 지구 경종 RY )
#define LOCAL_RY_H GPIOE-> ODR |= GPIO_PIN_5 //PE5---->1
#define LOCAL_RY_L GPIOE-> ODR &= ~GPIO_PIN_5 //PE5---->0

//MAIN_RY_EN( 주경종 RY )
#define MAIN_RY_H GPIOE-> ODR |= GPIO_PIN_6 //PE6---->1
#define MAIN_RY_L GPIOE-> ODR &= ~GPIO_PIN_6 //PE6---->0

//CHG_EN( 배터리 충전 EN )
#define CHG_H GPIOE-> ODR |= GPIO_PIN_7 //PE7---->1
#define CHG_L GPIOE-> ODR &= ~GPIO_PIN_7 //PE7---->0

// GUID_RY_EN( 유도등 EN )
#define GUID_RY_H GPIOE-> ODR |= GPIO_PIN_8 //PE8---->1
#define GUID_RY_L GPIOE-> ODR &= ~GPIO_PIN_8 //PE8---->0

/** BUZZER PIN **/
#define BUZZER_ON GPIOC-> ODR |= GPIO_PIN_3 //PC3---->1
#define BUZZER_OFF GPIOC-> ODR &= ~GPIO_PIN_3 //PC3---->0

/** 테스트 용 **/
#define IN_LED1_TOGGLE GPIOE-> ODR ^= GPIO_PIN_0 //PE0---->1