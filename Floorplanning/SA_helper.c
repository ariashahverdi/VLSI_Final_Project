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



void print_polish(int num, int * polish){
    int i;
    for(i=0;i<2*num-1;i++){
        printf("%d  ", polish[i]);
    }
    printf("\n\n");
}


int * smart_move(int module_count,int *polish_exp)
{
    int * new_polish;
    int i;
    new_polish = (int*)malloc(((module_count * 2) - 1) * sizeof(int));
    int vertical, horizontal;
    vertical = module_count + 1;
    horizontal = module_count + 2;
    int move_num = rand()%2;
    int idx = rand()%(2*module_count-1);
    int idx1, idx2;
    idx1 = idx;
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
            //printf("idx1: %d, idx2 : %d\n",idx1,idx2);
            int temp = new_polish[idx1];
            new_polish[idx1] = new_polish[idx2];
            new_polish[idx2] = temp;
            //print_polish(module_count, polish_exp);
            break;

        case 1: //Move2 : (Chain Invert) Complement some chain OK!
            while(1){
                idx = idx%(2*module_count-1);
                if(new_polish[idx] != vertical){
                    if(new_polish[idx] != horizontal){idx++;} //just a number
                    else {//it was horizontal move so we make it vertical
                        new_polish[idx] = vertical; break;
                    }
                }
                else{ //it was vertical move so we make it horizontal
                    new_polish[idx] = horizontal; break;
                }
            }
            //print_polish(module_count, new_polish);
            break;

        case 2: //Move3: (Operator/Operand Swap) Swap two adjacent operand and operator Needs to be checked!
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



/* generates optimal design output file */
float optimal_design(int module_count, struct module_dim **module_array, int *polish_exp)
{
	int i;
	float area;
	struct module_dim *temp_module;
	int vertical, horizontal;
	int temp_polish [2*module_count-1];
	for(i=0; i<(2*module_count-1); i++){temp_polish[i] = polish_exp[i];}
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

	//area = width[polish_exp[cur]-1] * height[polish_exp[cur]-1];

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

	for(i=0; i<(2*module_count-1); i++){polish_exp[i] = temp_polish[i];}

	/*
	for (i=0; i<module_count; i++){
		printf("%d Width: %f , Height: %f, X_axis: %f, Y_axis: %f\n",i,width[i],height[i], x_axis[i], y_axis[i]);
    }
	*/

	for(i=0; i<module_count; i++){
		temp_module = module_array[i];
		while(temp_module != NULL){
				temp_module->x_axis = x_axis[i];
				temp_module->y_axis = y_axis[i];
			   temp_module = temp_module->next;
		}
	}

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
	//printf ("w_chip : %f\n", w_chip);

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
	area = w_chip * l_chip;
	float s_sink = 0.060000;
	float s_spreader = 0.030000;

	if (w_chip > s_sink || l_chip > s_sink ||
		w_chip > s_spreader || l_chip > s_spreader) {
		return 0;
	}

	//printf("\n\n ** Done **\n");
    return area;
}

void save_optimal_design(int module_count, struct module_dim **module_array, int *polish_exp)
{
    FILE *fp;
    int i;
    struct module *head;
    struct module_dim *temp_module;
    char buf[module_count][100];
    int vertical, horizontal;
    int temp_polish [2*module_count-1];
    for(i=0; i<(2*module_count-1); i++){temp_polish[i] = polish_exp[i];}
    vertical = module_count + 1;
    horizontal = module_count + 2;

    if((fp = fopen("../data/113077548.txt","w")) == NULL){
        printf("\nCould not create out_design.flp file!\n");
        exit(1);
    }

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

    for(i=0; i<(2*module_count-1); i++){polish_exp[i] = temp_polish[i];}

    for(i=0; i<module_count; i++){
        temp_module = module_array[i];
        temp_module->x_axis = x_axis[i];
        temp_module->y_axis = y_axis[i];
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

void print_design(int module_count,struct module_dim **module_array)
{
	printf("Printing Design...\n");
	int i;
	FILE *fp;
	char buf[module_count][100];
	struct module_dim *temp_module;

	if((fp = fopen("../data/ev6.flp","w")) == NULL){
		printf("\nCould not create out_design.flp file!\n");
		exit(1);
	}

	for(i=0; i<module_count; i++){
		temp_module = module_array[i];
		while(temp_module != NULL){

				printf("%d:\t%f %f\t %f %f \n",i, temp_module->w, temp_module->h, temp_module->x_axis, temp_module->y_axis);
				sprintf(buf[i],"%d\t%f\t%f\t%f\t%f\n",i+1, temp_module->w, temp_module->h, temp_module->x_axis, temp_module->y_axis);
			temp_module = temp_module->next;
		}
	}

	for(i=0; i<module_count; i++){
		fprintf(fp, buf[i]);
	}
	fclose(fp);

}







