#include"SA_helper.h"


/*  generats a random polish expression. to have a equal probability for vertical  *
 *  and horizontal section, number space selected double of the module number.	   *
 *  second half of the number space divided equally between H and V	           */
int * getRandom(int module_count)
{
  int  *r;
  int i,j;
  int duplicate = 1;
  int symbol = 1;
  int random_no;
  int digit_count = 2;
  int symbol_count = 0;

        r = (int*)malloc(((module_count * 2) - 1) * sizeof(int));
        r[0] = (rand() % module_count) + 1;
        while((r[1] = (rand() % module_count) + 1) == r[0]);
	/* start filling polish expression */
        for(i=2; i<((2*module_count)-1); i++){
		/* generats number accross entire number space*/
		random_no =  (rand() % (module_count * 2)) + 1;
		/* detect number belongs to module number*/
		if(random_no < (module_count+1)){
			digit_count++;

			if(digit_count>module_count){
                                i--;
                                continue;
                        }
			/*check duplicate number*/
			while(duplicate){
                                duplicate = 0;
                                r[i]  = (rand() % module_count) + 1;
                                for(j=0; j<i; j++){

                                        if(r[j] == r[i])
                                                duplicate = 1;
                                }
                        }
                        duplicate = 1;
                        symbol = 1;
		}
		/* numbers associated with V and H*/
		else{
			symbol_count++;
			/*Symbol should be n-1*/
			if(symbol_count > digit_count || (symbol_count == digit_count)){
				symbol_count--;
				i--;
				continue;
			}
			/*numberspace devides eqaully between V and H for fair probability*/
		 	if(random_no < (module_count+(module_count/2)+1))
				r[i] = module_count+1;
			else
				r[i] = module_count+2;
       		}
        }
	return r;
}


void print_polish(int module_count, int * polish_exp){
    int i;
    int vertical = module_count + 1;
    int horizontal = module_count + 2;
    for(i=0;i<2*module_count-1;i++){
        if(polish_exp[i] == vertical) printf(" V ");
        else if(polish_exp[i] == horizontal) printf(" H ");
        else printf(" %d ", polish_exp[i]);
    }
    printf("\n");
}


int * init_polish(int module_count){
	int * new_polish;
	int i;
	int vertical = module_count + 1;
	int horizontal = module_count + 2;
	new_polish = (int*)malloc(((module_count * 2) - 1) * sizeof(int));
	int zero_cnt = 0;
	int one_cnt = 0;

	int idx;
	int flag[100], cand_idx[100];
	for(i=0;i<module_count;i++) flag[i] = 1;
	for(i=0;i<module_count;i++) {
		idx = rand()%module_count;
		while(flag[idx] == 0) idx=(idx+1)%module_count;
		flag[idx] = 0;
		cand_idx[i] = idx+1;
#ifdef PRINT
		printf("idx: %d \n", idx);
#endif
	}


	int place_this = 0;
	for(i=0;i<2*module_count-1;i++){
		if(zero_cnt+1 >= one_cnt) {
			new_polish[i] = cand_idx[place_this];
			place_this++;
			one_cnt++;
		}
		else{
			int selection = rand()%2;
			if(selection == 0 || one_cnt == module_count){
				zero_cnt++;
                if(new_polish[i-1] == vertical) new_polish[i] = horizontal;
                else if(new_polish[i-1] == horizontal) new_polish[i] = vertical;
                else {
                    int rot = rand()%2;
                    if(rot == 0) new_polish[i] = vertical;
                    else new_polish[i] = horizontal;
                }
			}
			else{
				new_polish[i] = cand_idx[place_this];
				place_this++;
				one_cnt++;
			}
		}
	}
	return new_polish;
}

