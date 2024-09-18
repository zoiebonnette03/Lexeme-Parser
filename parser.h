#ifndef PARSER_H
#define PARSER_H

void pro();
void sent();
void u();
void comp();
void expr();
void term();
void factor();

extern int nextToken;
extern char lexeme[100];
#endif
