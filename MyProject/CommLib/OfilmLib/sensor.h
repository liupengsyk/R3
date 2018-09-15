#ifndef SENSOR_INCLUDED
#define SENSOR_INCLUDED

//20110826 added...
#define HS_IMGKIT_SERIES 0
#define PE_IMGKIT_SERIES 1
#define IMGKIT_VER HS_IMGKIT_SERIES

//这里之前的demo代码需要定义这个，后面用数据库的应该注释掉这个。
#define  _OLD_HS200_DEFINE_H
////pin 
//don't change these definitions 


#define AF_DRV_AD5820  0
#define AF_DRV_DW9710  0
#define AF_DRV_DW9714  0

#define AF_DRV_AD5823  1
#define AF_DRV_FP5512  2
#define AF_DRV_DW9718  3
#define AF_DRV_BU64241 4
#define AF_DRV_LV8498  5
#define AF_DRV_MAX     6


#define KEY_ROI_B0   0x100
#define KEY_ROI_B1   0x80
#define KEY_ROI_B2   0x40
#define KEY_ROI_B3   0x10
#define KEY_ROI_B4   0x20

#define KEY_ROI_GRID 0x04
#define KEY_FULL     0x08
#define KEY_PLAY     0x01
#define KEY_CAM      0x02
#define KEY_NOTHING  0x00 

#define PREVIEW_ROI_B0   0x00
#define PREVIEW_ROI_B1   0x01
#define PREVIEW_ROI_B2   0x02
#define PREVIEW_ROI_B3   0x03
#define PREVIEW_ROI_B4   0x04
#define PREVIEW_ROI_GRID 0x05
#define PREVIEW_FULL     0x06
#define PREVIEW_NOTHING  0x07

#define COM_OK            0
#define ERR_DEVICE_OPEN   1
#define ERR_I2C_NACK	  2
#define ERR_SENSOR_NOT_ME 3
#define ERR_GRABFRAME     4
#define ERR_COM           5
#define ERR_VERSION       6
#define ERR_KIT_NOT_ME    7
#define ERR_I2C_TIMEOUT   8

////////////////////////////////////////////
//HS100-200
#define FPGA_RESET 0
#define FPGA_PWDN  1
#define FPGA_MCLK  2
#define FPGA_GPIO0 3
#define FPGA_GPIO1 4
#define FPGA_SCL   5
#define FPGA_SDA   6
#define FPGA_NC    8

#define GPI_BASE 40
#define GPI_0  0 + GPI_BASE
#define GPI_1  1 + GPI_BASE
#define GPI_2  2 + GPI_BASE
#define GPI_3  3 + GPI_BASE
#define GPI_4  4 + GPI_BASE
#define GPI_5  5 + GPI_BASE
#define GPI_6  6 + GPI_BASE
#define GPI_7  7 + GPI_BASE
#define GPI_8  8 + GPI_BASE
#define GPI_9  9 + GPI_BASE
#define GPI_10  10 + GPI_BASE
#define GPI_11  11 + GPI_BASE
#define GPI_12  12 + GPI_BASE
#define GPI_13  13 + GPI_BASE
#define GPI_14  14 + GPI_BASE
#define GPI_15  15 + GPI_BASE

#define GPO_BASE 60
#define GPO_0  0 + GPO_BASE
#define GPO_1  1 + GPO_BASE
#define GPO_2  2 + GPO_BASE
#define GPO_3  3 + GPO_BASE
#define GPO_4  4 + GPO_BASE
#define GPO_5  5 + GPO_BASE
#define GPO_6  6 + GPO_BASE
#define GPO_7  7 + GPO_BASE
#define GPO_8  8 + GPO_BASE
#define GPO_9  9 + GPO_BASE
#define GPO_10 10 + GPO_BASE
#define GPO_11 11 + GPO_BASE
#define GPO_12 12 + GPO_BASE
#define GPO_13 13 + GPO_BASE
#define GPO_14 14 + GPO_BASE
#define GPO_15 15 + GPO_BASE
#define GPO_16 16 + GPO_BASE
#define GPO_17 17 + GPO_BASE
#define GPO_18 18 + GPO_BASE
#define GPO_19 19 + GPO_BASE
#define GPO_20 20 + GPO_BASE
#define GPO_21 21 + GPO_BASE
#define GPO_22 22 + GPO_BASE
#define GPO_23 23 + GPO_BASE
#define GPO_24 24 + GPO_BASE
#define GPO_25 25 + GPO_BASE
//end 

