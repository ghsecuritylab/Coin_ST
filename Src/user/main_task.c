
#include "tim.h"
#include "main.h"
#include "user_app.h"


int8_t alertflag = 0; 		 //�����־λ

void coin_clear (void)
{
}
void coin_print (void)
{
}
void coin_start (void)
{
}
void coin_stop (void)
{
}
int16_t is_repeate (int16_t _coin_index)
{
	return 0;
}

void coin_clear_alarm (void)
{
}

void main_task(void)
{
	static unsigned int running_state = 0;
	running_state++;
	if (running_state >= 1000){
		running_state = 0;
		HAL_GPIO_TogglePin (GPIOC, User_Led_Pin);
	} 
	if (sys_env.sys_runing_time){
		sys_env.sys_runing_time++;
	}
	
	switch (sys_env.workstep)
	{
		case 10:{        //main  proceed
			cy_ad0_valueget();    //check coin wave and get ADSAMPNUM of ad  values
			cy_ad1_valueget();    //check coin wave and get ADSAMPNUM of ad  values
			cy_ad2_valueget();    //check coin wave and get ADSAMPNUM of ad  values
			//////////////////////////////////////////////////////////////////////
			cy_precoincount();   //��α������
			IR_detect_func();   //�ڶ����߱ҳ���
			break;
		}
		case 22:{
			cy_ad0_valueget();    //check coin wave and get ADSAMPNUM of ad  values
			cy_ad1_valueget();    //check coin wave and get ADSAMPNUM of ad  values
			cy_ad2_valueget();    //check coin wave and get ADSAMPNUM of ad  values
			//////////////////////////////////////////////////////////////////////
			cy_coinlearn();   //����ѧϰ
			break;
		}
		case 103:{
			cy_ad0_valueget();    //check coin wave and get ADSAMPNUM of ad  values
			cy_ad1_valueget();    //check coin wave and get ADSAMPNUM of ad  values
			cy_ad2_valueget();    //check coin wave and get ADSAMPNUM of ad  values
			break;
		}
		default:{
			break;
		}
	}
}

//ģ��������
uint32_t tim3_v_old = 0, tim3_v_new = 0;
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
	tim3_v_old = htim3.Instance->CNT;
	sys_env.ad_rd = 1;
	main_task ();
#if 0
	int i, j;
	for (i = 0; i < 10; i++){
		for (j = 0; j < 180; j++)
			sys_env.ad_rd = 1;
	}
#endif
	tim3_v_new = htim3.Instance->CNT;
		if ((tim3_v_new - tim3_v_old > 90) || (tim3_v_new < tim3_v_old)){
			cy_println("main_task use: %d - %d = %d", tim3_v_new, tim3_v_old, tim3_v_new - tim3_v_old);
			cy_println ("task too busy, stop here...!!!");
			while (1);
		}
}

//������

