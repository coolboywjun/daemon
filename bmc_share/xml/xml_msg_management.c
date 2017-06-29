#include <stdlib.h>
#include "xml_msg_management.h"

#define RESP_ROOT_KEY (BAD_CAST "ResponseMsg")

#define REQ_ROOT_KEY (BAD_CAST "RequestMsg")

/*######################xml ½âÎö###########################*/

xmlNodePtr get_resp_return_code_node(xmlNodePtr *pnode, xmlNodePtr proot)
{
	xmlNodePtr  tmp_node = NULL;
	tmp_node = get_children_node(proot, BAD_CAST "MsgHead");
	*pnode = get_children_node(tmp_node, BAD_CAST "ReturnCode");
	return *pnode;
}

int is_req_msg(xmlNodePtr  proot)
{
	if(proot == NULL) {
		return 0;
	}

	if(xmlStrcmp(proot->name, REQ_ROOT_KEY) != 0) {
		return 0;
	}

	return 1;
}
int is_resp_msg(xmlNodePtr  proot)
{
	if(proot == NULL) {
		return 0;
	}

	if(xmlStrcmp(proot->name, RESP_ROOT_KEY) != 0) {
		return 0;
	}

	return 1;
}

xmlNodePtr get_req_msg_code_node(xmlNodePtr *pnode, xmlNodePtr proot)
{
	xmlNodePtr  tmp_node = NULL;
	tmp_node = get_children_node(proot, BAD_CAST "MsgHead");
	*pnode = get_children_node(tmp_node, BAD_CAST "MsgCode");
	return *pnode;
}

xmlNodePtr get_req_pass_key_node(xmlNodePtr *pnode, xmlNodePtr proot)
{
	xmlNodePtr  tmp_node = NULL;
	tmp_node = get_children_node(proot, BAD_CAST "MsgHead");
	*pnode = get_children_node(tmp_node, BAD_CAST "PassKey");
	return *pnode;
}

