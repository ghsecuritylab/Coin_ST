#include "main.h"
#include "user_app.h"
//#include "s3c2416.h"

#define SET_CLE HAL_GPIO_WritePin(GPIOD, Nand_CLE_Pin, GPIO_PIN_SET)
#define CLR_CLE HAL_GPIO_WritePin(GPIOD, Nand_CLE_Pin, GPIO_PIN_RESET)
#define SET_ALE HAL_GPIO_WritePin(GPIOC, Nand_ALE_Pin, GPIO_PIN_SET)
#define CLR_ALE HAL_GPIO_WritePin(GPIOC, Nand_ALE_Pin, GPIO_PIN_RESET)
#define SET_CE HAL_GPIO_WritePin(GPIOD, Nand_CE_Pin, GPIO_PIN_SET)
#define CLR_CE HAL_GPIO_WritePin(GPIOD, Nand_CE_Pin, GPIO_PIN_RESET)
#define SET_WE HAL_GPIO_WritePin(GPIOC, Nand_WE_Pin, GPIO_PIN_SET)
#define CLR_WE HAL_GPIO_WritePin(GPIOC, Nand_WE_Pin, GPIO_PIN_RESET)
#define SET_RE HAL_GPIO_WritePin(GPIOD, Nand_RE_Pin, GPIO_PIN_SET)
#define CLR_RE HAL_GPIO_WritePin(GPIOD, Nand_RE_Pin, GPIO_PIN_RESET)

/********************
  延时函数
  形参：uint8_t time
  功能：延时数值为time值

*********************/
void delay_us(uint32_t time)
{
	uint32_t c;
	for(c=time;c;c--){ //定时=time*3*333.6=time*1000.8ns    
   //for(b=2;b;b--); //8*13.9*3= 333.6     
  }
}
/*等待芯片不忙*/
void wait_NAND_readay()
{
	uint8_t wait=0;//忙闲信号
//等待芯片不忙
	do
	{
		wait = HAL_GPIO_ReadPin(Nand_RB_GPIO_Port, Nand_RB_Pin);//读取忙闲引脚
	}
	while(0x00==wait);
}

void setNandDataInput (void)
{  
  GPIO_InitTypeDef GPIO_InitStruct;
  /*Configure GPIO pins : PEPin PEPin PEPin PEPin 
                           PEPin PEPin PEPin */
  GPIO_InitStruct.Pin = Nand_D2_Pin|Nand_D3_Pin|Nand_D7_Pin|Nand_D6_Pin 
                          |Nand_D5_Pin|Nand_D0_Pin|Nand_D1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pin : PtPin */
  GPIO_InitStruct.Pin = Nand_D4_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(Nand_D4_GPIO_Port, &GPIO_InitStruct);
}
void setNandDataOutput(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;
	HAL_GPIO_WritePin(GPIOC, Nand_WP_Pin, GPIO_PIN_SET);
  /*Configure GPIO pins : PEPin PEPin PEPin PEPin 
                           PEPin PEPin PEPin */
  GPIO_InitStruct.Pin = Nand_D2_Pin|Nand_D3_Pin|Nand_D7_Pin|Nand_D6_Pin 
                          |Nand_D5_Pin|Nand_D0_Pin|Nand_D1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pin : PtPin */
  GPIO_InitStruct.Pin = Nand_D4_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(Nand_D4_GPIO_Port, &GPIO_InitStruct);
}

void nandWriteDataToPort(uint8_t data)
{
	HAL_GPIO_WritePin (GPIOE, Nand_D7_Pin, ((data & (1 << 7)) == 0) ? GPIO_PIN_RESET : GPIO_PIN_SET);
	HAL_GPIO_WritePin (GPIOE, Nand_D6_Pin, ((data & (1 << 6)) == 0) ? GPIO_PIN_RESET : GPIO_PIN_SET);
	HAL_GPIO_WritePin (GPIOE, Nand_D5_Pin, ((data & (1 << 5)) == 0) ? GPIO_PIN_RESET : GPIO_PIN_SET);
	HAL_GPIO_WritePin (Nand_D4_GPIO_Port, Nand_D4_Pin, ((data & (1 << 4)) == 0) ? GPIO_PIN_RESET : GPIO_PIN_SET);
	HAL_GPIO_WritePin (GPIOE, Nand_D3_Pin, ((data & (1 << 3)) == 0) ? GPIO_PIN_RESET : GPIO_PIN_SET);
	HAL_GPIO_WritePin (GPIOE, Nand_D2_Pin, ((data & (1 << 2)) == 0) ? GPIO_PIN_RESET : GPIO_PIN_SET);
	HAL_GPIO_WritePin (GPIOE, Nand_D1_Pin, ((data & (1 << 1)) == 0) ? GPIO_PIN_RESET : GPIO_PIN_SET);
	HAL_GPIO_WritePin (GPIOE, Nand_D0_Pin, ((data & (1 << 0)) == 0) ? GPIO_PIN_RESET : GPIO_PIN_SET);
}
uint8_t nandReadDataFromPort (void)
{
	uint8_t data_t = 0;
	data_t = ((HAL_GPIO_ReadPin (GPIOE, Nand_D7_Pin) << 7) |
						(HAL_GPIO_ReadPin (GPIOE, Nand_D6_Pin) << 6) |
						(HAL_GPIO_ReadPin (GPIOE, Nand_D5_Pin) << 5) |
						(HAL_GPIO_ReadPin (Nand_D4_GPIO_Port, Nand_D4_Pin) << 4) |
						(HAL_GPIO_ReadPin (GPIOE, Nand_D3_Pin) << 3) |
						(HAL_GPIO_ReadPin (GPIOE, Nand_D2_Pin) << 2) |
						(HAL_GPIO_ReadPin (GPIOE, Nand_D1_Pin) << 1) |
						(HAL_GPIO_ReadPin (GPIOE, Nand_D0_Pin) << 0)
				);
	return data_t;
}

