#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "calc.h"

int main(int argc, char *argv[]) {
    if (argc != 4) {
        printf("Usage: %s [number1] [operation] [number2]\n", argv[0]);
        return 1;
    }

    double num1, num2;

    if (sscanf(argv[1], "%lf", &num1) != 1) {
        printf("Error: '%s' is not a valid number.\n", argv[1]);
        return 1;
    }

    if (sscanf(argv[3], "%lf", &num2) != 1) {
        printf("Error: '%s' is not a valid number.\n", argv[3]);
        return 1;
    }

    if (strcmp(argv[2], "+") != 0 && strcmp(argv[2], "-") != 0 &&
        strcmp(argv[2], "*") != 0 && strcmp(argv[2], "/") != 0) {
        printf("Error: '%s' is not a valid operation.\n", argv[2]);
        return 1;
    }

    if (strcmp(argv[2], "/") == 0 && num2 == 0) {
        printf("Error: division by zero is not allowed.\n");
        return 1;
    }


    // Big number with sicentific notation
    if (isScientificNotation(argv[1]) && isScientificNotation(argv[3])){
        calc_sci(argv[1], argv[3], argv[2]);
        return 0;
    }

    // Big number multiplication
    if (strlen(argv[1]) + strlen(argv[3]) > 15) {
        char* result1 = multiply(argv[1], argv[3]);
        printf("%s * %s = %s\n", argv[1], argv[3], result1);
        return 0;
    }


    double result;
    char op_char;

    if (strcmp(argv[2], "+") == 0) {
        result = num1 + num2;
        op_char = '+';
    } else if (strcmp(argv[2], "-") == 0) {
        result = num1 - num2;
        op_char = '-';
    } else if (strcmp(argv[2], "*") == 0) {
        result = num1 * num2;
        op_char = '*';
    } else if (strcmp(argv[2], "/") == 0) {
        result = num1 / num2;
        op_char = '/';
    }

    printf("%f %c %f = %f\n", num1, op_char, num2, result);
    return 0;
}
