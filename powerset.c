/* Importing modules */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Declare the array size I am going to store elements in */
#define BUFFSIZE 10000

/**
 * Gets the number of elements in an array by dividing the number of bits used by the whole array and number of bits used
 * by the first element in the array.
 */
#define NELEMS(x) (sizeof(x) / sizeof((x)[0]))

/**
 * @brief Checks for the number of occurances of the character c in the string s
 * @param s The string we want to check the number of occurances of a character on
 * @param c The character we want to check the number of occurances of
 * @return The number of occurances of c
 */
int check(char *s,char c) 
{
    int i, count = 0;
    for (i = 0; s[i]; i++) 
    {
        if (s[i] == c) 
        {
            count++;
        }
    }
    return count;
}

/**
 * @brief Uses a binary counter to print all the possible power set given a set of elements
 * @param set_size the set size that will be determined by the counter later, will be used in the for loop as well
 * @param set the set that i will use to store all the elements the user inputter and preform strtok on
 */
void printPowerSet(int set_size, char *set[set_size][BUFFSIZE])
{
    /*set_size of power set of a set with set_size n is (2**n -1)*/
    unsigned int pow_set_size = pow(2, set_size);
    printf("power set size: %d\n", pow_set_size);
    int counter, j;

    /*Run binary counter that starts at 000 to 111 (or the number of input)*/
    for(counter = 0; counter < pow_set_size; counter++)
    {
        printf("{");
        for(j = 0; j < set_size; j++)
        {
            /* Check if jth bit in the counter is set
               If set then print jth element from set */
            if(counter & (1<<j))
            {
                printf("%s ", set[j]);
            }
        }
        printf("}");
        printf("\n");
    }
}
/**
 * @brief Asks for the user to input a number of elements that is less than a 10000 and seperates each element with a
 * comma and returns an array of strings where every element separated by a comma is in an index of the array.
 * Checks for overflow, newline words and whether nothing was entered or not, and deals with them accordingly.
 * @author: Icen Zeyada
 */
int main(void) {
    /* Declare the token that will be used with strtok() to store the string in it.*/
    char *token;
    const char *comma = ",";
    char words[BUFFSIZE];
    size_t i, j, slen; /* can replace size_t with int if you prefer */


    /* read input of one string, with error checking */
    printf("Enter the element of the sets. Seperate each element with a comma(,): ");
    if (fgets(words, BUFFSIZE, stdin) == NULL)
    {
        fprintf(stderr, "Error reading string into words.\n");
        exit(EXIT_FAILURE);
    }
    /* removing newline from words, along with checking for overflow from words */
    slen = strlen(words);
    if (slen > 0)
    {
        if (words[slen-1] == '\n') {
            words[slen-1] = '\0';
        } else {
            printf("Exceeded words length of %d.\n", BUFFSIZE);
            exit(EXIT_FAILURE);
        }
    }

    /* checking if nothing was entered */
    if (!*words)
    {
        printf("No string entered.\n");
        exit(EXIT_FAILURE);
    }

    /* Checks the number of elements in the input by checking how many commas exist in it and adding one */
    int counter = check(words,',')+1;

    /* Declaring the array of strings with the amount of elements inputted */
    char *storedWord[counter][BUFFSIZE];

    /* get the first token */
    token = strtok(words, comma);

    /* walk through other tokens */
    i = 0;
    while (token != NULL)
    {
        strcpy(storedWord[i++], token);
        token = strtok(NULL, comma);
    }
    printPowerSet(counter,storedWord);
    return(0);
}

