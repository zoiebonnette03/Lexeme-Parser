/**
 * This the example lexical analyzer code in pages 173 - 177 of the
 * textbook,
 *
 * Sebesta, R. W. (2012). Concepts of Programming Languages. 
 * Pearson, 10th edition.
 *
 */

/* front.c - a lexical analyzer system for simple arithmetic expressions */
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#include "front.h"
#include "parser.h"

/* Global Variable */
int nextToken;
char lexeme[100];

/* Local Variables */
static int charClass;
static char nextChar;
static int lexLen;
static FILE *in_fp;

/* Local Function declarations */
static void addChar();
static void getChar();
static void getNonBlank();

/******************************************************/
/* main driver */
int main(int argc, char* argv[])
{  
    char* fp = argv[1];

    printf("DCooke Analyzer :: R11910353\n");
    /* Open the input data file and process its contents */
    if (fp == NULL) {
        printf("ERROR - no file provided. \n");
        exit(2);
    }
    else if ((in_fp = fopen(fp, "r")) == NULL) {
        printf("ERROR - file does not exist. \n");
        exit(3);
    } else {
        getChar();
        do {
            lex();
            pro();
        } while (nextToken != EOF);
    }

    return 0;
}

/*****************************************************/
/* lookup - a function to lookup operators and parentheses and return the token 
 ADDED: if-else statements to check for operators with two characters */
static int lookup(char ch) { 
    switch (ch) {
        case '(':
            addChar();
            nextToken = LEFT_PAREN;
            getChar();
            break;
        case ')':
            addChar();
            nextToken = RIGHT_PAREN;
            getChar();
            break;
        case '+':
            addChar();
            getChar();
            if (nextChar == '+') {
                addChar();
                nextToken = INC_OP;
                getChar();
            }
            else {
                nextToken = ADD_OP;
            }
            break;
        case '-':
            addChar();
            getChar();
            if (nextChar == '-') {
                addChar();
                nextToken = DEC_OP;
                getChar();
            }
            else {
                nextToken = SUB_OP;
            }
            break;
        case '*':
            addChar();
            nextToken = MULT_OP;
            getChar();
            break;
        case '/':
            addChar();
            nextToken = DIV_OP;
            getChar();
            break;
        case '>':
            addChar();
            getChar();
            if (nextChar == '=') {
                addChar();
                nextToken = GEQUAL_OP;
                getChar();
            }
            else {
                nextToken = GREATER_OP;
            }
            break;
        case '<':
            addChar();
            getChar();
            if (nextChar == '=') {
                addChar();
                nextToken = LEQUAL_OP;
                getChar();
            }
            else if (nextChar == '>') {
                addChar();
                nextToken = NEQUAL_OP;
                getChar();
            }
            else {
                nextToken = LESSER_OP;
            }
            break;
        case '=':
            addChar();
            nextToken = EQUAL_OP;
            getChar();
            break;
        case ';':
            addChar();
            nextToken = SEMICOLON;
            getChar();
            break;
        case ':':
            addChar();
            getChar();
            if (nextChar == '=') {
                addChar();
                nextToken = ASSIGN_OP;
                getChar();
            }
            else {
                nextToken = COLON;
            }
            break;
        case '%':
            addChar();
            nextToken = MOD_OP;
            getChar();
            break;
        default:
            addChar();
            nextToken = UNKNOWN;
            getChar();
            break;
    }
    return nextToken;
}

/******************************************************/
/* ADDED: checkKey - a function to compare the current lexeme to key words and return the token */
static int checkKey(char lexA[]) {
    char read[] = "read";
    char write[] = "write";
    char iff[] = "if";
    char fi[] = "fi";
    char elsee[] = "else";
    char then[] = "then";

    if (strcmp(lexA, read) == 0) {
        nextToken = KEY_READ;
    }
    else if (strcmp(lexA, write) == 0) {
        nextToken = KEY_WRITE;
    }
    else if (strcmp(lexA, iff) == 0) {
        nextToken = KEY_IF;
    }
    else if (strcmp(lexA, fi) == 0) {
        nextToken = KEY_FI;
    }
    else if (strcmp(lexA, elsee) == 0) {
        nextToken = KEY_ELSE;
    }
    else if (strcmp(lexA, then) == 0) {
        nextToken = KEY_THEN;
    }
    else {
        nextToken = IDENT;
    }
    return nextToken;
}

