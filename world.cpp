#include "world.h"
#include <iostream>
#include <fstream>
#include <cstdlib>  // for calling rand()
#include <ctime>    // for seeding with system time
#include <QString>

using namespace std;

const char WORLD_FILE_PATH_STR[] = "C:\\Users\\Mosiu\\Documents\\studia_II\\MSI\\World1.txt";

void detect_world_dimensions();

float extract_bol_float();

void extract_and_update_parameters();

float gamma;
float r;
float t;
float u;
float b;
bool u_flg = false;
bool b_flg = false;

int vertical_size = 0;
int horizontal_size = 0;

probability_t event_probability[4];
state_t state[max_vertical_world_size][max_horizontal_world_size];

int current_row = 0;
int current_column = 0;

int start_row;
int start_column;

ifstream file;


void read_file()
{
    file.open(WORLD_FILE_PATH_STR, ios_base::in);

    // for single-character file input
    char input;

    detect_world_dimensions();

    srand(static_cast<unsigned>(time(0)));

    // read world matrix
    for (int row_idx=0; row_idx < vertical_size; row_idx ++)
    {
        for (int col_idx = 0; col_idx < horizontal_size; col_idx ++)
        {
            // assign initial utility
            float initial_state_utility = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
            state[row_idx][col_idx].current_utility = initial_state_utility;

            // save file character to char 'input'
            file.get(input);

            switch(input)
            {
            case 's':
                state[row_idx][col_idx].state_desc = START;
                start_row = row_idx;
                start_column = col_idx;
                break;
            case 'b':
                state[row_idx][col_idx].state_desc = SPECIAL;
                b_flg = true;
                break;
            case 'f':
                state[row_idx][col_idx].state_desc = FORBIDDEN;
                state[row_idx][col_idx].current_utility = 0;
                break;
            case 't':
                state[row_idx][col_idx].state_desc = TERMINAL_T;
                break;
            case 'u':
                state[row_idx][col_idx].state_desc = TERMINAL_U;
                u_flg = true;
                break;
            default:
                state[row_idx][col_idx].state_desc = REGULAR;
                break;
            }
        }
        file.ignore(10, '\n');  // ignore spaces and \r if it exists in a file
        //file.ignore();  // ignore \n itself;
    }
    // ignore the '-' line
    file.ignore(100, '\n');

    extract_and_update_parameters();

}


// slow, but should work
void detect_world_dimensions()
{
    char input;
    horizontal_size = 0;
    file.get(input);
    while((input != '\n')&&(input != '\r')&&(input != ' '))
    {
        horizontal_size++;
        file.get(input);
    }
    file.close();

    file.open(WORLD_FILE_PATH_STR, ios_base::in); // reload the file
    vertical_size = 0;
    file.get(input);
    while(input != '-')
    {
        vertical_size++;
        file.ignore(max_horizontal_world_size, '\n');   // ignore the rest of this line
        //file.ignore();  // throw out \n from the stream too
        file.get(input);
    }
    file.close();
    file.open(WORLD_FILE_PATH_STR, ios_base::in);
}

void extract_and_update_parameters()
{
    gamma = extract_bol_float();
    r = extract_bol_float();
    event_probability[FORWARDS] = extract_bol_float(); // p1
    event_probability[TO_THE_LEFT] = extract_bol_float(); // p2
    event_probability[TO_THE_RIGHT] = extract_bol_float(); // p3
    event_probability[BACKWARDS] = 1 - event_probability[FORWARDS]
            - event_probability[TO_THE_LEFT] - event_probability[TO_THE_RIGHT];
    t = extract_bol_float();
    if (u_flg)
        u = extract_bol_float();
    if (b_flg)
        b = extract_bol_float();

    for (int row_idx = 0; row_idx < vertical_size; row_idx ++)
    {
        for (int col_idx=0; col_idx < horizontal_size; col_idx ++)
        {
            switch(state[row_idx][col_idx].state_desc)
            {
            case START:
                state[row_idx][col_idx].reward = r; // punishment, as in the rest of the states
                break;
            case SPECIAL:
                state[row_idx][col_idx].reward = b; // specially defined reward/punishment
                break;
            case TERMINAL_T:
                state[row_idx][col_idx].reward = t; // specially defined reward/punishment
                state[row_idx][col_idx].current_utility = t; // specially defined reward/punishment
                break;
            case TERMINAL_U:
                state[row_idx][col_idx].reward = u; // specially defined reward/punishment
                state[row_idx][col_idx].current_utility = u;
                break;
            case REGULAR:
                state[row_idx][col_idx].reward = r; // punishment, as in the rest of the states
                break;
            default:
                break;
            }
        }
    }
}

float extract_bol_float()
{
    float result;
    char input;
    QString parameter = "";

    // begin wherever in the file you are!!
    file.get(input);
    while (input != ' ')
    {
        parameter += input;
        file.get(input);
    }
    result = parameter.toFloat();
    // go to newline
    file.ignore(100, '\n');
    return result;
}

void print_world_and_param()
{
    cout << "\n\nVertical: " << vertical_size
         << "\nHorizontal: " << horizontal_size << endl;
    cout << endl << "World read result: " << endl;
    for (int row_idx = 0; row_idx < vertical_size; row_idx ++)
    {
        for (int col_idx=0; col_idx < horizontal_size; col_idx ++)
        {
            cout << state[row_idx][col_idx].state_probability << '\t';
        }
        cout << endl;
    }
    cout << "\ngamma: " << gamma ;
    cout << "\nr: " << r ;
}

void go_to_start()
{
    current_row = start_row;
    current_column = start_column;
}

//void print_world_and_param()
//{
//    cout << "\n\nVertical: " << vertical_size
//         << "\nHorizontal: " << horizontal_size << endl;
//    cout << endl << "World read result: " << endl;
//    for (int row_idx = 0; row_idx < vertical_size; row_idx ++)
//    {
//        for (int col_idx=0; col_idx < horizontal_size; col_idx ++)
//        {
//            char c;
//            switch(state[row_idx][col_idx].state_desc)
//            {
//            case START:
//                c = 's';
//                break;
//            case SPECIAL:
//                c = 'b';
//                break;
//            case FORBIDDEN:
//                c = 'f';
//                break;
//            case TERMINAL_T:
//                c = 't';
//                break;
//            case TERMINAL_U:
//                c = 'u';
//                break;
//            case REGULAR:
//                c = 'x';
//                break;
//            default:
//                c = 'h';
//                break;
//            }
//            cout << c;
//        }
//        cout << endl;
//    }
//    cout << "\ngamma: " << gamma ;
//    cout << "\nr: " << r ;
//}