/*读取NAND闪存的ID序列号串口发送*/
void readNandId(void)
{
	uint8_t a=0, b=0, c=0, d=0;
	
	setNandDataOutput ();
  CLR_CE;//开启片选

  SET_CLE;//命令锁存开启
  CLR_WE;//写使能
  CLR_ALE;//地址锁存关闭
  SET_RE;//读关闭
 // GPIO_Write(GPIOE, 0x90); //读ID命令
 nandWriteDataToPort (0x90);
  SET_WE;//关闭写

  CLR_CLE;
  SET_ALE;
  CLR_WE;
//  GPIO_Write(GPIOE, 0x00); //地址00
 nandWriteDataToPort (0x00);
  SET_WE;

  /*设置PE口得低八位为输入*/
	setNandDataInput ();

  CLR_ALE; //注意啊，这里一定把他放在下面应为是有时间要求的；时序图TAR=15纳秒之后才是RE的操作；
	//读出的结果是AD F1 80 1D ，2011年8月26日，与天津第四项目部宿舍
	//AD F1,不用关心，80 1D代表本闪存为8位组织结构，页面大小为2KB，快大小为128KB，备用区为每512字节有16字节
  CLR_RE;

	a=nandReadDataFromPort ();//(GPIOE->IDR);//读出数据
  SET_RE;

	CLR_RE;
  b=nandReadDataFromPort ();//(GPIOE->IDR);
  SET_RE;

	CLR_RE;
	c=nandReadDataFromPort ();//(GPIOE->IDR);
	SET_RE;

  CLR_RE;
	d=nandReadDataFromPort ();//(GPIOE->IDR);//delay_1us(1);
	SET_RE;

  SET_CE;//关闭片选

	cy_println ("NandFlash ID: %02x %02x %02x %02x", a, b, c, d);
}

/*读闪存一页*/
void read_NAND_PAGE(void)
{
	uint32_t n;
	uint8_t sd;

/*设置PE口得低八位为输出*/
	setNandDataOutput ();
	
	SET_WE;
	SET_RE;
	CLR_ALE;
	CLR_CLE;

	CLR_CE;//片选开启
	SET_CLE;
	CLR_ALE;
	SET_RE;
	CLR_WE;
	//GPIO_Write(GPIOE, 0x00); //  读命令
	nandWriteDataToPort (0x00);
	SET_WE;
	CLR_CLE;
	SET_ALE;
	SET_RE;
	CLR_WE;
	//GPIO_Write(GPIOE, 0x00); //页地址
	nandWriteDataToPort (0x00);
	SET_WE;
	CLR_WE;
	//GPIO_Write(GPIOE, 0x00);//页地址
	nandWriteDataToPort (0x00);
	SET_WE;
	CLR_WE;
	//GPIO_Write(GPIOE, 0x00); //快地址
	nandWriteDataToPort (0x00);
	SET_WE;
	CLR_WE;
	//GPIO_Write(GPIOE, 0x00);//快地址
	nandWriteDataToPort (0x00);
//	SET_WE;
//	CLR_WE;
//	//GPIO_Write(GPIOE, 0x00);//快地址
//	nandWriteDataToPort (0x00);
	SET_WE;

	CLR_ALE;
	SET_CLE;
	SET_RE;
	CLR_WE;
	//GPIO_Write(GPIOE, 0x30); //开始读出命令
	nandWriteDataToPort (0x30);
	SET_WE;
	CLR_CLE;
	//GPIOE->CRL=0x88888888; //接口转换
	setNandDataInput ();
	//for(n=30;n;n--);//满足时序
	//delay_1us(20); //等待
	wait_NAND_readay();
	for(n=0;n < 2112; n++){
		CLR_RE;
		sd=nandReadDataFromPort ();//读出数据
		SET_RE;
		if (n < 3000){
			if (n % 16 == 0){
				cy_println ();
			}
			cy_print("%02x, ", sd);//发送到计算机
		}
	}

	SET_CE;
	SET_WE;
	SET_RE;
	CLR_ALE;
	CLR_CLE;
	cy_println();
}

