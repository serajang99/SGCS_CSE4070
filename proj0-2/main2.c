#include "main2.h"

int main(){
	int i, j, k;
	int a, b, c;
	char input[50], input_tmp[50];
	size_t size, tmp1, tmp2, tmp3;
	unsigned int tmp4;
	bool result;

	//init list's name to NULL for use strcpy
	for(i=0; i<10; i++){
		list_arr[i].name[0] = '\0';
		bit_arr[i].name[0] = '\0';
		hash_arr[i].name[0] = '\0';
	}

	//start command
	while(1){
		fgets(input, sizeof(input), stdin);
		input[strlen(input)-1] = '\0';
		strcpy(input_tmp, input);

		//divide by space
		command = strtok(input_tmp, " ");
		inst1 = strtok(NULL, " ");
		inst2 = strtok(NULL, " ");
		inst3 = strtok(NULL, " ");
		inst4 = strtok(NULL, " ");
		inst5 = strtok(NULL, " ");

/********basic function(create, delete, dumpdata, quit, command)*****/
		if(strcmp(command, "create") == 0){
			//when create list
			if(strcmp(inst1, "list") == 0){
				list_arr[l_cnt].link = list_create();
				list_init(list_arr[l_cnt].link);
				strcpy(list_arr[l_cnt++].name, inst2);	
			}
			//when create hashtable
			else if(strcmp(inst1, "hashtable") == 0){
				hash_arr[h_cnt].link = hash_create();
				hash_init(hash_arr[h_cnt].link, hash_hash_function, hash_less_function, NULL);
				strcpy(hash_arr[h_cnt++].name, inst2);
			}
			//when create bitmap
			else if(strcmp(inst1, "bitmap") == 0){
				sscanf(inst3, "%zu", &size);
				bit_arr[b_cnt].link = bitmap_create(size);
				strcpy(bit_arr[b_cnt++].name, inst2);
			}
		}
		else if(strcmp(command, "delete") == 0){
			for(i=0; i<10; i++){
				if(strcmp(list_arr[i].name, inst1) == 0){
					for(j=i; j<9; j++){
						list_arr[j] = list_arr[j+1];
					}
					break;
				}
				else if(strcmp(bit_arr[i].name, inst1) == 0){
					bitmap_destroy(bit_arr[i].link);
					break;
				}
			}
			
		}
		else if(strcmp(command, "dumpdata") == 0){
			for(i=0; i<10; i++){
				if(strcmp(list_arr[i].name, inst1) == 0){
					dumpdata_list(inst1);
					break;
				}
				else if(strcmp(hash_arr[i].name, inst1) == 0){
					dumpdata_hash(inst1);
					break;
				}
				else if(strcmp(bit_arr[i].name, inst1) == 0){
					dumpdata_bitmap(inst1);
					break;
				}
			}
		}
		else if(strcmp(command, "quit") == 0){
			break;
		}
/***************************list function***************************/
		else if(strcmp(command, "list_insert") == 0){
			struct list_elem *before;
			struct list_data *d = list_data_create();

			l_idx = find_idx('l', l_cnt, inst1);

			sscanf(inst2, "%d", &a);
			sscanf(inst3, "%d", &b);
			
			if(&(list_arr[l_idx].link->head) == list_arr[l_idx].link->tail.prev){
				before = &(d->e);
				list_push_back(list_arr[l_idx].link, before);
				list_arr[l_idx].link->head.next = before;
				list_arr[l_idx].link->tail.prev = before;
			}
			else{
				before = list_begin(list_arr[l_idx].link);
				for(i=0; i<a; i++){
					before = before->next;
				}
				list_insert(before, &(d->e));
			}

			d = list_entry(&(d->e), struct list_data, e);
			d->data = b;
		}
		else if(strcmp(command, "list_splice") == 0){
			struct list_elem *before, *first, *last;
		
			i = find_idx('l', l_cnt, inst1);
			j = find_idx('l', l_cnt, inst3);

			sscanf(inst2, "%d", &a);
			sscanf(inst4, "%d", &b);
			sscanf(inst5, "%d", &c);

			before = list_begin(list_arr[i].link);
			first = list_begin(list_arr[j].link);
			last = list_begin(list_arr[j].link);

			for(k=0; k<a; k++){
				before = before->next;
			}

			for(k=0; k<b; k++){
				first = first->next;
			}

			for(k=0; k<c; k++){
				last = last->next;
			}

			list_splice(before, first, last);
		}
		else if(strcmp(command, "list_push_front") == 0){
			struct list_data *d = list_data_create();

			l_idx = find_idx('l', l_cnt, inst1);

			sscanf(inst2, "%d", &a);
			list_push_front(list_arr[l_idx].link, &(d->e));

			d = list_entry(&(d->e), struct list_data, e);
			d->data = a;
		}
		else if(strcmp(command, "list_push_back") == 0){
			struct list_data *d = list_data_create();

			l_idx = find_idx('l', l_cnt, inst1);

			sscanf(inst2, "%d", &a);
			list_push_back(list_arr[l_idx].link, &(d->e));

			d = list_entry(&(d->e), struct list_data, e);
			d->data = a;
		}
		else if(strcmp(command, "list_remove") == 0){
			struct list_elem *el;

			l_idx = find_idx('l', l_cnt, inst1);

			sscanf(inst2, "%d", &a);

			el = list_begin(list_arr[l_idx].link);
			for(i=0; i<a; i++){
				el = el->next;
			}

			list_remove(el);
		}
		else if(strcmp(command, "list_pop_front") == 0){
			l_idx = find_idx('l', l_cnt, inst1);

			list_pop_front(list_arr[l_idx].link);
		}
		else if(strcmp(command, "list_pop_back") == 0){
			l_idx = find_idx('l', l_cnt, inst1);

			list_pop_back(list_arr[l_idx].link);
		}
		else if(strcmp(command, "list_front") == 0){
			struct list_elem *el;
			struct list_data *d;

			l_idx = find_idx('l', l_cnt, inst1);

			el = list_front(list_arr[l_idx].link);
			d = list_entry(el, struct list_data, e);
			
			printf("%d\n", d->data);
		}
		else if(strcmp(command, "list_back") == 0){
			struct list_elem *el;
			struct list_data *d;

			l_idx = find_idx('l', l_cnt, inst1);

			el = list_back(list_arr[l_idx].link);
			d = list_entry(el, struct list_data, e);

			printf("%d\n", d->data);
		}
		else if(strcmp(command, "list_size") == 0){
			l_idx = find_idx('l', l_cnt, inst1);

			printf("%zu\n", list_size(list_arr[l_idx].link));
		}
		else if(strcmp(command, "list_empty") == 0){
			l_idx = find_idx('l', l_cnt, inst1);
			
			result = list_empty(list_arr[l_idx].link);
			if(result == true) printf("true\n");
			else printf("false\n");
		}
		else if(strcmp(command, "list_reverse") == 0){
			l_idx = find_idx('l', l_cnt, inst1);

			list_reverse(list_arr[l_idx].link);
		}
		else if(strcmp(command, "list_sort") == 0){
			l_idx = find_idx('l', l_cnt, inst1);

			list_sort(list_arr[l_idx].link, less, NULL);
		}
		else if(strcmp(command, "list_insert_ordered") == 0){
			struct list_data *d = list_data_create();

			l_idx = find_idx('l', l_cnt, inst1);

			sscanf(inst2, "%d", &a);
			
			d->data = a;
			list_insert_ordered(list_arr[l_idx].link, &(d->e), less, NULL);
		}
		else if(strcmp(command, "list_unique") == 0){
			l_idx = find_idx('l', l_cnt, inst1);

			if(inst2 == NULL){
				list_unique(list_arr[l_idx].link, NULL, less, NULL);
			}
			else{
				i = find_idx('l', l_cnt, inst2);
				list_unique(list_arr[l_idx].link, list_arr[i].link, less, NULL);
			}
		}
		else if(strcmp(command, "list_max") == 0){
			struct list_elem *el;
			struct list_data *d;

			l_idx = find_idx('l', l_cnt, inst1);

			el = list_max(list_arr[l_idx].link, less, NULL);
			d = list_entry(el, struct list_data, e);
			printf("%d\n", d->data);
		}
		else if(strcmp(command, "list_min") == 0){
			struct list_elem *el;
			struct list_data *d;

			l_idx = find_idx('l', l_cnt, inst1);

			el = list_min(list_arr[l_idx].link, less, NULL);
			d = list_entry(el, struct list_data, e);
			printf("%d\n", d->data);
		}
		else if(strcmp(command, "list_swap") == 0){
			struct list_elem *el1, *el2;

			l_idx = find_idx('l', l_cnt, inst1);

			el1 = list_begin(list_arr[l_idx].link);
			el2 = list_begin(list_arr[l_idx].link);

			sscanf(inst2, "%d", &a);
			sscanf(inst3, "%d", &b);

			for(i=0; i<a; i++){
				el1 = el1->next;
			}
			for(i=0; i<b; i++){
				el2 = el2->next;
			}

			list_swap(el1, el2);
		}
		else if(strcmp(command, "list_shuffle") == 0){
			l_idx = find_idx('l', l_cnt, inst1);

			list_shuffle(list_arr[l_idx].link);
		}
/************************hash function****************************/
		else if(strcmp(command, "hash_insert") == 0){
			struct hash_data *d = hash_data_create();

			h_idx = find_idx('h', h_cnt, inst1);
			sscanf(inst2, "%d", &(d->data));

			hash_insert(hash_arr[h_idx].link, &(d->e));
		}
		else if(strcmp(command, "hash_replace") == 0){
			struct hash_data *d = hash_data_create();

			h_idx = find_idx('h', h_cnt, inst1);
			sscanf(inst2, "%d", &(d->data));

			hash_replace(hash_arr[h_idx].link, &(d->e));
		}
		else if(strcmp(command, "hash_find") == 0){
			struct hash_data *d = hash_data_create();

			h_idx = find_idx('h', h_cnt, inst1);
			sscanf(inst2, "%d", &(d->data));
	
			if(hash_find(hash_arr[h_idx].link, &(d->e)) != NULL)
				printf("%d\n", d->data);
		}
		else if(strcmp(command, "hash_delete") == 0){
			struct hash_data *d = hash_data_create();

			h_idx = find_idx('h', h_cnt, inst1);
			sscanf(inst2, "%d", &(d->data));

			hash_delete(hash_arr[h_idx].link, &(d->e));
		}
		else if(strcmp(command, "hash_clear") == 0){
			h_idx = find_idx('h', h_cnt, inst1);

			hash_clear(hash_arr[h_idx].link, hash_destructor);
		}
		else if(strcmp(command, "hash_size") == 0){
			h_idx = find_idx('h', h_cnt, inst1);

			printf("%zu\n", hash_size(hash_arr[h_idx].link));
		}
		else if(strcmp(command, "hash_empty") == 0){
			h_idx = find_idx('h', h_cnt, inst1);

			if(hash_empty(hash_arr[h_idx].link) == true)
				printf("true\n");
			else printf("false\n");
		}
		else if(strcmp(command, "hash_apply") == 0){
			h_idx = find_idx('h', h_cnt, inst1);

			if(strcmp(inst2, "square") == 0)
				hash_apply(hash_arr[h_idx].link, hash_square);
			else if(strcmp(inst2, "triple") == 0)
				hash_apply(hash_arr[h_idx].link, hash_triple);
		}
/*************************bitmap function***************************/
		else if(strcmp(command, "bitmap_size") == 0){
			b_idx = find_idx('b', b_cnt, inst1);

			size = bitmap_size(bit_arr[b_idx].link);
			printf("%zu\n", size);
		}
		else if(strcmp(command, "bitmap_set") == 0){
			b_idx = find_idx('b', b_cnt, inst1);
		
			sscanf(inst2, "%zu", &tmp1);

			if(strcmp(inst3, "true") == 0){
				bitmap_set(bit_arr[b_idx].link, tmp1, true);
			}
			else if(strcmp(inst3, "false") == 0){
				bitmap_set(bit_arr[b_idx].link, tmp1, false);
			}
		}
		else if(strcmp(command, "bitmap_mark") == 0){
			b_idx = find_idx('b', b_cnt, inst1);

			sscanf(inst2, "%zu", &tmp1);

			bitmap_mark(bit_arr[b_idx].link, tmp1);
		}
		else if(strcmp(command, "bitmap_reset") == 0){
			b_idx = find_idx('b', b_cnt, inst1);

			sscanf(inst2, "%zu", &tmp1);

			bitmap_reset(bit_arr[b_idx].link, tmp1);
		}
		else if(strcmp(command, "bitmap_flip") == 0){
			b_idx = find_idx('b', b_cnt, inst1);

			sscanf(inst2, "%zu", &tmp1);

			bitmap_flip(bit_arr[b_idx].link, tmp1);
		}
		else if(strcmp(command, "bitmap_test") == 0){
			b_idx = find_idx('b', b_cnt, inst1);

			sscanf(inst2, "%zu", &tmp1);

			if(bitmap_test(bit_arr[b_idx].link, tmp1) == true)
				printf("true\n");
			else
				printf("false\n");
		}
		else if(strcmp(command, "bitmap_set_all") == 0){
			b_idx = find_idx('b', b_cnt, inst1);

			if(strcmp(inst2, "true") == 0){
				bitmap_set_all(bit_arr[b_idx].link, true);
			}
			else if(strcmp(inst2, "false") == 0){
				bitmap_set_all(bit_arr[b_idx].link, false);
			}
		}
		else if(strcmp(command, "bitmap_set_multiple") == 0){
			b_idx = find_idx('b', b_cnt, inst1);

			sscanf(inst2, "%zu", &tmp1);
			sscanf(inst3, "%zu", &tmp2);

			if(strcmp(inst4, "true") == 0){
				bitmap_set_multiple(bit_arr[b_idx].link, tmp1, tmp2, true);
			}
			else if(strcmp(inst4, "false") == 0){
				bitmap_set_multiple(bit_arr[b_idx].link, tmp1, tmp2, false);
			}
		}
		else if(strcmp(command, "bitmap_count") == 0){
			b_idx = find_idx('b', b_cnt, inst1);

			sscanf(inst2, "%zu", &tmp1);
			sscanf(inst3, "%zu", &tmp2);

			if(strcmp(inst4, "true") == 0){
				tmp3 = bitmap_count(bit_arr[b_idx].link, tmp1, tmp2, true);
			}
			else if(strcmp(inst4, "false") == 0){
				tmp3 = bitmap_count(bit_arr[b_idx].link, tmp1, tmp2, false);
			}

			printf("%zu\n", tmp3);
		}
		else if(strcmp(command, "bitmap_contains") == 0){
			b_idx = find_idx('b', b_cnt, inst1);

			sscanf(inst2, "%zu", &tmp1);
			sscanf(inst3, "%zu", &tmp2);

			if(strcmp(inst4, "true") == 0){
				result = bitmap_contains(bit_arr[b_idx].link, tmp1, tmp2, true);
			}
			else if(strcmp(inst4, "false") == 0){
				result = bitmap_contains(bit_arr[b_idx].link, tmp1, tmp2, false);
			}

			if(result == true) printf("true\n");
			else if(result == false) printf("false\n");
		}
		else if(strcmp(command, "bitmap_any") == 0){
			b_idx = find_idx('b', b_cnt, inst1);

			sscanf(inst2, "%zu", &tmp1);
			sscanf(inst3, "%zu", &tmp2);

			result = bitmap_any(bit_arr[b_idx].link, tmp1, tmp2);

			if(result == true) printf("true\n");
			else if(result == false) printf("false\n");
		}
		else if(strcmp(command, "bitmap_none") == 0){
			b_idx = find_idx('b', b_cnt, inst1);

			sscanf(inst2, "%zu", &tmp1);
			sscanf(inst3, "%zu", &tmp2);

			result = bitmap_none(bit_arr[b_idx].link, tmp1, tmp2);

			if(result == true) printf("true\n");
			else if(result == false) printf("false\n");
		}
		else if(strcmp(command, "bitmap_all") == 0){
			b_idx = find_idx('b', b_cnt, inst1);

			sscanf(inst2, "%zu", &tmp1);
			sscanf(inst3, "%zu", &tmp2);

			result = bitmap_all(bit_arr[b_idx].link, tmp1, tmp2);

			if(result == true) printf("true\n");
			else if(result == false) printf("false\n");
		}
		else if(strcmp(command, "bitmap_scan") == 0){
			b_idx = find_idx('b', b_cnt, inst1);

			sscanf(inst2, "%zu", &tmp1);
			sscanf(inst3, "%zu", &tmp2);

			if(strcmp(inst4, "true") == 0){
				tmp4 = bitmap_scan(bit_arr[b_idx].link, tmp1, tmp2, true);
			}
			else if(strcmp(inst4, "false") == 0){
				tmp4 = bitmap_scan(bit_arr[b_idx].link, tmp1, tmp2, false);
			}

			printf("%u\n", tmp4);
		}
		else if(strcmp(command, "bitmap_scan_and_flip") == 0){
			b_idx = find_idx('b', b_cnt, inst1);

			sscanf(inst2, "%zu", &tmp1);
			sscanf(inst3, "%zu", &tmp2);

			if(strcmp(inst4, "true") == 0){
				tmp4 = bitmap_scan_and_flip(bit_arr[b_idx].link, tmp1, tmp2, true);
			}
			else if(strcmp(inst4, "false") == 0){
				tmp4 = bitmap_scan_and_flip(bit_arr[b_idx].link, tmp1, tmp2, false);
			}

			printf("%u\n", tmp4);
		}
		else if(strcmp(command, "bitmap_dump") == 0){
			b_idx = find_idx('b', b_cnt, inst1);

			bitmap_dump(bit_arr[b_idx].link);
		}
		else if(strcmp(command, "bitmap_expand") == 0){
			b_idx = find_idx('b', b_cnt, inst1);

			sscanf(inst2, "%d", &i);

			bit_arr[b_idx].link = bitmap_expand(bit_arr[b_idx].link, i);
		}
	}

	return 0;
}
