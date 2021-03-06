
#ifndef __TQ2416_YQSQLITE3_H__
#define __TQ2416_YQSQLITE3_H__


#define FRAM_PARA_SET_VALUE_ADDR 2048
#define FRAM_COIN_PRE_VALUE_ADDR	FRAM_PARA_SET_VALUE_ADDR + 4096


#define APP_NAND_ADDR 10*64*2048 //应用程序存放地址 为Block 10 Page 0处
#define APP_NAND_SIZE 256*1024 //应用程序大小暂定为256K (共占2个BLOCK)


#define DBINSERT 1
#define DBDELETE 2
#define DBDISPLAY 3
#define DBDISPLAYBACK 4

#define HISTORYSAVANUM (32)

#define PAGE_BYTE_SIZE (2048)
#define BLOCK_PAGE_SIZE (64)
#define BLOCK_BYTE_SIZE (PAGE_BYTE_SIZE * BLOCK_PAGE_SIZE)

#define ITEM_SIZE HISTORYSAVANUM  //记录的大小
#define PAGE_ITEM_NUM_SIZE (PAGE_BYTE_SIZE / ITEM_SIZE) //一页中记录的条数


						//条数				//日期					   //工号 //金额				//数量				//异币		
/////////////////////////(BLOCK * (64 * 2048) + PAGE)	
#define PUBULIC_DATA_START_BLOCK_NUM 20
#define PUBULIC_DATA_START_PAGE_NUM 0
#define HISTORY_START_BLOCK_NUM (PUBULIC_DATA_START_BLOCK_NUM + 1)
#define HISTORY_START_PAGE_NUM (0)

#define DATA_START_BLOCK  (PUBULIC_DATA_START_BLOCK_NUM	* (64 * 2048) + PUBULIC_DATA_START_PAGE_NUM * 2048)  //保存公共信息页 START BLOCK 20 PAGE 0
#define YQNDHISTORYBLOCK (HISTORY_START_BLOCK_NUM * (64 * 2048))  //nations保存历史 信息页币种0-50块-3200页 0-2048  22个字节   4--110块--7040 条数是93的多少倍就换加多少页

// 初始化完成了  单条写入  判断   读出显示 任意读也可以 所以 可以写一个类似数据库的方式  任意读  任意写 删除数据库最简单了
#define YQNDHISTORYADDR (YQNDHISTORYBLOCK + (para_set_value.data.db_total_item_num * ITEM_SIZE) / PAGE_BYTE_SIZE)  //nations保存历史 的  地址  由条数 国家币种决定 页

#define YQNDHISTORY_DB_ID_PAGE_ADDR (YQNDHISTORYBLOCK + (db_id * ITEM_SIZE / PAGE_BYTE_SIZE))

//{100,50,50,10,10,5,2,1,10},	//人民币 的面值 倍数  用于计算金额

#define COUNTRY_ID 0
#define ALL_COIN

#ifdef ALL_COIN
#define MONEY_10_00 1000
#define MONEY_5_00 	500
#define MONEY_1_00 	100
#define MONEY_0_50 	50
#define MONEY_0_10 	10
#define MONEY_0_05 	5
#define MONEY_0_02 	2
#define MONEY_0_01 	1
#else
#define MONEY_10_00 100
#define MONEY_5_00 	50
#define MONEY_1_00 	10
#define MONEY_0_50 	5
#define MONEY_0_10 	1
#define MONEY_0_05 	0.5
#define MONEY_0_02 	0.2
#define MONEY_0_01 	0.1
#endif


#define NDGETKICKD 3// 公共信息页 地址定义getkickd
#define NDGETKICKT 4// 公共信息页 地址定义getkickt
#define NDPANBLOCKTIME 5// 公共信息页 地址定义panblocktime
#define NDCOINCHOOSE 7// 公共信息页 地址定义coinchoose
#define NDGHNOW 8// 公共信息页 地址定义gh_now

//		para_set_value.data.db_total_item_num == 0;
//		yqnddata[NDdb_total_item_num(0)] = 0x00;    //公共信息 para_set_value.data.db_total_item_num

// 每种币给 300个字节 4一个程序限4种币 *300 = 1200
#define NDdb_total_item_num (9+300*coinchoose)  //para_set_value.data.db_total_item_num  2BYTE
#define NDZENUM (9+300*coinchoose+2)// para_set_value.data.total_money 4BYTE 
#define NDZSNUM (9+300*coinchoose+6)// para_set_value.data.total_good 4BYTE
#define NDFGNUM (9+300*coinchoose+10)// para_set_value.data.total_ng 4BYTE

#define NDCN0VALUELEVEL (9+300*coinchoose+14)// accoinamount[coinchoose]. fg_coin 3BYTE	  15 16
 

