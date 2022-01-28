#include <stdio.h>
#include <stdlib.h>

#define MAX 100

const struct FL {
    char from[20];
    char to[20];
    int distance;
    char skip;
};

const struct stack {
    char from[20];
    char to[20];
    int distance;
};

struct FL flight[MAX];
struct stack bt_stack[MAX];

int f_pos = 0, find_pos = 0;
int tos = 0;

void setup(void), assert_flight(char *from, char*to, int dist);
int match(char *from, char *to), find(char *from, char *anywhere);
void push(char *from, char *to, int dist), pop(char *from, char *to, int *dist);
void isFLight(char *from, char *to), route(char *to);
void isFlight1(char *from, char *to);

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

int match(char *from, char *to){
    register int t;

    for(t = f_pos-1; t > -1; t--)
        if(!strcmp(flight[t].from, from) &&
        !strcmp(flight[t].to, to)) return flight[t].distance;

    return 0;
}

int find(char *from, char *anywhere){
    find_pos = 0;

    while(find_pos < f_pos){
        if(!strcmp(flight[find_pos].from, from) && !flight[find_pos].skip){
            strcpy(anywhere, flight[find_pos].to);
            flight[find_pos].skip = 1;
            return flight[find_pos].distance;
        }
        find_pos++;
    }
    return 0;
}

void push(char *from, char *to, int dist){

    if(tos < MAX){
        strcpy(bt_stack[tos].from, from);
        strcpy(bt_stack[tos].to, to);
        bt_stack[tos].distance = dist;
        tos++;
    }
    else printf("stack is full.\n");
}

void pop(char *from, char *to, int *dist){
    if(tos > 0){
        tos--;
        strcpy(from, bt_stack[tos].from);
        strcpy(to, bt_stack[tos].to);
        *dist = bt_stack[tos].distance;
    }
    else printf("stack underflow.\n");
}

void isFLight(char *from, char *to){
    int matchHolder, dist;
    char anywhere[20];

    if(matchHolder = match(from, to)){
        push(from, to, matchHolder);
    }
    
    if(dist = find(from, anywhere)){
        push(from, to, dist);
        isFLight(anywhere, to);
    }else if(tos > 0){
        pop(from, to, &dist);
        isFLight(from, to);
    }
}

void isFlight1(char *from, char *to){
    int matchHolder, dist;
    char anywhere[20];

    while(dist = find(from, anywhere)){
        if(matchHolder = match(anywhere, to)){
            push(from, to, dist);
            push(anywhere, to, matchHolder);
            return;
        }
    }
    if(dist = find(from, anywhere)){
        push(from, to, dist);
        isFLight1(anywhere, to);
    }
    else if(tos>0){
        pop(from, to, &dist);
        isFLight1(from,to);
    }
}

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