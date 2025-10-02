#include <stdio.h>
#include <cs50.h>
#include <math.h>
#include <stdlib.h>

int get_len(long input){
    double common_log;
    int len = 0;

    common_log = log10(input);
    len = floor(common_log) + 1;

    return len;
}

int get_nth_digit(long input, int n){
    //nth from the 'right', starting from 0.
    int len = get_len(input);

    long divisor = 1;
        for (int i = 0; i < n; i++){
            divisor *= 10;
        }

    long truncated_input = input / divisor;
    truncated_input = floor(truncated_input);

    int nth_digit = truncated_input % 10;

    return(nth_digit);
}

int do_luhn(long input){
    int len = get_len(input);

    int first_sum = 0;
    for (int i = 1;  i < len; i += 2){
        int nth_digit = get_nth_digit(input, i);
        int doubled = nth_digit * 2;

        if(doubled >= 10){
            int first = doubled / 10;

            int second = doubled % 10;

            int sum = first + second;

            first_sum += sum;
            }
         else{
         first_sum += doubled;
        }
    }

    int second_sum = 0;
    for (int i = 0; i < len; i +=2){
        int nth_digit = get_nth_digit(input, i);
        second_sum += nth_digit;
    }

    int grand_total = first_sum + second_sum;

    if (grand_total % 10 == 0){
        return 0;
    } else {
        return 1;
    }
}

int pre_process(long input){
    int str_len = get_len(input);
    int valid_lens[3] = {13, 15, 16};

    for (int i = 0; i < 3; i++){
        if (str_len == valid_lens[i]){
            int luhn = do_luhn(input);
                if(luhn == 0){
                    return 0;
                }
            }
        }
    return 1;
}

int get_leading_digits(long input){
    int str_len = get_len(input);
    int first_digit = get_nth_digit(input, str_len - 1);

    if(first_digit == 4){
        return 4;
    }

    if(first_digit == 3 || first_digit == 5){
        int second_digit = get_nth_digit(input, (str_len - 2));
        int combined = (first_digit * 10) + second_digit;
        return combined;
    }
    printf("INVALID\n");
    exit(0);
}

int main(void){
//AMEX numbers start with 34 or 37.
//  & 15 Digits

//MASTER CARD starts with 51, 52, 53, 54, or 55
//  & 16 Digits

//VISA starts with 4
//  & 13 or 16 digits.
    long input = get_long("Input your credit card number, lol\n");
    int input_len = get_len(input);

    //Step 1; Initial preprocessing, check length is valid, check luhn's algorithm works
    int p_process = pre_process(input);
    if (p_process == 1){
        printf("INVALID\n");
        exit(0);
    }
    //Step 2; Get leading digits.
    int leading_digits = get_leading_digits(input);
    //Step 3; Check for AMEX
    if((leading_digits == 34 || leading_digits == 37) &&
        (input_len == 15)){
        printf("AMEX\n");
        exit(0);
    }
    //Step 4; Check for Master Card
    if((leading_digits == 51 || leading_digits == 52 || leading_digits == 53 || leading_digits == 54 || leading_digits == 55) &&
        (input_len == 16)){
            printf("MASTERCARD\n");
            exit(0);
        }
    //Step 5; Check for Visa
    if((leading_digits == 4) &&
        (input_len == 13 || input_len == 16)){
            printf("VISA\n");
            exit(0);
        }
    else {
        printf("INVALID\n");
        exit(0);
    }
}
