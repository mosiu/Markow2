#include "value_iteration.h"
#include "world.h"
#include "math.h"

#include <iostream>
using namespace std;

const float stop_diff_value = 0.0001;

typedef struct {
    direction_t direction;
    probability_t probability;
    utility_t utility;
} occurence_desc;

// for the use of one function
direction_t action_to_event_relation_tab[4][4] = {  // [action][event]
    {UP, DOWN, LEFT, RIGHT}, // action == UP
    {DOWN, UP, RIGHT, LEFT}, // action == DOWN
    {LEFT, RIGHT, DOWN, UP}, // action == LEFT
    {RIGHT, LEFT, UP, DOWN}  // action == RIGHT
};

occurence_desc get_occurence(direction_t action, event_t event);

void increase_probability(occurence_desc & occurence);

void update_utilities();

void clear_neighbouring_states_probabilities();

void move(state_t *& what, direction_t where);

// written since last version
void execute_iteration_step();

float get_max_action_score_for_current_state();

float evaluate_single_action(direction_t action);

void print_final_utilities();




void test_all()
{
    read_file();
    // we would think it's necessary to go_to_start(), but we're wrong (Paluszynski's slides say
    // optimal policy does NOT depend on the starting point in MDP

    float stop_utility = 0;
    int debug_iter_counter = 0;
    while(debug_iter_counter < 100)//(fabs(state[1][2].current_utility - stop_utility) > stop_diff_value)    // current utility
    {
        stop_utility = state[1][2].current_utility; // previous utility

        execute_iteration_step();

        debug_iter_counter ++;
    }

    print_final_utilities();
}

void execute_iteration_step()
{

    for (int row_idx = 0; row_idx < vertical_size; row_idx ++)      // for states
    {
        for (int col_idx=0; col_idx < horizontal_size; col_idx ++)
        {
            if ((state[row_idx][col_idx].state_desc != FORBIDDEN))
            {
                set_current_state(row_idx, col_idx);

                float max_action_score;

                max_action_score = get_max_action_score_for_current_state();

                get_current_state()->next_utility = get_current_state()->reward + gamma*max_action_score;
            }
        }
    }
    update_utilities();
}


float get_max_action_score_for_current_state()
{
    float max_action_score = 0;
    float action_score[4];

    for (int action_desc = 0; action_desc <4; action_desc ++)
    {
        action_score[action_desc] = evaluate_single_action(static_cast<direction_t>(action_desc));
    }

    for (int action_desc = 0; action_desc <4; action_desc ++)
    {
        if (max_action_score < action_score[action_desc])
        {
            max_action_score = action_score[action_desc];
        }
    }

    return max_action_score;
}


float evaluate_single_action(direction_t action)
{
    // for probabilities stored in states
    clear_neighbouring_states_probabilities();
    float action_score = 0;

    for(int event_desc = 0; event_desc < 4; event_desc ++)  // for events (actually, for s')
    {
        // determine direction and probability for this event, BUT NOT utility (occurence incomplete)
        occurence_desc occurence = get_occurence(action, static_cast<event_t>(event_desc));

        action_score += occurence.probability * occurence.utility;  // P(s'|s,a)*U(s')
    }

    return action_score;
}



occurence_desc get_occurence(direction_t action, event_t event)
{
    occurence_desc result;

    result.direction = action_to_event_relation_tab[action][event];
    result.probability = event_probability[event];

    // save utility of the state corresponding to the occurence, update its probability
    increase_probability(result);

    return result;
}


void increase_probability(occurence_desc & occurence)
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
    occurence.utility = ptr ->current_utility;
}


// ---------------GROUP OF LOW-LEVEL FUNCTIONS----------------

// function to call at the end of the step
void update_utilities()
{
    for (int row_idx=0; row_idx < vertical_size; row_idx ++)
    {
        for (int col_idx = 0; col_idx < horizontal_size; col_idx ++)
        {
            // prevent changing the terminal/forbidden state utility
            if ((state[row_idx][col_idx].state_desc == REGULAR)
                    || (state[row_idx][col_idx].state_desc == START)
                    || (state[row_idx][col_idx].state_desc == SPECIAL))
            {
                state[row_idx][col_idx].current_utility = state[row_idx][col_idx].next_utility;
            }
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

void print_final_utilities()
{
    cout << "\n\n Final utilities: \n\n";
    for (int row_idx = 0; row_idx < vertical_size; row_idx ++)
    {
        for (int col_idx=0; col_idx < horizontal_size; col_idx ++)
        {
            cout << state[row_idx][col_idx].current_utility << '\t';
        }
        cout << endl;
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

