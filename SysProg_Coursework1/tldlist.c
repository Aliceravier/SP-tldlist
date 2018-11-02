#include "tldlist.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>

int add_node(TLDNode* root_node, char* hostname, TLDList * tld);
void node_destroy(TLDNode * node);
bool date_between(Date * date, Date * start_date, Date * end_date);
char *strlwr(char *str);
TLDNode *find_next_node(TLDNode* node);
void rebalance(TLDNode* node, TLDList * tld);
TLDNode * rotate_left(TLDNode * node);
TLDNode * rotate_right(TLDNode * node);
TLDNode * rotate_left_then_right(TLDNode * node);
TLDNode * rotate_right_then_left(TLDNode * node);
void re_height(TLDNode * node);
void set_balance(TLDNode * node);
int max(int int1, int int2);
int height(TLDNode *node);
TLDNode * create_node(char* tld_value, int nb_tlds, int height, int balance, TLDNode * left_node, TLDNode * right_node, TLDNode * parent);

//TODO go through gdb check what rebalance is doing using paper
//TODO fix valgrind uninitialised values?


struct tldlist{
	Date * start_date;
	Date * end_date;
	TLDNode * root_node;
	int nb_adds;
};

struct tldnode{
	char * tld_value;
	int nb_tlds;
	int height;
	int balance;
	TLDNode * left_node;
	TLDNode * right_node;
	TLDNode * parent;	
};

struct tlditerator{
	TLDNode * cur_node;
};


/*
 * tldlist_create generates a list structure for storing counts against
 * top level domains (TLDs)
 *
 * creates a TLDList that is constrained to the `begin' and `end' Date's
 * returns a pointer to the list if successful, NULL if not
 */
TLDList *tldlist_create(Date *begin, Date *end){
	TLDList *newTLDList = (TLDList*) malloc(sizeof(TLDList));
	if(newTLDList == NULL){
		return NULL;
	}
	newTLDList->start_date = begin;
	newTLDList->end_date = end;
	return newTLDList;
};


/*
 * tldlist_destroy destroys the list structure in `tld'
 *
 * all heap allocated storage associated with the list is returned to the heap
 */

void tldlist_destroy(TLDList *tld){
	node_destroy(tld->root_node);
	free(tld);
}

/*
 * recursively frees all nodes from the root node "node" including itslef
 */
void node_destroy(TLDNode * node){
	if(node == NULL){
		return;
	}
	node_destroy(node->left_node);
	node_destroy(node->right_node);
	free(node->tld_value);
	free(node);
}

/*
 * tldlist_add adds the TLD contained in `hostname' to the tldlist if
 * `d' falls in the begin and end dates associated with the list;
 * returns 1 if the entry was counted, 0 if not
 */

int tldlist_add(TLDList *tld, char *hostname, Date *d){
	if(date_between(d, tld->start_date, tld->end_date)){
		char * parsed_hostname = strlwr(strrchr(hostname, '.')+ 1); //+1 to eliminate the dot
		if(parsed_hostname == NULL){
			free(parsed_hostname); //throws error??? free (NULL)
			return 0;
		}
		if(tld->root_node == NULL){
			TLDNode * first_node = create_node(parsed_hostname, 1, 0, 0, NULL, NULL, NULL);
			if(first_node == NULL){
				return 0;
			}
			tld->root_node = first_node;
			tld->nb_adds = 1;
			return 1;
			}
		if(add_node(tld->root_node, parsed_hostname, tld)){
			tld->nb_adds+=1;
			return 1;
		}
	}
	return 0;	
};

/*checks if date is between start_date and end_date*/
bool date_between(Date * date, Date * start_date, Date * end_date){
	return ((date_compare(date,start_date) >= 0) && (date_compare(end_date, date) >= 0));
}

TLDNode * create_node(char* tld_value, int nb_tlds, int height, int balance, TLDNode * left_node, TLDNode * right_node, TLDNode * parent){
	TLDNode * new_node = (TLDNode*) malloc(sizeof(TLDNode));
	if(new_node == NULL){
			return NULL;
	}
	new_node->tld_value = tld_value;
	new_node->nb_tlds = nb_tlds;
	new_node->height = height;
	new_node->balance = balance;
	new_node->left_node = left_node;
	new_node->right_node = right_node;
	new_node->parent = parent;
	return new_node;
}

int add_node(TLDNode* root_node, char* hostname_to_add, TLDList* tld){
	if(strcmp(hostname_to_add, root_node->tld_value) < 0){
		if(root_node->left_node == NULL){
			TLDNode * node_to_add = create_node(hostname_to_add, 1, 0, 0, NULL, NULL, root_node);
			if(node_to_add == NULL){
				return 0;
			}
			root_node->left_node = node_to_add;
			rebalance(root_node, tld);
			return 1;
		}
		else{
			add_node(root_node->left_node, hostname_to_add, tld);
			return 1;	
		}
	}
	if(strcmp(hostname_to_add, root_node->tld_value) > 0){
		if(root_node->right_node == NULL){
			TLDNode * node_to_add = create_node(hostname_to_add, 1, 0, 0, NULL, NULL, root_node);
			if(node_to_add == NULL){
				return 0;
			}
			root_node->right_node = node_to_add;
			rebalance(root_node, tld);
			return 1;
		}
		else{
			add_node(root_node->right_node, hostname_to_add, tld);
			return 1;	
		}
	}
	else{
		root_node->nb_tlds+=1;
		free(hostname_to_add); //needed?
		return 1;
	}
		
	return 0;
}

