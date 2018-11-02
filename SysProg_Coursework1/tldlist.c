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


/*int main(){
	Date * startDate = date_create("01/02/2001");
	Date * endDate = date_create("22/02/2008");
	TLDList * tldlist = tldlist_create(startDate, endDate);
	
	Date * com_date = date_create("04/03/2002");
	char * hostname_com = "www.intel.com";
	printf("should be 1 : %d\n",tldlist_add(tldlist, hostname_com, com_date));
	printf("Tldlist should now have root node with\n tld_value com : %s\n nb_tlds 1 : %d\n", tldlist->root_node->tld_value, tldlist->root_node->nb_tlds);  

	printf("Tldlist should have 1 add : %d\n", tldlist->nb_adds);

	Date * uk_date = date_create("12/12/2002");
	char * hostname_uk = "www.dcs.gla.ac.uk";
	printf("should be 1 : %d\n",tldlist_add(tldlist, hostname_uk, uk_date));
	printf("Tldlist should now have right node with\n tld_value uk : %s\n nb_tlds 1 : %d\n", tldlist->root_node->right_node->tld_value, tldlist->root_node->right_node->nb_tlds); 
	
	printf("Tldlist should have 2 adds : %d\n", tldlist->nb_adds);

	
	Date * ac_date = date_create("12/10/2002");
	char * hostname_ac = "www.dcs.gla.ac.ac";
	printf("should be 1 : %d\n",tldlist_add(tldlist, hostname_ac, ac_date));
	printf("Tldlist should now have left node with\n tld_value ac : %s\n nb_tlds 1 : %d\n", tldlist->root_node->left_node->tld_value, tldlist->root_node->left_node->nb_tlds); 

	printf("Tldlist should have 3 adds : %d\n", tldlist->nb_adds);

	Date * uk_date2 = date_create("12/22/2002");
	char * hostname_uk2 = "www.dcs.gla.bc.uk";
	printf("should be 1 : %d\n",tldlist_add(tldlist, hostname_uk2, uk_date2));
	printf("Tldlist should now have right node with\n tld_value uk : %s\n nb_tlds 2 : %d\n", tldlist->root_node->right_node->tld_value, tldlist->root_node->right_node->nb_tlds); 

	printf("Tldlist should have 4 adds : %d\n", tldlist->nb_adds);


	TLDIterator * iter = tldlist_iter_create(tldlist);
	printf("Iterator current node value should be ac : %s\n", iter->cur_node->tld_value);

	TLDNode * next_node = tldlist_iter_next(iter);
	printf("Next node should have value com: %s\n", next_node->tld_value);

	next_node = tldlist_iter_next(iter);
	printf("Next node should have value uk: %s\n", next_node->tld_value);

	
	tldlist_destroy(tldlist);
}*/


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


//destroy parent?
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
			TLDNode * first_node = (TLDNode*) malloc(sizeof(TLDNode));
			if(first_node == NULL){
				return 0;
			}
			first_node->nb_tlds = 1;
			first_node->tld_value = parsed_hostname;
			first_node->height = 0;
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


int add_node(TLDNode* root_node, char* hostname_to_add, TLDList* tld){
	if(strcmp(hostname_to_add, root_node->tld_value) < 0){
		if(root_node->left_node == NULL){
			TLDNode * node_to_add = (TLDNode *) malloc(sizeof(TLDNode));
			if(node_to_add == NULL){
				return 0;
			}
			node_to_add->tld_value = hostname_to_add;
			root_node->left_node = node_to_add;
			node_to_add->parent = root_node;
			node_to_add->nb_tlds = 1;
			re_height(node_to_add);
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
			TLDNode * node_to_add = (TLDNode *) malloc(sizeof(TLDNode));
			if(node_to_add == NULL){
				return 0;
			}
			node_to_add->tld_value = hostname_to_add;
			root_node->right_node = node_to_add;
			node_to_add->parent = root_node;
			node_to_add->nb_tlds = 1;
			re_height(node_to_add);
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
		free(hostname_to_add);
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
