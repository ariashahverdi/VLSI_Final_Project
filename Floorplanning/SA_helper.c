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
	struct module_dim *temp1;
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


	if((fp = fopen("data/gcc.ptrace","w")) == NULL){
		printf("\nCould not create out_design.flp file!\n");
		exit(1);
	}
	for(int i=0;i<module_count;i++){
		fprintf(fp,"%d ", i+1);
	}
	fprintf(fp,"\n");
	for(int i=0;i<module_count;i++){
		fprintf(fp,"%f ", power[i]);
	}
	fprintf(fp,"\n");
	fclose(fp);

	return module_count;
}



/* generats optimal design output file */
void optimal_design(int module_count, struct cost cost, struct module_dim **module_array, int *polish_exp)
{
	FILE *fp;
	int i;
	struct module *head;
	struct module_dim *temp_module;
	char buf[module_count][100];
	int verticle, horizontal;

	verticle = module_count + 1;
	horizontal = module_count + 2;


	if((fp = fopen("data/ev6.flp","w")) == NULL){
		printf("\nCould not create out_design.flp file!\n");
		exit(1);
	}

	 head = cost.final_modules;

	head = cost.final_modules;
	/* Change Polish to Axes*/
	int mod_cnt = 0;
	for(i=0; i<(2*module_count-1); i++){
		if(verticle == polish_exp[i])
			mod_cnt++;
         else if(horizontal == polish_exp[i])
            mod_cnt++;
         else{
        	//printf("%d\n",polish_exp[mod_cnt]);
        	temp_module = module_array[(head[polish_exp[mod_cnt]-1].module)-1];
			temp_module->x_axis = (rand()%100)/1000;
			temp_module->y_axis = (rand()%100)/1000;


        	 mod_cnt++;
          }

	}
	printf("\n\n ** Done **\n");
	/*
	for(i=0; i<(2*module_count-1); i++){
		if(verticle == polish_exp[i])
                        fputc('V', fp);
                else if(horizontal == polish_exp[i])
                        fputc('H',fp);
                else
                        fprintf(fp,"%d", polish_exp[i]);
	}
	fprintf(fp,"\n");
        fprintf(fp,"Total Area: %f\n", cost.area);
        */
        for(i=0; i<module_count; i++){
                temp_module = module_array[(head[i].module)-1];
                while(temp_module != NULL){
                        if(temp_module->size_no == head[i].size){
                        	//printf("i: %d  (%d , %d)\n",i,temp_module->size_no,head[i].size);
                        	//printf("i: %d  (%d, %d , %d)\n",i,head[i].module,temp_module->w,temp_module->h);
				sprintf(buf[(head[i].module)-1],"%d %f %f %f %f\n",(head[i].module), temp_module->w, temp_module->h, temp_module->x_axis, temp_module->y_axis);
                //sprintf(buf[(head[i].module)-1],"%d:\t%d %d \n",(head[i].module), temp_module->w, temp_module->h);
			}
                        temp_module = temp_module->next;
                }
	}

	for(i=0; i<module_count; i++){
		fprintf(fp, buf[i]);
	}	

	
	fclose(fp);

}

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




