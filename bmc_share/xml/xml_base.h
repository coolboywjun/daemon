
#ifndef __XML_BASE__
#define __XML_BASE__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parser.h"
#include "tree.h"
/**
*@brief dom树的信息
*/
typedef struct parse_xmls {
	/** @dom树句柄*/
	xmlDocPtr pdoc;	
	/** @dom树的根节点*/
	xmlNodePtr  proot;	
} parse_xml_t;

/**
* @brief 将xml格式字符串生成dom树及获取其根节点
* @param  [OUT] parse_xml 生成dom树的信息
* @param  [IN] xml	xml字符串
* @exception  none  
* @return  none
* @note  none
* @remarks  none
*/
void *init_dom_tree(parse_xml_t *parse_xml, const char *xml);

/**
* @brief 将文件名为file_name的xml文件生成dom树及获取其根节点
* @param  [OUT] parse_xml 生成dom树的信息
* @param  [IN] file_name	xml文件名
* @exception  none  
* @return  none
* @note  none
* @remarks  none
*/
void *init_file_dom_tree(parse_xml_t *parse_xml, const char *file_name);

/**
* @brief 释放清理dom树
* @param  [OUT] pdoc dom树句柄
* @exception  none
* @return  none
* @note  none
* @remarks  none
*/
void release_dom_tree(xmlDocPtr pdoc);

/**
* @brief 获取子节点
* @param  [IN] curNode 当前节点
* @param  [IN] 子节点名
* @exception  none  
* @return  子节点
* @note  none
* @remarks  none
*/
xmlNodePtr get_children_node(xmlNodePtr curNode, const xmlChar * key);

/**
* @brief 获取当前节点的值
* @param  [OUT] value 存储获取的节点的值buffer
* @param  [OUT] pdoc dom树句柄
* @param  [IN] curNode 当前要获取值的节点
* @exception  none  
* @return  成功返回0 ，失败返回-1
* @note  none
* @remarks  none
*/
int32_t get_current_node_value(char *value, int32_t value_len, xmlDocPtr pdoc,  xmlNodePtr 
curNode);

/**
* @brief 获取当前同名节点个数
* @param  [IN]  curNode 当前节点
* @exception  none
* @return  成功返回当前同名节点个数
* @note  none
* @remarks  none
*/
int get_current_samename_node_nums(xmlNodePtr curNode);

/**
* @brief 增加新的节点
* @param  [IN]  parent 要增加节点的父节点
* @param  [IN]	name	要增加节点的名称
* @param  [IN]  content 增加节点的参数
* @exception  none
* @return  返回新增节点
* @note  none
* @remarks  none
*/
xmlNodePtr	add_new_node(xmlNodePtr parent,const xmlChar *name,const xmlChar *content);

/**
* @brief 修改节点的值
* @param  [IN]  cur 要修改的节点
* @param  [IN]	content	要修改节点的值
* @exception  none
* @return  返回0成功
* @note  none
* @remarks  none
*/
int	modify_node_value(xmlNodePtr cur,const xmlChar *content);

/**
* @brief 删除节点
* @param  [IN]  pnode 要删除的节点
* @exception  none
* @return  返回0成功
* @note  none
* @remarks  none
*/
xmlNodePtr del_cur_node(xmlNodePtr pnode);

/**
* @brief 设置节点属性
* @param  [IN]  node 要修改属性的节点
* @param  [IN]  name 节点中的属性
* @param  [IN]  value 属性的值
* @exception  none
* @return  返回xmlAttrPtr类型的结构体
* @note  none
* @remarks  none
*/
xmlAttrPtr xml_prop_set(xmlNodePtr node,const xmlChar *name,const xmlChar *value);

/**
* @brief删除节点属性
* @param  [IN]  curNode 要删除属性的节点
* @param  [IN]  attr_name 节点中的属性
* @exception  none
* @return  返回0
* @note  none
* @remarks  none
*/
int xml_prop_del(xmlNodePtr curNode, const xmlChar *attr_name);

/**
* @brief 添加新的节点
* @param  [IN]  parent 要增加节点的父节点
* @param  [IN]  ns 暂时传空
* @param  [IN]  name 增加节点的名字
* @param  [IN]  content 增加节点的值,传NULL时创建类型为<cctv />
* @exception  none
* @return  返回节点指针
* @note  none
* @remarks  none
*/
xmlNodePtr xml_add_new_child(xmlNodePtr parent, xmlNsPtr ns, 
	const xmlChar *name, const xmlChar *content);

/**
* @brief 设置节点值
* @param  [IN]  cur 要修改值的节点
* @param  [IN]  content 节点值
* @param  [IN]  len 节点值的长度
* @exception  none
* @return  返回节点指针
* @note  none
* @remarks  none
*/
int	xml_node_value_set(xmlNodePtr cur,const xmlChar *content,int len);

xmlNodePtr find_next_node(xmlNodePtr cur_node, const xmlChar *node_name);



#endif //__XML_BASE__

