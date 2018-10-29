#include "tldlist.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>

void add_node(TLDNode* root_node, TLDNode * node);
void node_destroy(TLDNode * node);
bool date_between(Date * date, Date * start_date, Date * end_date);
char *strlwr(char *str);

struct date{
	int day;
	int month;
	int year;
};

struct tldlist{
	Date * start_date;
	Date * end_date;
	TLDNode * root_node;
};

struct tldnode{
	char * tld_value;
	TLDNode * left_node;
	TLDNode * right_node;
};

struct tlditerator{
};


int main(){
	Date * startDate = date_create("01/02/2001");
	Date * endDate = date_create("22/02/2008");
	TLDList * tldlist = tldlist_create(startDate, endDate);
	printf("start date day should be 1: %d, end date day should be 22: %d\n", tldlist->start_date->day, tldlist->end_date->day);
	Date * com_date = date_create("04/03/2002");
	char com[3] = "com";
	tldlist_add(tldlist, com, com_date);

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
	TLDNode *new_root_node = (TLDNode*) malloc(sizeof(TLDNode));
	if(new_root_node == NULL){
		return NULL;
	}
	newTLDList->root_node = new_root_node;
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

void node_destroy(TLDNode * node){
	if(node == NULL){
		return;
	}
	node_destroy(node->left_node);
	node_destroy(node->right_node);
	free(node);
}

/*
 * tldlist_add adds the TLD contained in `hostname' to the tldlist if
 * `d' falls in the begin and end dates associated with the list;
 * returns 1 if the entry was counted, 0 if not
 */

//TODO refactor so don't switch btw hostname and node twice
int tldlist_add(TLDList *tld, char *hostname, Date *d){
	if(date_between(d, tld->start_date, tld->end_date)){
		TLDNode * new_node = (TLDNode*) malloc(sizeof(TLDNode));
		new_node->tld_value = strlwr(hostname);
		add_node(tld->root_node, new_node);
		return 1;
	}
	return 0;	
};

/*checks if date is between start_date and end_date*/
bool date_between(Date * date, Date * start_date, Date * end_date){
	return ((date_compare(date,start_date) >= 0) && (date_compare(end_date, date) >= 0));
}

void add_node(TLDNode* root_node, TLDNode * node){
	char * tld_value_in = node->tld_value;
	if(tld_value_in  < root_node->tld_value){
		if(root_node->left_node == NULL){
			root_node->left_node = node;
		}
		else{
			add_node(root_node->left_node, node);	
		}
	}
	if(tld_value_in >= root_node->tld_value){
		if(root_node->right_node == NULL){
			root_node->right_node = node;
		}
		else{
			add_node(root_node->right_node, node);	
		}
	}

	return;
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
long tldlist_count(TLDList *tld);

/*
 * tldlist_iter_create creates an iterator over the TLDList; returns a pointer
 * to the iterator if successful, NULL if not
 */
TLDIterator *tldlist_iter_create(TLDList *tld);

/*
 * tldlist_iter_next returns the next element in the list; returns a pointer
 * to the TLDNode if successful, NULL if no more elements to return
 */
TLDNode *tldlist_iter_next(TLDIterator *iter);

/*
 * tldlist_iter_destroy destroys the iterator specified by `iter'
 */
void tldlist_iter_destroy(TLDIterator *iter);

/*
 * tldnode_tldname returns the tld associated with the TLDNode
 */
char *tldnode_tldname(TLDNode *node);

/*
 * tldnode_count returns the number of times that a log entry for the
 * corresponding tld was added to the list
 */
long tldnode_count(TLDNode *node);
