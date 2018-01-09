#ifndef MP4_LIST_H
#define MP4_LIST_H

typedef struct list_node {
    struct list_node *next;
	int ref;
} listnode;

#define list_node_ref(N) {(N)->ref++;}
#define list_node_unref(N) {if((N)->ref > 0) (N)->ref--;}
#define list_node_is_ref(N) ((N)->ref > 0)

#define list_init(N)  {(N)->next = NULL;(N)->ref = 0;}
#define list_del(N) {((N)->next = NULL);list_node_unref(N);}

static inline struct list_node * list_last(struct list_node *node)
{
	struct list_node * first = node;
	struct list_node * prev = NULL;
	while(node != NULL)
	{
		prev = node;
		node=node->next;
		if(first == node || node == NULL)
		{
			node = prev;
			break;
		}
	}
	return node;
}

#define list_add_after(L,N) {(N)->next = NULL;(L)->next=(N);list_node_ref(N);}


#define member(ptr,element) (int)((char*)&(ptr)->element - (char*)(ptr))

#define base_ptr(node,ptr,element)  (node==NULL)?NULL:( (char*)(node) - member(ptr,element) )

#define member_ptr(ptr,element)  (ptr)==NULL?NULL:( (char*)(ptr) + member(ptr,element) )

#define list_walk_entry_forward(L,I,E) for(I= base_ptr( (L)->next , I , E ) ; I ; I = base_ptr( ((I)->E).next , I , E ) )

#define list_walk_entry_forward_safe(L,I,T,E) for(I= base_ptr( (L)->next , I , E ), T = (I != NULL) ? ((char*)((I)->E.next)) : NULL ; (I) ; I = base_ptr(  T , I , E  ) ,T = (I != NULL) ? ((char*)((I)->E.next)) : NULL )

#endif
