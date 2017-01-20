/*
 * Assignment 1
 * @Author: Jacob Luke
 * @Date: Jan 2017
 */
#include <stdio.h>
#include <string.h>

/*
 * Returns true if the given input is a digit
 * Returns false otherwise
 */
int isDigit(char c){

        int I = c;
        if(I >= 48 && I <= 57){return 1;}
        else{return 0;}
}

/*
 * Returns true if the given input is an uppercase character
 * Returns false otherwise
 */
int isUpperCase(char c){

        int U = c;
        if(U >= 65 && U <= 90){return 1;}
        else{return 0;}
}

/*
 * Returns true if the string matches mode A
 * Returns false otherwise
 * If useT is true then it will print out the converted form
 */
void matchA(char* s, int useT){

        // Booleans and counters
        int match = 1;
        int stage = 0;
        int pCount = 0;
        int dCount = 0;

        // Loop through all the letters in the string
        int i;
        for(i = 0; i < strlen(s); i++){
                // Match any number of c's
                if(stage == 0){
                        if(s[i] != 'c'){stage = 1;}
                }
                // Followed by 1-3 p's
                if(stage == 1){
                        if(s[i] == 'p'){pCount++;}
                        if(s[i] != 'p' && isDigit(s[i]) == 0){match = 0;}
                        if(isDigit(s[i]) != 0){stage = 2;}
                }
                // Followed by 1-3 digits
                if(stage == 2){
                        if(isDigit(s[i]) != 0){dCount++;}
                        else {match = 0;}
                }
        }

        // Check to make sure string fits
        if(stage != 2 || pCount < 1 || pCount > 3 ||  dCount < 1 || dCount > 3){match = 0;}

        if(match == 0){
                if(useT == 0){printf("%s\n", "no");}
                else{printf("");}
        }

        else{
                if(useT == 0){printf("%s\n", "yes");}

                // Conversion replaces each 'C' with 'F' but there are no 'C's so
                // conversion does nothing at all
                else{printf("%s\n", s);}
        }
}

/*
 * Returns true if the string matches mode B
 * Returns false otherwise
 * If useT is true then it will print out the converted form
 */
void matchB(char* s, int useT){

        // Booleans and counters
        int match = 1;
        int stage = 0;
        int gCount = 0;
        int X[3];
        int XIndex = 1;
        int dCount = 0;
        int nCount = 0;
        int uCount = 0;
        int DCount = 0;

        // Loop through all the letters in the string
        int i;
        for(i = 0; i < strlen(s); i++){

                // Match an even number of g's
                if(stage == 0){
                        if(s[i] != 'g'){stage = 1;}
                        else{gCount++;}
                }
                // Followed by 1-3 decimal digits (call this X)
                if(stage == 1){
                        if(isDigit(s[i]) != 0){
                                X[dCount] = s[i];
                                dCount++;
                        }
                        if(isDigit(s[i]) == 0 && s[i] != 'n'){match = 0;}
                        if(s[i] == 'n'){stage = 2;}
                }
                // Followed by an odd number of n's
                if(stage == 2){
                        if(s[i] == 'n'){nCount++;}
                        if(s[i] != 'n' && isUpperCase(s[i]) == 0){match = 0;}
                        if(isUpperCase(s[i]) != 0){stage = 3;}
                }
                // Followed by an odd number of uppercase letters
                if(stage == 3){
                        if(isUpperCase(s[i])!= 0){
                                uCount++;
                                if(s[i] == 'D'){DCount++;}
                        }
                        if(isUpperCase(s[i]) == 0 && isDigit(s[i]) == 0){match = 0;}
                        if(isUpperCase(s[i]) == 0 && isDigit(s[i]) != 0){stage = 4;}
                }

                // Followed by the same characters are the odd-positioned characters in X
                if(stage == 4){
                        if(s[i] != X[XIndex]){match = 0;}
                        XIndex += 2;
                }
        }

        // Check to make sure string fits
        if(stage < 3 || gCount %2 != 0 || dCount < 1 || dCount > 3 || nCount % 2 == 0 || uCount % 2 == 0){match = 0;}

        if(match == 0){
                if(useT == 0){printf("%s\n","no");}
                else{printf("");}
        }

        else{
                if(useT == 0){printf("%s\n", "yes");}

                // Converstion adds one F before each D
                else{
                        int k;
                        int index = 0;
                        char printString[strlen(s) + DCount + 1];
                        for(k = 0; k < strlen(s); k++){
                                if(s[k] == 'D'){
                                        printString[index] = 'F';
                                        printString[index + 1] = 'D';
                                        index += 2;
                                }
                                else{printString[index] = s[k]; index++;}
                        }
                        printString[index] = (char)0;
                        printf("%s\n", printString);
                }
        }
        return;
}

