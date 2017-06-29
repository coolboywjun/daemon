/********************************************************
  *           file:      bmc_daemon.c                   
  *          brief:      bmc daemon              
  *         author:      wjun
  *     Created on:      2017/04/6
  *
  ********************************************************/
#include "bmc_daemon.h"

DaemonHand *pDaemeonOper = NULL;

static void DaemonLock()
{
	pthread_mutex_lock(&pDaemeonOper->daemonlock);
}

static void DaemonunLock()
{
	pthread_mutex_unlock(&pDaemeonOper->daemonlock);
}

static int DaemonLockInit()
{
	return pthread_mutex_init(&pDaemeonOper->daemonlock, NULL);
}

static int DaemondeInit()
{
	return pthread_mutex_destroy(&pDaemeonOper->daemonlock);
}

void InitDaemon( void )
{
	int i;
	pid_t pid;

	//第一步
	if((pid=fork())) 
	{
		exit(0);//是父进程,结束父进程
	} 
	else
	{
	if( pid< 0 ) 
	 exit(1);//fork失败,退出
	}  

	//第二步
	setsid();//是第一个子进程,后台继续运行
	//第一个子进程成为新的会话组长和进程组长,并与控制终端分离

	//防止子进程结束时产生僵尸进程
	#if 0
	signal(SIGPIPE,SIG_IGN);
	signal(SIGHUP,SIG_IGN);
	signal(SIGTERM,SIG_IGN);
	signal(SIGINT,SIG_IGN);
	signal(SIGCHLD,SIG_IGN);
	#endif
	//第三步
	if((pid=fork()))//禁止进程重新打开控制终端 
	{
	exit(0);//是第一个子进程,结束第一个子进程
	} 
	else
	{
	if( pid<0 ) 
	exit(1);//fork失败,退出
	}  

	//第四步
	for(i=0; i<NOFILE; i++)
	{
		//文件描述符暂不关闭 解决继承出错问题
		//	 close(i);//关闭从父进程那里继承的一些已经打开的文件
	}
	//第五步
	chdir("/usr/local/bin");//改变工作目录

	//第六步
	umask(0);//重设文件权限掩码  
}	

uint32_ FindPidByName(NodeAtt* pmodeattr,uint32_ *pidList)
{
	DIR *dir;
	struct dirent *next;
	uint32_ i=0;

	if(pidList == NULL)
	{
		return 0;
	}
	
	///proc中包括当前的进程信息,读取该目录
	dir = opendir("/proc");
	if (!dir)
	{
		return 0;
	}
	
	//遍历
	while ((next = readdir(dir)) != NULL)
	{
		FILE *status;
		int8_ filename[READ_BUF_SIZE];
		int8_ buffer[READ_BUF_SIZE];
		int8_ name[READ_BUF_SIZE];
		int8_ param[READ_BUF_SIZE];
		
		if (strcmp((int8_*)next->d_name, (int8_*)"..") == 0)
			continue;
			
		if (!isdigit(*next->d_name))
			continue;
			
		sprintf((char *)filename, "/proc/%s/cmdline", next->d_name);
		if (! (status = fopen((char *)filename, "r")) )
		{
			continue;
		}

		memset(buffer, 0x0,READ_BUF_SIZE);
		if (fgets((char *)buffer, READ_BUF_SIZE-1, status) == NULL)
		{
			fclose(status);
			continue;
		}
		fclose(status);
		int8_ *p = buffer;


		//获取进程名字
		sscanf((char *)buffer, "%s", (char *)name);
		
		p = (int8_*)strchr((char *)buffer,0);
		
		p = p+1;

		if(*p != 0)
		{
			//获取运行参数
			sscanf((char *)p,"%s",(char *)param);
		}
		else
		{
			strcpy(param,(int8_*)"");
		}
		
		if ( (strcmp(name, (int8_*)pmodeattr->procname) == 0) 
			 &&(strcmp(param, (int8_*)pmodeattr->param) == 0))
		{
			//printf("find-->[%s] [%s] [%s] [%s] %d\n",buffer,param,pmodeattr->procname,pmodeattr->param,strtol(next->d_name, NULL, 0));
			//printf("%s %d\n",buffer,strtol(next->d_name, NULL, 0));
			//pidList= realloc( pidList, sizeof(long) * (i+2));
			pidList[i++] = strtol(next->d_name, NULL, 0);
		}
	}
	closedir(dir);
	pidList[i]=0;
	return i;
}

