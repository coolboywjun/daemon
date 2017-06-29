#ifndef __BMC_NET_SDK_H__
#define __BMC_NET_SDK_H__

#ifdef   __cplusplus
extern   "C"   {
#endif


#ifndef PACKSTRUCT
#ifdef __GNUC__
#ifdef _WIN32
#define PACKSTRUCT( decl ) decl __attribute__((__packed__,gcc_struct))
#else
#define PACKSTRUCT( decl ) decl __attribute__((__packed__))
#endif

#define ALIGNED __attribute__((aligned(0x4)))
#elif __IAR_SYSTEMS_ICC__

#define PACKSTRUCT( decl ) __packed decl

#define ALIGNED
#elif _MSC_VER  //msvc

#define PACKSTRUCT( decl ) __pragma( pack(push, 1) ) decl __pragma( pack(pop) )
#define ALIGNED
#else 

#define PACKSTRUCT(a) a PACKED 
#endif
#endif


//data type of 32bit os 
typedef signed char int8_;
typedef signed short int int16_;
typedef signed int int32_; //signed long
typedef signed long long int64_;
typedef unsigned char uint8_;
typedef unsigned short int uint16_;
typedef unsigned int uint32_; //unsigned long
typedef unsigned long long uint64_;

#ifndef OFFSET_OF
#define OFFSET_OF(type, member)     ((unsigned long)&((type *)0)->member)
#endif


#define BNPK_VER_MAJOR   0
#define BNPK_VER_MINOR   0
#define BNPK_VER_RELEASE 6
#define BNPK_VER(major,minor,release) (((major&0xff) << 16) + ((minor&0xff) << 8) + (release&0xff))

#define MSG_HEAD_MAGIC "bnpk"
#define MSG_TAIL_MAGIC "endk"

#define MAX_FILE_PATH_LEN 64
#define MAX_FILE_NAME_LEN 32
#define MD5_KEY_LEN 16
#define MAX_KEY_LEN MD5_KEY_LEN
#define MAX_SSID_LEN 32
#define MAX_PWD_LEN 16
#define MAX_DEVICE_NAME_LEN 32

typedef enum
{
	ERR_OK_                  = 0,    
	ERR_FAIL_                = -1,   //一般错误
	ERR_CHK_ERROR_           = -2,   //checksum错误
	ERR_FILE_EXCESS_         = -3,   //超过系统限定的文件传输数量
	ERR_FILE_NOT_FOUND_      = -4,   //未找到指定文件
	ERR_FILE_KEY_NOT_FOUND_  = -5,   //文件key没有找到
	ERR_INVALID_PARAM_       = -6,   //无效的参数
	ERR_POINT_INT_DOWNLOAD_  = -7,   //断点续传
	ERR_DOWNLOADING_         = -8,   //正在下载中...
	ERR_MODE_                = -9,   //模式错误
	ERR_RECORDING            = -10,  //正在录像中...
}error_code_t;


typedef enum
{
	MSG_TYPE_REQ = 1,
	MSG_TYPE_RSP = 2,
	MSG_TYPE_EVT = 3, 
	MSG_TYPE_MAX
}msg_type_t;

typedef enum
{
	CMD_CLASS_DEV_SERACH = 1,
	CMD_CLASS_DEV_CFG    = 2,
	CMD_CLASS_AV_STREAM  = 3, 
	CMD_CLASS_FILE_TRANS = 4,
	CMD_CLASS_SYS_CTRL   = 5,
	CMD_CLASS_DEV_REPORT = 6,
	CMD_CLASS_MAX
}cmd_class_t;

typedef enum
{
	CMD_DEV_CFG_GET   = 1,   //获取所有参数
	CMD_DEV_CFG_SET   = 2,   //设置所有参数
	CMD_COMM_CFG_GET  = 3,   //获取通用参数
	CMD_COMM_CFG_SET  = 4,   //设置通用参数
	CMD_PHOTO_CFG_GET = 5,   //获取photo参数
	CMD_PHOTO_CFG_SET = 6,   //设置photo参数
	CMD_MOVIE_CFG_GET = 7,   //获取movie参数
	CMD_MOVIE_CFG_SET = 8,	 //设置movie参数
	CMD_WIFI_CFG_GET  = 9,   //获取wifi参数
	CMD_WIFI_CFG_SET  = 10,  //设置wifi参数
	CMD_DEV_NAME_GET  = 11,  //获取设备名
	CMD_DEV_NAME_SET  = 12,  //设置设备名
	CMD_DEV_CFG_MAX
}cmd_dev_cfg_t;

typedef enum
{
	CMD_AV_VIDEO_PLAY    = 1,
	CMD_AV_VIDEO_STOP    = 2,
	CMD_AV_VIDEO_FRAME   = 3,
	CMD_AV_STREAM_MAX
}cmd_av_stream_t;


typedef enum
{
	CMD_FILE_UPLOAD            = 1,   //文件上传
	CMD_FILE_DOWNLOAD_START    = 2,   //开始文件下载
	CMD_FILE_DOWNLOAD_PAUSE    = 3,   //暂停文件下载
	CMD_FILE_DOWNLOAD_RESUME   = 4,   //恢复文件下载
	CMD_FILE_DOWNLOAD_STOP     = 5,   //停止文件下载
	CMD_FILE_FRAGMENT          = 6,   //文件数据切片
	CMD_FILE_THUMBNAIL         = 7,   //文件缩略图
	CMD_FILE_LIST              = 8,   //文件列表
	CMD_FILE_DEL               = 9,   //删除文件
	CMD_FILE_MAX
}cmd_file_trans_t;

typedef enum
{
	CMD_SYS_SNAPSHOT           = 1,   //拍照
	CMD_SYS_VIDREC             = 2,   //录像
	CMD_SYS_CHG_MODE           = 3,   //切模式
	CMD_SYS_REBOOT             = 4,   //重启设备
	CMD_SYS_SHUTDOWN           = 5,   //关机
	CMD_SYS_ENTER_HUNTING_MODE = 6,   //进入监控模式
	CMD_SYS_FORMAT_CARD        = 7,   //格式化卡
	CMD_SYS_RESET_DEFAULT      = 8,   //恢复出厂设置
	CMD_SYS_GET_DEV_STATUS     = 9,   //获取设备状态
	CMD_SYS_MAX
}cmd_sys_ctrl_t;

typedef enum
{
	CMD_REPORT_DEV_STATUS = 1,   //设备状态上报
	CMD_REPORT_MAX
}cmd_dev_report_t;


typedef enum
{
	FILE_TYPE_UNKNOW = 0,
	FILE_TYPE_JPG = (1<<0),
	FILE_TYPE_AVI = (1<<1),
	FILE_TYPE_MOV = (1<<2),
	FILE_TYPE_MP4 = (1<<3),
	FILE_TYPE_ALL = 0xff,
}file_type_t;

typedef enum
{
	VIDENC_TYPE_H264  = 0,
	VIDENC_TYPE_MJPEG = 1,
}video_encode_type_t;

typedef enum
{
	FRAME_TYPE_JPG    = 0,
	FRAME_TYPE_H264_I = 1,
	FRAME_TYPE_H264_P = 2,
	FRAME_TYPE_H264_B = 3,
}video_frame_type_t;

//相机模式
typedef enum
{
	CAPTURE_MODE_PHOTO = 0,
	CAPTURE_MODE_MOVIE = 1,
	CAPTURE_MODE_PHOTO_MOVIE = 2,
	CAPTURE_MODE_MAX,
}capture_mode_type_t;

//pir触发灵敏度
typedef enum
{
	SENSOR_LVL_LOW    = 0,
	SENSOR_LVL_NORMAL = 1,
	SENSOR_LVL_HIGH   = 2,	
	SENSOR_LVL_OFF    = 3,
	SENSOR_LVL_MAX,
}sensor_lvl_t;

//pir触发间隔时间
typedef enum 
{
	PIR_INT_0S = 0,
	PIR_INT_5S,
	PIR_INT_10S,
	PIR_INT_15S,
	PIR_INT_20S,
	PIR_INT_25S,
	PIR_INT_30S,
	PIR_INT_35S,
	PIR_INT_40S,
	PIR_INT_45S,
	PIR_INT_50S,
	PIR_INT_55S,
	PIR_INT_1M,
	PIR_INT_2M,
	PIR_INT_3M,
	PIR_INT_4M,
	PIR_INT_5M,
	PIR_INT_10M,
	PIR_INT_15M,
	PIR_INT_20M,
	PIR_INT_25M,
	PIR_INT_30M,
	PIR_INT_35M,
	PIR_INT_40M,
	PIR_INT_45M,
	PIR_INT_50M,
	PIR_INT_55M,
	PIR_INT_60M,
	PIR_INT_ID_MAX
}pir_interval_t;

//定时触发间隔时间
typedef enum 
{			
	TIME_INT_OFF = 0,
	TIME_INT_5M,
	TIME_INT_10M,
	TIME_INT_15M,
	TIME_INT_20M,
	TIME_INT_25M,
	TIME_INT_30M,
	TIME_INT_35M,
	TIME_INT_40M,
	TIME_INT_45M,
	TIME_INT_50M,
	TIME_INT_55M,
	TIME_INT_1H,
	TIME_INT_2H,
	TIME_INT_3H,
	TIME_INT_4H,
	TIME_INT_5H,
	TIME_INT_6H,
	TIME_INT_7H,
	TIME_INT_8H,
	TIME_INT_MAX
}time_interval_t;

//时间格式
typedef enum 
{
    TIME_FMT_24H = 0,
    TIME_FMT_12H,
    BMC_TIME_FMT_MAX
}time_format_type_t;

//图片像素尺寸
typedef enum 
{
    PIC_SIZE_5M = 0,
	PIC_SIZE_8M,
	PIC_SIZE_10M,
	PIC_SIZE_12M,
	PIC_SIZE_14M,
	PIC_SIZE_16M,
	PIC_SIZE_20M,
	PIC_SIZE_30M,
    PIC_SIZE_MAX
}photo_size_t;

//连拍张数
typedef enum
{
	CONT_SHOT__OFF  = 1,
	CONT_SHOT_NUM_2 = 2,
	CONT_SHOT_NUM_3 = 3,
	CONT_SHOT_NUM_4 = 4,
	CONT_SHOT_NUM_5 = 5,
	CONT_SHOT_NUM_6 = 6,
	CONT_SHOT_NUM_7 = 7,
	CONT_SHOT_NUM_8 = 8,
	CONT_SHOT_NUM_9 = 9,
	CONT_SHOT_NUM_10= 10,
	CONT_SHOT_MAX 
}cont_shot_num_t;

//视频分辨率
typedef enum 
{ 	
	VIDEO_VGA = 0,
	VIDEO_720P_30FPS,
	VIDEO_720P_60FPS,
	VIDEO_1080P_30FPS,
	VIDEO_1080P_60FPS,
	VIDEO_2K,
	VIDEO_4K,
	VIDEO_MAX
}video_resolution_t;

//录像长度
typedef enum 
{ 	
	VID_REC_LEN_10S = 1,
	VID_REC_LEN_20S,
	VID_REC_LEN_30S,
	VID_REC_LEN_40S,
	VID_REC_LEN_50S,
	VID_REC_LEN_60S,
	VID_REC_LEN_70S,
	VID_REC_LEN_80S,
	VID_REC_LEN_90S,
	VID_REC_LEN_100S,
	VID_REC_LEN_110S,
	VID_REC_LEN_120S,
	VID_REC_LEN_130S,
	VID_REC_LEN_140S,
	VID_REC_LEN_150S,
	VID_REC_LEN_160S,
	VID_REC_LEN_170S,
	VID_REC_LEN_180S,
	VID_REC_LEN_MAX
}video_record_len_t;

typedef enum
{
	PIC_TYPE_BIG = 0,   //大图  
	PIC_TYPE_SMALL = 1, //预览小图 
}snapshot_pic_type_t;

typedef enum
{
	DEV_TYPE_UNKNOW     = 0,
	DEV_TYPE_HUNT_CAM   = (1<<0),    //打猎相机
	DEV_TYPE_WIFI_AP    = (1<<1),    //wifi AP
	DEV_TYPE_WIFI_STA   = (1<<2),    //wifi STA
	DEV_TYPE_TCP_SERVER = (1<<3),    //tcp server
	DEV_TYPE_TCP_CLIENT = (1<<4),    //tcp client
	DEV_TYPE_CENTER_REC = (1<<5),    //中心存储服务 -- 母机
	DEV_TYPE_MOBILE_APP = (1<<6),    //移动app

/*********************************/
	DEV_TYPE_ALL        = (0xffff)
}dev_type_t;


typedef enum
{
	REPORT_LVL_NORMAL = 0,    //设备正常运行
	REPORT_LVL_CHANGE = 1,    //设备能力发生变化，如电量，卡容量变化
	REPORT_LVL_WARN   = 2,    //设备警告
	REPORT_LVL_ERROR  = 3,    //设备发生错误
	REPORT_LVL_FATAL  = 4,    //设备发生致命错误

/*********************************/
	REPORT_LVL_MAX,
}report_lvt_t;


PACKSTRUCT(typedef struct 
{
	int8_   magic[4];
	uint8_  msg_type;
	uint8_  cmd_class;
	uint16_ cmd_id;
	uint32_ data_len;
	uint32_ version; 
	int8_   reserved[4];   
})msg_head_t;

PACKSTRUCT(typedef struct 
{
	uint32_ check; 
	int8_   magic[4];
})msg_tail_t;

PACKSTRUCT(typedef struct
{
	uint16_ year;
	uint8_ month;
	uint8_ day; 
})date_t;

PACKSTRUCT(typedef struct
{
	uint8_ hour;
	uint8_ min;
	uint8_ sec; 
})time__t;

PACKSTRUCT(typedef struct
{
	date_t date;
	time__t time;
})date_time_t;

PACKSTRUCT(typedef struct
{
	date_time_t create_datetime;
})file_detail_t;


PACKSTRUCT(typedef struct
{
	uint8_ file_type;
	int8_ file_name[MAX_FILE_NAME_LEN];
	int8_ file_path[MAX_FILE_PATH_LEN];
	file_detail_t file_detail;
	uint32_ file_len; 
	uint8_ file_key[MAX_KEY_LEN];	
})file_info_t;

PACKSTRUCT(typedef struct
{
	uint32_ total_fragment_num;  //文件切片总数
	uint32_ fragment_unit_size;  //切片单元大小
	file_info_t file_info;
})file_upload_req_t;

PACKSTRUCT(typedef struct
{
	int16_  result;
	uint8_  file_key[MAX_KEY_LEN]; 
	uint32_ last_fragment_num;   //上次已传输的文件切片索引, 用于断点续传
})file_upload_rsp_t;


PACKSTRUCT(typedef struct
{
	uint8_  file_key[MAX_KEY_LEN];
	uint32_ total_fragment_num;
	uint32_ curr_fragment_num;
	uint32_ fragment_size;
	uint8_  fragment_data[];
})file_fragment_req_t;

PACKSTRUCT(typedef struct
{
	int16_ result;
	uint8_ file_key[MAX_KEY_LEN];
	uint32_ last_fragment_num;  //最后一次所接收到的切片索引
})file_fragment_rsp_t;


PACKSTRUCT(typedef struct
{
	uint8_ reserved[4];
})video_play_req_t;

PACKSTRUCT(typedef struct
{
	int16_ result;
})video_play_rsp_t;

PACKSTRUCT(typedef struct
{
	uint8_  encode_type;
	uint16_ width;
	uint16_ height;
	uint8_  frame_type;
	uint32_ frame_size;
	uint32_ timestamp;
	uint8_  frame_data[];
})video_frame_evt_t;

PACKSTRUCT(typedef struct
{
	uint8_ reserved[4];
})video_stop_req_t;

PACKSTRUCT(typedef struct
{
	int16_ result;
})video_stop_rsp_t;

PACKSTRUCT(typedef struct
{
	int8_ file_path[MAX_FILE_PATH_LEN];  //128 B 文件路径
})file_thumbnail_req_t;

PACKSTRUCT(typedef struct
{
	uint32_ thumbnail_size;        // 4B 缩略图的大小
	uint8_  thumbnail_data[];     // 缩略图数据,  thumbnail_size个字节
})file_thumbnail_t;

PACKSTRUCT(typedef struct
{
	int16_ result;
	file_thumbnail_t thumbnail;  // 缩略图信息
})file_thumbnail_rsp_t;

PACKSTRUCT(typedef struct
{
	uint8_ file_type;    // 1B 文件类型, file_type_t, 可多个类型或操作
	uint8_ get_thumbnail;  // 1B 是否获取缩略图， 0：否， 1：是
	uint8_ reserved[2];  // 2B 保留字段
})file_list_req_t;

PACKSTRUCT(typedef struct
{
	file_info_t file_info;           //文件信息
	file_thumbnail_t thumbnail;    //文件缩略图
})file_info_ex_t;

PACKSTRUCT(typedef struct
{
	int16_ result;        // 2B 响应端的返回值, 根据特定情况使用error_code_t错误码定义
	uint32_ file_num;    // 4B 文件数量
	file_info_ex_t file_info_ex[];  // 大小依文件个数及缩略图大小而定, 文件详细信息列表
})file_list_rsp_t;

PACKSTRUCT(typedef struct
{
	uint8_ file_key[MAX_KEY_LEN];        //首次下载时填0即可，断点续传下载时需要传入之前设备端返回的key
	uint32_ last_fragment_num;           //上次已传输的文件切片索引, 用于断点续传
	int8_ file_path[MAX_FILE_PATH_LEN];  //64 B 文件路径
})file_download_start_req_t;

PACKSTRUCT(typedef struct
{
	int16_ result;        // 2B 响应端的返回值, 根据特定情况使用error_code_t错误码定义
	uint32_ total_fragment_num;  //文件切片总数
	uint32_ fragment_unit_size;  //切片单元大小
	file_info_t file_info;  // 文件信息
})file_download_start_rsp_t;

PACKSTRUCT(typedef struct
{
	uint8_ file_key[MAX_KEY_LEN]; //设备端传过来的key，首次下载填 0, 断点/暂停/恢复/停止下载操作均需要此字段匹配才允许!!!
})file_download_pause_req_t;

typedef file_download_pause_req_t file_download_resume_req_t;
typedef file_download_resume_req_t file_download_stop_req_t;

PACKSTRUCT(typedef struct
{
	int16_ result;        // 2B 响应端的返回值, 根据特定情况使用error_code_t错误码定义
	uint8_ file_key[MAX_KEY_LEN]; 
})file_download_pause_rsp_t;

typedef file_download_pause_rsp_t file_download_resume_rsp_t;
typedef file_download_resume_rsp_t file_download_stop_rsp_t;

PACKSTRUCT(typedef struct
{
	uint8_ capture_mode;  //相机抓拍模式 capture_mode_type_t
	uint8_ sensor_lvl;    //触发灵敏度   sensor_lvl_t
 	uint8_ pir_interval;  //PIR触发间隔  pir_interval_t
	uint8_ timelapse_interval;  //定时触发间隔 time_interval_t
	uint8_ time_format;     //设置设备时间格式 time_format_type_t
	date_time_t date_time;  //时间设置
})common_config_t;

typedef common_config_t set_common_config_req_t;

PACKSTRUCT(typedef struct
{
	int16_ result;        // 2B error_code_t
})set_common_config_rsp_t;

PACKSTRUCT(typedef struct
{
	uint8_ reserved[4];      
})get_common_config_req_t;

PACKSTRUCT(typedef struct
{
	int16_ result;
	common_config_t cfg;
})get_common_config_rsp_t;

PACKSTRUCT(typedef struct
{
	uint8_ photo_size;     //照片尺寸  photo_size_t
	uint8_ cont_shot_num;  //连拍张数  cont_shot_num_t
})photo_config_t;

typedef photo_config_t set_photo_config_req_t;

PACKSTRUCT(typedef struct
{
	int16_ result;        // 2B error_code_t
})set_photo_config_rsp_t;

PACKSTRUCT(typedef struct
{
	uint8_ reserved[4];      
})get_photo_config_req_t;

PACKSTRUCT(typedef struct
{
	int16_ result;
	photo_config_t cfg;
})get_photo_config_rsp_t;

PACKSTRUCT(typedef struct
{
	uint8_ video_resolution;  //视频分辨率 video_resolution_t
	uint8_ video_record_len;   //录像长度，单位:秒  video_record_len_t
})movie_config_t;

typedef movie_config_t set_movie_config_req_t;

PACKSTRUCT(typedef struct
{
	int16_ result;        // 2B error_code_t
})set_movie_config_rsp_t;

PACKSTRUCT(typedef struct
{
	uint8_ reserved[4];      
})get_movie_config_req_t;

PACKSTRUCT(typedef struct
{
	int16_ result;
	movie_config_t cfg;
})get_movie_config_rsp_t;

PACKSTRUCT(typedef struct
{
    uint8_  a[4];  //eg: 192.168.1.2  a[0]:192 a[1]:168 a[2]:1 a[3]:2
})ipv4_t;

PACKSTRUCT(typedef struct
{
	uint8_ ssid[MAX_SSID_LEN];     //ap ssid
	uint8_ pwd[MAX_PWD_LEN];       //ap pwd
	uint8_ ap_is_dest_server;     //ap是否为server 0:no 1:yes
	ipv4_t server_ip;             //server ip address
	uint16_ server_tcp_port;      //server tcp listen port
})wifi_ap_server_info_t;

PACKSTRUCT(typedef struct
{
	wifi_ap_server_info_t mobile_app;     //手机app
	wifi_ap_server_info_t center_record;  //母机
})wifi_config__t;

typedef wifi_config__t set_wifi_config_req_t;

PACKSTRUCT(typedef struct
{
	int16_ result;        // 2B error_code_t
})set_wifi_config_rsp_t;

PACKSTRUCT(typedef struct
{
	uint8_ reserved[4];      
})get_wifi_config_req_t;

PACKSTRUCT(typedef struct
{
	int16_ result;
	wifi_config__t cfg;
})get_wifi_config_rsp_t;

PACKSTRUCT(typedef struct
{
	common_config_t common;
	photo_config_t photo;
	movie_config_t movie;
	wifi_config__t wifi;
})dev_config_t;

typedef dev_config_t set_dev_config_req_t;

PACKSTRUCT(typedef struct
{
	int16_ result;        // 2B error_code_t
})set_dev_config_rsp_t;

PACKSTRUCT(typedef struct
{
	uint8_ reserved[4];      
})get_dev_config_req_t;

PACKSTRUCT(typedef struct
{
	int16_ result;
	dev_config_t cfg;
})get_dev_config_rsp_t;

PACKSTRUCT(typedef struct
{
	uint8_ pic_type; //snapshot_pic_type_t 
	uint8_ get_pic;  //0:不需要回传图片  1:需要回传图片
	uint8_ reserved[2];      
})snapshot_req_t;

PACKSTRUCT(typedef struct
{
	int16_ result;    
	uint8_  file_type; //回传图片类型
	uint16_ width;     //回传图片宽
	uint16_ height;    //回传图片高
	uint32_ len;       //回传图片大小
	uint8_  data[];    //回传图片数据
})snapshot_rsp_t;

PACKSTRUCT(typedef struct
{
	uint8_ start;   // 1:start record  0:stop record
	uint8_ reserved[3];      
})video_record_req_t;

PACKSTRUCT(typedef struct
{
	int16_ result;   
})video_record_rsp_t;

PACKSTRUCT(typedef struct
{
	uint8_ capture_mode;   // capture_mode_type_t
	uint8_ reserved[3];      
})change_mode_req_t;

PACKSTRUCT(typedef struct
{
	int16_ result;   
})change_mode_rsp_t;

PACKSTRUCT(typedef struct
{
	uint8_ reserved[4];      
})common_ctrl_req_t;

PACKSTRUCT(typedef struct
{
	int16_ result;   
})common_ctrl_rsp_t;

typedef common_ctrl_req_t system_reboot_req_t;
typedef common_ctrl_rsp_t system_reboot_rsp_t;
typedef common_ctrl_req_t system_shutdown_req_t;
typedef common_ctrl_rsp_t system_shutdown_rsp_t;
typedef common_ctrl_req_t enter_hunting_mode_req_t;
typedef common_ctrl_rsp_t enter_hunting_mode_rsp_t;
typedef common_ctrl_req_t format_card_req_t;
typedef common_ctrl_rsp_t format_card_rsp_t;
typedef common_ctrl_req_t reset_default_req_t;
typedef common_ctrl_rsp_t reset_default_rsp_t;

typedef common_ctrl_req_t get_dev_name_req_t;
PACKSTRUCT(typedef struct
{
	int16_ result;
	uint8_ name[MAX_DEVICE_NAME_LEN];      
})get_dev_name_rsp_t;

PACKSTRUCT(typedef struct
{
	uint8_ name[MAX_DEVICE_NAME_LEN];      
})set_dev_name_req_t;

typedef get_dev_name_rsp_t set_dev_name_rsp_t;

PACKSTRUCT(typedef struct
{
	uint16_ dev_type;     //设备类型 dev_type_t, 可使用或操作
	uint8_ name[MAX_DEVICE_NAME_LEN];  //设备名
	uint8_ capture_mode; //当前工作模式
	uint8_ battery_lvl;  //电量
	uint8_ card_status;  //卡状态
	uint32_ free_space;  //剩余空间大小 单位:MB
	uint32_ total_space; //总空间大小 单位:MB
	union
	{
		uint32_ photo_num;  //photo模式下可拍照张数
		uint32_ movie_record_len; //movie模式下可录像秒数
		uint32_ val;
	}remain;
})dev_status_t;

typedef common_ctrl_req_t get_dev_status_req_t;

PACKSTRUCT(typedef struct
{
	int16_ result;
	dev_status_t status;  
})get_dev_status_rsp_t;

PACKSTRUCT(typedef struct
{
	uint8_ report_lvl;   //上报等级 report_lvt_t
	dev_status_t status;  
})dev_status_evt_t;

PACKSTRUCT(typedef struct
{
	int8_ file_path[MAX_FILE_PATH_LEN]; 
})file_path_t;

PACKSTRUCT(typedef struct
{
	uint8_ delete_all;  //删除所有 0:否 ;  1:是, 需要指定 file_type
	union
	{
		uint8_ file_type; //指定删除某类型文件，delete_all为 1 时有效, 无需指定文件名
		struct
		{
			uint32_ file_num;
			file_path_t file[]; 
		}part;
	}del;
})delete_file_req_t;


PACKSTRUCT(typedef struct
{
	int16_ result;
	int8_ file_path[MAX_FILE_PATH_LEN]; 
})delete_file_result_t;

PACKSTRUCT(typedef struct
{
	uint8_ delete_all;  //delete_file_req_t中指定的删除方式
	union
	{
		int16_ result;
		struct
		{
			uint32_ file_num;
			delete_file_result_t delete_result[]; 
		}part;
	}del;
})delete_file_rsp_t;


typedef uint32_ (*_htonl_)(uint32_ hostlong);
typedef uint16_ (*_htons_)(uint16_ hostshort);
typedef uint32_ (*_ntohl_)(uint32_ netlong);
typedef uint16_ (*_ntohs_)(uint16_ netshort);

typedef struct
{
	_htonl_ hnl;
	_htons_ hns;
	_ntohl_ nhl; 
	_ntohs_ nhs;
}net_callback_t;


/* you must call this function firstly for net sdk callback init */
int net_sdk_init(net_callback_t *cb);

/*do hton only, you need set field of head first*/
int net_htn_head(msg_head_t *head);
/*do hton only, you need set field of tail first*/
int net_htn_tail(msg_tail_t *tail);
/*do nth only*/
int net_nth_head(msg_head_t *head);
/*do nth only*/
int net_nth_tail(msg_tail_t *tail);

/* calu buf checksum */
uint32_ net_check_sum(uint8_ *buf, uint32_ len);

/* check data valid or not
   return code:
   		0: check ok
   	   -1: check fail, data maybe is error
*/
int net_check_data_valid(uint32_ dst_check_val, uint8_ *buf, uint32_ len);

/* check msg valid or not
   return code:
   		0: check ok
   	   -1: check fail, data maybe is error
*/
int net_check_msg_valid(uint32_ dst_check_val, uint8_ *msg, uint32_ msg_len);

/* get sdk version
   param[in]: get_string
              0: 获取整数version, 可用于版本较验  
              1:获取string形式version, 调用者需要强转到char*, 可用于print
   return code:
   		always ok.
*/
uint32_ net_get_sdk_version(uint8_ get_string);


/* you wno't need set field of head/tail, these functions set field of head/tail and do htn, 
   but you must set field of req/rsp/evt before call it, it will do htn for req/rsp/evt.
   return code:
   		0: ok
   	   -1: fail
 */
int net_pack_file_upload_req(msg_head_t *head, file_upload_req_t *req, msg_tail_t *tail);
int net_pack_file_upload_rsp(msg_head_t *head, file_upload_rsp_t *rsp, msg_tail_t *tail);
int net_pack_file_fragment_req(msg_head_t *head, file_fragment_req_t *req, msg_tail_t *tail);
int net_pack_file_fragment_rsp(msg_head_t *head, file_fragment_rsp_t *rsp, msg_tail_t *tail);
int net_pack_video_play_req(msg_head_t *head, video_play_req_t *req, msg_tail_t *tail);
int net_pack_video_play_rsp(msg_head_t *head, video_play_rsp_t *rsp, msg_tail_t *tail);
int net_pack_video_frame_evt(msg_head_t *head, video_frame_evt_t *evt, msg_tail_t *tail);
int net_pack_video_stop_req(msg_head_t *head, video_stop_req_t *req, msg_tail_t *tail);
int net_pack_video_stop_rsp(msg_head_t *head, video_stop_rsp_t *rsp, msg_tail_t *tail);
int net_pack_file_thumbnail_req(msg_head_t *head, file_thumbnail_req_t *req, msg_tail_t *tail);
int net_pack_file_thumbnail_rsp(msg_head_t *head, file_thumbnail_rsp_t *rsp, msg_tail_t *tail);
int net_pack_file_list_req(msg_head_t *head, file_list_req_t *req, msg_tail_t *tail);
int net_pack_file_list_rsp(msg_head_t *head, file_list_rsp_t *rsp, msg_tail_t *tail);
int net_pack_file_download_start_req(msg_head_t *head, file_download_start_req_t *req, msg_tail_t *tail);
int net_pack_file_download_start_rsp(msg_head_t *head, file_download_start_rsp_t *rsp, msg_tail_t *tail);
int net_pack_file_download_pause_req(msg_head_t *head, file_download_pause_req_t *req, msg_tail_t *tail);
int net_pack_file_download_pause_rsp(msg_head_t *head, file_download_pause_rsp_t *rsp, msg_tail_t *tail);
int net_pack_file_download_resume_req(msg_head_t *head, file_download_resume_req_t *req, msg_tail_t *tail);
int net_pack_file_download_resume_rsp(msg_head_t *head, file_download_resume_rsp_t *rsp, msg_tail_t *tail);
int net_pack_file_download_stop_req(msg_head_t *head, file_download_stop_req_t *req, msg_tail_t *tail);
int net_pack_file_download_stop_rsp(msg_head_t *head, file_download_stop_rsp_t *rsp, msg_tail_t *tail);
int net_pack_set_dev_config_req(msg_head_t *head, set_dev_config_req_t *req, msg_tail_t *tail);
int net_pack_set_dev_config_rsp(msg_head_t *head, set_dev_config_rsp_t *rsp, msg_tail_t *tail);
int net_pack_set_common_config_req(msg_head_t *head, set_common_config_req_t *req, msg_tail_t *tail);
int net_pack_set_common_config_rsp(msg_head_t *head, set_common_config_rsp_t *rsp, msg_tail_t *tail);
int net_pack_set_photo_config_req(msg_head_t *head, set_photo_config_req_t *req, msg_tail_t *tail);
int net_pack_set_photo_config_rsp(msg_head_t *head, set_photo_config_rsp_t *rsp, msg_tail_t *tail);
int net_pack_set_movie_config_req(msg_head_t *head, set_movie_config_req_t *req, msg_tail_t *tail);
int net_pack_set_movie_config_rsp(msg_head_t *head, set_movie_config_rsp_t *rsp, msg_tail_t *tail);
int net_pack_set_wifi_config_req(msg_head_t *head, set_wifi_config_req_t *req, msg_tail_t *tail);
int net_pack_set_wifi_config_rsp(msg_head_t *head, set_wifi_config_rsp_t *rsp, msg_tail_t *tail);
int net_pack_get_dev_config_req(msg_head_t *head, get_dev_config_req_t *req, msg_tail_t *tail);
int net_pack_get_dev_config_rsp(msg_head_t *head, get_dev_config_rsp_t *rsp, msg_tail_t *tail);
int net_pack_get_common_config_req(msg_head_t *head, get_common_config_req_t *req, msg_tail_t *tail);
int net_pack_get_common_config_rsp(msg_head_t *head, get_common_config_rsp_t *rsp, msg_tail_t *tail);
int net_pack_get_photo_config_req(msg_head_t *head, get_photo_config_req_t *req, msg_tail_t *tail);
int net_pack_get_photo_config_rsp(msg_head_t *head, get_photo_config_rsp_t *rsp, msg_tail_t *tail);
int net_pack_get_movie_config_req(msg_head_t *head, get_movie_config_req_t *req, msg_tail_t *tail);
int net_pack_get_movie_config_rsp(msg_head_t *head, get_movie_config_rsp_t *rsp, msg_tail_t *tail);
int net_pack_get_wifi_config_req(msg_head_t *head, get_wifi_config_req_t *req, msg_tail_t *tail);
int net_pack_get_wifi_config_rsp(msg_head_t *head, get_wifi_config_rsp_t *rsp, msg_tail_t *tail);
int net_pack_snapshot_req(msg_head_t *head, snapshot_req_t *req, msg_tail_t *tail);
int net_pack_snapshot_rsp(msg_head_t *head, snapshot_rsp_t *rsp, msg_tail_t *tail);
int net_pack_video_record_req(msg_head_t *head, video_record_req_t *req, msg_tail_t *tail);
int net_pack_video_record_rsp(msg_head_t *head, video_record_rsp_t *rsp, msg_tail_t *tail);
int net_pack_change_mode_req(msg_head_t *head, change_mode_req_t *req, msg_tail_t *tail);
int net_pack_change_mode_rsp(msg_head_t *head, change_mode_rsp_t *rsp, msg_tail_t *tail);
int net_pack_system_reboot_req(msg_head_t *head, system_reboot_req_t *req, msg_tail_t *tail);
int net_pack_system_reboot_rsp(msg_head_t *head, system_reboot_rsp_t *rsp, msg_tail_t *tail);
int net_pack_system_shutdown_req(msg_head_t *head, system_shutdown_req_t *req, msg_tail_t *tail);
int net_pack_system_shutdown_rsp(msg_head_t *head, system_shutdown_rsp_t *rsp, msg_tail_t *tail);
int net_pack_enter_hunting_mode_req(msg_head_t *head, enter_hunting_mode_req_t *req, msg_tail_t *tail);
int net_pack_enter_hunting_mode_rsp(msg_head_t *head, enter_hunting_mode_rsp_t *rsp, msg_tail_t *tail);
int net_pack_format_card_req(msg_head_t *head, format_card_req_t *req, msg_tail_t *tail);
int net_pack_format_card_rsp(msg_head_t *head, format_card_rsp_t *rsp, msg_tail_t *tail);
int net_pack_reset_default_req(msg_head_t *head, reset_default_req_t *req, msg_tail_t *tail);
int net_pack_reset_default_rsp(msg_head_t *head, reset_default_rsp_t *rsp, msg_tail_t *tail);
int net_pack_get_dev_name_req(msg_head_t *head, get_dev_name_req_t *req, msg_tail_t *tail);
int net_pack_get_dev_name_rsp(msg_head_t *head, get_dev_name_rsp_t *rsp, msg_tail_t *tail);
int net_pack_set_dev_name_req(msg_head_t *head, set_dev_name_req_t *req, msg_tail_t *tail);
int net_pack_set_dev_name_rsp(msg_head_t *head, set_dev_name_rsp_t *rsp, msg_tail_t *tail);
int net_pack_get_dev_status_req(msg_head_t *head, get_dev_status_req_t *req, msg_tail_t *tail);
int net_pack_get_dev_status_rsp(msg_head_t *head, get_dev_status_rsp_t *rsp, msg_tail_t *tail);
int net_pack_dev_status_evt(msg_head_t *head, dev_status_evt_t *evt, msg_tail_t *tail);
int net_pack_delete_file_req(msg_head_t *head, delete_file_req_t *req, msg_tail_t *tail);
int net_pack_delete_file_rsp(msg_head_t *head, delete_file_rsp_t *rsp, msg_tail_t *tail);



/* nth head & req/rsp/evt & tail 
   return code:
   		0: ok
   	   -1: fail
 */
int net_nth_file_upload_req(msg_head_t *head, file_upload_req_t *req, msg_tail_t *tail);
int net_nth_file_upload_rsp(msg_head_t *head, file_upload_rsp_t *rsp, msg_tail_t *tail);
int net_nth_file_fragment_req(msg_head_t *head, file_fragment_req_t *req, msg_tail_t *tail);
int net_nth_file_fragment_rsp(msg_head_t *head, file_fragment_rsp_t *rsp, msg_tail_t *tail);
int net_nth_video_play_req(msg_head_t *head, video_play_req_t *req, msg_tail_t *tail);
int net_nth_video_play_rsp(msg_head_t *head, video_play_rsp_t *rsp, msg_tail_t *tail);
int net_nth_video_frame_evt(msg_head_t *head, video_frame_evt_t *evt, msg_tail_t *tail);
int net_nth_video_stop_req(msg_head_t *head, video_stop_req_t *req, msg_tail_t *tail);
int net_nth_video_stop_rsp(msg_head_t *head, video_stop_rsp_t *rsp, msg_tail_t *tail);
int net_nth_file_thumbnail_req(msg_head_t *head, file_thumbnail_req_t *req, msg_tail_t *tail);
int net_nth_file_thumbnail_rsp(msg_head_t *head, file_thumbnail_rsp_t *rsp, msg_tail_t *tail);
int net_nth_file_list_req(msg_head_t *head, file_list_req_t *req, msg_tail_t *tail);
int net_nth_file_list_rsp(msg_head_t *head, file_list_rsp_t *rsp, msg_tail_t *tail);
int net_nth_file_download_start_req(msg_head_t *head, file_download_start_req_t *req, msg_tail_t *tail);
int net_nth_file_download_start_rsp(msg_head_t *head, file_download_start_rsp_t *rsp, msg_tail_t *tail);
int net_nth_file_download_pause_req(msg_head_t *head, file_download_pause_req_t *req, msg_tail_t *tail);
int net_nth_file_download_pause_rsp(msg_head_t *head, file_download_pause_rsp_t *rsp, msg_tail_t *tail);
int net_nth_file_download_resume_req(msg_head_t *head, file_download_resume_req_t *req, msg_tail_t *tail);
int net_nth_file_download_resume_rsp(msg_head_t *head, file_download_resume_rsp_t *rsp, msg_tail_t *tail);
int net_nth_file_download_stop_req(msg_head_t *head, file_download_stop_req_t *req, msg_tail_t *tail);
int net_nth_file_download_stop_rsp(msg_head_t *head, file_download_stop_rsp_t *rsp, msg_tail_t *tail);
int net_nth_set_dev_config_req(msg_head_t *head, set_dev_config_req_t *req, msg_tail_t *tail);
int net_nth_set_dev_config_rsp(msg_head_t *head, set_dev_config_rsp_t *rsp, msg_tail_t *tail);
int net_nth_set_common_config_req(msg_head_t *head, set_common_config_req_t *req, msg_tail_t *tail);
int net_nth_set_common_config_rsp(msg_head_t *head, set_common_config_rsp_t *rsp, msg_tail_t *tail);
int net_nth_set_photo_config_req(msg_head_t *head, set_photo_config_req_t *req, msg_tail_t *tail);
int net_nth_set_photo_config_rsp(msg_head_t *head, set_photo_config_rsp_t *rsp, msg_tail_t *tail);
int net_nth_set_movie_config_req(msg_head_t *head, set_movie_config_req_t *req, msg_tail_t *tail);
int net_nth_set_movie_config_rsp(msg_head_t *head, set_movie_config_rsp_t *rsp, msg_tail_t *tail);
int net_nth_set_wifi_config_req(msg_head_t *head, set_wifi_config_req_t *req, msg_tail_t *tail);
int net_nth_set_wifi_config_rsp(msg_head_t *head, set_wifi_config_rsp_t *rsp, msg_tail_t *tail);
int net_nth_get_dev_config_req(msg_head_t *head, get_dev_config_req_t *req, msg_tail_t *tail);
int net_nth_get_dev_config_rsp(msg_head_t *head, get_dev_config_rsp_t *rsp, msg_tail_t *tail);
int net_nth_get_common_config_req(msg_head_t *head, get_common_config_req_t *req, msg_tail_t *tail);
int net_nth_get_common_config_rsp(msg_head_t *head, get_common_config_rsp_t *rsp, msg_tail_t *tail);
int net_nth_get_photo_config_req(msg_head_t *head, get_photo_config_req_t *req, msg_tail_t *tail);
int net_nth_get_photo_config_rsp(msg_head_t *head, get_photo_config_rsp_t *rsp, msg_tail_t *tail);
int net_nth_get_movie_config_req(msg_head_t *head, get_movie_config_req_t *req, msg_tail_t *tail);
int net_nth_get_movie_config_rsp(msg_head_t *head, get_movie_config_rsp_t *rsp, msg_tail_t *tail);
int net_nth_get_wifi_config_req(msg_head_t *head, get_wifi_config_req_t *req, msg_tail_t *tail);
int net_nth_get_wifi_config_rsp(msg_head_t *head, get_wifi_config_rsp_t *rsp, msg_tail_t *tail);
int net_nth_snapshot_req(msg_head_t *head, snapshot_req_t *req, msg_tail_t *tail);
int net_nth_snapshot_rsp(msg_head_t *head, snapshot_rsp_t *rsp, msg_tail_t *tail);
int net_nth_video_record_req(msg_head_t *head, video_record_req_t *req, msg_tail_t *tail);
int net_nth_video_record_rsp(msg_head_t *head, video_record_rsp_t *rsp, msg_tail_t *tail);
int net_nth_change_mode_req(msg_head_t *head, change_mode_req_t *req, msg_tail_t *tail);
int net_nth_change_mode_rsp(msg_head_t *head, change_mode_rsp_t *rsp, msg_tail_t *tail);
int net_nth_system_reboot_req(msg_head_t *head, system_reboot_req_t *req, msg_tail_t *tail);
int net_nth_system_reboot_rsp(msg_head_t *head, system_reboot_rsp_t *rsp, msg_tail_t *tail);
int net_nth_system_shutdown_req(msg_head_t *head, system_shutdown_req_t *req, msg_tail_t *tail);
int net_nth_system_shutdown_rsp(msg_head_t *head, system_shutdown_rsp_t *rsp, msg_tail_t *tail);
int net_nth_enter_hunting_mode_req(msg_head_t *head, enter_hunting_mode_req_t *req, msg_tail_t *tail);
int net_nth_enter_hunting_mode_rsp(msg_head_t *head, enter_hunting_mode_rsp_t *rsp, msg_tail_t *tail);
int net_nth_format_card_req(msg_head_t *head, format_card_req_t *req, msg_tail_t *tail);
int net_nth_format_card_rsp(msg_head_t *head, format_card_rsp_t *rsp, msg_tail_t *tail);
int net_nth_reset_default_req(msg_head_t *head, reset_default_req_t *req, msg_tail_t *tail);
int net_nth_reset_default_rsp(msg_head_t *head, reset_default_rsp_t *rsp, msg_tail_t *tail);
int net_nth_get_dev_name_req(msg_head_t *head, get_dev_name_req_t *req, msg_tail_t *tail);
int net_nth_get_dev_name_rsp(msg_head_t *head, get_dev_name_rsp_t *rsp, msg_tail_t *tail);
int net_nth_set_dev_name_req(msg_head_t *head, set_dev_name_req_t *req, msg_tail_t *tail);
int net_nth_set_dev_name_rsp(msg_head_t *head, set_dev_name_rsp_t *rsp, msg_tail_t *tail);
int net_nth_get_dev_status_req(msg_head_t *head, get_dev_status_req_t *req, msg_tail_t *tail);
int net_nth_get_dev_status_rsp(msg_head_t *head, get_dev_status_rsp_t *rsp, msg_tail_t *tail);
int net_nth_dev_status_evt(msg_head_t *head, dev_status_evt_t *evt, msg_tail_t *tail);
int net_nth_delete_file_req(msg_head_t *head, delete_file_req_t *req, msg_tail_t *tail);
int net_nth_delete_file_rsp(msg_head_t *head, delete_file_rsp_t *rsp, msg_tail_t *tail);


#ifdef   __cplusplus
}
#endif

#endif

