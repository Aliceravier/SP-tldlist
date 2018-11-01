#include "tldlist.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>

//TODO fix tldlist_next_iter never ending


int add_node(TLDNode* root_node, char* hostname);
void node_destroy(TLDNode * node);
bool date_between(Date * date, Date * start_date, Date * end_date);
char *strlwr(char *str);
TLDNode *find_next_node(TLDNode* node);


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
	if(date_between(d, tld->start_date, tld->end_date)){
		char * parsed_hostname = strlwr(strrchr(hostname, '.')+ 1); //+1 to eliminate the dot
		if(tld->root_node == NULL){
			TLDNode * first_node = (TLDNode*) malloc(sizeof(TLDNode));
			if(first_node == NULL){
				return 0;
			}
			first_node->nb_tlds = 1;
			first_node->tld_value = parsed_hostname;
			tld->root_node = first_node;
			tld->nb_adds = 1;
			return 1;
			}
		if(add_node(tld->root_node, parsed_hostname)){
			tld->nb_adds++;
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
			return 1;
		}
		else{
			add_node(root_node->left_node, hostname_to_add );
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
			return 1;
		}
		else{
			add_node(root_node->right_node, hostname_to_add );
			return 1;	
		}
	}
	else{
		root_node->nb_tlds+=1;
			return 1;
	}
		
	return 0;
}

//Might cause problems with immutable strings
char *strlwr(char *str){
	char * newStr;
	newStr=(char*) malloc((strlen(str)+1)*sizeof(char));
	for(int i=  0; i<strlen(str); i++){			
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
