#ifndef __XML_MSG__
#define __XML_MSG__

/** @ingroup     */
/*@{*/

#include "xml_base.h"
/**
* @brief 存储xml格式字符串buffer最大值
*/
#define MAX_XML_LEN	(100*1024)
/**
* @brief 存储xml格式通道信息buffer的最大值
*/
#define ROOM_INFO_XML_LEN	256
/**
* @brief 存储验证码buffer大小
*/
#define IP_ADDR_LEN	16
#define PASS_KEY_LEN	128
#define ROOM_ID_MAX	10
#define REQ_CODE_TCP_SIZE  6
#define REQ_VALUE_KEY  64

/*###############################################*/

/**
* @brief 判断当前根节点是否是响应消息的根节点
* @param  [IN] proot 当前根节点
* @exception  none
* @return  是返回1 ，否返回0
* @note  none
* @remarks  none
*/
int is_resp_msg(xmlNodePtr  proot);

/**
* @brief 获取响应消息返回码节点
* @param  [OUT] *pnode 响应消息返回码节点
* @param  [IN] proot 响应消息返回码节点dom树根节点
* @exception  none
* @return  成功返回响应消息返回码节点，失败返回NULL
* @note  none
* @remarks  none
* @code
*	#include "xml_msg_management.h"
*	#define MSGHEAD "<?xml version=\"1.0\" encoding=\"UTF-8\"?><ResponseMsg><MsgHead><ReturnCode>1</ReturnCode><ReturnCode>2</ReturnCode><ReturnCode>3</ReturnCode><ReturnCode>4</ReturnCode></MsgHead><MsgBody/></ResponseMsg>"
*	char value[8] = {0};
*	xmlNodePtr pnode;
*	parse_xml_t parse_xml;
*	init_dom_tree(&parse_xml, MSGHEAD);
*	get_resp_return_code_node(&pnode, parse_xml.proot);
*	get_current_node_value(value,value_len, parse_xml.pdoc, pnode);
*	printf("msg_head:[%s]\n", value);
*	release_dom_tree(parse_xml.pdoc);
* @endcode
*/
xmlNodePtr get_resp_return_code_node(xmlNodePtr *pnode, xmlNodePtr proot);

/**
* @brief 判断当前根节点是否是请求消息的根节点
* @param  [IN] proot 当前根节点
* @exception  none
* @return  是返回1 ，否返回0
* @note  none
* @remarks  none
*/
int is_req_msg(xmlNodePtr  proot);

/**
* @brief 获取请求消息码节点
* @param  [OUT] *pnode 请求消息码节点
* @param  [IN] proot 请求消息码节点所在dom树的根节点
* @exception  none
* @return  成功返回消息码节点，失败返回NULL
* @note  none
* @remarks  none
* @see  get_resp_return_code_node
*/
xmlNodePtr get_req_msg_code_node(xmlNodePtr *pnode, xmlNodePtr proot);

/**
* @brief 获取请求消息验证码节点
* @param  [OUT] *pnode 请求消息验证码节点
* @param  [IN] proot 请求消息验证码节点所在dom树的根节点
* @exception  none
* @return  成功返回消息验证码节点，失败返回NULL
* @note  none
* @remarks  none
* @see  get_resp_return_code_node
*/
xmlNodePtr get_req_pass_key_node(xmlNodePtr *pnode, xmlNodePtr proot);


/*@}*/
#endif //__XML_MSG__