int * smart_move(int module_count,int *polish_exp,  int move, int *done)
{
    
    int * new_polish;
    int i;
    new_polish = (int*)malloc(((module_count * 2) - 1) * sizeof(int));
    int vertical, horizontal;
    vertical = module_count + 1;
    horizontal = module_count + 2;
    *done = 0;
    //int move_num = rand()%MOVE;
    int move_num = move;
    int idx = rand()%(2*module_count-1);
    int idx1, idx2;
    idx1 = idx;
    int loop_check = 0;
    //remove for original code
    
    //printf("move is : %d, idx is : %d\n",move_num, idx);
    //print_polish(module_count, polish_exp);
    for(i=0;i<2*module_count-1;i++){new_polish[i] = polish_exp[i];}
    switch(move_num){
        case 0: //Move1 : (Operand Swap) Swap two adjacent operands OK!
            while(1){
                idx1 = idx1%(2*module_count-1);
                if(new_polish[idx1] != vertical){
                    if(new_polish[idx1] != horizontal){ //just a number
                        //look for idx2 then
                        idx2 = idx1 + 1;
                        while(1){
                            idx2 = idx2%(2*module_count-1);
                            if(new_polish[idx2] != vertical){
                                if(new_polish[idx2] != horizontal){ //just a number
                                    break;
                                }
                                else{idx2++;} //it was horizontal
                            }
                            else{idx2++;}
                        }
                        if(idx2>idx1) break;
                        idx1 = 0; //we reach end of polish so we start from beginning
                    }
                    else{idx1++;} //it was horizontal
                }
                else{idx1++;}
            }
            int temp = new_polish[idx1];
            new_polish[idx1] = new_polish[idx2];
            new_polish[idx2] = temp;
#ifdef PRINT
            printf("idx1: %d, idx2 : %d\n",idx1,idx2);
            printf("I did Move 1\n");
#endif
            
#ifdef DEBUG
            printf("** M1 **\n");
#endif
            *done = 1;
            break;
            
        case 1: //Move2 : (Chain Invert) Complement some chain OK!
            while(1){
                //////////// safety check /////////////
                loop_check++;
                if(loop_check>2*module_count) break;
                ///////////////////////////////////////
                idx1 = idx1%(2*module_count-1);
                if(new_polish[idx1] != vertical){
                    if(new_polish[idx1] != horizontal){idx1++;} //just a number
                    else {//it was horizontal move so we make it vertical
                        idx2 = idx1;
                        while(1){
                            if (idx1-1 >= 0){
                                if( (new_polish[idx1-1] != new_polish[idx1]) && (new_polish[idx1-1] == horizontal ||new_polish[idx1-1] == vertical))
                                    idx1--;
                                else {break;}
                            }
                            else {break;}
                        }
                        while(1){
                            if (idx2+1 != 2*module_count -1){
                                if( (new_polish[idx2+1] != new_polish[idx2]) && (new_polish[idx2+1] == horizontal ||new_polish[idx2+1] == vertical))
                                    idx2++;
                                else {break;}
                            }
                            else {break;}
                        }
                        for(i=idx1;i<=idx2;i++){
                            if(new_polish[i] == vertical) new_polish[i] = horizontal;
                            else if(new_polish[i] == horizontal) new_polish[i] = vertical;
                        }
                        break;
                    }
                }
                else{ //it was vertical move so we make it horizontal
                    idx2 = idx1;
                    while(1){
                        if (idx1-1 >= 0){
                            if( (new_polish[idx1-1] != new_polish[idx1]) && (new_polish[idx1-1] == horizontal ||new_polish[idx1-1] == vertical))
                                idx1--;
                            else {break;}
                        }
                        else {break;}
                    }
                    while(1){
                        if (idx2+1 != 2*module_count -1){
                            if( (new_polish[idx2+1] != new_polish[idx2]) && (new_polish[idx2+1] == horizontal ||new_polish[idx2+1] == vertical))
                                idx2++;
                            else {break;}
                        }
                        else {break;}
                    }
                    for(i=idx1;i<=idx2;i++){
                        if(new_polish[i] == vertical) new_polish[i] = horizontal;
                        else if(new_polish[i] == horizontal) new_polish[i] = vertical;
                    }
                    break;
                }
            }
            
#ifdef PRINT
            printf("idx1: %d, idx2 : %d\n",idx1,idx2);
            printf("I did Move 2\n");
#endif
            
#ifdef DEBUG
            printf("** M2 **\n");
#endif
            *done = 1;
            break;
            
        case 2: //Move3: (Operator/Operand Swap) Swap two adjacent operand and operator Needs to be checked!
            while(1){
                //////////// safety check /////////////
                loop_check++;
                if(loop_check>2*module_count) break;
                ///////////////////////////////////////
                idx = idx%(2*module_count-1);
                if(new_polish[idx] != vertical){
                    if(new_polish[idx] != horizontal){//just a number
                        if(new_polish[idx+1] == horizontal || new_polish[idx+1] == vertical){//ok to move, check conditions
                            int Nk = 0;
                            for(i=0;i<=idx+1;i++){
                                if(new_polish[i] == horizontal || new_polish[i] == vertical){Nk++;}
                            }
                            if(idx+1>2*Nk && new_polish[idx-1] != new_polish[idx+1]) {//since it's from zero!!
                                int temp = new_polish[idx];
                                new_polish[idx] = new_polish[idx+1];
                                new_polish[idx+1] = temp;
#ifdef PRINT
                                printf("idx: %d\n",idx);
                                printf("I did Move 3\n");
#endif
                                
#ifdef DEBUG
                                printf("** M3 **\n");
#endif
                                *done = 1;
                                break;
                            }
                            idx++;
                        }
                        else{idx++;}
                    }
                    else {idx++;}//it was horizontal move so we make it vertical
                }
                else{idx++;}//it was vertical move so we make it horizontal
            }
            break;
    }
    
    return new_polish;
}

