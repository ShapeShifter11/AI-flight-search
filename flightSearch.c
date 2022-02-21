#include <stdio.h>
#include <stdlib.h>

#define MAX 100

/**
 * @brief structure of the flight database
 * 
 */
const struct FL {
    char from[20];
    char to[20];
    int distance;
    char skip; // used in backtracking 
};

/**
 * @brief structure used for backtracking
 * 
 */
const struct stack {
    char from[20];
    char to[20];
    int distance;
};

struct FL flight[MAX]; //array of database structures
struct stack bt_stack[MAX]; // array of backtracking stack

int f_pos = 0; //number of entries in flight database
int find_pos = 0; //index for searching flight database
int tos = 0; //top of stack

void setup(void), assert_flight(char *from, char*to, int dist);
int match(char *from, char *to), find(char *from, char *anywhere);
void push(char *from, char *to, int dist), pop(char *from, char *to, int *dist);
void isFLight(char *from, char *to), route(char *to);

int main(void){
    char from[20], to[20];

    setup();

    printf("From? ");
    gets(from);
    printf("To? ");
    gets(to);

    isFLight(from, to);

    route(to);

    return 0;
}

/**
 * @brief initializes the flight database
 * 
 * @param void
 * 
 * @return void
 */
void setup(void){
    assert_flight("New York", "Chicago", 1000);
    assert_flight("Chicago", "Denver", 1000);
    assert_flight("New York", "toronto", 800);
    assert_flight("New York", "Denver", 1900);
    assert_flight("Toronto", "Calgary", 1500);
    assert_flight("toronto", "Los Angelis", 1800);
    assert_flight("Toronto", "Chicago", 500);
    assert_flight("Denver", "Urbana", 1000);
    assert_flight("Denver", "Houston", 1500);
    assert_flight("Houston", "Los Angelis", 1500);
    assert_flight("Denver", "Los Angelis", 1000);
}

/**
 * @brief populates the flight database
 * 
 * @param from char type 
 * @param to char type
 * @param dist int type
 * 
 * @return void
 */
void assert_flight(char *from, char*to, int dist){
    if(f_pos<MAX){
        strcpy(flight[f_pos].to, to);
        strcpy(flight[f_pos].from, from);
        flight[f_pos].distance = dist;
        flight[f_pos].skip = 0;
        f_pos++;
    }
    else printf("Flight database full.\n");
}

/**
 * @brief searches the database for a direct flight and returns the distance, otherwise returns 0
 * 
 * @param from char type
 * @param to char type
 * 
 * @return int 
 */
int match(char *from, char *to){
    register int t;

    for(t = f_pos-1; t > -1; t--)
        if(!strcmp(flight[t].from, from) &&
        !strcmp(flight[t].to, to)) return flight[t].distance;

    return 0;
}

/**
 * @brief given from, finds anywhere and returns the distance
 * 
 * @param from char type
 * @param anywhere char type
 * 
 * @return int distance
 */
int find(char *from, char *anywhere){
    find_pos = 0;

    while(find_pos < f_pos){
        if(!strcmp(flight[find_pos].from, from) && !flight[find_pos].skip){
            strcpy(anywhere, flight[find_pos].to);
            flight[find_pos].skip = 1; //make active
            return flight[find_pos].distance;
        }
        find_pos++;
    }
    return 0;
}

/**
 * @brief Determins if there is a route between from and to.
 * 
 * @param from char type
 * @param to char type
 * 
 * @return void
 */
void isFlight(char *from, char *to){
    int d, dist;
    char anywhere[20];

    if(d = match(from, to)){
        push(from, to, d);
        return;
    }
    if(dist = find(from, anywhere)){
        push(from, to, dist);
        isFLight(anywhere, to);
    }
    else if(tos > 0){
        pop(from, to, &dist);
        isFLight(from, to);
    }
}

/**
 * @brief pushes on top of stack
 * 
 * @param from char type
 * @param to char type
 * @param dist int type
 * 
 * @return void
 */
void push(char *from, char *to, int dist){

    if(tos < MAX){
        strcpy(bt_stack[tos].from, from);
        strcpy(bt_stack[tos].to, to);
        bt_stack[tos].distance = dist;
        tos++;
    }
    else printf("stack is full.\n");
}

/**
 * @brief pops from top of stack
 * 
 * @param from char type
 * @param to char type
 * @param dist int type
 * 
 * @return void
 */
void pop(char *from, char *to, int *dist){
    if(tos > 0){
        tos--;
        strcpy(from, bt_stack[tos].from);
        strcpy(to, bt_stack[tos].to);
        *dist = bt_stack[tos].distance;
    }
    else printf("stack underflow.\n");
}

/**
 * @brief show the route and total distance 
 * 
 * @param to char type
 * 
 * @return void
 */
void route(char *to){
    int dist, t;

    dist = 0;
    t = 0;
    while(t < tos){
        printf("%s to ", bt_stack[t].from);
        dist += bt_stack[t].distance;
        t++;
    }
    printf("%s\n", to);
    printf("Distance is %d.\n", dist);
}