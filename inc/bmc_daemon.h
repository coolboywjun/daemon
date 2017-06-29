#ifndef _BMC_DAEMON_H_
#define _BMC_DAEMON_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <dirent.h>
#include <ctype.h>
#include <errno.h>
#include <sys/param.h>
#include <sys/procfs.h>
#include <sys/resource.h>
#include "xml_base.h"
#include <assert.h>
#include "bmc_net_protocol_sdk.h"
#include "bmc_debug.h"
#include <fcntl.h> 

#define XML  "/tmp/daemon.xml"

#define VERSION  "Bmc_Daemon_v1.0"
#define CPUNUM   2
#define READ_BUF_SIZE 1024
#define PAGE_SHIFT 12
#define bytetok(x)	(((x) + 512) >> 10)
#define pagetok(x)	((x) << (PAGE_SHIFT - 10))
#define PROCBUFFERLEN   4*1024
#define MAXSAMEPROC   10

#define RED				"\033[0;32;31m"
#define NONE				"\033[m"

typedef struct _list_head
{
   struct _list_head 	*next;
   struct _list_head	*prev;

}list_head;

typedef struct _DevAtt
{
	int8_ procname[200];   //进程名
	int8_ 	  param[200];	//进程参数
	uint32_  max_mem;	    //最大内存
	uint32_  max_cpu;      //最大cpu
	uint32_  active;       //是否激活
	uint32_  guardian;     //是否守护
	uint32_  time;         //检测的时间段
	int8_ 	  run[20];      //运行参数 咱无用

	list_head stlist;
	uint32_ livestate;		//存活状态
	uint32_ rtcputime[MAXSAMEPROC];       //实时线程cpu时间
	uint32_ rtsyscputime[MAXSAMEPROC];	   //实时系统cpu时间
	uint32_ rtmem[MAXSAMEPROC];           //实时内存
	uint32_ rtpid[MAXSAMEPROC];		   //线程ID
	uint32_ rtprent[MAXSAMEPROC];         //线程cpu百分比
	uint32_ rttotalcpu[MAXSAMEPROC];	//cpu总百分比累计
	uint32_ rttotalmem[MAXSAMEPROC];	//内存累计
	int32_t  rttotaltimes[MAXSAMEPROC];	//统计次数
	uint32_ samenum;
}NodeAtt;

typedef struct _system_info
{

    uint32_    TotalMem;
    uint32_    TotalCpu;
}system_info;

typedef struct _progress_state
{

	int8_     name[20];
    uint32_   Mem;
    uint32_   CpuTime;
	uint32_   State;
}ProState;

typedef struct _daemon_hand
{
	parse_xml_t *px;
	list_head *phead;
	pthread_mutex_t daemonlock;
}DaemonHand;

typedef struct _daemon_config
{
	int8_ name[200];
	int8_ param[20];
	int8_ run[200];
	int8_ active[20];
	int8_ guardian[20];
	int8_ maxcpu[20];
	int8_ maxmem[20];
	int8_ time[20];
}Daemon_config;

/* 初始化双向链表 */
#define list_init(head) do			\
{						\
	(head)->next = (head)->prev = (head);	\
} while(0)

/* 在指定元素(where)之后插入新元素(item) */
#define list_add(item, towhere) do	\
{					\
	(item)->next = (towhere)->next;	\
	(item)->prev = (towhere);	\
	(towhere)->next = (item);	\
	(item)->next->prev = (item);	\
} while(0)

/* 在指定元素(where)之前插入新元素(item) */
#define list_add_before(item, towhere)  \
	list_add(item,(towhere)->prev)

/* 删除某个元素 */
#define list_remove(item) do			\
{						\
	(item)->prev->next = (item)->next;	\
	(item)->next->prev = (item)->prev;	\
} while(0)

/* 正向遍历链表中所有元素 */
#define list_for_each_item(item, head)\
	for ((item) = (head)->next; (item) != (head); (item) = (item)->next)

/* 反向遍历链表中所有元素 */
#define list_for_each_item_rev(item, head) \
	for ((item) = (head)->prev; (item) != (head); (item) = (item)->prev)

/* 根据本节点(item)获取节点所在结构体(type)的地址 */
/* 节点item地址(member的地址) - 该链表元素member在结构体中的偏移 */
#define list_entry(item, type, member) \
	((type *)((char *)item - (char *)(&((type *)0)->member)))

/* 判断链表是否为空 */
#define list_is_empty(head)	\
	((head)->next == (head))

/* 获取指定位置上一元素 */
#define list_prev_item(item)\
	((head)->prev)
	
#endif
