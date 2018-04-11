#ifndef __YQDRIVE_H__
#define __YQDRIVE_H__


//#define _MY_DEBUG_NO_DOOR_CHECK_
#define _MY_DEBUG_NO_PAN_BLOCK_ERROR_
#define _MY_DEBUG_NO_CHANEL_BLOCK_ERROR_
#define _MY_DEBUG_NO_KICK_ERROR_
//��� �˿�
#define STARTRUN GPIO_PIN_RESET
#define STOPRUN GPIO_PIN_SET

////////////////////////////////////new pcb open this
	
#define OUT7(x)		HAL_GPIO_WritePin (GPIOB, OUT7_Pin, x)
#define OUT6(x) 	HAL_GPIO_WritePin (GPIOB, OUT6_Pin, x)
#define OUT5(x) 	HAL_GPIO_WritePin (GPIOB, OUT5_Pin, x)
#define OUT4(x) 	HAL_GPIO_WritePin (GPIOD, OUT4_Pin, x)
#define OUT3(x) 	HAL_GPIO_WritePin (GPIOD, OUT3_Pin, x)
#define OUT2(x) 	HAL_GPIO_WritePin (GPIOD, OUT2_Pin, x)
#define OUT1(x) 	HAL_GPIO_WritePin (GPIOC, OUT1_Pin, x)
#define OUT0(x) 	HAL_GPIO_WritePin (GPIOC, OUT0_Pin, x)


#define  STORAGE_MOTOR(x) 	 OUT2(x)			
#define  STORAGE_DIR(x) 	 OUT3(x)
#define  EMKICK1(x) 		 OUT1(x)
#define  EMKICK2(x) 		 OUT0(x)



#define STORAGE_MOTOR_STARTRUN() {STORAGE_MOTOR(STARTRUN);motor_pwm_count = 0;}
#define STORAGE_MOTOR_STOPRUN() STORAGE_MOTOR(STOPRUN)

#define STORAGE_MOTOR_STATUS() HAL_GPIO_ReadPin (GPIOD, OUT2_Pin)

#define STORAGE_DIR_P() STORAGE_DIR(STARTRUN) //��ת
#define STORAGE_DIR_N() STORAGE_DIR(STOPRUN) //��ת



#define ALL_STOP()	STORAGE_MOTOR_STOPRUN(); \
					STORAGE_DIR_P(); \
					OUT2(STOPRUN); \
					OUT3(STOPRUN); \
					OUT4(STOPRUN); \
					OUT5(STOPRUN); \
					OUT6(STOPRUN); \
					OUT7(STOPRUN)

#define A0IN7
#define A0IN6
#define A0IN5
#define A0IN4
#define A0IN3
#define A0IN2
#define A0IN1 	HAL_GPIO_ReadPin (GPIOE, IN1_Pin)
#define A0IN0

#define _MY_DEBUG_NO_DOOR_CHECK_

#ifdef _MY_DEBUG_NO_DOOR_CHECK_
	#define COIN_DETECT      1
	#define PANCOIN_OVER  0
	#define DC_COINOVER   1
	#define DOOR_MAG      0
	#define DOOR_PAN      0
	#define PRESS_MICROSW 0
	#define DT_METAL      1
	#define DT_NOMETAL    1	
#else
	#define PANCOIN_OVER	//A0IN0
	#define COIN_DETECT   A0IN1
	#define DC_COINOVER   //A0IN2	
	#define DOOR_MAG      //A0IN3	
	#define DOOR_PAN      //A0IN4	
	#define PRESS_MICROSW //A0IN5	
	#define DT_METAL      //A0IN6	
	#define MOTOR_NG      //A0IN7	
#endif

#define  IR_DETECT_ON   GPIO_PIN_RESET	//  //������䴫���� 0 ��ʾ���ڵ� 
#define  IR_DETECT_OFF   GPIO_PIN_SET	//  //������䴫���� 0 ��ʾ���ڵ� 

////////////////////////////
void deviceinit(void);	//����ǰ��ʼ��

#define STOP_TIME (para_set_value.data.motor_idle_t*20)//�ޱ�ͣ��ʱ��10��

extern volatile uint32_t runtime;   // // ִ�м��õļ�ʱ
extern uint16_t runstep;  //  �������������
extern volatile uint32_t kicktime;   // //�߱ҵ���� �õļ�ʱ
extern uint32_t motor_pwm_count;   // //�߱ҵ���� �õļ�ʱ


void runfunction(void);   //������������
void IR_detect_func(void);	 //�����������߱ҳ���










#endif /*__YQDRIVE_H__*/
