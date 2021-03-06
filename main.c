#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include<math.h>
#include"Floorplanning/SA_helper.h"
#include"HotSpot-5.02/hotspot.h"

void main(int argc , char* argv[])
{
	printf("input file : %s\n",argv[1]);

	int module_count=0;
	float lambda = 0;
	float total_size = 0;
	struct module_dim **module_array;
	struct module_dim **module_array_temp;
	struct module_dim *temp_module;
	struct module_dim *free_module;
	struct module *temp_modules;
	int i;
	int *polish_exp, *best_polish;
	int verticle, horizontal;
    unsigned int init_temp = 100000;
	struct cost cost;
	struct cost cost_new;
	struct cost cost_best;
	int *polish_exp_new;
	int *temp_polish;
	float delta_area;
	int random;
	float param_exp;

	int move_select = rand()%MOVE;
	float my_area;

	float best_temp = 1000;
	float cur_temp;

	//Calling Hotspot
	char *argv_hotspot[7]={
        "./hotspot",
		"-c",
		"../HotSpot-5.02/hotspot.config",
        "-f",
        "../data/ev6.flp",
        "-p",
        "../data/gcc.ptrace"};

	/*pseudo-random number generator is initialized using time feed*/
	srand( (unsigned)time( NULL ) );

	/*design file parser*/
	module_count = parse_design(argv[1], &module_array, &lambda, &total_size);

	/*vertical and horizontal section identify with number*/
	verticle = module_count + 1;
	horizontal = module_count + 2;

	/*random polish expression generator*/
	//polish_exp = getRandom(module_count);
	polish_exp = init_polish(module_count);
	best_polish = polish_exp;
	my_area = get_area(module_count, module_array,polish_exp);
    //cost.area = my_area;
    //cost_best.area = my_area;
    cost.area = 3*total_size;
    cost_best.area = 3*total_size;
    printf("size: %d, lambda: %f\n",module_count,lambda);

	int M = 0;
	int MT = 1;
	int uphill = 0;
	int reject = 0;
	float r = 0.85;
	float T = 1000;
	int k = 50000;
	int n = 2*module_count-1;
	int N = k * n;
	int r_cnt = 0;

	float best_lambda = 1;
	float cur_lambda;

    float update_to;
    update_to = 400*module_count;
    
    /*performing sizing slicing on floorplan generated by polish expression*/
	//cost = sizing_slicing(module_array, polish_exp,module_count);

    time_t rawtime;
    struct tm * timeinfo1;
    struct tm * timeinfo2;
    time ( &rawtime );
    timeinfo1 = localtime ( &rawtime );
    

	/* SA algorithm Starts */
	while(/*(reject/MT) < 0.95 &&*/ ( T > 400) ){

		while((uphill < N) && (MT<2*N)){

#ifdef PRINT
		while(1){
			int c;
			print_polish(module_count, polish_exp);
			save_design_ev6(module_count, module_array,polish_exp);
            my_area = get_area(module_count, module_array,polish_exp);
            printf("area before : < %f >\n", my_area);
            printf( "Enter Something, Not That We Care Tho :P\n");
            c = getchar( );
#endif
#ifdef DEBUG
			printf("** 2 **\n");
#endif

			if (r_cnt > update_to){
				r_cnt = 0;
				polish_exp = init_polish(module_count);
#ifdef PROGRESS
                //printf("delta area : %f\n",delta_area);
                printf("lambda (Current): %f, Lambda for best Temperature: %f, Best Temperature: %f\n",cur_lambda,best_lambda, best_temp);
#endif
			}
			r_cnt++;

			//no need for slides algorithm since the smart_move does it
			move_select = rand()%MOVE;
			polish_exp_new = smart_move(module_count, polish_exp,move_select);
			//polish_exp_new = getRandom(module_count);
			MT++;
			cost_new.area = get_area(module_count, module_array,polish_exp_new);
            cur_lambda = (cost_new.area-total_size)/total_size;
			cost.area = get_area(module_count, module_array,polish_exp);
			delta_area = cost_new.area - cost.area;


#ifdef DEBUG
			printf("** 3 **\n");
#endif
#ifdef PRINT
            print_polish(module_count, polish_exp_new);
            save_design_ev7(module_count, module_array,polish_exp_new);
            my_area = get_area(module_count, module_array,polish_exp_new);
            printf("area after : < %f >\n", my_area);
            printf( "Enter Something, Not That We Care Tho :P\n");
            c = getchar( );
			}
#endif

            // generates random values between 0.1 and 0.9
            random = (rand() % 9) / 10;

            // Acceptance probability calculation
            param_exp = (delta_area/init_temp);

            // If new cost is small
            if(delta_area < 0 /*|| random < exp(-(param_exp))*/){
            	//free(polish_exp);
            	if(delta_area > 0) uphill++;
            	polish_exp = polish_exp_new;
            	if(cur_lambda < lambda && /*cur_lambda > 0 &&*/ check_for_overlap(module_count, module_array, cur_lambda, polish_exp_new)) {
            		//printf("A design is saved %d\n",check_for_overlap(module_count, module_array, cur_lambda, polish_exp_new));
            		save_design_ev6(module_count, module_array, polish_exp);
            		cur_temp = get_temp(7, argv_hotspot);
            		if(cur_temp < best_temp){
            			best_lambda = cur_lambda;
            			best_temp = cur_temp;
            			best_polish = polish_exp;
                        save_design_final(module_count, module_array, best_polish, lambda);
                        save_design_ev7(module_count, module_array, best_polish);
            		}
            	}
            }
            else {reject++;}
#ifdef PROGRESS
            //printf("delta area : %f\n",delta_area);
            //printf("lambda (Current): %f, Lambda for best Temperature: %f, Best Temperature: %f\n",cur_lambda,best_lambda, best_temp);
#endif
		}
		T = r*T;
		MT = 1;
		uphill = 0;
		reject = 0;
    }


	/* SA algorith finish */
	//printf("\ncost after SA\n");
	printf("\ncost.area = %f\n", get_area(module_count, module_array,best_polish));
	printf("\nFinal Lambda = %f", best_lambda);

	/*generates output design file */
	save_design_ev6(module_count, module_array, best_polish);
    save_design_final(module_count, module_array, best_polish,lambda);
	cur_temp = get_temp(7, argv_hotspot);
    printf("\nMaximum Temperature: %f\n", cur_temp);
    
    printf ( "Current local time and date: %s", asctime (timeinfo1) );
    time ( &rawtime );
    timeinfo2 = localtime ( &rawtime );
    printf ( "Current local time and date: %s", asctime (timeinfo2) );
    
	/*free allocated dynamic memory*/
	module_array_temp = module_array;
	for(i=0; i<module_count; i++){
		temp_module = module_array_temp[i];
		while(temp_module != NULL){
			free_module = temp_module;
			temp_module = temp_module->next;
			free(free_module);
		}
	}

	free(module_array);
}