/******************************************************/
/* ADDED: print - a function to print the lexeme and its associated token */
static void print() {
    if (nextToken == 10)
        printf("\n%s\tINT_LIT", lexeme);
    else if (nextToken == 11)
        printf("\n%s\tIDENT", lexeme);
    else if (nextToken == 20)
        printf("\n%s\tASSIGN_OP", lexeme);
    else if (nextToken == 21)
        printf("\n%s\tADD_OP", lexeme);
    else if (nextToken == 22)
        printf("\n%s\tSUB_OP", lexeme);
    else if (nextToken == 23)
        printf("\n%s\tMULT_OP", lexeme);
    else if (nextToken == 24)
        printf("\n%s\tDIV_OP", lexeme);
    else if (nextToken == 25)
        printf("\n%s\tLEFT_PAREN", lexeme);
    else if (nextToken == 26)
        printf("\n%s\tRIGHT_PAREN", lexeme);
    else if (nextToken == 27)
        printf("\n%s\tLESSER_OP", lexeme);
    else if (nextToken == 28)
        printf("\n%s\tGREATER_OP", lexeme);
    else if (nextToken == 29)
        printf("\n%s\tEQUAL_OP", lexeme);
    else if (nextToken == 30)
        printf("\n%s\tCOLON", lexeme);
    else if (nextToken == 31)
        printf("\n%s\tSEMICOLON", lexeme);
    else if (nextToken == 32)
        printf("\n%s\tNEQUAL_OP", lexeme);
    else if (nextToken == 33)
        printf("\n%s\tLEQUAL_OP", lexeme);
    else if (nextToken == 34)
        printf("\n%s\tGEQUAL_OP", lexeme);
    else if (nextToken == 35)
        printf("\n%s\tMOD_OP", lexeme);
    else if (nextToken == 36)
        printf("\n%s\tKEY_READ", lexeme);
    else if (nextToken == 37)
        printf("\n%s\tKEY_WRITE", lexeme);
    else if (nextToken == 38)
        printf("\n%s\tKEY_IF", lexeme);
    else if (nextToken == 39)
        printf("\n%s\tKEY_FI", lexeme);
    else if (nextToken == 40)
        printf("\n%s\tKEY_ELSE", lexeme);
    else if (nextToken == 41)
        printf("\n%s\tKEY_THEN", lexeme);
    else if (nextToken == 42)
        printf("\n%s\tDEC_OP", lexeme);
    else if (nextToken == 43)
        printf("\n%s\tINC_OP", lexeme);
    else if(nextToken == 99)
        printf("\n%s\tUNKNOWN", lexeme);
}

/*****************************************************/
/* addChar - a function to add nextChar to lexeme */
static void addChar() {
    if (lexLen <= 98) {
        lexeme[lexLen++] = nextChar;
        lexeme[lexLen] = 0;
    } else {
        printf("Error - lexeme is too long \n");
    }
}

/*****************************************************/
/* getChar - a function to get the next character of input and determine its character class */
static void getChar() {
    if ((nextChar = getc(in_fp)) != EOF) {
        if (isalpha(nextChar)) 
            charClass = LETTER;
        else if (isdigit(nextChar))
            charClass = DIGIT;
        else charClass = UNKNOWN;
    } else {
        charClass = EOF;
    }
}

/*****************************************************/
/* getNonBlank - a function to call getChar until it returns a non-whitespace character */
static void getNonBlank() {
    while (isspace(nextChar)) getChar();
}

/*****************************************************/
/* lex - a simple lexical analyzer for arithmetic expressions */
int lex() {

    lexLen = 0;
    getNonBlank();

    switch (charClass) {
        /* Parse identifiers --> just using LETTERS now */
        case LETTER:
            addChar();
            getChar();
            while (charClass == LETTER) {
                addChar();
                getChar();
            }
            checkKey(lexeme);
            //getChar();
            break;

        /* Parse integer literals */
        case DIGIT:
            addChar();
            getChar();
            while (charClass == DIGIT) {
                addChar();
                getChar();
            }
            nextToken = INT_LIT;
            break;

        /* Parentheses and operators */
        case UNKNOWN:
            lookup(nextChar);
            break;

        /* EOF */
        case EOF:
            nextToken = EOF;
            lexeme[0] = 'E';
            lexeme[1] = 'O';
            lexeme[2] = 'F';
            lexeme[3] = 0;
            break;
    } /* End of switch */

    print();

    return nextToken;
} /* End of function lex */