/*写闪存的一页，数据=2112字节=2kb*/
void write_NAND_PAGE(void)
{
   uint8_t send_date=0;
   uint32_t numb;
   /*设置PE口得低八位为输出*/
	setNandDataOutput ();
	
	SET_WE;
	SET_RE;
	CLR_ALE;
	CLR_CLE;

	CLR_CE;//片选开启
	SET_CLE;//命令
	CLR_ALE;
	SET_WE;
	SET_RE;

	CLR_WE;
	//GPIO_Write(GPIOE, 0x80); //写一页命令
	nandWriteDataToPort (0x80);
	SET_WE;
	CLR_CLE;//释放命令总线


	SET_ALE;//地址写入允许
	
	CLR_WE;
	//GPIO_Write(GPIOE, 0x00); //页地址0
	nandWriteDataToPort (0x00);
	SET_WE;
	CLR_WE;
	//GPIO_Write(GPIOE, 0x00); //页地址
	nandWriteDataToPort (0x00);
	SET_WE;
	CLR_WE;
	//GPIO_Write(GPIOE, 0x00); //快递至
	nandWriteDataToPort (0x00);
	SET_WE;
	CLR_WE;
	//GPIO_Write(GPIOE, 0x00); //快递至
	nandWriteDataToPort (0x00);
//	SET_WE;
//	CLR_WE;
//	//GPIO_Write(GPIOE, 0x00); //快递至
//	nandWriteDataToPort (0x00);
	SET_WE;
	
	CLR_ALE;//释放地址总线

	for(numb=5;numb;numb--);//延时
	for(numb=2112;numb;numb--)
	{
		CLR_WE;
		//GPIO_Write(GPIOE, 0x89);  //写入数据89
		nandWriteDataToPort (0x5A);
		SET_WE;
	}
	SET_CLE;//开启命令总线
	CLR_WE;
	//GPIO_Write(GPIOE, 0x10);//页编程
	nandWriteDataToPort (0x10);
	SET_WE;
	//for(numb=3;numb;numb--);//略微延时
	wait_NAND_readay();
	//delay_1us(500);
	CLR_WE;
	//GPIO_Write(GPIOE, 0x70);//状态读 //这个状态要为E0才表示操作完成可以进行下一步操作
	nandWriteDataToPort (0x70);
	SET_WE;
	//GPIOE->CRL=0x88888888;
	setNandDataInput ();
	SET_WE;
	SET_RE;
	CLR_RE;
	send_date=nandReadDataFromPort ();//读出数据
	SET_CE;
	SET_WE;
	SET_RE;
	CLR_ALE;
	CLR_CLE;
	cy_println ("send-data: %0x", send_date);
}

/*擦除一个块*/
void erase_NAND_BLOK(void)
{ 
	uint8_t e;

/*设置PE口得低八位为输出*/
	setNandDataOutput ();


	SET_WE;
	SET_RE;
	CLR_ALE;  //初始化
	CLR_CLE;

	CLR_CE;
	SET_CLE;
	CLR_ALE;
	CLR_WE;
	//GPIO_Write(GPIOE, 0x60); //命令
	nandWriteDataToPort (0x60);
	SET_WE;
	CLR_CLE;
	SET_ALE;
	CLR_WE;
	//GPIO_Write(GPIOE, 0x00); //快地址0
	nandWriteDataToPort (0x00);
	SET_WE;
	CLR_WE;
	//GPIO_Write(GPIOE, 0x00); //块地址1
	nandWriteDataToPort (0x00);
//	SET_WE;
//	CLR_WE;
//	//GPIO_Write(GPIOE, 0x00); //块地址1
//	nandWriteDataToPort (0x00);
	SET_WE;
	
	CLR_ALE;
	SET_CLE;
	CLR_WE;
	//GPIO_Write(GPIOE, 0xd0); //开始擦除
	nandWriteDataToPort (0xd0);
	SET_WE;
	CLR_CLE;
	//for(m=5;m;m--);//延时满足时序
	wait_NAND_readay();
	//delay_1us(3000); //等待擦完
	SET_CLE;
	CLR_WE;
	//GPIO_Write(GPIOE, 0x70); // 读状态码状态读 //这个状态要为E0才表示操作完成可以进行下一步操作
	nandWriteDataToPort (0x70);
	SET_WE;
	CLR_CLE;
	//GPIOE->CRL=0x88888888; //数据口转换成输入
	setNandDataInput ();
	CLR_RE;
	//e=(GPIOE->IDR);//读出数据
	e=nandReadDataFromPort ();//读出数据
	SET_RE;
	//my_send_byte(e);  //穿行发送
	cy_println ("send-data: %0x", e);

}