//pin definition.....
#define PIN_D0 0
#define PIN_D1 1
#define PIN_D2 2
#define PIN_D3 3
#define PIN_D4 4
#define PIN_D5 5
#define PIN_D6 6
#define PIN_D7 7
#define PIN_D8 8
#define PIN_D9 9
#define PIN_PCLK  10
#define PIN_HSYNC 11
#define PIN_VSYNC 12
#define PIN_MCLK  13
#define PIN_RESET 14
#define PIN_PWDN  15
#define PIN_PWDN2 16
#define PIN_GPIO1 17
#define PIN_SDA   18
#define PIN_SCL   19
#define PIN_NC    20
#define PIN_GPIO2 21  //2011 05 14 added.
#define PIN_GPIO3 22  //2011 05 14 added...
#define PIN_GPIO4 23  //2011 05 14 added...
#define PIN_NC1   24  //2011 05 22 added...  
#define PIN_NC2   25  //2011 05 22 added...

#define PIN_D10   26
#define PIN_D11   27


///////////////////////////////////////////
//image kit version.....
#define VER_HS100   0x1000
#define VER_HS200   0x1020

#define VER_HS120   0x1020
#define VER_HS128	0x1200
#define VER_HS230   0x1230

#define VER_HS280	0x1280

#define VER_HS130	0x1300
#define VER_HS300   0x1330
#define VER_HS320   0x1332

#define VER_HV810   0x1380
#define VER_HV820   0x1382
#define VER_HV910   0x1390
#define VER_HV920   0x1392

////////////////////////////////////////////
//HS130 HS300 HV810 HV910


//PMU range....
#define PMU1_1		0x11
#define PMU1_2		0x10
#define PMU2_1		0x21
#define PMU2_2		0x20
#define PMU3_1		0x31
#define PMU3_2		0x30
#define PMU4_1		0x41
#define PMU4_2		0x40
#define PMU5_1		0x51
#define PMU5_2		0x50


//=========================================================
//RESET PWDN definition
//---------------------------------------------------------
#define RESET_H 0x02
#define RESET_L 0x00
#define PWDN_H  0x01
#define PWDN_L  0x00

#define AVDD_28      0x00
#define AVDD_25      0x01
#define AVDD_18      0x02
#define AVDD_DEFAULT 0x03

#define DOVDD_28      0x00
#define DOVDD_25      0x01
#define DOVDD_18      0x02
#define DOVDD_DEFAULT 0x03

#define DVDD_18       0x00
#define DVDD_15       0x01
#define DVDD_12       0x02
#define DVDD_DEFAULT  0x03

#define AFVCC_33      0x00
#define AFVCC_28      0x01
#define AFVCC_18      0x02
#define AFVCC_DEFAULT 0x03



#define _CODE_VER_HS300_ 0x600
#define _CODE_VER_HS200_ 0x500
#define _CODE_VER_HS100_ 0x400

#define _CODE_VER_M300_ 0x300
#define _CODE_VER_S330_ 0x200
#define _CODE_VER_S3A_  0x100

#define _CODE_CURRENT_  _CODE_VER_S330_

#define I2C_400K 1
#define I2C_100K 0

#define I_MAX_100mA 1
#define I_MAX_300mA 0

#define PMU_ON  0
#define PMU_OFF 1

#define POWER_ON	1
#define POWER_OFF   0

#define CLK_ON		1
#define CLK_OFF		0

#define IO_PULLUP	1
#define IO_NOPULL	0


