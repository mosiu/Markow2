#ifndef WORLD_H
#define WORLD_H


void read_file();
void print_world_and_param();
void go_to_start();


const int max_vertical_world_size = 10;
const int max_horizontal_world_size = 10;



typedef enum {
    REGULAR,    // x
    START,  // s
    SPECIAL,    // b
    FORBIDDEN,  // f
    TERMINAL_T,    // t
    TERMINAL_U  // u
} state_desc_t;

typedef float utility_t;
typedef float reward_t;
typedef float probability_t;

typedef enum {
    UP = 0,
    DOWN = 1,
    LEFT = 2,
    RIGHT = 3
} direction_t;

typedef enum {
    FORWARDS = 0,
    BACKWARDS = 1,
    TO_THE_LEFT = 2,
    TO_THE_RIGHT = 3
} event_t;

extern probability_t event_probability[4];

//typedef struct {
//    event_t event;
//    probability_t probability;
//} probability_model_t;


typedef struct {
    state_desc_t state_desc;
    utility_t current_utility;
    utility_t next_utility;
    reward_t reward;
    probability_t state_probability;

//    direction_t policy;
} state_t;

extern state_t state[max_vertical_world_size][max_horizontal_world_size];

// very important to change this consequently in state loop!
extern int current_row;
extern int current_column;

extern int vertical_size;
extern int horizontal_size;

inline state_t* get_current_state()
{
    return &state[current_row][current_column];
}




#endif // WORLD_H