/*sorts sizes of a given module accross their width*/
void sort_w(struct slicing_cntr *slice_cntr)
{
	int didSwap = 1;
	struct module_dim *head;
	struct module_dim *curr;
	struct module_dim *tail;
	struct module_dim *temp;
	head = slice_cntr->list;
	while(didSwap) {
        	didSwap = 0;
        	for(curr = head, tail = NULL; curr->next != NULL; tail = curr, curr = curr->next) {
                	if(curr->w < curr->next->w) {
                            temp = curr;
			    if(curr == head)
				head = curr->next;
			    curr = curr->next;
			    temp->next = curr->next;
                            curr->next = temp;
			    if(tail != NULL)
				tail->next = curr;
                            didSwap = 1;
                	}
           	}
	}
	slice_cntr->list = head;
}


/*sorts sizes of input module accros their height*/
void sort_h(struct slicing_cntr *slice_cntr)
{
	int didSwap = 1;
	struct module_dim *head;
	struct module_dim *curr;
	struct module_dim *tail;
	struct module_dim *temp;
	head = slice_cntr->list;
	while(didSwap) {
        	didSwap = 0;
        	for(curr = head, tail = NULL; curr->next != NULL; tail = curr, curr = curr->next) {
                	if(curr->h < curr->next->h) {
                            temp = curr;
			    if(curr == head)
				head = curr->next;
			    curr = curr->next;
			    temp->next = curr->next;
                            curr->next = temp;
			    if(tail != NULL)
				tail->next = curr;
                            didSwap = 1;
                	}
           	}
	}
	slice_cntr->list = head;
}


/*selects smallest size among the short listed sizes in sizing slicing process*/
struct small_size get_small_size(struct small_size *small_size_head)
{

	int didSwap = 1;
	struct small_size *head;
	struct small_size *curr;
	struct small_size *tail;
	struct small_size *temp;
	head = small_size_head;
	while(didSwap) {
        	didSwap = 0;
        	for(curr = head, tail = NULL; curr->next != NULL; tail = curr, curr = curr->next) {
                	if(((curr->h)*(curr->w)) > ((curr->next->h)*(curr->next->w))) {
                            temp = curr;
			    if(curr == head)
				head = curr->next;
			    curr = curr->next;
			    temp->next = curr->next;
                            curr->next = temp;
			    if(tail != NULL)
				tail->next = curr;
                            didSwap = 1;
                	}
           	}
	}

