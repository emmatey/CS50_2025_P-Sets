#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

const int VALUES[26] = {
    1,  3,  3,  2,  1,  4,  2,  4,  1,  8,
    5,  1,  3,  1,  1,  3, 10,  1,  1,  1,
    1,  4,  4,  8,  4, 10
};

int get_score(string input){
 int score = 0;
 //0 = nul = "\0" = string terminator in ascii
 for(int i = 0; input[i] != 0; i++){
    int letter_score = 0;
    char input_lower = tolower(input[i]);

    int distance_from_a = input_lower - 'a';

    if (0 > distance_from_a){
        letter_score = 0;
    }
    else if (distance_from_a >= 0 && distance_from_a < 26){
        letter_score = VALUES[distance_from_a];
    }
    score += letter_score;
 }
 return score;
}

int main(void){
 //get input
 string p1 = get_string("Player 1: ");
 string p2 = get_string("Player 2: ");

 int p1_score = get_score(p1);
 int p2_score = get_score(p2);

 if(p1_score > p2_score){
    printf("Player 1 wins!\n");
 }
 else if(p2_score > p1_score){
    printf("Player 2 wins!\n");
 }
 else if(p1_score == p2_score){
    printf("Tie!\n");
 }
}