#define DTDELAY    0xffff
#define DTMODE	   0xfffe	
#define DTOR       0xfffd
#define DTAND      0xfffc
#define DTPOLLT	   0xfffb
#define DTPOLL1    0xfffa
#define DTPOLL0    0xfff9
#define DTI2CADDR  0xfff8
#define DTI2CREG   0xfff7
#define DTAFTYPE   0xfff6  //20121223 added... modify the AF Device type
#define DTAFADDR   0xfff5  //20121223 added... modify the AF Device Address..	

#define DTEND	    0xfeff	
#define DTMACRO_ON  0xfef0
#define DTMACRO_OFF 0xfef1 	

#define REG_MASK	 0x0000
#define VAL_MASK	 0x0000


#define D_RAW10      0x00 
#define D_RAW8       0x01
#define D_MIPI_RAW8  0x01
#define D_YUV        0x02 
#define D_RAW16      0x03
#define D_RGB565     0x04
#define D_YUV_SPI    0x05
#define D_MIPI_RAW10 0x06    // 5bytes = 4 pixel...
#define D_MIPI_RAW12 0x07    // 3bytes = 2 pixel...
#define D_RAW12		 0x07
#define D_YUV_MTK_S  0x08    //MTK output...
//#define D_HISPI_SP	 0x09    //aptina hispi packet sp.


#define PORT_MIPI     0   //MIPI output
#define PORT_PARALLEL 1   //Parallel output
#define PORT_MTK	  2   //MTK output
#define PORT_SPI	  3   //SPI output
#define PORT_TEST	  4   //TEST ouput. FPGA output the image...
#define PORT_HISPI	  5   //aptina HISPI packet sp...
#define PORT_ZX2_4	  6   //zhanxun 2bit/4bit packet sp...
#define PORT_MAX	  7   //maxium... can't support >=PORT_MAX


#define MULTICAM_NORMAL             0x00
#define MULTICAM_PWDN_NOT           0x01
#define MULTICAM_RESET_PWDN_OVERLAP 0x02
#define MULTICAM_SPECIAL			0x03


//----------------------------------------------------------
//I2C mode definiton
//when read or write by I2c ,should use this definiton...
//Normal Mode:8 bit address,8 bit data,
//Samsung Mode:8 bit address,8 bit data,but has a stop between slave ID and addr...
//Micron:8 bit address,16bit data...
//Stmicro:16bit addr ,8bit data,such as eeprom and stmicro sensor...
//----------------------------------------------------------

enum I2CMODE
{
	I2CMODE_NORMAL=0,  //8 bit addr,8 bit value 
	I2CMODE_SAMSUNG, //8 bit addr,8 bit value,Stopen
	I2CMODE_MICRON,  //8 bit addr,16 bit value,
	I2CMODE_STMICRO, //16 bit addr,8 bit value, (eeprom also)
	I2CMODE_MICRON2, //16 bit addr,16 bit value,
};
enum CPLD_DATAMODE
{
	DATA_D9D2_8BIT=0,
	DATA_D7D0_8BIT,
	DATA_D9D0_10BIT,
	DATA_D16BIT,
};
enum OUTFORMAT_YUV
{
	OUTFORMAT_YCbYCr = 0,
	OUTFORMAT_YCrYCb,
	OUTFORMAT_CbYCrY,
	OUTFORMAT_CrYCbY,
};
//0:RGGB; first line is RGRG.... ;second line is GBGB....;
//1:GRBG; first line is GRGR.... ;second line is BGBG....; 
//2:GBRG; first line is GBGB.... ;second line is RGRG....; 
//3:BGGR; first line is BGBG.... ;second line is GRGR....; 
enum OUTFORMAT_RGB
{
	OUTFORMAT_RGGB = 0,
	OUTFORMAT_GRBG,
	OUTFORMAT_GBRG,
	OUTFORMAT_BGGR,
};

//you can use these enum type ,or use MHZ or hundred KHZ directly 
enum MCLKOUT
{
	MCLK_6M = 0,
	MCLK_8M,
	MCLK_10M,
	MCLK_11M4,
	MCLK_12M,
	MCLK_12M5,
	MCLK_13M5,
	MCLK_15M,
	MCLK_18M,
	MCLK_24M,
	MCLK_25M,
	MCLK_27M,
	MCLK_30M,
	MCLK_32M,
	MCLK_36M,
	MCLK_40M,
	MCLK_45M,
	MCLK_48M,
	MCLK_50M,
	MCLK_60M,
	MCLK_DEFAULT,
};