	return *head;
}


/* implementation of sizing slicing algorithm***********************************
 * input: struct module** : structure contaning all module and their sizes     *
 *        polish expression, number of modules                                 *
 * output: struct cost: structure holds area and floorplan                     */
struct cost sizing_slicing(struct module_dim **module_array, int *polish_array, int module_count)
{
	int i;
	int vertical = module_count + 1;
	int horizontal = module_count + 2;
	int breakall = 0;
	struct slicing_cntr *head = NULL;
	struct slicing_cntr *curr = NULL;
	struct module_dim *temp;
	struct module_dim *curr_list;
	struct module_dim *curr_list_head;
	struct module_dim *prev_list;
	struct module_dim *smallest_list;
	struct small_size *small_size_head = NULL;
	struct small_size *small_size_temp;
	struct small_size smallest_size;
	struct module *small_modules;
	struct module *small_modules_temp;
	struct cost cost;

	small_modules = (struct module*)malloc(module_count*sizeof(struct module));
	small_modules_temp = small_modules;

	for(i=0; i<((2*module_count) - 1); i++){
		/*fills polish expression if digit detected*/
		if((polish_array[i] != vertical) && (polish_array[i] != horizontal)){
			if(head == NULL){
				head = (struct slicing_cntr*)malloc(sizeof(struct slicing_cntr));
				head->list = (struct module_dim*)malloc(sizeof(struct module_dim));
				temp = head->list;
				curr_list = module_array[polish_array[i] - 1];

				while(curr_list != NULL){

					temp->w = curr_list->w;
					temp->h = curr_list->h;
					temp->module_no = curr_list->module_no;
					temp->size_no = curr_list->size_no;
					temp->next = (struct module_dim*)malloc(sizeof(struct module_dim));
					prev_list = temp;
					temp = temp->next;
					curr_list = curr_list->next;
				}

				prev_list->next = NULL;

				head->next = NULL;
				head->prev = NULL;
				curr = head;

			}
			else{

				curr->next = (struct slicing_cntr*)malloc(sizeof(struct slicing_cntr));

				curr->next->list = (struct module_dim*)malloc(sizeof(struct module_dim));
				temp = curr->next->list;
				curr_list = module_array[polish_array[i] - 1];


				while(curr_list != NULL){

					temp->w = curr_list->w;
					temp->h = curr_list->h;
					temp->module_no = curr_list->module_no;
					temp->size_no = curr_list->size_no;
					temp->next = (struct module_dim*)malloc(sizeof(struct module_dim));
					prev_list = temp;
					temp = temp->next;
					curr_list = curr_list->next;
				}

				prev_list->next = NULL;
				curr->next->next = NULL;
				curr->next->prev = curr;
				curr = curr->next;

			}
		}
		/* pop sizes of last two modules and perform selection and rejection of combined size
		   used in sizing and slicing algorithm*/
		else{
			if(polish_array[i] == vertical){

				sort_h(curr);
				sort_h(curr->prev);

				curr_list_head = curr->list;

				prev_list = curr->prev->list;
				while(prev_list != NULL){
					curr_list = curr_list_head;
					while(curr_list != NULL){
						if(small_size_head == NULL){
							small_size_head = (struct small_size*)malloc(sizeof(struct small_size));
							small_size_temp = small_size_head;
						}
						else{
							small_size_temp->next = (struct small_size*)malloc(sizeof(struct small_size));
							small_size_temp = small_size_temp->next;
						}
						small_size_temp->w = (curr_list->w) + (prev_list->w);
						small_size_temp->h = (curr_list->h > prev_list->h) ? curr_list->h : prev_list->h;
						small_size_temp->col_module_no = curr_list->module_no;
						small_size_temp->col_size_no = curr_list->size_no;
						small_size_temp->row_module_no = prev_list->module_no;
						small_size_temp->row_size_no = prev_list->size_no;
						small_size_temp->next = NULL;
						if(curr_list->h > prev_list->h)
							curr_list_head = curr_list->next;
						else if(curr_list->h < prev_list->h)
							break;
						else{
							breakall = 1;
							break;
						}

						curr_list = curr_list->next;
					}
					if(breakall)
						break;
					prev_list = prev_list->next;
				}
				breakall = 0;

				/*find the smallest combination*/
				smallest_size = get_small_size(small_size_head);
				smallest_list = (struct module_dim*)malloc(sizeof(struct module_dim));
				smallest_list->w = smallest_size.w;
				smallest_list->h = smallest_size.h;
				smallest_list->module_no = 0;
				smallest_list->size_no = 0;
				smallest_list->next = NULL;
				/*store selected sizes*/
				if(smallest_size.col_module_no != 0){
					small_modules_temp->module = smallest_size.col_module_no;
					small_modules_temp->size = smallest_size.col_size_no;
					small_modules_temp++;
				}
				if(smallest_size.row_module_no != 0){
					small_modules_temp->module = smallest_size.row_module_no;
 					small_modules_temp->size = smallest_size.row_size_no;
					small_modules_temp++;
				}

				//----todo: free prev list
				/*temp = curr->prev->list;
				while(temp!=NULL){
					temp1 = temp;
					temp = temp->next;
					free(temp1);
				}*/

				curr->prev->list = smallest_list;
				curr = curr->prev;

				//----todo: free curr->list
				free(curr->next);
				curr->next = NULL; //----------------new line
				free(small_size_head);
				small_size_head = NULL;

			}
			else{
				sort_w(curr);
				sort_w(curr->prev);
				temp = curr->list;

				curr_list_head = curr->list;

				prev_list = curr->prev->list;
				while(prev_list != NULL){
					curr_list = curr_list_head;
					while(curr_list != NULL){
						if(small_size_head == NULL){
							small_size_head = (struct small_size*)malloc(sizeof(struct small_size));
							small_size_temp = small_size_head;
						}
						else{
							small_size_temp->next = (struct small_size*)malloc(sizeof(struct small_size));
							small_size_temp = small_size_temp->next;
						}
						small_size_temp->h = (curr_list->h) + (prev_list->h);
						small_size_temp->w = (curr_list->w > prev_list->w) ? curr_list->w : prev_list->w;
						small_size_temp->col_module_no = curr_list->module_no;
						small_size_temp->col_size_no = curr_list->size_no;
						small_size_temp->row_module_no = prev_list->module_no;
						small_size_temp->row_size_no = prev_list->size_no;
						small_size_temp->next = NULL;
						if(curr_list->w > prev_list->w)
							curr_list_head = curr_list->next;
						else if(curr_list->w < prev_list->w)
							break;
						else{
							breakall = 1;
							break;
						}

						curr_list = curr_list->next;
					}
					if(breakall)
						break;
					prev_list = prev_list->next;
				}
				breakall = 0;

				//------get smallest size of all----------
				smallest_size = get_small_size(small_size_head);
				smallest_list = (struct module_dim*)malloc(sizeof(struct module_dim));
				smallest_list->w = smallest_size.w;
				smallest_list->h = smallest_size.h;
				smallest_list->module_no = 0;
				smallest_list->size_no = 0;
				smallest_list->next = NULL;
				//------------store sizes of individual module-------
				if(smallest_size.col_module_no != 0){
					small_modules_temp->module = smallest_size.col_module_no;
					small_modules_temp->size = smallest_size.col_size_no;
					small_modules_temp++;
				}
				if(smallest_size.row_module_no != 0){
					small_modules_temp->module = smallest_size.row_module_no;
 					small_modules_temp->size = smallest_size.row_size_no;
					small_modules_temp++;
				}
				//-------linklist rearrangment--------------
				/*temp = curr->prev->list;
				while(temp!=NULL){
					temp1 = temp;
					temp = temp->next;
					free(temp1);
				}*/
				curr->prev->list = smallest_list;
				curr = curr->prev;
				free(curr->next);
				curr->next = NULL;
				free(small_size_head);
				small_size_head = NULL;



			}

		}

	}



