#include "value_iteration.h"
#include "world.h"

#include <iostream>
using namespace std;



typedef struct {
    direction_t direction;
    probability_t probability;
} occurence_desc;

// for the use of one function
direction_t action_to_event_relation_tab[4][4] = {  // [action][event]
    {UP, DOWN, LEFT, RIGHT}, // action == UP
    {DOWN, UP, RIGHT, LEFT}, // action == DOWN
    {LEFT, RIGHT, DOWN, UP}, // action == LEFT
    {RIGHT, LEFT, UP, DOWN}  // action == RIGHT
};


occurence_desc get_occurence(direction_t action, event_t event);

void increase_probability(occurence_desc occurence);

void update_utilities();

void clear_neighbouring_states_probabilities();

void move(state_t *& what, direction_t where);

void test_all()
{
    read_file();
    print_world_and_param();

    //go_to_start();
    current_row = 2;
    current_column = 0;

    clear_neighbouring_states_probabilities();
    direction_t action = UP;
    for(int event_desc = 0; event_desc < 4; event_desc ++)  // for events
    {
        // determine direction and probability for this event
        occurence_desc occurence = get_occurence(action, static_cast<event_t>(event_desc));
        // assign the probability somewhere
        increase_probability(occurence);
        print_world_and_param();
    }
}






















occurence_desc get_occurence(direction_t action, event_t event)
{
    occurence_desc result;

    result.direction = action_to_event_relation_tab[action][event];
    result.probability = event_probability[event];

    return result;
}


// clears five probabilities
void increase_probability(occurence_desc occurence)
{
    state_t *ptr = get_current_state();

    // move to the considered direction if possible. If not, just forget it - the bump will
    // make you stay in the same place.
    // If it's theoritically possible and you moved,
    // but the state is forbidden,
    // then move back.
    switch(occurence.direction)
    {
    case UP:
        if(current_row > 0)
        {
            move(ptr, UP);

            // move back if the state is forbidden!
            if(ptr->state_desc == FORBIDDEN)
                move(ptr, DOWN);
        }
        break;
    case DOWN:
        if(current_row < vertical_size - 1)
        {
            move(ptr, DOWN);

            if(ptr->state_desc == FORBIDDEN)
                move(ptr, UP);
        }
        break;
    case LEFT:
        if (current_column > 0)
        {
            move(ptr, LEFT);

            if(ptr->state_desc == FORBIDDEN)
                move(ptr, RIGHT);
        }
        break;
    case RIGHT:
        if (current_column < horizontal_size - 1)
        {
            move(ptr, RIGHT);

            if(ptr->state_desc == FORBIDDEN)
                move(ptr, LEFT);
        }
        break;
    }

    ptr ->state_probability += occurence.probability;
    int i=1;
}


// ---------------GROUP OF LOW-LEVEL FUNCTIONS----------------

// function to call at the end of the step
void update_utilities()
{
    for (int row_idx=0; row_idx < vertical_size; row_idx ++)
    {
        for (int col_idx = 0; col_idx < horizontal_size; col_idx ++)
        {
            state[row_idx][col_idx].current_utility = state[row_idx][col_idx].next_utility;
        }
    }
}

// clears five probabilities
void clear_neighbouring_states_probabilities()
{
    state_t *ptr = get_current_state();
    // own
    ptr->state_probability = 0;

    // field above
    if(current_row > 0)
    {
        move(ptr, UP);
        ptr->state_probability = 0;
    }
    ptr = get_current_state();

    // field below
    if(current_row < vertical_size - 1)
    {
        move(ptr, DOWN);
        ptr->state_probability = 0;
    }
    ptr = get_current_state();

    // field to the right
    if (current_column < horizontal_size - 1)
    {
        move(ptr, RIGHT);
        ptr ->state_probability = 0;
    }
    ptr = get_current_state();

    // field to the left
    if (current_column > 0)
    {
        move(ptr, LEFT);
        ptr ->state_probability = 0;
    }
    ptr = get_current_state();
}

// very low-level ptr fun
void move(state_t *& what, direction_t where)
{
    switch (where) {
    case UP:
        what -= max_horizontal_world_size;
        break;
    case DOWN:
        what += max_horizontal_world_size;
        break;
    case LEFT:
        what -= 1;
        break;
    case RIGHT:
        what += 1;
        break;
    default:
        break;
    }
}


//void update_possible_states(int col_idx, int row_idx, direction_t action)
//{
//    state_t * possible_states_for_action[4];
//    int possible_states_number = 4; // if no bumps occur

//    bool bump;

//    switch () {
//    case UP:

//        break;
//    case DOWN:

//        break;
//    case LEFT:

//        break;
//    case RIGHT:

//        break;
//    default:
//        break;
//    }
//}