uint32_ CheckProgramNum(uint32_ *list)
{
	uint32_ num = 0;
	if(list == NULL)
	{
		return num;
	}
	
	while(*list)
	{
		num++;
		list++;
	}
	return num;
}

char *skip_ws(const char *p)
{
    while (isspace(*p)) p++;
    return (char *)p;
}
    
char *skip_token(const char *p)
{
    while (isspace(*p)) p++;
    while (*p && !isspace(*p)) p++;
    return (char *)p;
}

uint32_ GetSysCpuTime()
{
	char buffer[PROCBUFFERLEN];
	uint32_ cp_time = 0;
    int32_t fd, len;
    char *p;
	fd = open((int8_*)"/proc/stat", (int32_t)O_RDONLY);
	if(fd == -1)
	{
		return -1;
	}

	
	len = read(fd, buffer, sizeof(buffer)-1);
	if(len <= 0)
	{
		close(fd);
		return -1;
	}
	
	close(fd);
	
	buffer[len] = '\0';
	
	p = skip_token(buffer);			/* "cpu" */
	cp_time  = strtoul(p, &p, 10);
	cp_time  += strtoul(p, &p, 10);
	cp_time  += strtoul(p, &p, 10);
	cp_time  += strtoul(p, &p, 10);
	cp_time  += strtoul(p, &p, 10);
	cp_time  += strtoul(p, &p, 10);
	cp_time  += strtoul(p, &p, 10);
	
	return cp_time;
   
}

uint32_ GetSysMemPrent()
{
	char  buffer[PROCBUFFERLEN];
	int totalmem = 1;
	int MemFree = 0;
	int Buffers = 0;
	int Cached = 0;
    int fd, len;
    char *p;

	fd = open("/proc/meminfo", O_RDONLY);
	if(fd == -1)
	{
		return -1;
	}
	
	len = read(fd, buffer, sizeof(buffer)-1);
	if(len <= 0)
	{
		close(fd);
		return -1;
	}
	close(fd);
	buffer[len] = '\0';

	p = buffer;
	p = skip_token(p);
	totalmem = strtoul(p, &p, 10); /* total memory */

	p = strchr(p, '\n');
	p = skip_token(p);
	MemFree = strtoul(p, &p, 10); /* MemFree memory */

	p = strchr(p, '\n');
	p = skip_token(p);
	Buffers = strtoul(p, &p, 10); /* Buffers memory */

	p = strchr(p, '\n');
	p = skip_token(p);
	Cached = strtoul(p, &p, 10); /* Cached memory */

	//BMC_INFO_LOG("totalmem[%d] MemFree[%d] Buffers[%d]  Cached[%d]\n",totalmem,MemFree,Buffers,Cached);
	
	return (totalmem - MemFree - Buffers - Cached)*100/totalmem;
}

