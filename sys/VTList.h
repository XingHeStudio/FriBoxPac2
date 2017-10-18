#ifndef VT_LIST_H
#define VT_LIST_H

#define VTLIST_ENTRY(type) \
    type *next; type *prev

#define VTLIST_INIT_VALUE {0}

#define VTListInit(list) \
    (list)->next = (list); \
    (list)->prev = (list);

#define VTListFirst(list) ((list)->next == (list) ? NULL : (list)->next)

#define VTListLast(list) ((list)->prev == (list) ? NULL : (list)->prev)

#define __VTLIST_DOINIT(list) if(!((list)->next != NULL)){VTListInit(list);}

#define VTListAdd(list, node) \
    __VTLIST_DOINIT(list) \
    node->next = (list); \
    (list)->prev->next = node; \
    node->prev = (list)->prev; \
    (list)->prev = node;

#define VTListDel(node) \
if((node)->next != NULL) \
{ \
    (node)->prev->next = (node)->next; \
    (node)->next->prev = (node)->prev; \
    (node)->next = NULL; \
}

#define _VT_LIST_TMP_NODE(node, line) node##line
#define VT_LIST_TMP_NODE(node, line) _VT_LIST_TMP_NODE(node, line)

//VT_LIST_TMP_NODE 用于记录next节点，用于保证遍历过程中Del节点不会导致错误
#if 1
#define VTListForeach(list, node) \
    __VTLIST_DOINIT(list) \
    void *VT_LIST_TMP_NODE(node, __LINE__); \
for(node = (list)->next, VT_LIST_TMP_NODE(node, __LINE__) = node->next; \
    node != (list); node = VT_LIST_TMP_NODE(node, __LINE__), VT_LIST_TMP_NODE(node, __LINE__) = node->next)
#else
#define VTListForeach(list, node) \
for(node = (list)->next; node != (list); node = node->next)
#endif

#endif // VT_LIST_H
