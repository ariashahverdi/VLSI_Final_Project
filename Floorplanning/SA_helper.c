#include "SA_helper.h"


/*  generats a random polish expression. to have a equal probability for vertical  *
 *  and horizontal section, number space selected float of the module number.	   *
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
        else if(polish_exp[i] == horizontal) printf(" H ",i+1);
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
		idx = (rand()%module_count);
		while(flag[idx] == 0) idx=(idx+1)%module_count;
		flag[idx] = 0;
		cand_idx[i] = idx+1;
#ifdef PRINT
		printf("idx: %d \n", idx);
#endif
	}

	int place_this = 0;
//Balanced
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

	/*// Non Balanced Polish
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
                int rot = rand()%2;
                if(rot == 0) new_polish[i] = vertical;
                else new_polish[i] = horizontal;
			}
			else{
				new_polish[i] = cand_idx[place_this];
				place_this++;
				one_cnt++;
			}
		}
	}
	*/
	return new_polish;
}

int * smart_move(int module_count,int *polish_exp,  int move)
{
    
    int * new_polish;
    int i;
    new_polish = (int*)malloc(((module_count * 2) - 1) * sizeof(int));
    int vertical, horizontal;
    vertical = module_count + 1;
    horizontal = module_count + 2;

    int idx = rand()%(2*module_count-1);
    int idx1, idx2;
    idx1 = idx;
    int loop_check = 0;
    
    for(i=0;i<2*module_count-1;i++){new_polish[i] = polish_exp[i];}
    switch(move){
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
    int my_vertical, my_horizontal;
    int saved_polish [MAX_NUM];
    for(i=0; i<(2*module_count-1); i++){saved_polish[i] = polish_exp[i];}

    my_vertical = module_count + 1;
    my_horizontal = module_count + 2;

    int vertical = -1;
    int horizontal = -2;

    for(i=0; i<(2*module_count-1); i++){
    	if (polish_exp[i] == my_vertical) polish_exp[i] = vertical;
    	if (polish_exp[i] == my_horizontal) polish_exp[i] = horizontal;
    }

    // Change Polish to Axes
    float width[MAX_NUM];
    float height[MAX_NUM];
    int next[MAX_NUM];

    
    for(i=0; i<(2*module_count-1); i++){
        next[i] = i+1;
        if(vertical != polish_exp[i] && horizontal != polish_exp[i] ){
            temp_module = module_array[polish_exp[i]-1];
            width[polish_exp[i]-1] = temp_module->w;
            height[polish_exp[i]-1] = temp_module->h;
        }
    }
    next[2*module_count-2] = -1;
    
    int new_node[MAX_NUM];
    int first_node[MAX_NUM];
    int second_node[MAX_NUM];
    int operator_vh[MAX_NUM];
    float x_axis[MAX_NUM];
    float y_axis[MAX_NUM];
    
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
    
    /*
    float s_sink = 0.60000;
    float s_spreader = 0.30000;
    if (w_chip > s_sink || l_chip > s_sink || w_chip > s_spreader || l_chip > s_spreader) {
        return 0;
    }
    */

	return w_chip * l_chip;
}



void save_design_ev6(int module_count, struct module_dim **module_array, int * polish_exp)
{
    FILE *fp;
    int i;
    struct module_dim *temp_module;
    char buf[MAX_NUM][100];

    
    if((fp = fopen("../data/ev6.flp","w")) == NULL){
        printf("\nCould not create out_design.flp file!\n");
        exit(1);
    }
    
    polish_2_cord(module_count, module_array, polish_exp);

    
    for(i=0; i<module_count; i++){
        temp_module = module_array[i];
        sprintf(buf[i],"%d %f %f %f %f\n",i+1, temp_module->w, temp_module->h, temp_module->x_axis, temp_module->y_axis);
    }
    
    
    for(i=0; i<module_count; i++){
        fprintf(fp, buf[i]);
    }
    fclose(fp);
    
}

void save_design_ev7(int module_count, struct module_dim **module_array, int * polish_exp)
{
    FILE *fp;
    int i;
    struct module_dim *temp_module;
    char buf[MAX_NUM][100];
    
    if((fp = fopen("../data/ev7.flp","w")) == NULL){
        printf("\nCould not create out_design.flp file!\n");
        exit(1);
    }
    
    // Change Polish to Axes
    polish_2_cord(module_count,module_array,polish_exp);
    
    for(i=0; i<module_count; i++){
        temp_module = module_array[i];
        sprintf(buf[i],"%d %f %f %f %f\n",i+1, temp_module->w, temp_module->h, temp_module->x_axis, temp_module->y_axis);
    }
    
    
    for(i=0; i<module_count; i++){
        fprintf(fp, buf[i]);
    }
    fclose(fp);
    
}

void save_design_final(int module_count, struct module_dim **module_array, int * polish_exp, float lambda)
{
    FILE *fp;
    int i;
    struct module_dim *temp_module;
    char buf[MAX_NUM][100];
    
    if((fp = fopen("../data/113077548.txt","w")) == NULL){
        printf("\nCould not create out_design.flp file!\n");
        exit(1);
    }
    
    // Change Polish to Axes
    polish_2_cord(module_count,module_array,polish_exp);
    
    for(i=0; i<module_count; i++){
        temp_module = module_array[i];
        sprintf(buf[i],"%f %f\n",temp_module->x_axis, temp_module->y_axis);
    }
    
    
    for(i=0; i<module_count; i++){
        fprintf(fp, buf[i]);
    }
    fclose(fp);
    
}


double trunc(double d)
{
    return (d>0) ? floor(d) : ceil(d) ;
}


int check_for_overlap(int module_count, struct module_dim **module_array, float lambda, int * polish){
	int i,j;
	struct module_dim *temp_module1;
	struct module_dim *temp_module2;


	for(i=0; i<module_count; i++){
		temp_module1 = module_array[i];
		for(j=i+1; j<module_count; j++){
			temp_module2 = module_array[j];

			double l1x = trunc(10000000. *temp_module1 -> x_axis);
			double r1x = trunc(10000000. * (temp_module1 -> x_axis + temp_module1 -> w));
			double l2x = trunc(10000000. *temp_module2 -> x_axis);
			double r2x = trunc(10000000. *(temp_module2 -> x_axis + temp_module2 -> w));

			double l1y = trunc(10000000. *(temp_module1 -> y_axis + temp_module1 -> h));
			double r1y = trunc(10000000. *temp_module1 -> y_axis);
			double l2y = trunc(10000000. *(temp_module2 -> y_axis + temp_module2 -> h));
			double r2y = trunc(10000000. *temp_module2 -> y_axis);

		    if (fabs(l1x - r2x) > 1 || fabs(l2x - r1x) > 1)
		        continue;

		    if (fabs(l1y - r2y) > 1 || fabs(l2y - r1y) > 1)
		        continue;

			printf("l1x: %lf, r1x: %lf\n", l1x, r1x);
			printf("l1y: %lf, r1y: %lf\n", l1y, r1y);
			printf("l2x: %lf, r2x: %lf\n", l2x, r2x);
			printf("l2y: %lf, r2y: %lf\n", l2y, r2y);
		    printf("i: %d, j: %d (*)\n", i+1,j+1);
			return 0;
		}
	}
	return 1;
}


