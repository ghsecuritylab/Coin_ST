


#ifndef __TQ2416_YQFILE_H__
#define __TQ2416_YQFILE_H__

extern int8_t alertflag;  //�����־λ

//���ݱ��� ���� ����ֵ
//���ֶ���
#define CN0 0   // �����
#define CN1 1   // ŷԪ
#define CN2 2   // ��Ԫ
#define CN3 3   // Ӣ��

///////tubiao number of coin name 
#define CNNM0 0   // 1Ԫ
#define CNNM1 1		// 5��ͭ
#define CNNM2 2		// 5�Ǹ� 
#define CNNM3 3		// 1�Ǵ���
#define CNNM4 4		// 1��С��
#define CNNM5 5		// 5��
#define CNNM6 6		// 2��
#define CNNM7 7		// 1��
#define CNNM8 8		// 1��С��

extern uint8_t coinchoose;    // ����ѡ��
void yqi2c_init(void);   //Ӳ����ֻ� I2C��ʼ�� 
extern uint8_t dgus_readt[6];  // only need y-m-d h:m  ��ʱ��
void print_func(void);   //��ӡ ���� 






#endif