//Might cause problems with immutable strings
char *strlwr(char *str){
	char * newStr;
	newStr=(char*) malloc((strlen(str)+1)*sizeof(char));
	if(newStr == NULL){
		return NULL;
	}
	for(int i = 0; i < strlen(str); i++){			
		newStr[i] = tolower(str[i]);
	}
	return newStr;
}

/*
 * tldlist_count returns the number of successful tldlist_add() calls since
 * the creation of the TLDList
 */
long tldlist_count(TLDList *tld){
	return tld->nb_adds;		
};

/*
 * tldlist_iter_create creates an iterator over the TLDList; returns a pointer
 * to the iterator if successful, NULL if not
 */
TLDIterator *tldlist_iter_create(TLDList *tld){
	if(tld->root_node == NULL){
		return NULL;
	}
	TLDIterator * new_iterator = (TLDIterator *) malloc(sizeof(TLDIterator));
	if(new_iterator == NULL){
		return NULL;
	}
	TLDNode * prev_node = tld->root_node;
	TLDNode * cur_node = prev_node->left_node;
	while(cur_node != NULL){
		prev_node = cur_node;
		cur_node = cur_node->left_node;
	}
	new_iterator->cur_node = prev_node;
	return new_iterator;
};

/*
 * tldlist_iter_next returns the next element in the list; returns a pointer
 * to the TLDNode if successful, NULL if no more elements to return
 */

//init iterator with tldlist
//in next check for null cur_node if y then do create code else do existing code add if null to next_node so don't loop
TLDNode *tldlist_iter_next(TLDIterator *iter){	
	if(iter == NULL){
		return NULL;
	}
	TLDNode * cur_node = iter->cur_node;
	TLDNode * next_node = find_next_node(cur_node);
	iter->cur_node = next_node;
	return cur_node;

};


TLDNode *find_next_node(TLDNode* node){
	if(!node){
		return NULL;
	}
	if(node->right_node){
		TLDNode * cur_node = node->right_node;
		TLDNode * parent_node = node;
		while(cur_node){
			parent_node = cur_node;
			cur_node = cur_node->left_node;
		}
		return parent_node;
	}
	else if(node->parent == NULL){
		return NULL;
	}
	else{
		if(node->parent->left_node == node){
			return node->parent;
		}
		else{
			TLDNode * parent_node = node->parent;
			TLDNode * cur_node = node;
			while(parent_node && (parent_node->right_node == cur_node)){
				cur_node = parent_node;
				parent_node = parent_node->parent;
			}
			if(parent_node == NULL){
				return NULL;
			}

			else{
				return parent_node;
			}
		}		
	}		
}

/*
 * tldlist_iter_destroy destroys the iterator specified by `iter'
 */
void tldlist_iter_destroy(TLDIterator *iter){
	free(iter);
}

/*
 * tldnode_tldname returns the tld associated with the TLDNode
 */
char *tldnode_tldname(TLDNode *node){
	return node->tld_value;
};

/*
 * tldnode_count returns the number of times that a log entry for the
 * corresponding tld was added to the list
 */
long tldnode_count(TLDNode *node){
	return node->nb_tlds;
};

void rebalance(TLDNode* node, TLDList * tld){
	set_balance(node);
	
	if(node->balance == -2){
		if(height(node->left_node->left_node) >= height(node->left_node->right_node)){
			node = rotate_right(node);
		}
		else{
			node = rotate_left_then_right(node);
		}
	}
	else if(node->balance == 2){
		if(height(node->right_node->right_node) >= height(node->right_node->left_node)){
			node = rotate_left(node);
		}
		else{
			node = rotate_right_then_left(node);
		}
	}
	
	if(node->parent != NULL){
		rebalance(node->parent, tld);
		}
	else{
		tld->root_node = node;
		}
}


TLDNode * rotate_left(TLDNode * node){

	TLDNode * right_child = node->right_node;
	right_child->parent = node->parent;
	
	node->right_node = right_child->left_node;
		
	if(node->right_node != NULL){
		node->right_node->parent = node;
	}
	right_child->left_node = node;
	node->parent = right_child;
	
	if(right_child->parent != NULL){
		if(right_child->parent->right_node == node){
			right_child->parent->right_node = right_child;
		}
		else { 
			right_child->parent->left_node = right_child;
		}
	}
	
	set_balance(node);
	set_balance(right_child);
	
	return right_child;
}


TLDNode * rotate_right(TLDNode * node){

	TLDNode * left_child = node->left_node;
	left_child->parent = node->parent;
	
	node->left_node = left_child->right_node;
		
	if(node->left_node != NULL){
		node->left_node->parent = node;
	}
	left_child->right_node = node;
	node->parent = left_child;
	
	if(left_child->parent != NULL){
		if(left_child->parent->right_node == node){
			left_child->parent->right_node = left_child;
		}
		else { 
			left_child->parent->left_node = left_child;
		}
	}
	
	set_balance(node);
	set_balance(left_child);
	
	return left_child;
}

TLDNode * rotate_left_then_right(TLDNode * node){
	node->left_node = rotate_left(node->left_node);
	return rotate_right(node);
}

TLDNode * rotate_right_then_left(TLDNode * node){
	node->right_node = rotate_right(node->right_node);
	return rotate_left(node);
}

void set_balance(TLDNode * node){
	re_height(node);
	node->balance = height(node->right_node) - height(node->left_node);
}

int height(TLDNode *node){
	if(node == NULL){
		return -1;
	}
	return node->height;
}

void re_height(TLDNode * node){
	if(node != NULL){
		node->height = 1 + max(height(node->left_node), height(node->right_node));
	}
}

int max(int int1, int int2){
	if(int1 < int2){
		return int2;
	}
	else{
		return int1;
	}
}