//void delay_us(unsigned int t)
//{
//	volatile int i;
//	while (t--) {
//		for (i=0 ; i<9 ; i++)
//			;
//	}
//}
void delay_ms1(unsigned int t)
{
	volatile int i;
	while (t--) {
		for (i=0 ; i<5555 ; i++)
			;
	}
}




void rNF_Reset()
{
	NF_CE_L();//nf =0 select chip
	NF_CLEAR_RB();//RnB 0 to 1, transition is detected,clear this
	NF_CMD(NAND_CMD_RESET);  //NFCMMD=0xff memory command value 0xff是什么命令
	NF_DETECT_RB(); //if RnB is 0,waiting ,if 1,go on  
	NF_CE_H();// nf =1 not select chip
}

void rNF_Init(void)
{
	//NANAFLASH CONTROL CONFIG
//	rNFCONF=(rNFCONF&~(0x1<<3))|(0x1<<2);
	//pagesize_ext = 1 large size nandflash--- pagesize set 2048byte/page 
//	rNFCONF = ( rNFCONF&~(0x3<<23) )|(0x2<<23)|(0x7<<12)|(0x7<<8)|(0x7<<4);
	//indicate will use 4bit ECC,CLE/ALE duration = HCLK*0x7,TWRPH0/1 duration = HCLK*(0X7+1),
//	rNFCONT = ( rNFCONT&~(0x1<<18) ) |(1<<18)|(0<<16)|(1<<0); // Init NFCONT
	//encoding 4bit ECC,disable lock,nandflash work	
//	rNFSTAT |= ((1<<6)|(1<<5)|(1<<4));
	//nf chip select(nfchoose),init main/spare ECC encoder/decoder
//	rNFSBLK=0;
	//nf programable start block address 
//	rNFEBLK=0X00ffffff;
	//nf programable end block address   2M?
	
	rNF_Reset();
}

unsigned char BlockErase(unsigned int pageaddr)
{
	unsigned char EraseStatus;
		
	rNF_Reset();
	
	NF_CE_L();
//	rNFSTAT |= (1<<4); // RnB Clear

	NF_CMD(NAND_CMD_ERASE1);
	NF_ADDR(pageaddr&0xFF);
	NF_ADDR((pageaddr>>8)&0xFF);
	NF_ADDR((pageaddr>>16)&0xFF);
	//NF_ADDR((blocknum>>16)&0xFF);
	NF_CMD(NAND_CMD_ERASE2);
	NF_DETECT_RB();
//	rNFSTAT |= (1<<4); // RnB Clear
	
	NF_CMD(NAND_CMD_STATUS);
//	EraseStatus = NF_DATA_R();
	
	NF_nFCE_H();

	if(EraseStatus & 0x01)	return FALSE;	
	
	return	TRUE;		
}
unsigned char rNF_WritePage(unsigned int page_num, unsigned char *buf)
{
	unsigned int i;
	unsigned char stat;
	
	
	//rNF_wInit();
	NF_CE_L();
	NF_CLEAR_RB();

	NF_CMD(NAND_CMD_SEQIN);
	NF_ADDR(0x00);
	NF_ADDR(0x00);
	NF_ADDR((page_num) & 0xFF);
	NF_ADDR((page_num>>8) & 0xFF);
	NF_ADDR((page_num>>16) & 0xFF);
 
	for (i=0; i<2048; i++)
	{
		NF_DATA_W(buf[i]);
		delay_us(5);
	}

	NF_CMD(NAND_CMD_PAGEPROG);
	delay_ms1(1);
//	Delay(1);
	NF_CMD(NAND_CMD_STATUS);

	do {
//		stat = NF_RDDATA8();
	} while (!(stat & 0x40));
	NF_CE_H();

	NF_nFCE_H();
	return TRUE;
}

unsigned char rNF_RamdomRead(unsigned int page_num, unsigned int addr)
{
	unsigned char temp;
	NF_CE_L();
	NF_CLEAR_RB();
	NF_CMD(NAND_CMD_READ0);
	NF_ADDR(0x00);
	NF_ADDR(0x00);
	NF_ADDR((page_num) & 0xFF);
	NF_ADDR((page_num>>8) & 0xFF);
	NF_ADDR((page_num>>16) & 0xFF);

	NF_CMD(NAND_CMD_READSTART);
	NF_DETECT_RB();
	NF_CMD(NAND_CMD_RNDOUT);
	NF_ADDR((unsigned char) (addr & 0xFF));
	NF_ADDR((unsigned char) ((addr>>8) & 0x0F));
	NF_CMD(NAND_CMD_RNDOUTSTART);
//	temp = NF_RDDATA8();
	NF_nFCE_H();
	return(temp);
}


