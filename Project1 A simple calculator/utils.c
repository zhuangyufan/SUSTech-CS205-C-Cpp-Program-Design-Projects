#include "calc.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>


char* multiply(char a[], char b[]) {
    // Determine the length of the input arrays
    int m = strlen(a);
    int n = strlen(b);
    
    // Initialize the result array with zeros
    char* result = (char*) malloc((m+n+1) * sizeof(char));
    memset(result, '0', m+n+1);
    result[m+n] = '\0';
    
    // Multiply each digit of a with each digit of b
    for (int i = m-1; i >= 0; i--) {
        int carry = 0;
        for (int j = n-1; j >= 0; j--) {
            int temp = (a[i] - '0') * (b[j] - '0') + carry + (result[i+j+1] - '0');
            carry = temp / 10;
            result[i+j+1] = (temp % 10) + '0';
        }
        result[i] += carry;
    }
    
    // Remove leading zeros from the result array
    int i = 0;
    while (result[i] == '0') {
        i++;
    }
    if (i == m+n) {
        return "0";
    } else {
        return result+i;
    }
}

int isScientificNotation(char* input) {
    int i = 0;
    int hasDecimal = 0;
    int hasExponent = 0;
    int hasDigit = 0;
    
    // Check for optional sign character
    if (input[i] == '+' || input[i] == '-') {
        i++;
    }
    
    // Check for decimal digits before the decimal point
    while (isdigit(input[i])) {
        i++;
        hasDigit = 1;
    }
    
    // Check for decimal point
    if (input[i] == '.') {
        i++;
        hasDecimal = 1;
    }
    
    // Check for decimal digits after the decimal point
    while (isdigit(input[i])) {
        i++;
        hasDigit = 1;
    }
    
    // Check for exponent character
    if (input[i] == 'e') {
        i++;
        hasExponent = 1;
        
        // Check for optional sign character after the exponent character
        if (input[i] == '+' || input[i] == '-') {
            i++;
        }
        
        // Check for integer digits after the exponent character
        while (isdigit(input[i])) {
            i++;
            hasDigit = 1;
        }
    }
    
    // Check if the input contains any non-digit characters
    while (input[i] != '\0') {
        if (!isdigit(input[i])) {
            return 0;
        }
        i++;
    }
    
    // Check if the input contains at least one digit, decimal point, and exponent
    // return hasDigit && hasDecimal && hasExponent;
    return hasDigit && hasExponent;
}

int calc_sci(char a[], char b[], char op[]){
    // Extract base part and exponential part
    char input1[20];
    char input2[20];
    strcpy(input1, a);
    strcpy(input2, b);

    char *base_str1, *exp_str1, *base_str2, *exp_str2;
    double base1, base2;
    int exp1, exp2;

    base_str1 = strtok(input1, "e");
    exp_str1 = strtok(NULL, "e");

    base1 = atof(base_str1);
    exp1 = atoi(exp_str1);

    base_str2 = strtok(input2, "e");
    exp_str2 = strtok(NULL, "e");

    base2 = atof(base_str2);
    exp2 = atoi(exp_str2);

    // Check if the number is big enough
    if (exp1 < 10 && exp2 < 10){
        return (0);
    }

    // Calculation
    double result_base = 0;
    int result_exp = 0;

    if (strcmp(op, "*") == 0) {
        result_base = base1 * base2;
        result_exp = exp1 + exp2;
    } else if (strcmp(op, "/") == 0) {
        result_base = base1 / base2;
        result_exp = exp1 - exp2;
    } else{
        while (exp1 > exp2){
            base1 *= 10.0;
            exp1 --;
        }
        while (exp2 > exp1){
            base2 *= 10.0;
            exp2 --;
        }
        if (strcmp(op, "+") == 0) {
            result_base = base1 + base2;
            result_exp = exp1;
        }
        else if (strcmp(op, "-") == 0){
            result_base = base1 - base2;
            result_exp = exp1;
        }
    }

    // Regulization
    while (result_base >= 10.0 || result_base <= -10.0) {
        result_base /= 10.0;
        result_exp ++;
    }
    while (result_base < 1.0 && result_base > -1.0) {
        result_base *= 10.0;
        result_exp --;
    }
    
    // Print the result
    if (result_exp == 0){
        printf("%s %s %s = %lf\n", a, op, b, result_base);
    }
    else{
        printf("%s %s %s = %lfe%d\n", a, op, b, result_base, result_exp);
    }

    return (0);
 }