#define NDCOIN0MAXNUM(i) (9+300*coinchoose+17+20*i)// COIN0 2BYTE 14
#define NDCOIN0MINNUM(i) (9+300*coinchoose+19+20*i)// COIN0 2BYTE 16
#define NDCOIN1MAXNUM(i) (9+300*coinchoose+21+20*i)// COIN0 2BYTE 18
#define NDCOIN1MINNUM(i) (9+300*coinchoose+23+20*i)// COIN0 2BYTE 20
#define NDCOIN2MAXNUM(i) (9+300*coinchoose+25+20*i)// COIN0 2BYTE 22
#define NDCOIN2MINNUM(i) (9+300*coinchoose+27+20*i)// COIN0 2BYTE 24
#define NDCOIN0STDNUM(i) (9+300*coinchoose+29+20*i)// COIN0 2BYTE 26
#define NDCOIN1STDNUM(i) (9+300*coinchoose+31+20*i)// COIN0 2BYTE 28
#define NDCOIN2STDNUM(i) (9+300*coinchoose+33+20*i)// COIN0 2BYTE 30

#define NDCOINYZNUM(i) (9+300*coinchoose+35+20*i)// COIN0 yuzhi value	32




typedef struct
{
	uint32_t	index;
	uint8_t  time[8];
	uint32_t ID;
	uint32_t money;
	uint32_t total_good;
	uint32_t total_ng;
	uint32_t RESERVE;
}s_db_item_info;

typedef struct
{
	s_db_item_info item_info_array[PAGE_BYTE_SIZE / ITEM_SIZE];
}s_db_item_block;


#define COUNTRY0_COIN_PRE_VALUE_START_BLOCK_NUM PUBULIC_DATA_START_BLOCK_NUM
#define COUNTRY0_COIN_PRE_VALUE_START_PAGE_NUM (2 + PUBULIC_DATA_START_PAGE_NUM)
#define COUNTRY0_COIN_PRE_VALUE_START_ADDR (DATA_START_BLOCK + COUNTRY0_COIN_PRE_VALUE_START_PAGE_NUM * 2048)
#define COIN_TYPE_NUM 11
#define COUNTRY_NUM 1

#define MAGIC_NUM 0xA55A

typedef struct
{
	uint16_t magic_num;
	uint16_t db_total_item_num;
	uint16_t country_id;
	uint16_t coin_id;
	uint16_t op_id;
	uint16_t rej_level;
	//---------------------------------
	uint32_t precoin_set_num[COIN_TYPE_NUM];
	uint32_t total_money;
	uint32_t total_good;
	uint32_t total_ng;
	uint16_t kick_start_delay_t1;
	uint16_t kick_keep_t1;
	uint16_t kick_start_delay_t2;
	uint16_t kick_keep_t2;
	uint16_t motor_idle_t;
	uint16_t pre_count_stop_n;
	uint16_t coin_full_rej_pos;
	uint16_t adj_offset_position;
	uint16_t system_mode;
	uint16_t system_boot_delay;
}s_coin_parameter_value;

typedef union
{
	uint8_t fill[PAGE_BYTE_SIZE];
	s_coin_parameter_value data;
}u_coin_parameter_value;

extern u_coin_parameter_value para_set_value;


typedef struct
{
	uint16_t magic_num;
	int16_t max0;
	int16_t min0;
	int16_t max1;
	int16_t min1;
	int16_t max2;
	int16_t min2;
	int16_t std0;
	int16_t std1;
	int16_t std2;
	int16_t offsetmax0;
	int16_t offsetmin0;
	int16_t offsetmax1;
	int16_t offsetmin1;
	int16_t offsetmax2;
	int16_t offsetmin2;
	uint32_t * p_pre_count_set;
	uint32_t * p_pre_count_cur;
	uint32_t * p_pre_count_full_flag;
	uint32_t * p_coinval;
	uint32_t coin_type;
	uint32_t accept;
	uint16_t hmi_pre_count_set_addr;
	uint16_t money;
}s_coin_cmp_value;

typedef union
{
	uint8_t fill[64];
	s_coin_cmp_value data;
}u_coin_cmp_value;

typedef union
{
	uint8_t fill[PAGE_BYTE_SIZE];
	u_coin_cmp_value coin[COIN_TYPE_NUM];

}u_coin_pre_value;

typedef struct
{
	u_coin_pre_value country[COUNTRY_NUM];
}s_country_coin;

extern s_country_coin pre_value;

extern volatile char dbsave[HISTORYSAVANUM]; 




void Writekick_value(void);		//写入 当前踢币时延  踢币持续时间 转盘堵转时间  传送带传送时间

void read_para (void);
void write_para (void);
void read_coin_value(void); 	 // read  COIN  0--8
void write_coin_value (void);

//void Writecoinall_value(uint16_t coin);   	//write coin 0--8




void ini_picaddr(void); //币种切换时的 初始化地址函数
void ini_screen (void);


void initial_nandflash(void);    //nandflash

void yqsql_exec(uint16_t chos);    


int16_t test_read_r_code (int16_t r_code);
int16_t test_write_r_code (int16_t r_code);
int16_t test_erase_r_code (int16_t r_code);



#endif