void rLB_ReadPage(unsigned int addr, unsigned char * to)
{
	unsigned int  i;

	//rNF_Init();
	rNF_Reset();

	//  Enable the chip
	NF_nFCE_L();   
	NF_CLEAR_RB();

	// Issue Read command
	NF_CMD(CMD_READ);

	//  Set up address
	NF_ADDR(0x00);
	NF_ADDR(0x00);
	NF_ADDR((addr) & 0xff);
	NF_ADDR((addr >> 8) & 0xff);
	NF_ADDR((addr >> 16) & 0xff);

	NF_CMD(NAND_CMD_READSTART);

	NF_DETECT_RB();		// wait tR(max 12us)

	for (i = 0; i < 2048; i++)
	{
//		to[i] =  NF_RDDATA8();
	}

	NF_nFCE_H();
}





//////////////////////////////////////////////////////////////////////
// Block为Nand块区号，Page为对应块中的页号，Address为页内随机地址，
// Length为读取长度，Buffer为数据缓存区
static unsigned char Nand_RamdomRead(unsigned int Block, unsigned int Page,
				unsigned int Address, unsigned short Length,
				unsigned char *Buffer)
{
	unsigned short i;
	if (Address + Length > 2048+64) {
		return 1; // 页内地址及写入长度不能超出该页范围
	}
	if (Buffer == (void *)0 || Length == 0) {
		return 2; // 参数错误
	}	
		
	Page += (Block << 6); // Block转换为页数	
	NF_CE_ENABLE();
	NF_CLEAR_RB();

	NF_CMD(NAND_CMD_READ0); // page read cycle 1
	NF_ADDR(0); // column address
	NF_ADDR(0); // columu address
	NF_ADDR(Page & 0xff); // 传输3字节的页地址
	NF_ADDR((Page>>8) & 0xff);
	NF_ADDR((Page>>16) & 0xff);	
	NF_CMD(NAND_CMD_READSTART); // page read cycle 2	
	
	NF_WAIT_READY(); // 等待页读完成
	NF_CMD(NAND_CMD_RNDOUT); // ramdom read cycle 1
	NF_ADDR(Address & 0xff); // 2 cycle address in page
	NF_ADDR((Address>>8) & 0xf);
	NF_CMD(NAND_CMD_RNDOUTSTART); // ramdom read cycle 2
	
	for (i=0; i<Length; i++) {
//		Buffer[i] = NF_READ_BYTE(); // 读取Length长度数据
	}	
	
	NF_CE_DISABLE();
	return 0;
}

// Block为Nand块区号，Page为对应块中的页号，Address为页内随机地址，
// Length为写入长度，Buffer为数据缓存区
static unsigned char Nand_RamdomWrite(unsigned int Block,unsigned int Page,
				unsigned int Address, unsigned short Length,
				unsigned char *Buffer)
{
	unsigned short i;
	unsigned char State;
	if (Address + Length > 2048+64) {
		return 1; // 页内地址及写入长度不能超出该页范围
	}
	if (Buffer == (void *)0 || Length == 0) {
		return 2; // 参数错误
	}
	Page += (Block << 6); // Block转换为页数	
	NF_CE_ENABLE();
	NF_CLEAR_RB();

	NF_CMD(NAND_CMD_SEQIN); // 页写周期1
	NF_ADDR(0); // column address
	NF_ADDR(0); // columu address
	NF_ADDR(Page & 0xff); // 3字节页地址
	NF_ADDR((Page>>8) & 0xff);
	NF_ADDR((Page>>16) & 0xff);	

	NF_CMD(NAND_CMD_RNDIN); // 页内随机写命令
	NF_ADDR(Address & 0xff); // 2字节页内地址
	NF_ADDR((Address>>8) & 0xf);		
	for (i=0; i<Length; i++) {
		NF_WRITE_BYTE(Buffer[i]); // 写入Length长数据
	}
	NF_CMD(NAND_CMD_PAGEPROG); // 页写周期2

	NF_WAIT_READY(); // 等待写完	
	NF_CMD(NAND_CMD_STATUS); // 读取写结果状态
	do {
//		State = NF_READ_BYTE();
	} while(!(State & (1<<6))); // 等待状态变成Ready
	
	NF_CE_DISABLE();	
	if (State & (1<<0)) {
		return 3; // ramdom write error
	}
	return 0;
}

unsigned char Nand_IsBadBlock(unsigned int Block)
{
	unsigned char Data;
	// 每个block第一页spare区第6字节非0xff标记为好坏
	Nand_RamdomRead(Block, 0, 2054, 1, &Data);
	if (Data != 0xff) {
		return 1; // 坏块
	}
	return 0;
}