	cost.area = head->list->w * head->list->h;
	cost.final_modules = small_modules;

	small_modules_temp = small_modules;

	return cost;


}


/* parse input design file. Extract data and store in dynamic data structure*/
int parse_design(char *filename, struct module_dim ***module_array, float *lambda,float *total_size)
{
	FILE *fp;
	int i;
	int module_count=0;
	float power[100];
	float temp_size = 0;
	struct module_dim **module_array_temp;
	struct module_dim *temp_module;
	int module_no = 1;
	if((fp = fopen(filename, "rw+")) == NULL){
		printf("ERROR: File not open!\n");
		exit(1);
	}
	fscanf(fp,"%d\n",&module_count);
    fscanf(fp, "%f\n", lambda);
	*module_array = (struct module_dim**)malloc(module_count * sizeof(struct module_dim*));
	module_array_temp = *module_array;

	float W, H, P;
    while (!feof(fp)) {
        fscanf(fp, "%f %f %f\n", &W, &H, &P);
        temp_size += W*H;
        power[module_no-1] = P;
        //printf("%f %f %f\n", W, H, P);

        temp_module = (struct module_dim*)malloc(sizeof(struct module_dim));
		*module_array_temp = temp_module;

		temp_module->w = W;
		temp_module->h = H;
		temp_module->power = P;
		temp_module->size_no = 1;
		temp_module->module_no = module_no;
		temp_module->x_axis = 0;
		temp_module->y_axis = 0;
		temp_module->next = NULL;
		module_array_temp++;
		module_no++;
    }

	fclose(fp);
	*total_size = temp_size;


	if((fp = fopen("../data/gcc.ptrace","w")) == NULL){
		printf("\nCould not create out_design.flp file!\n");
		exit(1);
	}
	for(i=0;i<module_count;i++){
		fprintf(fp,"%d ", i+1);
	}
	fprintf(fp,"\n");
	for(i=0;i<module_count;i++){
		fprintf(fp,"%f ", power[i]);
	}
	fprintf(fp,"\n");
	fclose(fp);

	return module_count;
}