//此处的宏定义的长度可以自由修改，修改后不需要其他部分的代码
#define NameLen       (64)   
#define CopyRightLen  (50)     //版权信息的长度
#define UpdateTimeLen (32)
#define VersionLen    (32)

#define FavItemCout	  (30)   //收藏夹的项数
#define ExtendLen     (32)

//下面部分的宏定义看清楚后再修改
#define FavoritesLen  (FavItemCout)*(NameLen)   //收藏夹的总大小  
#define CatalogSpaceLen 1000    //目录区中留的空白字节数

#define DefSysDatabaseName "Dothinkey"
#define DefCopyRight    "CopyRight reserved by DOTHINKEY"
#define DefUpdateTime   "2011-08-29"
#define DefVersion      "0.1 Beta"

#define DefSysDataBaseFileName "Dothinkey.dat"   //系统数据库名称
#define DefFavoritesName "Favorites"    //收藏夹的默认名称,程序运行后不允许修改

#define  DefDBNameEncryption DefSysDatabaseName
#define  DefCopyRightEncryption DefCopyRight

#define SysDatabaseName DefSysDatabaseName

//----------------------------------------------------------
//This define the sensor information such as width and height
//width:sensor preview width in fact..,
//height:sensor preview height in fact..,
//type:0:rgb raw 16 bit;1:rgb raw 8 bit;2:YUV422 data...
//pin:the RESET and PWDN PIN value when work!!!(not reset or pwdn state,but work)
//SlaveID:the I2C slave address,use for choose I2C device...
//mode:I2CMODE_NORMAL,I2CMODE_SAMSUNG,I2CMODE_MICRON,I2CMODE_STMICRO
//FlagReg:the flag register address use for find the sensor...
//FlagData:the flag register's value
//FlagMask:flag mask use for the serial sensor
//name:sensor name you can define..
//ParaList:sensor parameter pointor for initialtion
//ParaListSize:sizeof the paralist...
//----------------------------------------------------------

#ifdef _OLD_HS200_DEFINE_H
	typedef struct _SensorTab
	{
		USHORT width;              //sensor width
			USHORT height;         //sensor height
			BYTE type;             //sensor type :rgb raw ,ycbcr
			BYTE pin;              //Reset and Pwdn pin set (when work)
			BYTE SlaveID;          //sensor slave address
			BYTE mode;
			USHORT FlagReg;
			USHORT FlagData;
			USHORT FlagMask;
			USHORT FlagReg1;
			USHORT FlagData1;
			USHORT FlagMask1;
			LPTSTR name;
			USHORT* ParaList;
			USHORT ParaListSize;
			USHORT* SleepParaList;
			USHORT SleepParaListSize;
			BYTE outformat;        //YUV/	//0:YCbYCr;	//1:YCrYCb;	//2:CbYCrY;	//3:CrYCbY;
			int mclk;              //0:12M; 1:24M; 2:48M.
			BYTE avdd;              // 
			BYTE dovdd;             //
			BYTE dvdd;
			BYTE port; 	
			USHORT Ext0;
			USHORT Ext1;
			USHORT Ext2; 
			LPTSTR vendor;
			USHORT* AF_InitParaList;
			USHORT  AF_InitParaListSize;
			USHORT* AF_AutoParaList;
			USHORT  AF_AutoParaListSize;
			USHORT* AF_FarParaList;
			USHORT  AF_FarParaListSize;
			USHORT* AF_NearParaList;
			USHORT  AF_NearParaListSize;
			USHORT* Exposure_ParaList;
			USHORT  Exposure_ParaListSize;
			USHORT* Gain_ParaList;
			USHORT	Gain_ParaListSize;

	} SensorTab, *pSensorTab;

	  typedef struct _SensorLib
	  {
	  pSensorTab* VensorInfoTab;
	  LPTSTR VensorName;
	  }SensorLib, *pSensorLib;
  
		typedef struct _VensorCam
		{
		int Vendor;
		int Camera;
		}VendorCam, *pVendorCam;
		
		  
	pSensorLib DOTHINKEY_SensorLib[];

	pSensorTab OV_InfoTab[];
	pSensorTab GC_InfoTab[];
	pSensorTab BYD_InfoTab[];
	pSensorTab SET_InfoTab[];
	pSensorTab Samsung_InfoTab[];
	pSensorTab Hynix_InfoTab[];
	pSensorTab Himax_InfoTab[];
	pSensorTab PixelPlus_InfoTab[];
	pSensorTab PixArt_InfoTab[];
	pSensorTab Aptina_InfoTab[];
	pSensorTab BG_InfoTab[];
	pSensorTab Other_InfoTab[];
	pSensorTab Custom_InfoTab[];
	pSensorTab SensorInfoTab[];