int32_t GetProcStat(pid_t pid, ProState *state)
{
    char buffer[PROCBUFFERLEN], *p;
	int fd, len;

	sprintf(buffer, "/proc/%d/stat", pid);

	fd = open(buffer, O_RDONLY);
	if(fd == -1)
	{
		return -1;
	}
	
	len = read(fd, buffer, sizeof(buffer)-1);
	close(fd);
	
	buffer[len] = '\0';
  
    p = buffer;
    p = strchr(p, '(')+1;			/* skip pid */
    {
		char *q = strrchr(p, ')');
		int len = q-p;
		if (len >= sizeof(state->name))
		    len = sizeof(state->name)-1;
		memcpy(state->name, p, len);
		state->name[len] = 0;
		p = q+1;
    }

    p = skip_ws(p);
    switch (*p++)
    {
      case 'R': state->State = 1; break;
      case 'S': state->State = 2; break;
      case 'D': state->State = 3; break;
      case 'Z': state->State = 4; break;
      case 'T': state->State = 5; break;
      case 'W': state->State = 6; break;
    }

    p = skip_token(p);				/* skip ppid */
    p = skip_token(p);				/* skip pgrp */
    p = skip_token(p);				/* skip session */
    p = skip_token(p);				/* skip tty */
    p = skip_token(p);				/* skip tty pgrp */
    p = skip_token(p);				/* skip flags */
    p = skip_token(p);				/* skip min flt */
    p = skip_token(p);				/* skip cmin flt */
    p = skip_token(p);				/* skip maj flt */
    p = skip_token(p);				/* skip cmaj flt */
  
    state->CpuTime  = strtoul(p, &p, 10);		/* utime */
    state->CpuTime += strtoul(p, &p, 10);		/* stime */
    p = skip_token(p);				/* skip cutime */
    p = skip_token(p);				/* skip cstime */

    p = skip_token(p);		/* priority */
    p = skip_token(p);		/* nice */

    p = skip_token(p);				/* skip timeout */
    p = skip_token(p);				/* skip it_real_val */
    p = skip_token(p);				/* skip start_time */

    p = skip_token(p);
    state->Mem = pagetok(strtoul(p, &p, 10));	/* rss */

	state->CpuTime = state->CpuTime*CPUNUM;
	if(state->CpuTime <= 0)
		state->CpuTime = 0;
	return 1;
}

int32_t GetCpuOccupancy(uint32_ ToalTime1,uint32_ ToalTime0,uint32_ ProTime1,uint32_ ProTime0)
{

	int32_t prent =  0;
	if((ToalTime1 <= 0) || (ToalTime0 <= 0) || ProTime1 <= 0 || ProTime0 <= 0)
	{
		return 0;
	}

	//printf("time %d  %d\n",ToalTime1-ToalTime0,ProTime1-ProTime0);
	prent = ((ProTime1 - ProTime0)*100)/(ToalTime1 - ToalTime0);//(102*CPUNUM);//(ToalTime1 - ToalTime0);
	if((prent < 0) || (prent > 101*CPUNUM))
	{
		return -1;
	}
	return prent;
}

static int32_t CreateXmlProc(xmlNodePtr pnode,Daemon_config *pinfo)
{
	if(NULL == pnode || pinfo == NULL){
		return -1;
	}

	xml_add_new_child(pnode, NULL, BAD_CAST "name", (xmlChar *)pinfo->name);
	xml_add_new_child(pnode, NULL, BAD_CAST "param", (xmlChar *)pinfo->param);
	xml_add_new_child(pnode, NULL, BAD_CAST "active", (xmlChar *)pinfo->active);
	xml_add_new_child(pnode, NULL, BAD_CAST "guardian", (xmlChar *)pinfo->guardian);
	xml_add_new_child(pnode, NULL, BAD_CAST "maxcpu", (xmlChar *)pinfo->maxcpu);
	xml_add_new_child(pnode, NULL, BAD_CAST "maxmem", (xmlChar *)pinfo->maxmem);
	xml_add_new_child(pnode, NULL, BAD_CAST "time", (xmlChar *)pinfo->time);
	xml_add_new_child(pnode, NULL, BAD_CAST "run", (xmlChar *)pinfo->run);
	return 0;
}

