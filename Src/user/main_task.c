
#include "tim.h"
#include "main.h"
#include "user_app.h"


int8_t alertflag = 0; 		 //�����־λ

void coin_start (void)
{			
	if (sys_env.system_delay == 0){
		prepic_num =JSJM;
		comscreen(Disp_Indexpic[JSYX],Number_IndexpicB);	 // back to the  picture before alert
		sys_env.workstep = 3;
		dbg("begin working %s %d", __FILE__, __LINE__);
	}
}

void coin_stop (void)
{		
	runstep = 40;   //��ʼͣ��
	dbg("end working %s %d", __FILE__, __LINE__);
}

void coin_clear (void)
{	
	counter_clear();
}

void coin_clear_alarm (void)
{		
	prepic_num = prepic_prenum;
	comscreen(Disp_Indexpic[prepic_num],Number_IndexpicB);	 // back to the  picture before alert
}

void coin_print (void)
{
	print_func();	 //��ӡ����
	sys_env.workstep = 0;	//ֹͣ	���ж���  // �ȴ� ����
}


int16_t is_repeate (int16_t _coin_index)
{int16_t ei, i;
	ei = 0;
	
	int16_t temp_coin_maxvalue0;
	int16_t temp_coin_minvalue0;
	int16_t temp_coin_maxvalue1;
	int16_t temp_coin_minvalue1;
	int16_t temp_coin_maxvalue2;
	int16_t temp_coin_minvalue2;
	
	adstd_offset ();
	
	temp_coin_maxvalue0 = coin_maxvalue0 + pre_value.country[coinchoose].coin[_coin_index].data.offsetmax0;
	temp_coin_minvalue0 = coin_minvalue0 + pre_value.country[coinchoose].coin[_coin_index].data.offsetmin0;
	temp_coin_maxvalue1 = coin_maxvalue1 + pre_value.country[coinchoose].coin[_coin_index].data.offsetmax1;
	temp_coin_minvalue1 = coin_minvalue1 + pre_value.country[coinchoose].coin[_coin_index].data.offsetmin1;
	temp_coin_maxvalue2 = coin_maxvalue2 + pre_value.country[coinchoose].coin[_coin_index].data.offsetmax2;
	temp_coin_minvalue2 = coin_minvalue2 + pre_value.country[coinchoose].coin[_coin_index].data.offsetmin2;
	
	for (i = 0; i < COIN_TYPE_NUM; i++){
		if ( _coin_index != i){
			//if (ng_info_echo_off != 1)
				cy_println ("Compare with Coin %d", i);
			if( 
				(
					((temp_coin_maxvalue0 >= coin_cmp_value[i].compare_max0) && (temp_coin_minvalue0 <= coin_cmp_value[i].compare_min0)) || //��1Ԫ   ͨ��0��
					((temp_coin_maxvalue0 <= coin_cmp_value[i].compare_max0) && (temp_coin_maxvalue0 >= coin_cmp_value[i].compare_min0)) || 
					((temp_coin_minvalue0 <= coin_cmp_value[i].compare_max0) && (temp_coin_minvalue0 >= coin_cmp_value[i].compare_min0))
				) && (pre_value.country[coinchoose].coin[i].data.max0 > pre_value.country[coinchoose].coin[i].data.min0) &&
				(
					((temp_coin_maxvalue1 >= coin_cmp_value[i].compare_max1) && (temp_coin_minvalue1 <= coin_cmp_value[i].compare_min1)) || //��1Ԫ   ͨ��0��
					((temp_coin_maxvalue1 <= coin_cmp_value[i].compare_max1) && (temp_coin_maxvalue1 >= coin_cmp_value[i].compare_min1)) || 
					((temp_coin_minvalue1 <= coin_cmp_value[i].compare_max1) && (temp_coin_minvalue1 >= coin_cmp_value[i].compare_min1))
				) && (pre_value.country[coinchoose].coin[i].data.max1 > pre_value.country[coinchoose].coin[i].data.min1) &&
				(
					((temp_coin_maxvalue2 >= coin_cmp_value[i].compare_max2) && (temp_coin_minvalue2 <= coin_cmp_value[i].compare_min2)) || //��1Ԫ   ͨ��0��
					((temp_coin_maxvalue2 <= coin_cmp_value[i].compare_max2) && (temp_coin_maxvalue2 >= coin_cmp_value[i].compare_min2)) || 
					((temp_coin_minvalue2 <= coin_cmp_value[i].compare_max2) && (temp_coin_minvalue2 >= coin_cmp_value[i].compare_min2))
				) && (pre_value.country[coinchoose].coin[i].data.max2 > pre_value.country[coinchoose].coin[i].data.min2) 
			   ){
				cy_println ("Note!!! value Repeat with Coin %d please comfirm !!!", i);
				cy_println ("coin %d value is:", _coin_index);
				cy_println ("min0 = %d max0 = %d", temp_coin_minvalue0, temp_coin_maxvalue0);
				cy_println ("min1 = %d max1 = %d", temp_coin_minvalue1, temp_coin_maxvalue1);
				cy_println ("min2 = %d max2 = %d", temp_coin_minvalue2, temp_coin_maxvalue2);
				cy_println ("coin %d value is:", i);
				cy_println ("min0 = %d max0 = %d", coin_cmp_value[i].compare_min0, coin_cmp_value[i].compare_max0);
				cy_println ("min1 = %d max1 = %d", coin_cmp_value[i].compare_min1, coin_cmp_value[i].compare_max1);
				cy_println ("min2 = %d max2 = %d", coin_cmp_value[i].compare_min2, coin_cmp_value[i].compare_max2);
				ei = i+1;	//��ʾ ����
				return ei;
			}
		}
	}
	if(((coin_maxvalue0 - coin_minvalue0) > 100)){
		ei = 1002;//��ͼ����������ʾ ѧϰ��Χ̫��
	}
	return ei;
}

