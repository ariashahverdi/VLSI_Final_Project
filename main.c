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
	float area,area_new;
	float fin_lambda, lambda_new;
	int *polish_exp;
    unsigned int init_temp = 100000;
	int *polish_exp_new;
	float delta_area;
	int random;
	double param_exp;
	double accept = 9.0;

	//Calling Hotspot
	char *argv_hotspot[5]={
        "./hotspot",
        "-f",
        "../data/ev6.flp",
        "-p",
        "../data/gcc.ptrace"};
    float cur_temp;

	/*pseudo-random number generator is initialized using time feed*/
	srand( (unsigned)time( NULL ) );

	/*design file parser*/
	module_count = parse_design(argv[1], &module_array, &lambda, &total_size);


	/*random polish expression generator*/
	polish_exp = getRandom(module_count);
    printf("size: %d, lambda: %f\n",module_count,lambda);
	/*for(i=0;i<(2*module_count)-1;i++){
		printf("%d\n",polish_exp[i]);
	}*/

    /*generating floorplan from the given polish expression*/
	area = optimal_design(module_count, module_array,polish_exp);

	int cnt = 0;
	/* SA algorithm Starts */
	 while(init_temp > 0){

		for(i=0; i<80; i++){

        //polish_exp_new = getRandom(module_count);
		polish_exp_new = smart_move(module_count, polish_exp);
		area_new = optimal_design(module_count, module_array, polish_exp_new);
		lambda_new = (area_new-total_size)/total_size;
		delta_area = area_new - area;
		/* generates random values between 0.1 and 0.9*/
		random = (rand() % 9) / 10;

		/* Acceptance probability calculation */
		param_exp = (delta_area/init_temp);

		cnt++;
		if(cnt==10000){
			printf("size: %f, lambda: %f\n",area_new,lambda_new);
			print_design(module_count,module_array);
			cnt = 0;
		}

		/*If new cost is small accept solution */
		if(delta_area < 0 && lambda_new>0){
			//free(polish_exp);
			polish_exp = polish_exp_new;
			area = area_new;
		}

		/*Accept with probability if new cost is more*/
		else if(random > exp(-(param_exp)) && lambda_new>0 ){
			//free(polish_exp);
			polish_exp = polish_exp_new;
			area = area_new;
		}

		//free(polish_exp_new);
		}
		init_temp--;
    }

	/* SA algorithm finish */

	//area = optimal_design(module_count, module_array, polish_exp);
    printf("\nTotal size = %f\n", total_size);
	printf("\nFinal area = %f\n", area);
	fin_lambda = (area-total_size)/total_size;
	printf("\nFinal Lambda = %f", fin_lambda);


	/*
	printf("\nfinal modules\n");
	for(i=0; i<module_count; i++){
		printf("(%d, %f)\t", temp_modules[i].module, temp_modules[i].size);
	}
	*/
	printf("\n");

	/*generates output design file */

	 if(area!=0) {
		 print_design(module_count,module_array);
	     cur_temp = get_temp(5, argv_hotspot);
         printf("\nMaximum Temperature: %f\n", cur_temp);
	}



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