static int32_t CreateXmlConfig()
{
	//以下是所监控的进程
	parse_xml_t px;
	xmlNodePtr proc;
	Daemon_config config = {{0}};
	int ret = -1;
	int i = 0;
	
	px.pdoc = xmlNewDoc(BAD_CAST"1.0"); 			//定义文档和节点指针
	if(NULL == px.pdoc)
	{
		BMC_ERROR_LOG("xmlNewDoc failed, file: .UoloadConfig\n");
		return ret;
	}
	
	px.proot= xmlNewNode(NULL, BAD_CAST"Daemon");
	xmlDocSetRootElement(px.pdoc, px.proot);		//设置根节点

	strcpy(config.active,(int8_*)"1");
	strcpy(config.guardian,(int8_*)"1");
	strcpy(config.maxcpu, (int8_*)"300");
	strcpy(config.maxmem, (int8_*)"4000000");
	strcpy(config.time,  (int8_*) "60");

	proc = xmlNewNode(NULL, BAD_CAST "proc");
	xmlAddChild(px.proot, proc);
	strcpy(config.name,(int8_*)"/usr/local/bin/wlan_service");
	strcpy(config.param,(int8_*)"");
	CreateXmlProc(proc, &config);


	proc = xmlNewNode(NULL, BAD_CAST "proc");
	xmlAddChild(px.proot, proc);
	strcpy(config.name,(int8_*)"/usr/local/bin/video");
	strcpy(config.param,(int8_*)"");
	CreateXmlProc(proc, &config);

	proc = xmlNewNode(NULL, BAD_CAST "proc");
	xmlAddChild(px.proot, proc);
	strcpy(config.name,(int8_*)"/tmp/bmc_server");//TODO
	strcpy(config.param,(int8_*)"");
	CreateXmlProc(proc, &config);

	ret = xmlSaveFormatFileEnc(XML, px.pdoc, "UTF-8", 1);
	if(-1 == ret)
	{
		BMC_ERROR_LOG("xml save params table failed !!!\n");
	}
	else
	{
		ret = 1;
	}

	release_dom_tree(px.pdoc);
	xmlCleanupParser();

	return ret;
}