unsigned char Nand_MarkBadBlock(unsigned int Block)
{
	// 每个block第一页spare区第6字节标记非0xff坏块
	unsigned char Data = 0x55;
	return Nand_RamdomWrite(Block, 0, 2054, 1, &Data);
}

// Block为Nand块区号，Page为对应块中的页号，Buffer为数据缓存区
int Nand_ReadPage(unsigned int Block, unsigned int Page,
							unsigned char *Buffer)
{
	unsigned int i;
//	unsigned int MECC, SECC;
	if (Buffer == (unsigned char *)0) {
		return 1; // 缓冲区为空，参数错误
	}
	Page &= (64-1); // 64 page in one block
	Page += (Block << 6); // Block转换为页数
	NF_INIT_MECC(); // main区ECC清空
	NF_INIT_SECC(); // spare区ECC清空
	NF_MECC_UNLOCK(); // main区ECC解锁，开始ECC计算
	NF_CE_ENABLE(); // 使能片选
	NF_CLEAR_RB(); // 清数据传输标志
	
	NF_CMD(NAND_CMD_READ0); // page read cycle 1
	NF_ADDR(0); // column address
	NF_ADDR(0); // columu address
	NF_ADDR(Page & 0xff); // 写入3字节的页地址
	NF_ADDR((Page>>8) & 0xff);
	NF_ADDR((Page>>16) & 0xff);	
	NF_CMD(NAND_CMD_READSTART); // page read cycle 2

	NF_WAIT_READY(); // 等待命令完成	
	for (i=0; i<2048; i++) { // 读取main区数据
//		Buffer[i] = NF_READ_BYTE();
	}
	NF_MECC_LOCK(); // 锁定main ECC
	NF_SECC_UNLOCK(); // 解锁spare ECC

//	MECC = NF_READ_WORD(); // spare区前4字节为main区ECC
	// main区的ECC放入到NFMECCD0/1中相应的位中
//	rNFMECCD0=((MECC&0xff00)<<8) | (MECC&0xff);	
//	rNFMECCD1=((MECC&0xff000000)>>8) | ((MECC&0xff0000)>>16);
	NF_SECC_LOCK(); // 锁定spare ECC
	// spare区第5,6这两字节为spare区ECC,剩下部分未使用
//	SECC = NF_READ_WORD();
	// spare区的ECC放入到NFMECCD0/1中相应的位中	
//	rNFSECCD=((SECC&0xff00)<<8)|(SECC&0xff);	
	NF_CE_DISABLE();
	
	// check whether spare/main area bit fail error occurred
//	if ((rNFECCERR0 & 0xf) == 0) {
//		return 0; // 数据读取正确
//	} else {
//		return 2; // ECC检验不一致，数据读取有误
//	}
	return 0;
}

// Block为Nand块区号，Page为对应块中的页号，Buffer为数据缓存区
int Nand_WritePage(unsigned int Block, unsigned int Page, 
							unsigned char *Buffer)
{
	unsigned int i;
	unsigned char State;
//	unsigned int MECC, SECC;
	if (Buffer == (unsigned char *)0) {
		return 1; // 数据缓存参数错误
	}
	if (Nand_IsBadBlock(Block)) {
		return 2; // 是坏块，返回坏块错误码
	}
	
	Page &= (64-1); // 1 block最大64页
	Page += (Block << 6); // block转换成页
	NF_INIT_MECC(); // main区ECC清空
	NF_INIT_SECC(); // spare区ECC清空
	NF_MECC_UNLOCK(); // main区ECC解锁，开始ECC计算
	NF_CE_ENABLE(); // 使能片选
	NF_CLEAR_RB(); // 清数据传输标志
	
	NF_CMD(NAND_CMD_SEQIN); // page program cycle 1
	NF_ADDR(0); // column address
	NF_ADDR(0); // columu address
	NF_ADDR(Page & 0xff); // 写入3字节页地址
	NF_ADDR((Page>>8) & 0xff);
	NF_ADDR((Page>>16) & 0xff);	
	
	for (i=0; i<2048; i++) { // 写入2k数据到main区
		NF_WRITE_BYTE(Buffer[i]);		
	}
	NF_MECC_LOCK(); // 锁定main ECC
//	MECC = rNFMECC0; // 4字节写main区数据的ECC	
	NF_SECC_UNLOCK(); // 解锁spare ECC
	NF_WRITE_BYTE(MECC&0xff);// 写4字节main ECC到spare区
	NF_WRITE_BYTE((MECC>>8) & 0xff);	
	NF_WRITE_BYTE((MECC>>16) & 0xff);	
	NF_WRITE_BYTE((MECC>>24) & 0xff);
	NF_SECC_LOCK(); // 锁定spare ECC
//	SECC = rNFSECC; // 2字节的spare写数据ECC
	NF_WRITE_BYTE(SECC & 0xff); // 继续写入SECC
	NF_WRITE_BYTE((SECC>>8) & 0xff);
	NF_CMD(NAND_CMD_PAGEPROG); // page program cycle 2
	
	NF_WAIT_READY(); // 等待写完
	NF_CMD(NAND_CMD_STATUS); // 读取nand状态
	do {
//		State = NF_READ_BYTE();
	} while(!(State & (1<<6))); // 等待状态变成Ready
	
	NF_CE_DISABLE();
	
	// 是否写成功,第0位为0则pass,不然fail
	if (State & (1<<0)) {
		if (Nand_MarkBadBlock(Block)) { // 标志坏块
			return 3; // 写不成功并且坏块标记不成功
		} else {
			return 4; // 写不成功坏块标记成功
		}
	}
	return 0;
}
int Nand_ReadID(Nand_ID_Info *pInfo)
{
	if (pInfo == (Nand_ID_Info *)0) {
		return 1; // 参数错误
	}
	NF_CE_ENABLE();
	NF_CLEAR_RB();
	NF_CMD(NAND_CMD_READID); // 发送读ID命令
	NF_ADDR(0x0); // 写0x0地址
	
//	pInfo->Maker = NF_READ_BYTE(); // Maker:0xEC
//	pInfo->Device = NF_READ_BYTE(); // Device:0xDA
//	pInfo->ID_Data3 = NF_READ_BYTE(); //0x10
//	pInfo->ID_Data4 = NF_READ_BYTE(); //0x95
//	pInfo->ID_Data5 = NF_READ_BYTE();  //0x44	
	
	NF_CE_DISABLE();
	return 0;
}