void refresh_data (void)
{
	pc_print("%d,%d;",1, para_set_value.data.kick_start_delay_t1);
	pc_print("%d,%d;",2, para_set_value.data.kick_keep_t1);
	pc_print("%d,%d;",3, para_set_value.data.kick_start_delay_t2);
	pc_print("%d,%d;",4, para_set_value.data.kick_keep_t1);
	pc_print("%d,%d;",5, *pre_value.country[COUNTRY_ID].coin[0].data.p_pre_count_set);
	pc_print("%d,%d;",6, *pre_value.country[COUNTRY_ID].coin[1].data.p_pre_count_set);
	pc_print("%d,%d;",7, *pre_value.country[COUNTRY_ID].coin[4].data.p_pre_count_set);
	pc_print("%d,%d;",8, *pre_value.country[COUNTRY_ID].coin[6].data.p_pre_count_set);
	pc_print("%d,%d;",9, *pre_value.country[COUNTRY_ID].coin[7].data.p_pre_count_set);
	pc_print("%d,%d;",10, *pre_value.country[COUNTRY_ID].coin[8].data.p_pre_count_set);
	pc_print("%d,%d;",11, *pre_value.country[COUNTRY_ID].coin[9].data.p_pre_count_set);
	pc_print("%d,%d;",12, *pre_value.country[COUNTRY_ID].coin[10].data.p_pre_count_set);
	pc_print("%d,%d;",22, para_set_value.data.motor_idle_t);
	pc_print("%d,%d;",23, para_set_value.data.pre_count_stop_n);
	pc_print("%d,%d;",24, para_set_value.data.coin_full_rej_pos);
	pc_print("%d,%d;",25, para_set_value.data.system_boot_delay);
	pc_print("%d,%d;",26, para_set_value.data.system_mode);
	pc_print("%d,%d;",27, *pre_value.country[COUNTRY_ID].coin[3].data.p_pre_count_set);
	pc_print("%d,%d;",51, sys_env.coin_index);
	pc_print("%d,%d;",52, pre_value.country[coinchoose].coin[sys_env.coin_index].data.max0);
	pc_print("%d,%d;",53, pre_value.country[coinchoose].coin[sys_env.coin_index].data.min0);
	pc_print("%d,%d;",54, pre_value.country[coinchoose].coin[sys_env.coin_index].data.max1);
	pc_print("%d,%d;",55, pre_value.country[coinchoose].coin[sys_env.coin_index].data.min1);
	pc_print("%d,%d;",56, pre_value.country[coinchoose].coin[sys_env.coin_index].data.max2);
	pc_print("%d,%d;",57, pre_value.country[coinchoose].coin[sys_env.coin_index].data.min2);
	disp_allcount_to_pc ();
}