int32_t AnalysisXML(DaemonHand *pDaemeonOper)
{
	xmlNodePtr FileNode = NULL;
	xmlNodePtr NodeAttr = NULL;
	xmlChar    *pattr  = NULL;
	uint32_ num = 0;
	uint32_ roomnum = 0;
	if(pDaemeonOper == NULL)
	{
		return -1;
	}

	FileNode = pDaemeonOper->px->proot->children;
	
	BMC_INFO_LOG("|             name             |param| maxmem |maxcpu|    run   | guardian |  time | num |");
	while(FileNode != NULL)
	{
		num++;
		NodeAtt *pnodeatt = NULL;
		pnodeatt = (NodeAtt *)malloc(sizeof(NodeAtt)); 
		if(pnodeatt == NULL)
		{
			return -1;
		}
		
		memset(pnodeatt, 0x0, sizeof(NodeAtt));
		
		NodeAttr = get_children_node(FileNode, BAD_CAST "name");
		if(NodeAttr == NULL)
		{
			free(pnodeatt);
			return -1;
		}
		pattr    = xmlNodeListGetString(pDaemeonOper->px->pdoc, NodeAttr->xmlChildrenNode, 1);

		if(pattr == NULL)
		{
			strcpy(pnodeatt->param,(int8_*)"");
		}
		else
		{
			strcpy(pnodeatt->procname,(int8_*)pattr);
			xmlFree(pattr);
			pattr = NULL;
		}
		
		NodeAttr = get_children_node(FileNode, BAD_CAST "param");
		if(NodeAttr == NULL)
		{
			free(pnodeatt);
			return -1;
		}
		pattr    = xmlNodeListGetString(pDaemeonOper->px->pdoc, NodeAttr->xmlChildrenNode, 1);	
		if(pattr == NULL)
		{
			strcpy(pnodeatt->param,(int8_*)"");
		}
		else
		{
			strcpy(pnodeatt->param,(int8_*)pattr);
			xmlFree(pattr);
			pattr = NULL;
		}
		

		NodeAttr = get_children_node(FileNode, BAD_CAST "active");
		if(NodeAttr == NULL)
		{
			free(pnodeatt);
			return -1;
		}
		pattr    = xmlNodeListGetString(pDaemeonOper->px->pdoc, NodeAttr->xmlChildrenNode, 1);	
		if(pattr == NULL)
		{
			pnodeatt->active = 0;
		}
		else
		{
			pnodeatt->active = 1;
		
			xmlFree(pattr);
			pattr = NULL;
		}

		NodeAttr = get_children_node(FileNode, BAD_CAST "guardian");
		if(NodeAttr == NULL)
		{
			free(pnodeatt);
			return -1;
		}
		pattr    = xmlNodeListGetString(pDaemeonOper->px->pdoc, NodeAttr->xmlChildrenNode, 1);	

		if(pattr == NULL)
		{
			pnodeatt->guardian = 0;
		}
		else
		{
			pnodeatt->guardian = atoi((char*)pattr);
			xmlFree(pattr);
			pattr = NULL;
		}
		
		NodeAttr = get_children_node(FileNode, BAD_CAST "maxcpu");
		if(NodeAttr == NULL)
		{
			free(pnodeatt);
			return -1;
		}
		pattr    = xmlNodeListGetString(pDaemeonOper->px->pdoc, NodeAttr->xmlChildrenNode, 1);

		if(pattr == NULL)
		{
			pnodeatt->max_cpu = 100;
		}
		else
		{
			pnodeatt->max_cpu = atoi((char*)pattr);
			xmlFree(pattr);
			pattr = NULL;
		}
	
		NodeAttr = get_children_node(FileNode, BAD_CAST "maxmem");
		if(NodeAttr == NULL)
		{
			free(pnodeatt);
			return -1;
		}
		pattr    = xmlNodeListGetString(pDaemeonOper->px->pdoc, NodeAttr->xmlChildrenNode, 1);	
		if(pattr == NULL)
		{
			pnodeatt->max_mem = 0;
		}
		else
		{
			pnodeatt->max_mem= atoi((char*)pattr);
			xmlFree(pattr);
			pattr = NULL;
		}

		NodeAttr = get_children_node(FileNode, BAD_CAST "time");
		if(NodeAttr == NULL)
		{
			free(pnodeatt);
			return -1;
		}
		pattr    = xmlNodeListGetString(pDaemeonOper->px->pdoc, NodeAttr->xmlChildrenNode, 1);	
		if(pattr == NULL)
		{
			pnodeatt->time = 5;
		}
		else
		{
			pnodeatt->time= atoi((char*)pattr);
			xmlFree(pattr);
			pattr = NULL;
		}
		
		NodeAttr = get_children_node(FileNode, BAD_CAST "run");
		if(NodeAttr == NULL)
		{
			free(pnodeatt);
			return -1;
		}
		pattr    = xmlNodeListGetString(pDaemeonOper->px->pdoc, NodeAttr->xmlChildrenNode, 1);	
		if(pattr == NULL)
		{
			strcpy(pnodeatt->run,(int8_*)"");
		}
		else
		{
			strcpy(pnodeatt->run,(int8_*)pattr);
			xmlFree(pattr);
			pattr = NULL;
		}

		if(pnodeatt->active == 1)
		{
		
			list_add_before(&pnodeatt->stlist, pDaemeonOper->phead);

			BMC_INFO_LOG("|%30s|%5s|%8d|%6d|%10s|%10d|%7d|%5d|",pnodeatt->procname,\
			pnodeatt->param,pnodeatt->max_mem,pnodeatt->max_cpu,pnodeatt->run,pnodeatt->guardian,pnodeatt->time,num);
		}
		else
		{
			free(pnodeatt);
		}
		FileNode = FileNode->next;
	}
	
	if(pDaemeonOper->px->pdoc != NULL)
	{
		release_dom_tree(pDaemeonOper->px->pdoc);
		xmlCleanupParser();
		pDaemeonOper->px->pdoc = NULL;
	}


	if(pDaemeonOper->px != NULL)
	{
		free(pDaemeonOper->px);
		pDaemeonOper->px = NULL;
	}
	return 1;
}