void polish_2_cord(int module_count, struct module_dim **module_array, int * polish_exp){

    int i;
    struct module_dim *temp_module;
    int vertical, horizontal;
    int saved_polish [2*module_count-1];
    for(i=0; i<(2*module_count-1); i++){saved_polish[i] = polish_exp[i];}
    vertical = module_count + 1;
    horizontal = module_count + 2;
    
    // Change Polish to Axes
    float width[2*module_count-2];
    float height[2*module_count-2];
    int next[2*module_count-1];
    
    for(i=0; i<(2*module_count-1); i++){
        next[i] = i+1;
        if(vertical != polish_exp[i] && horizontal != polish_exp[i] ){
            temp_module = module_array[polish_exp[i]-1];
            width[polish_exp[i]-1] = temp_module->w;
            height[polish_exp[i]-1] = temp_module->h;
        }
    }
    
    next[2*module_count-2] = -1;
    
    int new_node[2*module_count-1];
    int first_node[2*module_count-1];
    int second_node[2*module_count-1];
    int operator_vh[2*module_count-1];
    float x_axis[2*module_count-1];
    float y_axis[2*module_count-1];
    
    int nodectr = module_count;
    int stackptr=0;
    int cur=0;
    int next_1;
    int next_2;
    float w1,w2,h1,h2;
    
    while(next[cur]!= -1){
        next_1 = next[cur];
        next_2 = next[next_1];
        if(vertical == polish_exp[next_2]){
            first_node[stackptr] = polish_exp[cur];
            second_node[stackptr] = polish_exp[next_1];
            operator_vh[stackptr] = vertical;
            new_node[stackptr++] = ++nodectr;
            
            w1 = width[polish_exp[cur]-1];
            w2 = width[polish_exp[next_1]-1];
            h1 = height[polish_exp[cur]-1];
            h2 = height[polish_exp[next_1]-1];
            
            width[nodectr-1] = w1+w2;
            height[nodectr-1] = (h1>h2)? h1 : h2;
            
            polish_exp[cur] = nodectr;
            next[cur] = next[next_2];
            cur=0;
        }
        
        else if(horizontal == polish_exp[next_2]){
            first_node[stackptr] = polish_exp[cur];
            second_node[stackptr] = polish_exp[next_1];
            operator_vh[stackptr] = horizontal;
            new_node[stackptr++] = ++nodectr;
            
            w1 = width[polish_exp[cur]-1];
            w2 = width[polish_exp[next_1]-1];
            h1 = height[polish_exp[cur]-1];
            h2 = height[polish_exp[next_1]-1];
            
            width[nodectr-1] = (w1>w2)? w1 : w2;
            height[nodectr-1] = h1+h2;
            
            polish_exp[cur] = nodectr;
            next[cur] = next[next_2];
            cur=0;
        }
        
        else{
            cur = next_1;
            next_1 = next_2;
            next_2 = next[next_2];
        }
    }
    
    --stackptr;
    x_axis[new_node[stackptr]-1] =0;
    y_axis[new_node[stackptr]-1] =0;
    
    stackptr++;
    
    float x_axis_block, y_axis_block;
    
    
    while(stackptr > 0){
        --stackptr;
        x_axis_block = x_axis[new_node[stackptr]-1];
        y_axis_block = y_axis[new_node[stackptr]-1];
        
        if(operator_vh[stackptr] == vertical){
            x_axis[first_node[stackptr]-1] = x_axis_block;
            x_axis[second_node[stackptr]-1] = x_axis_block + width[first_node[stackptr]-1];
            y_axis[first_node[stackptr]-1] = y_axis_block;
            y_axis[second_node[stackptr]-1] = y_axis_block;
        }
        
        if(operator_vh[stackptr] == horizontal){
            x_axis[first_node[stackptr]-1] = x_axis_block;
            x_axis[second_node[stackptr]-1] = x_axis_block;
            y_axis[first_node[stackptr]-1] = y_axis_block;
            y_axis[second_node[stackptr]-1] = y_axis_block + height[first_node[stackptr]-1];
        }
        
    }
    
    for(i=0; i<(2*module_count-1); i++){polish_exp[i] = saved_polish[i];}
    
    for(i=0; i<module_count; i++){
        temp_module = module_array[i];
        temp_module->x_axis = x_axis[i];
        temp_module->y_axis = y_axis[i];
    }
    
}


