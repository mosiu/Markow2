#include <QCoreApplication>
#include <iostream>
#include <fstream>
#include <QString>
const char WORLD_FILE_PATH_STR[] = "C:\\Users\\mosiu\\Documents\\studia_II\\Metody sztucznej inteligencji\\Markow\\World1.txt";

using namespace std;
const int max_vertical_world_size = 10;
const int max_horizontal_world_size = 10;
int vertical_size = 0;
int horizontal_size = 0;
float gamma;
float r;
float t;
float u;
float b;
float p1, p2, p3;
bool u_flg = false;
bool b_flg = false;

void read_file();
void detect_world_dimensions();
void print_world_and_param();
float extract_bol_float();
void extract_parameters();

int main(int argc, char *argv[])
{
    cout << "costam\n";
    QCoreApplication a(argc, argv);
    cout << "costam2\n";
    read_file();
    cout << "costam3\n";
    print_world_and_param();
    cout << "\ncostam4";
    return a.exec();
}

typedef enum {
    REGULAR,    // x
    START,  // s
    SPECIAL,    // b
    FORBIDDEN,  // f
    TERMINAL_1,    // t
    TERMINAL_2  // u
} field_t;

//typedef enum {
//    UP,
//    DOWN,
//    LEFT,
//    RIGHT
//} direction_t;

field_t world[max_vertical_world_size][max_horizontal_world_size];

ifstream file;

void read_file()
{
    file.open(WORLD_FILE_PATH_STR, ios_base::in);
    char input;

    detect_world_dimensions();

    // read world matrix
    for (int row_idx=0; row_idx < vertical_size; row_idx ++)
    {
        for (int col_idx = 0; col_idx < horizontal_size; col_idx ++)
        {
            file.get(input);
            switch(input)
            {
            case 's':
                world[row_idx][col_idx] = START;
                break;
            case 'b':
                world[row_idx][col_idx] = SPECIAL;
                b_flg = true;
                break;
            case 'f':
                world[row_idx][col_idx] = FORBIDDEN;
                break;
            case 't':                
                world[row_idx][col_idx] = TERMINAL_1;
                break;
            case 'u':
                world[row_idx][col_idx] = TERMINAL_2;
                u_flg = true;
                break;
            default:
                world[row_idx][col_idx] = REGULAR;
                break;
            }
        }
        file.ignore(10, '\n');  // ignore spaces and \r if it exists in a file
        //file.ignore();  // ignore \n itself;
    }
    // ignore the '-' line
    file.ignore(100, '\n');

    extract_parameters();
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

void extract_parameters()
{
    gamma = extract_bol_float();
    r = extract_bol_float();
    p1 = extract_bol_float();
    p2 = extract_bol_float();
    p3 = extract_bol_float();
    t = extract_bol_float();
    if (u_flg)
        u = extract_bol_float();
    if (b_flg)
        b = extract_bol_float();
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
            char c;
            switch(world[row_idx][col_idx])
            {
            case START:
                c = 's';
                break;
            case SPECIAL:
                c = 'b';
                break;
            case FORBIDDEN:
                c = 'f';
                break;
            case TERMINAL_1:
                c = 't';
                break;
            case TERMINAL_2:
                c = 'u';
                break;
            case REGULAR:
                c = 'x';
                break;
            default:
                c = 'h';
                break;
            }
            cout << c;
        }
        cout << endl;
    }
    cout << "\ngamma: " << gamma ;
    cout << "\nr: " << r ;
    cout << "\np1 :" << p1;
    cout << "\np2 :" << p2;
    cout << "\np3 :" << p3;
}