int32_t DamemonInit(DaemonHand **ppDaemeonOper)
{

	int32_t rc;
	int8_ *pnum = NULL;

	if(ppDaemeonOper == NULL)
	{
		return -1;
	}
	
	DaemonHand	*pDaemeonOper = (DaemonHand *)malloc(sizeof(DaemonHand));
	if(NULL == pDaemeonOper)
	{
		return -1;
	}

	memset(pDaemeonOper, 0x0, sizeof(DaemonHand));
	*ppDaemeonOper = pDaemeonOper;	
	free(pnum);

	parse_xml_t *px = (parse_xml_t*)malloc(sizeof(parse_xml_t));
	if(NULL == px)
	{
		BMC_ERROR_LOG("ServiceInit: malloc parse_xml_t fail");
		return -1;
	}

	/* 初始化xml */
	memset(px,0x0,sizeof(parse_xml_t));
	char* ret = init_file_dom_tree(px, XML);
	if(ret == NULL)
	{
		BMC_ERROR_LOG("init_file_dom_tree failed1, xml file: UploadConfig.xml");

		//创建配置文件
		if(1 != CreateXmlConfig())
		{
			return -1;
		}

		char* ret = init_file_dom_tree(px, XML);
		if(ret == NULL)
		{
			BMC_ERROR_LOG("init_file_dom_tree failed2, xml file: UploadConfig.xml");
			return -1;
		}
	}

	list_head *pheadnode = (list_head *)malloc(sizeof(list_head));
	if(NULL == pheadnode)
	{
		BMC_ERROR_LOG("list_head malloc failed");
		return -1;
	}
	
	pDaemeonOper->phead = pheadnode;
	list_init(pDaemeonOper->phead);
	
	pDaemeonOper->px = px;

	DaemonLockInit();
	return 1;
}

int32_t deDamemonInit(DaemonHand *pDaemeonOper)
{
	
	if(pDaemeonOper != NULL)
	{
		if(pDaemeonOper->px != NULL)
		{
			if(pDaemeonOper->px->pdoc != NULL)
			{
				release_dom_tree(pDaemeonOper->px->pdoc);
				pDaemeonOper->px->pdoc = NULL;
			}

			free(pDaemeonOper->px);
			pDaemeonOper->px = NULL;
		}

		if(pDaemeonOper->phead != NULL)
		{
			NodeAtt *pmodeattr = NULL;
			list_head *pcurnode = NULL;
			/* 查找是否存在该设备 */
			list_for_each_item(pcurnode, pDaemeonOper->phead)  
		    {  
		        if(NULL != pcurnode)  
		        {  
		            pmodeattr = list_entry(pcurnode, NodeAtt, stlist);  
		         
					list_remove(pcurnode);
					free(pmodeattr);
					pmodeattr = NULL;
					
		        }  
		    }  
			
			free(pDaemeonOper->phead);
			pDaemeonOper->phead = NULL;
		}
	
		free(pDaemeonOper);
		pDaemeonOper = NULL;
	}

	DaemondeInit();

	xmlCleanupParser();

	return 1;
}

int32_t StartApp(NodeAtt *pdaemon)
{
	pid_t childpid;
	char cmd[1024] = {0};

	if((childpid = fork())== -1) //fork( )命令
	{
		
	}
	else if(childpid == 0) // This is child process!
	{
		pid_t pid;
		int ret  = 0;
		if ((pid = fork()) < 0)     
		{      
			BMC_ERROR_LOG("[%s %s] Fork error! [%s]",pdaemon->procname,pdaemon->param,strerror(errno)); 
	 		exit(-1);     
	    }     
	    else if (pid > 0)    
	    {
	        exit(0); 
	    }
		
		setsid();
		umask(0);
		chdir("/usr/local/bin");
		//重新拉起进程
		sprintf(cmd,"%s %s",(char *)pdaemon->procname,(char *)pdaemon->param);
		ret = execl("/bin/sh", "sh", "-c", cmd, (char *)0);
		if(ret == -1)
		{
			BMC_ERROR_LOG("[%s %s] execl fail! [%s]",pdaemon->procname,pdaemon->param,strerror(errno)); 
		}
		exit(0);
	}
	else //This is  parent process! 
	{
		
	}

	if (waitpid(childpid, NULL, 0) != childpid) /* wait for first child */    
	{     
		BMC_ERROR_LOG("[%s %s] Waitpid error!",pdaemon->procname,pdaemon->param);       
	}    
	return 1;
}

