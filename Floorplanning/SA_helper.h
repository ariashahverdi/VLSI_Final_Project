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

/*structure to store indices of modules selected in optimal design*/
struct module{
	int module;
	float size;
	
};

/*this structure holds area and all selected modules*/
struct cost{
	float area;
	struct module *final_modules;
};


/*data structure used for performing sizing slicing algorithm*/
struct slicing_cntr{
	struct module_dim *list;
	struct slicing_cntr *next;
	struct slicing_cntr *prev;
};

struct small_size{
	float w;
	float h;
	int col_module_no;
	int col_size_no;
	int row_module_no;
	int row_size_no;
	struct small_size *next;
};


/* helper routing for SA algorithm */
int *getRandom(int module_count);
void sort_w(struct slicing_cntr *slice_cntr);
void sort_h(struct slicing_cntr *slice_cntr);
struct small_size get_small_size(struct small_size *small_size_head);
struct cost sizing_slicing(struct module_dim **module_array, int *polish_array, int module_count);
int parse_design(char *filename, struct module_dim ***module_array, float * lambda,float * total_size);
void optimal_design(int module_count, struct cost cost, struct module_dim **module_array, int *polish_exp);
void print_design(int module_count,struct cost cost,struct module_dim **module_array);