/* generats optimal design output file */
float get_area(int module_count, struct module_dim **module_array, int *polish_exp)
{
	int i;
	struct module_dim *temp_module;

    //get x_axis
    polish_2_cord(module_count,module_array,polish_exp);

	temp_module = module_array[0];
	float min_x = temp_module->x_axis;
	float max_x = temp_module->x_axis + temp_module->w;

	for (i=1; i < module_count; i++) {
		temp_module = module_array[i];
		if (temp_module->x_axis < min_x)
			min_x = temp_module->x_axis;
		if (temp_module->x_axis + temp_module->w > max_x)
			max_x = temp_module->x_axis + temp_module->w;
	}

	float w_chip =  (max_x - min_x);

	temp_module = module_array[0];
	float min_y = temp_module->y_axis;
	float max_y = temp_module->y_axis + temp_module->h;

	for (i=1; i < module_count; i++) {
		temp_module = module_array[i];
		if (temp_module->y_axis < min_y)
			min_y = temp_module->y_axis;
		if (temp_module->y_axis + temp_module->h > max_y)
			max_y = temp_module->y_axis + temp_module->h;
	}
	float l_chip =  (max_y - min_y);


	return w_chip * l_chip;
}


/* generats optimal design output file */
void save_design(int module_count, struct module_dim **module_array, int *polish_exp)
{
	FILE *fp;
	int i;
    struct module_dim *temp_module;
	char buf[module_count][100];

	if((fp = fopen("../data/ev6.flp","w")) == NULL){
		printf("\nCould not create out_design.flp file!\n");
		exit(1);
	}

	for(i=0; i<module_count; i++){
		temp_module = module_array[i];
		sprintf(buf[i],"%d %f %f %f %f\n",i+1, temp_module->w, temp_module->h, temp_module->x_axis, temp_module->y_axis);
	}


	for(i=0; i<module_count; i++){
		fprintf(fp, buf[i]);
	}
	fclose(fp);
}


