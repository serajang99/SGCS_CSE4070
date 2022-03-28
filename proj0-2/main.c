#include "list.h"
#include "hash.h"
#include "bitmap.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>


struct list_item
{
	struct list_elem elem;
	int data;
	//Other members you want
};


typedef struct list_{
	char name[20];
	struct list *link;
}list_arr;
list_arr ll[10];

struct hash_item
{
	struct hash_elem elem;
	int data;
	//Other members you want
};

typedef struct hash_{
	char name[20];
	struct hash *link;
}hash_arr;
hash_arr hh[10];

typedef struct bitmap_{
	char name[20];
	struct bitmap *link;
}bitmap_arr;
bitmap_arr bb[10];

int find_idx(char c, int cnt, char *inst){
	int i;
	if(c == 'l'){
		for(i=0; i<cnt; i++){
			if(!strcmp(inst, ll[i].name)) break;
		}
	}
	else if(c == 'h'){
		for(i=0; i<cnt; i++){
			if(!strcmp(inst, hh[i].name)) break;
		}
	}
	else if(c == 'b'){
		for(i=0; i<cnt; i++){
			if(!strcmp(inst, bb[i].name)) break;
		}
	}
	return i;
}

/*list functions*/
bool list_less(const struct list_elem *a, const struct list_elem *b, void *aux){
	struct list_item *i1, *i2;

	i1 = list_entry(a, struct list_item, elem);
	i2 = list_entry(b, struct list_item, elem);

	if(i1->data < i2->data) return true;
	else return false;
}

/*hash functions*/
bool hash_less(const struct hash_elem *a, const struct hash_elem *b, void *aux){
	struct hash_item *i1, *i2;

	i1 = hash_entry(a, struct hash_item, elem);
	i2 = hash_entry(b, struct hash_item, elem);

	if(i1->data < i2->data) return true;
	else return false;
}

unsigned hash_hash(const struct hash_elem *e, void *aux){
	struct hash_item *temp;

	temp = hash_entry(e, struct hash_item, elem);
	return hash_int(temp->data);
};

/*hash_action_functions*/
void destructor(struct hash_elem *e, void *aux){
	struct hash_item *temp;
	temp = hash_entry(e, struct hash_item, elem);
	free(temp);
}

void square(struct hash_elem *e, void *aux){
	struct hash_item *temp;
	temp = hash_entry(e, struct hash_item, elem);
	temp->data *= temp->data;
}

void triple(struct hash_elem *e, void *aux){
	struct hash_item *temp;
	temp = hash_entry(e, struct hash_item, elem);
	temp->data *= (temp->data)*(temp->data);
}