#else

	typedef struct _SensorTab
	{
   		USHORT width;          //sensor width
		USHORT height;         //sensor height
		BYTE type;             //sensor type :rgb raw ,ycbcr
		BYTE pin;              //Reset and Pwdn pin set (when work)
		BYTE SlaveID;          //sensor slave address
		BYTE mode;
		USHORT FlagReg;
		USHORT FlagData;
		USHORT FlagMask;
		USHORT FlagReg1;
		USHORT FlagData1;
		USHORT FlagMask1;
		char name[NameLen];

		USHORT* ParaList;
		USHORT  ParaListSize;

		USHORT* SleepParaList;
		USHORT  SleepParaListSize;

		BYTE outformat;         //YUV/	//0:YCbYCr;	//1:YCrYCb;	//2:CbYCrY;	//3:CrYCbY;
		int mclk;               //0:12M; 1:24M; 2:48M.
		BYTE avdd;              // 
		BYTE dovdd;             //
		BYTE dvdd;

		BYTE port; 	
		USHORT Ext0;
		USHORT Ext1;
		USHORT Ext2; 
		
		USHORT* AF_InitParaList;        //AF_InitParaList
		USHORT  AF_InitParaListSize;
		
		USHORT* AF_AutoParaList;
		USHORT  AF_AutoParaListSize;
		
		USHORT* AF_FarParaList;
		USHORT  AF_FarParaListSize;
		
		USHORT* AF_NearParaList;
		USHORT  AF_NearParaListSize;
		
		USHORT* Exposure_ParaList;      //曝光
		USHORT  Exposure_ParaListSize;
		
		USHORT* Gain_ParaList;          //增益
		USHORT	Gain_ParaListSize;
		
		_SensorTab()
		{
			width=0;
			height=0;
			type=0;
			pin=0;
			SlaveID=0;
			mode=0;
			FlagReg=0;
			FlagData=0;
			FlagMask=0;
			FlagReg1=0;
			FlagData1=0;
			FlagMask1=0;
			memset(name,0,NameLen);
			
			ParaList=NULL;
			ParaListSize=0;
			SleepParaList=NULL;
			SleepParaListSize=0;
			
			outformat=0;
			mclk=0;               //0:12M; 1:24M; 2:48M.
			avdd=0;               // 
			dovdd=0;              //
			dvdd=0;

			port=0; 	
			Ext0=0;
			Ext1=0;
			Ext2=0; 
			
			AF_InitParaList=NULL;        //AF_InitParaList
			AF_InitParaListSize=0;
			
			AF_AutoParaList=NULL;
			AF_AutoParaListSize=0;
			
			AF_FarParaList=NULL;
			AF_FarParaListSize=0;
			
			AF_NearParaList=NULL;
			AF_NearParaListSize=0;
			
			Exposure_ParaList=NULL;      //曝光
			Exposure_ParaListSize=0;
			
			Gain_ParaList=NULL;          //增益
			Gain_ParaListSize=0;
		}
	}SensorTab, *pSensorTab;
#endif

#endif