unsigned char Nand_EraseBlock(unsigned int Block)
{
	unsigned char State;
	NF_CE_ENABLE();
	NF_CLEAR_RB();

	NF_CMD(NAND_CMD_ERASE1); // erase block command cycle 1
	// write 3 cycle block address[A28:A18]
	NF_ADDR((Block<<6) & 0xff); // [A19:A18]
	NF_ADDR((Block>>2) & 0xff); // [A27:A20]
	NF_ADDR((Block>>10) & 0xff); // A28

	NF_CMD(NAND_CMD_ERASE2); // erase block command cycle 2

	NF_WAIT_READY();
	NF_CMD(NAND_CMD_STATUS);
	do {
//		State = NF_READ_BYTE();
	} while(!(State & (1<<6))); // 等待状态变成Ready
	
	NF_CE_DISABLE();
	// 是否擦写成功,第0位为0则pass,不然fail
	if (State & (1<<0)) {
		if (Nand_MarkBadBlock(Block)) {
			return 3; // 擦除不成功并且坏块标记不成功
		} else {
			return 4; // 擦除不成功坏块标记成功
		}
	}	
	return 0; // 成功擦除
}



// Address为Nand字节偏移地址，Buffer为数据缓存区
// Length为读取长度
unsigned char Nand_ReadSkipBad(unsigned int Address,
			unsigned char *Buffer, unsigned int Length)
{
	unsigned int BlockIndex, PageIndex;
	unsigned int ReadBytes;
	unsigned char i;
	unsigned char State;
	
	if (Length == 0 || Buffer == (void *)0) {
		return 1; // 参数错误
	}
	if ((Address & 0x7ff) != 0) {
		return 2; // nand地址非页对齐,读最小单位为1页
	}

	State = 0;
	ReadBytes = 0; // 已读字节数
	PageIndex = (Address >> 11) & 0x3f; // 块中的页偏移位置
	BlockIndex = Address >> 17; // 块读位置
	while (1) {
		if (Nand_IsBadBlock(BlockIndex)) {
			BlockIndex++; // 坏块,跳过读取下一块
			continue;
		}
		for (i=PageIndex; i<64; i++) { // 读取一个block
			if (Nand_ReadPage(BlockIndex, i, Buffer)) {
				if (State == 1) {
					State=2; // read error twice
					// 再次读错误,认为是坏块,取消这块读的数据
					ReadBytes -= 2048 * (i-PageIndex);
					// 调整内存位置到读取这块之前
					Buffer -= 2048 * (i-PageIndex);				
					break;
				} else {
					State = 1;
					i -= 1; // 读出错，尝试再次读该页
					continue;
				}
			}
			ReadBytes += 2048; // 读取了一页数据
			if (ReadBytes >= Length) {
				break; // 读取数据足够则退出
			}
			State = 0;
			Buffer += 2048; // 下一页内存存储位置	
		}
		if (State == 2) { // 两次读取均出错,跳过到下一块
			BlockIndex++;
			State = 0;
			continue;
		}
		if (ReadBytes >= Length) {
			break; // 读完退出循环
		}
		PageIndex = 0; // 下一块从第一页开始读
		BlockIndex++; // 读下一个块	
	}
	return 0;
}

