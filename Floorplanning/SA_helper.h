#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MOVE 3
#define MAX_NUM 10000

//#define PRINT
#define PROGRESS
//#define DEBUG

#define BALANCED_POLISH

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
int * init_polish(int module_count);
void print_polish(int num, int * polish);
int *getRandom(int module_count);
int *smart_move(int module_count, int *polish_exp, int num_move);
int parse_design(char *filename, struct module_dim ***module_array, float * lambda,float * total_size);
void polish_2_cord(int module_count,struct module_dim **module_array, int * polish_exp);

float get_area(int module_count, struct module_dim **module_array, int *polish_exp);
int check_for_overlap(int module_count, struct module_dim **module_array, float lambda, int * polish);

void save_design_ev6(int module_count, struct module_dim **module_array, int *polish_exp);
void save_design_ev7(int module_count, struct module_dim **module_array, int *polish_exp);
void save_design_final(int module_count, struct module_dim **module_array, int *polish_exp, float lambda);