#define KICK_Q_SCAN(N) if (coin_env.kick_Q[N] > 0) {\
	coin_env.kick_Q[N]--; \
	if (coin_env.kick_Q[N] == 0){ \
		EMKICK1(STARTRUN);	  \
		processed_coin_info.total_ng++; \
		coin_env.kick_keep_t1 = para_set_value.data.kick_keep_t1;\
	}\
}
#define FULL_KICK_Q_SCAN(N) if (coin_env.full_kick_Q[N] > 0) {\
	coin_env.full_kick_Q[N]--; \
	if (coin_env.full_kick_Q[N] == 0){ \
		EMKICK2(STARTRUN);	  \
		coin_env.full_kick_keep_t2 = para_set_value.data.kick_keep_t2;\
	}\
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
	if (kicktime > 0){kicktime--;}	
	
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
		case 100:
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
	
	KICK_Q_SCAN(0);
	KICK_Q_SCAN(1);
	FULL_KICK_Q_SCAN(0);
	FULL_KICK_Q_SCAN(1);
	
	if (coin_env.kick_keep_t1 > 0){
		coin_env.kick_keep_t1--;
		if (coin_env.kick_keep_t1 == 0){
			EMKICK1(STOPRUN);
		}
	}
	if (coin_env.full_kick_keep_t2 > 0){
		coin_env.full_kick_keep_t2--;
		if (coin_env.full_kick_keep_t2 == 0){
			EMKICK2(STOPRUN);	
		}
	}
	coin_cross_time++;
}