/*
 * Returns true if the string matches mode C
 * Returns false otherwise
 * If useT is true then it will print out the converted form
 */
void matchC(char* s, int useT){

        // Booleans and counters
        int match = 1;
        int stage = 0;
        int gCount = 0;
        int X[3];
        int dCount = 0;
        int XIndex = 0;
        int xCount = 0;
        int uCount = 0;
        int EFCount = 0;

        // Loop through all the letters in the string
        int i;
        for(i = 0; i < strlen(s); i++){

                // Match 3-6 g's
                if(stage == 0){
                        if(s[i] == 'g'){gCount++;}
                        if(s[i] != 'g' && isDigit(s[i]) == 0){match = 0;}
                        if(s[i] != 'g' && isDigit(s[i]) != 0){stage = 1;}
                }
                // Followed by 1-3 decimal digits (call this X)
                if(stage == 1){
                        if(isDigit(s[i]) != 0){
                                X[dCount] = s[i];
                                dCount++;
                        }
                        if(isDigit(s[i]) == 0 && s[i] != 'x'){match = 0;}
                        if(s[i] == 'x'){
                                stage = 2;
                                XIndex = dCount - 1;
                        }
                }
                // Followed by an odd number of x's
                if(stage == 2){
                        if(s[i] == 'x'){xCount++;}
                        if(s[i] != 'x' && isUpperCase(s[i]) == 0){match = 0;}
                        if(isUpperCase(s[i]) != 0){stage = 3;}
                }
                // Followed by an odd number of uppercase letters
                if(stage == 3){
                        if(isUpperCase(s[i])!= 0){
                                uCount++;
                                if(s[i] == 'E' || s[i] == 'F'){EFCount++;}
                        }
                        if(isUpperCase(s[i]) == 0 && isDigit(s[i]) == 0){match = 0;}
                        if(isUpperCase(s[i]) == 0 && isDigit(s[i]) != 0){stage = 4;}
                }
                // Followed by the same characters as X but reversed
                if(stage == 4){
                        if(s[i] != X[XIndex]){match = 0;}
                        XIndex--;
                }
        }

        // Check to make sure string fits
        if(stage != 4 || gCount < 3 || gCount > 6 || dCount < 1 || dCount > 3 || xCount % 2 == 0 || uCount % 2 == 0){match = 0;}

        if(match == 0){
                if(useT == 0){printf("%s\n","no");}
                else{printf("");}
        }

        else{
                if(useT == 0){printf("%s\n", "yes");}

                // Converstion removes all E's and F's
                else{
                        int k;
                        int index = 0;
                        char printString[strlen(s) - EFCount + 1];
                        for(k = 0; k < strlen(s); k++){
                                if(s[k] == 'E' || s[k] == 'F'){}
                                else{printString[index] = s[k]; index++;}
                        }
                        printString[index] = (char)0;
                        printf("%s\n", printString);
                }
        }
        return;
}

main(int argk, char** argv){

        // Does nothing if there are no arguments
        if(argk == 1){ return 0;}

        // Booleans to keep track of which flags are being used
        int useA = 0;
        int useB = 0;
        int useC = 0;
        int useT = 0;

        // Find out which flags are being used
        int index = 1;
        if(argv[1][0] == '-'){
                index++;
                if(argv[1][1] == 'a'){useA = 1;}
                if(argv[1][1] == 'b'){useB = 1;}
                if(argv[1][1] == 'c'){useC = 1;}
                if(argv[1][1] == 't'){useT = 1;}
        }
        if(argk > 2){
                if(argv[2][0] == '-'){
                        index++;
                        if(argv[2][1] == 't'){useT = 1;}
                        if(argv[2][1] == 'a' && useB == 0 && useC == 0){useA = 1;}
                        if(argv[2][1] == 'b' && useA == 0 && useC == 0){useB = 1;}
                        if(argv[2][1] == 'c' && useA == 0 && useB == 0){useC = 1;}
                }
        }

        // Default mode is A
        if(useA == 0 && useB == 0 && useC == 0){useA =1;}

        // Loop through all non-flag arguments and see if they match. Print accordingly.
        for(;index < argk; index++){

                // MODE A
                if(useA != 0){matchA(argv[index], useT);}
                // MODE B
                if(useB != 0){matchB(argv[index], useT);}
                // MODE C
                if(useC != 0){matchC(argv[index], useT);}
        }

        return 0;
}
