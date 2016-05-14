#include<stdio.h>
#include<stdlib.h>
#include<string.h>


/* link list to hold different sizes of same module*/
struct module_dim{
	float w;
	float h;
	float power;
	int module_no;
	float x_axis;
	float y_axis;
	int size_no;
	struct module_dim* next;
};


/* helper routing for SA algorithm */
void print_polish(int num, int * polish);
int *getRandom(int module_count);
int *smart_move(int module_count, int *polish_exp);
int parse_design(char *filename, struct module_dim ***module_array, float * lambda,float * total_size);
float optimal_design(int module_count, struct module_dim **module_array, int *polish_exp);
void save_optimal_design(int module_count, struct module_dim **module_array, int *polish_exp);
void print_design(int module_count, struct module_dim **module_array);