int32_t CheckPid(NodeAtt *pdaemon)
{
	int32_t i = 0;
	DaemonLock();
	
	//检测进程异常
	if((pdaemon->livestate == 0) && (pdaemon->guardian == 1) && (pdaemon->samenum == 0))
	{
		//拉起进程
		StartApp(pdaemon);
		pdaemon->livestate++;
		BMC_ERROR_LOG("StartApp name[%s %s]",pdaemon->procname,pdaemon->param);
	}
	
	for(i = 0; i < pdaemon->samenum; i++)
	{
		
		if(pdaemon->rttotaltimes[i] >= pdaemon->time)
		{	
			uint32_ prent = pdaemon->rttotalcpu[i]/pdaemon->rttotaltimes[i];
			uint32_ mem   = pdaemon->rttotalmem[i]/pdaemon->rttotaltimes[i];
			uint32_ memprent = GetSysMemPrent();
			
			BMC_ERROR_LOG("pid[%d] name[%s %s] live[%d] guardian[%d]"\
				" cpu[%d] mem[%u] time[%d] rttime[%d] prent[%d] mem[%u] usemem[%d%%]",\
				pdaemon->rtpid[i],pdaemon->procname,pdaemon->param,pdaemon->livestate,pdaemon->guardian,\
				pdaemon->rtprent[i],pdaemon->rtmem[i],pdaemon->time,pdaemon->rttotaltimes[i],prent,mem,memprent);
			
			//检测内存和cpu状态
			if( ( prent  >= pdaemon->max_cpu)
				|| 
				( ( mem  >= pdaemon->max_mem) && (memprent >= 90) && (memprent <= 100))
		  	  )
			{
				BMC_ERROR_LOG("Kill pid[%10d] name[%30s %5s] maxmem[%u] maxcpu[%d] rtmem[%u] rtcpu[%d] use[%d%%]",
				pdaemon->rtpid[i],pdaemon->procname,pdaemon->param,pdaemon->max_mem,pdaemon->max_cpu,mem,prent,memprent);

				//杀死进程						
				if(0 == kill(pdaemon->rtpid[i], SIGKILL))
				{
					pdaemon->rtpid[i]   = 0;
					pdaemon->rtmem[i]   = 0;
					pdaemon->rtprent[i] = 0;

					//拉起进程
					StartApp(pdaemon);
					BMC_ERROR_LOG("StartApp name[%s %s]",pdaemon->procname,pdaemon->param);
				}
				else
				{
					BMC_ERROR_LOG("Don't kill pid[%10d] name[%30s] param[%5s]",pdaemon->rtpid[i],pdaemon->procname,pdaemon->param);
				}
			}

			pdaemon->rttotaltimes[i] = 0;
			pdaemon->rttotalcpu[i] = 0;
			pdaemon->rttotalmem[i] = 0;
		}
			
	}

	 DaemonunLock();
	 return 1;
}