void normalTask (void)
{
	int32_t i = 0;
	//while(1)
	{
		switch (sys_env.workstep)
		{
			case 0:{ 
				ALL_STOP();//ͣ�����е����
				if (sys_env.sys_runing_time_total > 0){
					sys_env.coin_speed = ((processed_coin_info.total_coin - processed_coin_info.total_coin_old) * 60) / (sys_env.sys_runing_time_total / 10000);
					refresh_data ();
				}
				disp_allcount ();
				sys_env.workstep = 1;
				break;
			}
			case 1://����״̬
				break;
			case 3:{  //�������
				sys_env.workstep = 6;  //������ �����		
				deviceinit();//��ʼ������
				break;
			}
			case 6: {    
				setStdValue	();//���ü�α��׼ֵ
				//if( adstd_offset() == 1){//  ����׼ֵ�������в���
				if (1) {//  ����׼ֵ�������в���
					sys_env.stop_time = STOP_TIME;//�ޱ�ͣ��ʱ��
					sys_env.workstep =10;
					if ((sys_env.auto_clear == 1) || para_set_value.data.coin_full_rej_pos == 3){//��������Զ����㣬��ÿ���������������
						for (i = 0; i < COIN_TYPE_NUM; i++){
							*pre_value.country[COUNTRY_ID].coin[i].data.p_pre_count_full_flag = 0; //
							*pre_value.country[COUNTRY_ID].coin[i].data.p_pre_count_cur = 0; //
							coin_env.full_stack_num = 0;
						}
						processed_coin_info.total_money =0;
						processed_coin_info.total_coin = 0;
						processed_coin_info.total_good = 0;
						processed_coin_info.total_ng = 0;
						processed_coin_info.coinnumber = 0;
						good_value_index = 0;
						ng_value_index = 0;
						disp_allcount();
						disp_data(ADDR_CPZE,ADDR_CPZS,ADDR_CPFG);			//when counting pre ze zs foege data variable 
					}
				}else{
					SEND_ERROR(ADSTDEEROR);   //���������б�
					dbg("the voltage is wrong");
				}
				break;
			}
			case 10:{        //main  proceed
				runfunction();	 //ת�̶�������
				if (sys_env.coin_over == 1){
					sys_env.coin_over = 0;	
					disp_allcount ();
				}
				if (sys_env.stop_time == 0){
					sys_env.stop_flag++;
					if (sys_env.stop_flag == 1){
						STORAGE_DIR_N();//��ת
						sys_env.stop_time = 1000;//��תһ��
					}else if (sys_env.stop_flag == 2){
						STORAGE_DIR_P();//��ת
						sys_env.stop_time = STOP_TIME;//�ޱ�ͣ��ʱ��10��
					}else if (sys_env.stop_flag == 3){
						STORAGE_MOTOR_STOPRUN();	//  ת�̵��
						sys_env.stop_time = 2000;//STOP_TIME;//�ޱ�ͣ��ʱ��2��
					}else if (sys_env.stop_flag == 4){
						comscreen(Disp_Indexpic[JSJM],Number_IndexpicB);	 // back to the  picture before alert
						sys_env.workstep =0;	
					}
				}
				if (sys_env.print_wave_to_pc == 1){
					if (sys_env.AD_buf_sending == 1){
						send_sample_data (sys_env.Detect_AD_buf_p, sys_env.AD_data_len);
						sys_env.AD_buf_sending = 0;
					}
				}
				break;
			}
			/////////////////
			case 13: {//����ѧϰ
				sys_env.workstep = 20;  //������ �����		
				deviceinit();
				break;
			}
			case 20:{
				//if( adstd_offset() == 1){//��� ��׼ֵ    �����Ե�����ֵ  ���ܽ��� ��ѧϰ
				if (1) {//  ����׼ֵ�������в���
					sys_env.workstep =22;
				}else{
					SEND_ERROR(ADSTDEEROR);   //  �������׼ֵ
					//cy_print("the voltage is wrong\n");
				}
				break;
			}
			case 22:{
				runfunction();	 //ת�̶�������
				if (sys_env.coin_over == 1){
					sys_env.coin_over = 0;	
					dgus_tf1word(ADDR_A0MA,coin_maxvalue0);	//	 real time ,pre AD0  max
					dgus_tf1word(ADDR_A0MI,coin_minvalue0);	//	 real time ,pre AD0  min
					dgus_tf1word(ADDR_A1MA,coin_maxvalue1);	//	 real time ,pre AD1  max	
					dgus_tf1word(ADDR_A1MI,coin_minvalue1);	//	 real time ,pre AD1  min
					dgus_tf1word(ADDR_A2MA,coin_maxvalue2);	//	 real time ,pre AD2  max
					dgus_tf1word(ADDR_A2MI,coin_minvalue2);	//	 real time ,pre AD2  min
				}
				break;
			}
			case 88:{//����	
				ALL_STOP();
				alertfuc(alertflag);
				sys_env.workstep = 0; 
				break;
			}
			case 100:{////��׼����
				cy_adstd_adj ();
				break;
			}
			case 101:{////���⴫������ȡ
				detect_read ();
				break;
			}
			case 103:{// ��������ֵ���β������ϴ�����
				if (sys_env.AD_buf_sending == 1){
					send_sample_data (sys_env.Detect_AD_buf_p, sys_env.AD_data_len);
					sys_env.AD_buf_sending = 0;
				}
				break;
			}
			default:{
				break;
			}
		}
	}
}

//ģ��������
uint32_t tim3_v_old = 0, tim3_v_new = 0;
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
	tim3_v_old = htim3.Instance->CNT;
	main_task ();
	sys_env.ad_rd = 1;
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
//			while (1);
		}
}