void coin_init (void)
{
	uint16_t i=0;
//	rWTCON = 0;	// �رտ��Ź�
	system_env_init ();
	coin_env_init ();
	//////////////  
	alertflag = 0; 		 //�����־λ

	coinchoose = CN0;   // ����ѡ�� ���Ҽ����
	touch_flag =0;   // ����2���� ��־λ 
	uartcount = 0;  // ����2���� �ֽ� ���� 
	sys_env.coin_index = 0;   //��ǰ  ѧϰ ���� ���� 
//	tscount = 0;
	blockflag = ADBLOCKT; //�˱�����yqadc.c�ļ��ж���
	adtime = 0;    //��ʱ�ж��� ��ʱ

	
//	db_id = 0;   //��ʷ���� ����Ѿ���ʾ ��
	
	ch0_count = 0;
	
	ch0_coin_come = 0;;
	
	coinlearnnumber =0;  //��ѧϰ ����
	prepic_prenum = 0;     // ���ڼ�¼ ����ǰ�Ľ��� 
	db_id = 0;   //��ʷ���� ����Ѿ���ʾ ��
	
	coin_maxvalue0 = AD0STDSET;
	coin_minvalue0 = AD0STDSET;
	coin_maxvalue1 = AD1STDSET;
	coin_minvalue1 = AD1STDSET;
	coin_maxvalue2 = AD2STDSET;
	coin_minvalue2 = AD2STDSET;

	std_ad0 = AD0STDSET;
	std_ad1 = AD1STDSET;
	std_ad2 = AD2STDSET;

	for(i = 0;i<TSGET_NUM;i++) 
	{
		touchnum[i] = 0;
	}
	////////////////

//	uart1_init();//���ڴ�ӡ��
//	uart2_init();//��Ļ
	cy_println ("\n#####    Program For YQ ##### ");
	i = 1;
//	watchdog_reset();/*��ʼ�����Ź�,T = WTCNT * t_watchdog*/
//	Timer_Init ();
	
	//init RTC************************************************************
//	RTC_Time Time = {
//		2017, 6, 28, 3, 12, 0, 0
//	};	
	

//	adc_init();    //��ʼ��ADC 	
//	init_Iic();
//	yqi2c_init();
    rNF_Init();
	initial_nandflash();    //nandflash
	//Hsmmc_Init ();//SD��
//	cy_println ("Hsmmc_init_flag is %d", Hsmmc_exist ());
	
	ini_picaddr();	 //��ʼ������������
	/*����Ѵ������ϵ�һЩ������ʼ��*/
	ini_screen ();
	prepic_num = JSJM;
	prepic_prenum = JSJM;
	cy_println ("Coin Detector System start");
//	if (code_flag == 0x55555555){//�����JTAG�����������͸��³���Flash		
//		U8 r_code;
//		extern unsigned int __CodeAddr__;
//		extern unsigned int __CodeSize__;	
//	#if 1
//		cy_println ("Code_flag = 0x%x, Begin Write code to flash...", code_flag);
//		r_code = WriteAppToAppSpace ((U32)__CodeAddr__, __CodeSize__);
//		if (r_code == 0)
//			cy_println ("write code to nand flash block 10 page 0 nand addr 0 completed");   
//		else
//			cy_println ("write code to nand flash block 10 page 0 nand addr 0 failed");  
//	#endif
//		code_flag = 0;
//	}else{
//		cy_println ("Code_flag = 0x%x, No Need Update code!", code_flag);
//	}
	
	//begin init RTC************************************************************
//	while(1) {    // read time 
//		comscreen(dgus_readt,6);	//read time
//		tscount = 50;//1000ms ��ʱ
//		while(touch_flag == 0 ){if (tscount == 0) break;}//1���û�յ�����������Ϣ���п���û�н�����ֱ������
//		Time.Year 	= HEX_TO_DEC (touchnum[6]) + 2000;	
//		Time.Month 	= HEX_TO_DEC (touchnum[7]);	
//		Time.Day 	= HEX_TO_DEC (touchnum[8]);	
//		Time.Week 	= HEX_TO_DEC (touchnum[9]);	
//		Time.Hour 	= HEX_TO_DEC (touchnum[10]);	
//		Time.Min 	= HEX_TO_DEC (touchnum[11]);	
//		Time.Sec 	= HEX_TO_DEC (touchnum[12]);
//		touch_flag = 0;
//		break;
//	}
	
//	RTC_Init(&Time);
//	RTC_GetTime(&Time);
//	cy_println("Time: %4d/%02d/%02d %02d:%02d:%02d", Time.Year,
//				Time.Month, Time.Day, Time.Hour, Time.Min, Time.Sec);
	//end init RTC************************************************************

	//LED1_ON;
	
 	sys_env.workstep = 0; //ͣ��״̬
//	print_system_env_info ();//���ڴ�ӡ������Ϣ��ϵͳ�������������ڵ��ԡ�
	setStdValue	();//���ü�α��׼ֵ������ÿ������֮ǰ��������һ�Σ���Ϊ��α��׼ֵ�����¶���һ����Χ�ڱ仯
	adstd_offset ();//���ò���ֵ������ÿ������֮ǰ���Ჹ��һ�Σ���Ϊ��α��׼ֵ�����¶���һ����Χ�ڱ仯
	
	comscreen(Disp_Indexpic[JSJM],Number_IndexpicB);	  // ��ת��������
	sys_env.system_delay = para_set_value.data.system_boot_delay;
	/*����Ԥ�ȣ����ʱ�䲻�������ʵ��ӳ�*/
	//delay_ms(ELECTRICTIME);    //���� ��ʱ ��Щʱ���ٸ� ��Ƭ����	 
}








