#ifndef _BMC_DEBUG_H_
#define _BMC_DEBUG_H_

#include <stdio.h>

#define BMC_DEBUG

#ifdef BMC_DEBUG
#define BMC_INFO_LOG(fmt, args...)		\
	fprintf(stderr,"\33[33m[%s : %s : %d] "fmt"\33[37m\n",__FILE__, __func__, __LINE__, ##args)
#define BMC_ERROR_LOG(fmt, args...)		\
	fprintf(stderr,"\33[31m[%s : %s : %d] "fmt"\33[37m\n",__FILE__, __func__, __LINE__, ##args)
#define BMC_WARN_LOG(fmt, args...)		\
	fprintf(stderr,"\33[31m[%s : %s : %d] "fmt"\33[37m\n",__FILE__, __func__, __LINE__, ##args)
#else
#define BMC_INFO_LOG(fmt, args...)
#define BMC_ERROR_LOG(fmt, args...)
#define BMC_WARN_LOG(fmt, args...)
#endif

#endif