//������

void coin_init (void)
{
	uint16_t i=0;
	
	system_env_init ();
	coin_env_init ();
	//////////////  
	alertflag = 0; 		 //�����־λ

	coinchoose = CN0;   // ����ѡ�� ���Ҽ����
	touchScreenDataFlag =0;   // ����2���� ��־λ 
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

	for(i = 0;i<TOUCH_SCREEN_DATA_BUF_LEN;i++) 
	{
		touchScreenDataBuffer[i] = 1;
	}

//	uart1_init();//���ڴ�ӡ��
//	uart2_init();//��Ļ
	cy_println ("\n#####    Program For YQ ##### ");
	i = 1;
//	watchdog_reset();/*��ʼ�����Ź�,T = WTCNT * t_watchdog*/
	//init RTC************************************************************
	RTC_Time Time = {
		2017, 6, 28, 3, 12, 0, 0
	};	
	
	initial_nandflash();    //nandflash
	ini_picaddr();	 //��ʼ������������
	/*����Ѵ������ϵ�һЩ������ʼ��*/
	ini_screen ();
	prepic_num = JSJM;
	prepic_prenum = JSJM;
	cy_println ("Coin Detector System start");

	
	sys_env.system_delay = 3000;//1000ms ��ʱ
	while (sys_env.system_delay);
	//begin init RTC************************************************************
	while(1) {    // read time 
		comscreen(dgus_readt,6);	//read time
		//HAL_UART_Transmit(&huart3, (uint8_t *)dgus_readt, 6, 0xFFFF);
		sys_env.system_delay = 1000;//1000ms ��ʱ
		while(touchScreenDataFlag == 0 ){
			if (sys_env.system_delay == 0){
				dgus_tfbeep(TIMEBP2); 
				//HAL_UART_Transmit(&huart3, (uint8_t *)dgus_beep, 6, 0xFFFF);
				cy_println ("TimeOut");
				break;
			}
		}//1���û�յ�����������Ϣ���п���û�н�����ֱ������
		Time.Year 	= HEX_TO_DEC (touchScreenDataBuffer[6]) + 2000;	
		Time.Month 	= HEX_TO_DEC (touchScreenDataBuffer[7]);	
		Time.Day 	= HEX_TO_DEC (touchScreenDataBuffer[8]);	
		Time.Week 	= HEX_TO_DEC (touchScreenDataBuffer[9]);	
		Time.Hour 	= HEX_TO_DEC (touchScreenDataBuffer[10]);	
		Time.Min 	= HEX_TO_DEC (touchScreenDataBuffer[11]);	
		Time.Sec 	= HEX_TO_DEC (touchScreenDataBuffer[12]);
		touchScreenDataFlag = 0;
		break;
	}

	cy_println("Time: %4d/%02d/%02d %02d:%02d:%02d", Time.Year,
				Time.Month, Time.Day, Time.Hour, Time.Min, Time.Sec);
	//end init RTC************************************************************
	
 	sys_env.workstep = 0; //ͣ��״̬
	print_system_env_info ();//���ڴ�ӡ������Ϣ��ϵͳ�������������ڵ��ԡ�
	setStdValue	();//���ü�α��׼ֵ������ÿ������֮ǰ��������һ�Σ���Ϊ��α��׼ֵ�����¶���һ����Χ�ڱ仯
	adstd_offset ();//���ò���ֵ������ÿ������֮ǰ���Ჹ��һ�Σ���Ϊ��α��׼ֵ�����¶���һ����Χ�ڱ仯
	
	comscreen(Disp_Indexpic[JSJM],Number_IndexpicB);	  // ��ת��������
	sys_env.system_delay = para_set_value.data.system_boot_delay;
	/*����Ԥ�ȣ����ʱ�䲻�������ʵ��ӳ�*/
	//delay_ms(ELECTRICTIME);    //���� ��ʱ ��Щʱ���ٸ� ��Ƭ����	 
}