static void *DaemonDealTask(void *args)
{

	DaemonHand	*pdaemon = (DaemonHand *)args;
	if(NULL == pdaemon)
	{
		return NULL;
	}

	while(1)
	{
		NodeAtt *pmodeattr = NULL;
		list_head *pcurnode = NULL;

		sleep(5);
		/* 查找是否存在该设备 */
		list_for_each_item(pcurnode, pdaemon->phead)  
	    {  
	        if(NULL != pcurnode)  
	        {
				pmodeattr = list_entry(pcurnode, NodeAtt, stlist);
				CheckPid(pmodeattr);
				
	        }
		}
	}
}
/*==============================================================================
    函数: main
    功能: 守护进程,对app的一些重要的进程进行守护,比如video进程/wlan_service进程等.
    	  当这些进程突然挂掉之后由守护进程将之重新拉起来.
    参数: 
    返回值: 
==============================================================================*/
int main(int argc, char *argv[])   
{
	//子进程会继续继承父进程文件描述符
	InitDaemon();

	pthread_t thid;
	int ret = 0;

	//优先获取cpu
	setpriority(PRIO_PROCESS, 0, -20);

	//初始化
	if(-1 == DamemonInit(&pDaemeonOper))
	{
		BMC_ERROR_LOG("DamemonInit is faile!!!\n");
		goto FAIL;
	}

	//解析配置文件
	if(-1 == AnalysisXML(pDaemeonOper))
	{
		BMC_ERROR_LOG("AnalysisXML is faile!!!\n");
		goto FAIL;
	}

	BMC_INFO_LOG("DamemonInit Done ... Version[%s]",VERSION);

	//处理任务
	ret = pthread_create(&thid, NULL, DaemonDealTask, (void *)pDaemeonOper);
	if(ret)
	{
		BMC_ERROR_LOG("DealTask failed, err msg: %s\n", strerror(errno));
		goto FAIL;
	}

	//统计实时cpu mem
	while(1)
	{
		NodeAtt *pmodeattr = NULL;
		list_head *pcurnode = NULL;

		/* 查找是否存在该设备 */
		list_for_each_item(pcurnode, pDaemeonOper->phead)  
	    {  
	        if(NULL != pcurnode)  
	        {  
				uint32_ i = 0;
				uint32_ pidlist[1024] = {0};
				
	            pmodeattr = list_entry(pcurnode, NodeAtt, stlist);  
				/* 是否激活此任务监视 */
				if(pmodeattr->active == 1)
				{
					uint32_ num = 0;
					DaemonLock();

					//寻找进程
					num = FindPidByName(pmodeattr,pidlist);
					if(0 == num)
					{
						//初始参数
						pmodeattr->samenum = 0;
						pmodeattr->livestate = 0;
						memset(pmodeattr->rtcputime, 0x0, sizeof(uint32_)*MAXSAMEPROC*7);
						DaemonunLock();
						BMC_INFO_LOG("FindPidByName Don't [%s %s] pid",pmodeattr->procname, pmodeattr->param);
						continue;
					}

					//同名 同参进程个数
					pmodeattr->samenum = num;
					pmodeattr->livestate = num;
					
					if(pmodeattr->active == 1)
					{
						for(i = 0; i < num ; i++)
						{	
							ProState state = {{0}};
							int32_t ret = 0;
							pmodeattr->rtpid[i] = pidlist[i];
							ret =  GetProcStat((pid_t)pidlist[i], &state);	
							if(ret == -1)
							{	
								pmodeattr->rtmem[i]        = 0;
								pmodeattr->rtcputime[i]    = 0;
								pmodeattr->rtsyscputime[i] = 0;
								BMC_INFO_LOG("GetProcStat is fail: pid[%d] name[%s]",pidlist[i],pmodeattr->procname);
							}
							else
							{
								int32_t prent = 0;
								uint32_ curcputime = GetSysCpuTime();							
								prent = GetCpuOccupancy(curcputime,pmodeattr->rtsyscputime[i],state.CpuTime, pmodeattr->rtcputime[i]);
								if(-1 == prent)
								{
									BMC_INFO_LOG("GetCpuOccupancy is error pid[%d] name[%s]",pidlist[i],pmodeattr->procname);
									prent = 0;
								}
						
								pmodeattr->rtprent[i] = prent;
								BMC_INFO_LOG("name[%s %s] cpu[%d] mem[%d]",pmodeattr->procname,pmodeattr->param,prent,pmodeattr->rtmem[i]);			
								pmodeattr->rtsyscputime[i] = curcputime;
								pmodeattr->rtcputime[i]    = state.CpuTime;
								pmodeattr->rtmem[i] 	   = state.Mem;
								pmodeattr->rttotalcpu[i]  += prent;
								pmodeattr->rttotalmem[i]  += state.Mem;
  								pmodeattr->rttotaltimes[i]++;
							}
							
						}
					}
					DaemonunLock();
				}
			}  
	    }  
		sleep(1);		
	}	
FAIL:
	BMC_INFO_LOG("deDamemonInit Done ...\n");
	deDamemonInit(pDaemeonOper);
	return 0;
}