int main(){

	char input[50],input_tmp[50];
	char cmd[50];
	char tmp1[10],tmp2[10],tmp3[10],tmp4[10],tmp5[10];

	int l_cnt=0,l_idx=-1;
	int h_cnt=0,h_idx=-1;
	size_t b_cnt=0,b_idx=-1;
	
	while(1){
		fgets(input,sizeof(input),stdin);
		input[strlen(input)-1]='\0';
		strcpy(input_tmp,input);
		strcpy(cmd,strtok(input_tmp," "));
		
/*----------------------------------------------------------*/
/*------------------------------------------------------------*/
//	                   basic functions                        //
/*------------------------------------------------------------*/


		if(!strcmp(cmd,"create")){
			strcpy(tmp1,strtok(NULL," "));
			if(!strncmp(tmp1,"list",4)){
				ll[l_cnt].link = (struct list*)malloc(sizeof(struct list));
				list_init(ll[l_cnt].link);
				strcpy(tmp2,strtok(NULL," "));
				strcpy(ll[l_cnt].name, tmp2);
				l_cnt++;
			}
			else if(!strncmp(tmp1,"hash",4)){
				void *aux;
				hh[h_cnt].link = (struct hash*)malloc(sizeof(struct hash));
				struct hash_elem *e;
				hash_init(hh[h_cnt].link,hash_hash,hash_less,aux);
				strcpy(tmp2,strtok(NULL," "));
				strcpy(hh[h_cnt].name, tmp2);
				h_cnt++;
			}
			else if(!strncmp(tmp1,"bitmap",6)){
				strcpy(tmp2,strtok(NULL," "));
				strcpy(tmp3,strtok(NULL," "));
				size_t bit_cnt = atoi(tmp3);
				//printf("%zu\n",bit_cnt);
				bb[b_cnt].link = bitmap_create(bit_cnt); 
				strcpy(bb[b_cnt].name, tmp2);
				b_cnt++;
			}

		}
		else if(!strcmp(cmd,"delete")){
			
			strcpy(tmp1,strtok(NULL," "));
			for(int i=0;i<10;i++){
				if(!strcmp(tmp1,ll[i].name)){
					for(int j=i;j<9;j++){
						ll[j] = ll[j+1];
					}
					break;
				}
				else if(!strcmp(tmp1,hh[i].name)){
					hash_destroy(hh[i].link, destructor);
					break;
				}
				else if(!strcmp(tmp1,bb[i].name)){
					bitmap_destroy(bb[i].link);		
					break;
				}
			}
		}
		else if(!strcmp(cmd,"dumpdata")){
		
			strcpy(tmp1,strtok(NULL," "));
			if(!strncmp(tmp1,"list",4)){
				l_idx = find_idx('l', l_cnt, tmp1);
				struct list_elem *e;
				for(e=list_begin(ll[l_idx].link);e!=list_end(ll[l_idx].link);e=list_next(e)){
					struct list_item *temp = list_entry(e, struct list_item, elem);
					printf("%d ",temp->data);
				}
				printf("\n");
			}
			else if(!strncmp(tmp1,"hash",4)){
				h_idx = find_idx('h', h_cnt, tmp1);
				struct list_elem *l;
				for(int i=0;i<hh[h_idx].link->bucket_cnt;i++){
					for(l = list_begin(&(hh[h_idx].link->buckets[i]));l!=list_end(&(hh[h_idx].link->buckets[i]));l=list_next(l)){

						struct hash_elem *e = list_entry(l, struct hash_elem, list_elem);
						struct hash_item *temp = hash_entry(e, struct hash_item, elem);
						printf("%d ",temp->data);
					}
				}
				printf("\n");
			}
			else if(!strncmp(tmp1,"bm",2)){
				b_idx = find_idx('b', b_cnt, tmp1);
				for(int i=0;i<bitmap_size(bb[b_idx].link);i++){
					if(bitmap_test(bb[b_idx].link,i)==0)
						printf("0");
					else
						printf("1");
				}
				printf("\n");
			}
		}
		else if(!strcmp(cmd,"quit")){
			break;
		}
/*----------------------------------------------------------*/

/*------------------------------------------------------------*/
//	                         list                             //
/*------------------------------------------------------------*/

/*-----------list_insert-----------------*/
		else if(!strcmp(cmd,"list_insert")){
			strcpy(tmp1,strtok(NULL," "));
			strcpy(tmp2,strtok(NULL," "));
			int b_idx = atoi(tmp2);
			strcpy(tmp3,strtok(NULL," "));
			int data = atoi(tmp3);

			l_idx = find_idx('l', l_cnt, tmp1);

			struct list_elem *before;
			struct list_item *i = (struct list_item*)malloc(sizeof(struct list_item));	
	
			before = list_begin(ll[l_idx].link);
			for(int i=0;i<b_idx;i++){
				before = list_next(before);
			}
			list_insert(before,&(i->elem));
			i = list_entry(&(i->elem),struct list_item,elem);
			i->data = data;
		}
/*-----------list_push_front_back-----------------*/
		else if(!strcmp(cmd,"list_push_front")){
			strcpy(tmp1,strtok(NULL," "));
			strcpy(tmp2,strtok(NULL," "));
			int data = atoi(tmp2);
			l_idx = find_idx('l', l_cnt, tmp1);

			struct list_item *i = (struct list_item*)malloc(sizeof(struct list_item));

			list_push_front(ll[l_idx].link, &(i->elem));
			
			i = list_entry(&(i->elem),struct list_item,elem);
			i->data = data;

		}
		else if(!strcmp(cmd,"list_push_back")){
			strcpy(tmp1,strtok(NULL," "));
			strcpy(tmp2,strtok(NULL," "));
			int data = atoi(tmp2);
			l_idx = find_idx('l', l_cnt, tmp1);

			struct list_item *i = (struct list_item*)malloc(sizeof(struct list_item));

			list_push_back(ll[l_idx].link, &(i->elem));
			
			i = list_entry(&(i->elem),struct list_item,elem);
			i->data = data;
		}

/*-----------list_splice-----------------*/
		else if(!strcmp(cmd,"list_splice")){

			strcpy(tmp1,strtok(NULL," "));//tmp1: list#(1)
			strcpy(tmp2,strtok(NULL," "));//tmp2: ~l_idx(1)
			int b_idx = atoi(tmp2);
			strcpy(tmp3,strtok(NULL," "));//tmp3: list#(2)
			strcpy(tmp4,strtok(NULL," "));//tmp4: l_idx~(2)
			int f_idx = atoi(tmp4);
			strcpy(tmp5,strtok(NULL," "));//tmp5: ~l_idx(2)
			int lst_idx = atoi(tmp5);

			int l_idx1 = find_idx('l', l_cnt, tmp1);
			int l_idx2 = find_idx('l', l_cnt, tmp3);

			struct list_elem *before, *first, *last;

			before = list_begin(ll[l_idx1].link);
			for(int i=0;i<b_idx;i++){
				before = list_next(before);
			}
			first = list_begin(ll[l_idx2].link);
			for(int i=0;i<f_idx;i++){
				first = list_next(first);
			}
			last = list_begin(ll[l_idx2].link);
			for(int i=0;i<lst_idx;i++){
				last = list_next(last);
			}

			list_splice(before, first, last);

		}
	
/*-----------list_front_back-----------------*/
		else if(!strcmp(cmd,"list_front")){
			strcpy(tmp1,strtok(NULL," "));//tmp1: list#(1)
			l_idx = find_idx('l', l_cnt, tmp1);
			
			struct list_elem *e; 
			e = list_front(ll[l_idx].link);
			struct list_item *temp = list_entry(e, struct list_item, elem);
			printf("%d\n",temp->data);

		}		
		else if(!strcmp(cmd,"list_back")){
			strcpy(tmp1,strtok(NULL," "));//tmp1: list#(1)
			l_idx = find_idx('l', l_cnt, tmp1);
			struct list_elem *e; 
			e = list_back(ll[l_idx].link);
			struct list_item *temp = list_entry(e, struct list_item, elem);
			printf("%d\n",temp->data);
		}
/*-----------list_pop_front_back-----------------*/
		else if(!strcmp(cmd,"list_pop_front")){
			strcpy(tmp1,strtok(NULL," "));//tmp1: list#(1)
			l_idx = find_idx('l', l_cnt, tmp1);
			list_pop_front(ll[l_idx].link);
		}
		else if(!strcmp(cmd,"list_pop_back")){
			strcpy(tmp1,strtok(NULL," "));//tmp1: list#(1)
			l_idx = find_idx('l', l_cnt, tmp1);
			list_pop_back(ll[l_idx].link);
		}

/*-----------list_insert_ordered-----------------*/
		else if(!strcmp(cmd,"list_insert_ordered")){
			strcpy(tmp1,strtok(NULL," "));
			strcpy(tmp2,strtok(NULL," "));
			int data = atoi(tmp2);
			l_idx = find_idx('l', l_cnt, tmp1);
			void *aux;

			struct list_item *i = (struct list_item*)malloc(sizeof(struct list_item));
			i = list_entry(&(i->elem),struct list_item,elem);
			i->data = data;

			list_insert_ordered(ll[l_idx].link, &(i->elem), list_less, aux);
			
		}
	
/*-----------list_maxminsizeempty-----------------*/
		else if(!strcmp(cmd, "list_empty")){
			strcpy(tmp1,strtok(NULL," "));//tmp1: list#(1)
			l_idx = find_idx('l', l_cnt, tmp1);
			int result = list_empty(ll[l_idx].link);
			if(result == true) printf("true\n");
			else printf("false\n");
		}
	
		else if(!strcmp(cmd, "list_size")){
			strcpy(tmp1,strtok(NULL," "));//tmp1: list#(1)
			l_idx = find_idx('l', l_cnt, tmp1);
			printf("%zu\n", list_size(ll[l_idx].link));
		}

		else if(!strcmp(cmd, "list_max")){
			strcpy(tmp1,strtok(NULL," "));//tmp1: list#(1)
			l_idx = find_idx('l', l_cnt, tmp1);
			void *aux;

			struct list_elem *e; 
			e = list_max(ll[l_idx].link,list_less,aux);
			struct list_item *temp = list_entry(e, struct list_item, elem);
			printf("%d\n",temp->data);
		}
		
		else if(!strcmp(cmd, "list_min")){
			strcpy(tmp1,strtok(NULL," "));//tmp1: list#(1)
			l_idx = find_idx('l', l_cnt, tmp1);
			void *aux;

			struct list_elem *e; 
			e = list_min(ll[l_idx].link,list_less,aux);
			struct list_item *temp = list_entry(e, struct list_item, elem);
			printf("%d\n",temp->data);

		}

/*-----------list_remove----------------*/
		else if(!strcmp(cmd,"list_remove")){
			strcpy(tmp1,strtok(NULL," "));
			strcpy(tmp2,strtok(NULL," "));
			int r_idx = atoi(tmp2);
			l_idx = find_idx('l', l_cnt, tmp1);

			struct list_elem *e;
			e = list_begin(ll[l_idx].link);
			for(int i=0;i<r_idx;i++){
				e = list_next(e);
			}
			list_remove(e);
		}

/*-----------list_swap----------------*/
		else if(!strcmp(cmd,"list_swap")){
			strcpy(tmp1,strtok(NULL," "));//tmp1: list#
			strcpy(tmp2,strtok(NULL," "));//tmp2: a
			int a_idx = atoi(tmp2);
			strcpy(tmp3,strtok(NULL," "));//tmp3: b
			int b_idx = atoi(tmp3);

			l_idx = find_idx('l', l_cnt, tmp1);
			
			struct list_elem *e1, *e2;
			e1 = list_begin(ll[l_idx].link);
			e2 = list_begin(ll[l_idx].link);
		
			for(int i=0;i<a_idx;i++){
				e1 = list_next(e1);
			}
			for(int i=0;i<b_idx;i++){
				e2 = list_next(e2);
			}

			list_swap(e1,e2);
		}

/*-----------list_unique----------------*/
		else if(!strcmp(cmd,"list_unique")){	
			void *aux;
			strcpy(tmp1,strtok(NULL," "));//tmp1: list#(1)
			int l_idx1 = find_idx('l', l_cnt, tmp1);
			char* tmp = strtok(NULL," ");
			if(tmp !=NULL){
				strcpy(tmp2,tmp);//tmp2: list#(2)
				int l_idx2 = find_idx('l', l_cnt, tmp2);
				list_unique(ll[l_idx1].link,ll[l_idx2].link,list_less,aux);
			}
			else{
				list_unique(ll[l_idx1].link,NULL,list_less,aux);
			}
		}

/*-----------list_reverse----------------*/
		else if(!strcmp(cmd,"list_reverse")){
			strcpy(tmp1,strtok(NULL," "));//tmp1: list#
			l_idx = find_idx('l', l_cnt, tmp1);
			list_reverse(ll[l_idx].link);
		}

/*-----------list_sort----------------*/
		else if(!strcmp(cmd,"list_sort")){
			strcpy(tmp1,strtok(NULL," "));//tmp1: list#
			l_idx = find_idx('l', l_cnt, tmp1);
			list_sort(ll[l_idx].link, list_less, NULL);
		}

/*-----------list_shuffle----------------*/
		else if(!strcmp(cmd,"list_shuffle")){
			strcpy(tmp1,strtok(NULL," "));//tmp1: list#
			l_idx = find_idx('l', l_cnt, tmp1);
			list_shuffle(ll[l_idx].link);
		}

/*------------------------------------------------------------*/
//	                         hash                             //
/*------------------------------------------------------------*/


/*-----------hash_insert----------------*/
		else if(!strcmp(cmd,"hash_insert")){
			strcpy(tmp1,strtok(NULL," "));//tmp1: hash#
			h_idx = find_idx('h', h_cnt, tmp1);
			strcpy(tmp2,strtok(NULL," "));
			int data = atoi(tmp2);

			struct hash_elem *e = (struct hash_elem*)malloc(sizeof(struct hash_elem));
			struct hash_item *i = (struct hash_item*)malloc(sizeof(struct hash_item));
			i = hash_entry(e,struct hash_item,elem);
			i->data = data;
			hash_insert(hh[h_idx].link,e);
		}

/*-----------hash_replace----------------*/
		else if(!strcmp(cmd,"hash_replace")){
			strcpy(tmp1,strtok(NULL," "));//tmp1: hash#
			h_idx = find_idx('h', h_cnt, tmp1);
			strcpy(tmp2,strtok(NULL," "));
			int data = atoi(tmp2);

			struct hash_elem *e = (struct hash_elem*)malloc(sizeof(struct hash_elem));
			struct hash_item *i = (struct hash_item*)malloc(sizeof(struct hash_item));
			i = hash_entry(e,struct hash_item,elem);
			i->data = data;
			hash_replace(hh[h_idx].link,e);
		}

/*-----------hash_find----------------*/
		else if(!strcmp(cmd,"hash_find")){
			strcpy(tmp1,strtok(NULL," "));//tmp1: hash#
			h_idx = find_idx('h', h_cnt, tmp1);
			strcpy(tmp2,strtok(NULL," "));
			int data = atoi(tmp2);
			
			struct hash_elem *e = (struct hash_elem*)malloc(sizeof(struct hash_item));
			struct hash_item *i = (struct hash_item*)malloc(sizeof(struct hash_item));
			i->data = data;
			e = hash_find(hh[h_idx].link,&(i->elem));
			if(e!=NULL){
				i = hash_entry(e, struct hash_item, elem);
				printf("%d\n",i->data);
			}
		}

/*-----------hash_delete----------------*/
		else if(!strcmp(cmd,"hash_delete")){
			strcpy(tmp1,strtok(NULL," "));//tmp1: hash#
			h_idx = find_idx('h', h_cnt, tmp1);
			strcpy(tmp2,strtok(NULL," "));
			int data = atoi(tmp2);
			
			struct hash_elem *e = (struct hash_elem*)malloc(sizeof(struct hash_item));
			struct hash_item *i = (struct hash_item*)malloc(sizeof(struct hash_item));
			i->data = data;
			e = hash_delete(hh[h_idx].link,&(i->elem));
		}

/*-----------hash_apply----------------*/
		else if(!strcmp(cmd,"hash_apply")){
			strcpy(tmp1,strtok(NULL," "));//tmp1: hash#
			h_idx = find_idx('h', h_cnt, tmp1);
			strcpy(tmp2,strtok(NULL," "));
			
			struct hash_elem *e = (struct hash_elem*)malloc(sizeof(struct hash_item));
			if(!strcmp(tmp2,"square"))
				hash_apply(hh[h_idx].link,square);
			else if(!strcmp(tmp2,"triple"))
				hash_apply(hh[h_idx].link,triple);
		}	

/*-----------hash_etc----------------*/
		else if(!strcmp(cmd,"hash_size")){
			strcpy(tmp1,strtok(NULL," "));
			h_idx = find_idx('h', h_cnt, tmp1);
			printf("%zu\n", hash_size(hh[h_idx].link));

		}
		else if(!strcmp(cmd,"hash_empty")){
			strcpy(tmp1,strtok(NULL," "));
			h_idx = find_idx('h', h_cnt, tmp1);
			int result = hash_empty(hh[h_idx].link);
			if(result == true) printf("true\n");
			else printf("false\n");


		}
		else if(!strcmp(cmd,"hash_clear")){
			strcpy(tmp1,strtok(NULL," "));
			h_idx = find_idx('h', h_cnt, tmp1);
			hash_clear(hh[h_idx].link,destructor);
		}

/*------------------------------------------------------------*/
//	                          bitmap                          //
/*------------------------------------------------------------*/

/*-----------bitmap_size----------------*/
		else if(!strcmp(cmd,"bitmap_size")){
			strcpy(tmp1,strtok(NULL," "));
			b_idx = find_idx('b', b_cnt, tmp1);
			printf("%zu\n",bitmap_size(bb[b_idx].link));
		}

/*-----------bitmap_set_test_single----------------*/
		else if(!strcmp(cmd,"bitmap_set")){
			strcpy(tmp1,strtok(NULL," "));//bitmap name
			strcpy(tmp2,strtok(NULL," "));//idx
			size_t bit_idx = atoi(tmp2);
			strcpy(tmp3,strtok(NULL," "));//value
			bool val=0;
			if(!strcmp(tmp3,"true"))
				val = 1;
			b_idx = find_idx('b', b_cnt, tmp1);
			bitmap_set(bb[b_idx].link,bit_idx,val);
		}

		else if(!strcmp(cmd,"bitmap_mark")){
			strcpy(tmp1,strtok(NULL," "));//bitmap name
			strcpy(tmp2,strtok(NULL," "));//idx
			size_t bit_idx = atoi(tmp2);

			b_idx = find_idx('b', b_cnt, tmp1);
			bitmap_mark(bb[b_idx].link,bit_idx);
		}
	
		else if(!strcmp(cmd,"bitmap_test")){
			strcpy(tmp1,strtok(NULL," "));//bitmap name
			strcpy(tmp2,strtok(NULL," "));//idx
			size_t bit_idx = atoi(tmp2);

			b_idx = find_idx('b', b_cnt, tmp1);
			bool result = bitmap_test(bb[b_idx].link,bit_idx);
			if(result == true) printf("true\n");
			else printf("false\n");
		}

		else if(!strcmp(cmd,"bitmap_reset")){
			strcpy(tmp1,strtok(NULL," "));//bitmap name
			strcpy(tmp2,strtok(NULL," "));//idx
			size_t bit_idx = atoi(tmp2);

			b_idx = find_idx('b', b_cnt, tmp1);
			bitmap_reset(bb[b_idx].link,bit_idx);
		}

		else if(!strcmp(cmd,"bitmap_flip")){
			strcpy(tmp1,strtok(NULL," "));//bitmap name
			strcpy(tmp2,strtok(NULL," "));//idx
			size_t bit_idx = atoi(tmp2);

			b_idx = find_idx('b', b_cnt, tmp1);
			bitmap_flip(bb[b_idx].link,bit_idx);
		}

/*-----------bitmap_set_multiple_all----------------*/
		else if(!strcmp(cmd,"bitmap_set_multiple")){
			strcpy(tmp1,strtok(NULL," "));//bitmap name
			strcpy(tmp2,strtok(NULL," "));//idx
			size_t bit_idx1 = atoi(tmp2);
			strcpy(tmp3,strtok(NULL," "));//idx
			size_t bit_idx2 = atoi(tmp3);
			strcpy(tmp4,strtok(NULL," "));//value
			bool val=0;
			if(!strcmp(tmp4,"true"))
				val = 1;
			b_idx = find_idx('b', b_cnt, tmp1);
			bitmap_set_multiple(bb[b_idx].link,bit_idx1,bit_idx2,val);
		}
		
		else if(!strcmp(cmd,"bitmap_set_all")){
			strcpy(tmp1,strtok(NULL," "));//bitmap name	
			strcpy(tmp2,strtok(NULL," "));//value
			bool val=0;
			if(!strcmp(tmp2,"true"))
				val = 1;
			b_idx = find_idx('b', b_cnt, tmp1);
			bitmap_set_all(bb[b_idx].link,val);
		}
		
		else if(!strcmp(cmd,"bitmap_count")){
			strcpy(tmp1,strtok(NULL," "));//bitmap name
			strcpy(tmp2,strtok(NULL," "));//idx
			size_t bit_idx1 = atoi(tmp2);
			strcpy(tmp3,strtok(NULL," "));//idx
			size_t bit_idx2 = atoi(tmp3);
			strcpy(tmp4,strtok(NULL," "));//value
			bool val=0;
			if(!strcmp(tmp4,"true"))
				val = 1;
			b_idx = find_idx('b', b_cnt, tmp1);
			printf("%zu\n",bitmap_count(bb[b_idx].link,bit_idx1,bit_idx2,val));
		}

		else if(!strcmp(cmd,"bitmap_contains")){
			strcpy(tmp1,strtok(NULL," "));//bitmap name
			strcpy(tmp2,strtok(NULL," "));//idx
			size_t bit_idx1 = atoi(tmp2);
			strcpy(tmp3,strtok(NULL," "));//idx
			size_t bit_idx2 = atoi(tmp3);
			strcpy(tmp4,strtok(NULL," "));//value
			bool val=0;
			if(!strcmp(tmp4,"true"))
				val = 1;
			b_idx = find_idx('b', b_cnt, tmp1);
			printf("%s\n",bitmap_contains(bb[b_idx].link,bit_idx1,bit_idx2,val)?"true":"false");
		}

		else if(!strcmp(cmd,"bitmap_any")){
			strcpy(tmp1,strtok(NULL," "));//bitmap name
			strcpy(tmp2,strtok(NULL," "));//idx
			size_t bit_idx1 = atoi(tmp2);
			strcpy(tmp3,strtok(NULL," "));//idx
			size_t bit_idx2 = atoi(tmp3);
			
			b_idx = find_idx('b', b_cnt, tmp1);
			printf("%s\n",bitmap_any(bb[b_idx].link,bit_idx1,bit_idx2)?"true":"false");
		}

		else if(!strcmp(cmd,"bitmap_none")){
			strcpy(tmp1,strtok(NULL," "));//bitmap name
			strcpy(tmp2,strtok(NULL," "));//idx
			size_t bit_idx1 = atoi(tmp2);
			strcpy(tmp3,strtok(NULL," "));//idx
			size_t bit_idx2 = atoi(tmp3);
			
			b_idx = find_idx('b', b_cnt, tmp1);
			printf("%s\n",bitmap_none(bb[b_idx].link,bit_idx1,bit_idx2)?"true":"false");
		}

		else if(!strcmp(cmd,"bitmap_all")){
			strcpy(tmp1,strtok(NULL," "));//bitmap name
			strcpy(tmp2,strtok(NULL," "));//idx
			size_t bit_idx1 = atoi(tmp2);
			strcpy(tmp3,strtok(NULL," "));//idx
			size_t bit_idx2 = atoi(tmp3);
			
			b_idx = find_idx('b', b_cnt, tmp1);
			printf("%s\n",bitmap_all(bb[b_idx].link,bit_idx1,bit_idx2)?"true":"false");
		}


/*-----------bitmap_scan_flip----------------*/
		else if(!strcmp(cmd,"bitmap_scan")){
			strcpy(tmp1,strtok(NULL," "));//bitmap name
			strcpy(tmp2,strtok(NULL," "));//idx
			size_t bit_idx1 = atoi(tmp2);
			strcpy(tmp3,strtok(NULL," "));//idx
			size_t bit_idx2 = atoi(tmp3);
			strcpy(tmp4,strtok(NULL," "));//value
			bool val=0;
			if(!strcmp(tmp4,"true"))
				val = 1;
			b_idx = find_idx('b', b_cnt, tmp1);
			printf("%zu\n",bitmap_scan(bb[b_idx].link,bit_idx1,bit_idx2,val));
		}
		
		else if(!strcmp(cmd,"bitmap_scan_and_flip")){
			strcpy(tmp1,strtok(NULL," "));//bitmap name
			strcpy(tmp2,strtok(NULL," "));//idx
			size_t bit_idx1 = atoi(tmp2);
			strcpy(tmp3,strtok(NULL," "));//idx
			size_t bit_idx2 = atoi(tmp3);
			strcpy(tmp4,strtok(NULL," "));//value
			bool val=0;
			if(!strcmp(tmp4,"true"))
				val = 1;
			b_idx = find_idx('b', b_cnt, tmp1);
			printf("%zu\n",bitmap_scan_and_flip(bb[b_idx].link,bit_idx1,bit_idx2,val));
		}
/*-----------bitmap_dump----------------*/
		else if(!strcmp(cmd,"bitmap_dump")){
			strcpy(tmp1,strtok(NULL," "));
			b_idx = find_idx('b', b_cnt, tmp1);
			bitmap_dump(bb[b_idx].link);
		}
/*-----------bitmap_expand----------------*/
		else if(!strcmp(cmd,"bitmap_expand")){
			strcpy(tmp1,strtok(NULL," "));
			b_idx = find_idx('b', b_cnt, tmp1);
			strcpy(tmp2,strtok(NULL," "));
			int size = atoi(tmp2);
			bb[b_idx].link = bitmap_expand(bb[b_idx].link,size);
		}
	}

	return 0;
}