/*
void print_design(int module_count,struct cost cost,struct module_dim **module_array)
{
	printf("Printing Desing...\n");
	int i;
	struct module *head;
	struct module_dim *temp_module;
	head = cost.final_modules;
	for(i=0; i<module_count; i++){
		temp_module = module_array[(head[i].module)-1];
		while(temp_module != NULL){
			if(temp_module->size_no == head[i].size){
				printf("%d:\t%f %f\t %f %f \n",(head[i].module), temp_module->w, temp_module->h, temp_module->x_axis, temp_module->y_axis);
			}
			temp_module = temp_module->next;
		}
	}

}
 */


void save_design_ev6(int module_count, struct module_dim **module_array, int *polish_exp)
{
    FILE *fp;
    int i;
    struct module_dim *temp_module;
    char buf[module_count][100];

    
    if((fp = fopen("../data/ev6.flp","w")) == NULL){
        printf("\nCould not create out_design.flp file!\n");
        exit(1);
    }
    
    polish_2_cord(module_count, module_array, polish_exp);
    
    //head = cost.final_modules;
    
    for(i=0; i<module_count; i++){
        temp_module = module_array[i];
        sprintf(buf[i],"%d %f %f %f %f\n",i+1, temp_module->w, temp_module->h, temp_module->x_axis, temp_module->y_axis);
    }
    
    
    for(i=0; i<module_count; i++){
        fprintf(fp, buf[i]);
    }
    fclose(fp);
    
}


void save_design_ev7(int module_count, struct module_dim **module_array, int *polish_exp)
{
	FILE *fp;
	int i;
	struct module_dim *temp_module;
	char buf[module_count][100];


	if((fp = fopen("../data/ev7.flp","w")) == NULL){
		printf("\nCould not create out_design.flp file!\n");
		exit(1);
	}

	// Change Polish to Axes
    polish_2_cord(module_count,module_array,polish_exp);

	//head = cost.final_modules;

	for(i=0; i<module_count; i++){
		temp_module = module_array[i];
		sprintf(buf[i],"%d %f %f %f %f\n",i+1, temp_module->w, temp_module->h, temp_module->x_axis, temp_module->y_axis);
	}


	for(i=0; i<module_count; i++){
		fprintf(fp, buf[i]);
	}
	fclose(fp);

}


int check_for_overlap(int module_count, struct module_dim **module_array, float lambda, int * polish){
	int i,j;
	struct module_dim *temp_module1;
	struct module_dim *temp_module2;
	int flag =0;

	for(i=0; i<module_count; i++){
		temp_module1 = module_array[i];
		for(j=i+1; j<module_count; j++){
			temp_module2 = module_array[j];
			if((temp_module1 -> x_axis >= temp_module2 -> x_axis + temp_module2->w) || (temp_module2 -> x_axis >= temp_module1 -> x_axis + temp_module1->w))
				{
				if (lambda < 0.2) printf("%d and %d are hortizontaly ok!\n",i,j);
				continue;
				}
			else if((temp_module1 -> y_axis >= temp_module2 -> y_axis + temp_module2->h) || (temp_module2 -> y_axis >= temp_module1 -> y_axis + temp_module1->h))
				{
				if (lambda < 0.2) printf("%d and %d are verticaly ok!\n",i,j);
				continue;
				}
			else {
				printf("%d and %d are not OK!\n",i,j);
				save_design_ev6(module_count, module_array, polish);
				exit(1);

				return 0;}
		}
	}
	return 1;
}