// Address为Nand字节偏移地址，Buffer为数据缓存区
// Length为写长度
unsigned char Nand_WriteSkipBad(unsigned int Address,
			unsigned char *Buffer, unsigned int Length)
{
	unsigned int BlockIndex, PageIndex;
	unsigned int WriteBytes;
	unsigned char i;
	unsigned char State;

	if (Length == 0 || Buffer == (void *)0) {
		return 1; // 参数错误
	}
	if ((Address & 0x7ff) != 0) {
		return 2; // nand地址非页对齐,读最小单位为1页
	}
	
	State = 0;
	WriteBytes = 0; // 己写字节数
	PageIndex = (Address >> 11) & 0x3f; // 块中的页偏移位置
	BlockIndex = Address >> 17; // 块写位置	
	while (1) 
	{
		if (Nand_EraseBlock(BlockIndex)) 
		{
			BlockIndex++; // 坏块,跳过到下一块
			continue;
		}
		for (i=PageIndex; i<64; i++) { 
			if (Nand_WritePage(BlockIndex, i, Buffer)) {// 写一页
				if (State == 1) {
					State=2; // write error twice
					// 再次写错误,认为是坏块,取消这块写的数据
					WriteBytes -= 2048 * (i-PageIndex);
					// 调整内存位置到写这块之前	
					Buffer -= 2048 * (i-PageIndex);			
					break;
				} else {
					State = 1;
					i -= 1; // 写出错，尝试再次写该页
					continue;
				}
			}
			WriteBytes += 2048; // 写取了一页数据
			if (WriteBytes >= Length) {
				break; // 数据写入完退出
			}
			State = 0;
			Buffer += 2048; // 下一页内存存储位置	
		}
		if (State == 2) { // 两次写均出错,跳过到下一块
			BlockIndex++;
			State = 0;
			continue;
		}
		if (WriteBytes >= Length) {
			break; // 写完退出循环
		}
		PageIndex = 0; // 下一块从第一页开始写
		BlockIndex++; // 再写下一个块	
	}	
	return 0;
}

static void Nand_Reset()
{
	NF_CE_ENABLE();	
	NF_CLEAR_RB();	
	NF_CMD(NAND_CMD_RESET);
	NF_WAIT_READY();	
	NF_CE_DISABLE();	
}

void Nand_Init()
{
	// 配置nand控制引脚
//	rGPACON = (rGPACON &~(0x3f<<17)) | (0x3f<<17);  
	// 配置K9F2G08U0B timing(HCLK@133M)
	// TACLS=1, (tALS或tCLS-tWP=0)(ALE或CLE有效后需保持才能发出写脉冲)
	// TWRPH0=2,tWP=12ns(最小写脉冲宽度)
	// TWRPH1=1,tALH或tCLH=5ns(写脉冲后ALE或CLE需保持有效时间)
	// SLC nand 1位检验就行,1-bit ECC
//	rNFCONF = (1<<12)|(2<<8)|(1<<4)|(0<<0);

	// 上升沿检查nand准备好信号线
//	rNFCONT = (0<<12)|(0<<10)|(0<<9)|(0<<8)
//				|(0x3<<6)|(0x3<<4)|(0x3<<1)|(1<<0);
	
	Nand_Reset();
}

// 从运行代码的RAM位置一定长度的代码烧写进Nand 0x0偏移处
unsigned char WriteCodeToNand()
{
// 在板级代码LowLevelInit.s中引出了链接器生成的代码大小，代码运行地址信息
// __CodeAddr__为代码到拷贝到的RAM位置，链接文件中设定
// __CodeSize__为二进制代码编译生成的大小，链接器最终链接后给出
//	extern unsigned int __CodeAddr__;
//	extern unsigned int __CodeSize__;	
//	unsigned char State;
//	Nand_Init(); // Nand时序初始化
//	State = Nand_WriteSkipBad(0, (unsigned char *)__CodeAddr__, __CodeSize__);
//	return State;
	return 0;
}

// 从运行代码的RAM位置一定长度的代码烧写进Nand 0x0偏移处
unsigned char WriteAppToNand(unsigned int CodeAddr, unsigned int CodeSize)
{
	//Nand前两块留给bootload 共256K，第三块开始给应用程序
	unsigned char State;
	Nand_Init(); // Nand时序初始化
	State = Nand_WriteSkipBad(0, (unsigned char *)CodeAddr, CodeSize);
	return State;
}

// 从运行代码的RAM位置一定长度的代码烧写进Nand 0x0偏移处
unsigned char WriteAppToAppSpace(unsigned int CodeAddr, unsigned int CodeSize)
{
	//Nand前两块留给bootload 共256K，第三块开始给应用程序
	unsigned char State;
	Nand_Init(); // Nand时序初始化
	State = Nand_WriteSkipBad(APP_NAND_ADDR, (unsigned char *)CodeAddr, CodeSize);
	return State;
}



