#include "tldlist.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>

int add_node(TLDNode* root_node, char* hostname);
void node_destroy(TLDNode * node);
bool date_between(Date * date, Date * start_date, Date * end_date);
char *strlwr(char *str);
TLDNode *find_next_node(TLDNode* node);

struct date{
	int day;
	int month;
	int year;
};

struct tldlist{
	Date * start_date;
	Date * end_date;
	TLDNode * root_node;
	int nb_nodes;
};

struct tldnode{
	char * tld_value;
	int nb_tlds;
	int height;
	TLDNode * left_node;
	TLDNode * right_node;
	TLDNode * parent;	
};

struct tlditerator{
	TLDNode * cur_node;
};


int main(){
	Date * startDate = date_create("01/02/2001");
	Date * endDate = date_create("22/02/2008");
	TLDList * tldlist = tldlist_create(startDate, endDate);
	printf("start date day should be 1: %d, end date day should be 22: %d\n", tldlist->start_date->day, tldlist->end_date->day);
	Date * com_date = date_create("04/03/2002");
	char com[3] = "com";
	tldlist_add(tldlist, com, com_date);
	TLDIterator * iter = tldlist_iter_create(tldlist);
	//TOFIX segfaults
	TLDNode * node = tldlist_iter_next(iter);
	
	tldlist_destroy(tldlist);
}


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

//TODO find way to test this
void tldlist_destroy(TLDList *tld){
	date_destroy(tld->start_date);
	date_destroy(tld->end_date);
	node_destroy(tld->root_node);
}


//destroy parent?
void node_destroy(TLDNode * node){
	if(node == NULL){
		return;
	}
	free(node->tld_value);
	node_destroy(node->left_node);
	node_destroy(node->right_node);
	free(node);
}

/*
 * tldlist_add adds the TLD contained in `hostname' to the tldlist if
 * `d' falls in the begin and end dates associated with the list;
 * returns 1 if the entry was counted, 0 if not
 */

int tldlist_add(TLDList *tld, char *hostname, Date *d){
	if(tld->root_node == NULL){
		TLDNode * first_node = (TLDNode*) malloc(sizeof(TLDNode));
		if(first_node == NULL){
				return 0;
			}
		first_node->nb_tlds = 1;
		tld->root_node = first_node;
		tld->nb_nodes = 1;
		return 1;
		}
	if(date_between(d, tld->start_date, tld->end_date)){
		if(add_node(tld->root_node, strlwr(hostname))){
			tld->nb_nodes++;
			return 1;
		}
	}
	return 0;	
};

/*checks if date is between start_date and end_date*/
bool date_between(Date * date, Date * start_date, Date * end_date){
	return ((date_compare(date,start_date) >= 0) && (date_compare(end_date, date) >= 0));
}


int add_node(TLDNode* root_node, char* hostname_to_add){
	if(hostname_to_add < root_node->tld_value){
		if(root_node->left_node == NULL){
			TLDNode * node_to_add = (TLDNode *) malloc(sizeof(TLDNode));
			if(node_to_add == NULL){
				return 0;
			}
			node_to_add->tld_value = hostname_to_add;
			root_node->left_node = node_to_add;
			node_to_add->parent = root_node->left_node;
			node_to_add->nb_tlds = 1;
			return 1;
		}
		else{
			add_node(root_node->left_node, hostname_to_add );	
		}
	}
	if(hostname_to_add > root_node->tld_value){
		if(root_node->right_node == NULL){
			TLDNode * node_to_add = (TLDNode *) malloc(sizeof(TLDNode));
			if(node_to_add == NULL){
				return 0;
			}
			node_to_add->tld_value = hostname_to_add;
			root_node->right_node = node_to_add;
			node_to_add->parent = root_node->right_node;
			node_to_add->nb_tlds = 1;
			return 1;
		}
		else{
			add_node(root_node->right_node, hostname_to_add );	
		}
	}
	else{
		root_node->nb_tlds++;
		return 1;
	}
		
	return 0;
}

//Might cause problems with immutable strings
char *strlwr(char *str){
	int i = 0;	
	while(str[i]){
		str[i] = tolower(str[i]);
	i++;
	}
return str;
}


/*
 * tldlist_count returns the number of successful tldlist_add() calls since
 * the creation of the TLDList
 */
long tldlist_count(TLDList *tld){
	return tld->nb_nodes;		
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
		cur_node = cur_node->left_node;
		prev_node = prev_node->left_node;
	}
	new_iterator->cur_node = prev_node;
	return new_iterator;
};

/*
 * tldlist_iter_next returns the next element in the list; returns a pointer
 * to the TLDNode if successful, NULL if no more elements to return
 */
TLDNode *tldlist_iter_next(TLDIterator *iter){
	TLDNode * cur_node = iter->cur_node;
	return find_next_node(cur_node);

};


//segfaults TODO
TLDNode *find_next_node(TLDNode* node){
	if(node->right_node){
		TLDNode * cur_node = node->right_node;
		while(cur_node){
			cur_node = cur_node->left_node;
		}
		return cur_node->parent;
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
};

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
