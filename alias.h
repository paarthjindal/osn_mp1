#ifndef ALIAS_H
#define ALIAS_H
#include "main.h"

typedef struct AliasNode {
    char alias_name[256];
    char alias_command[256];
    struct AliasNode *next;
} AliasNode;

extern AliasNode* start;
void print(AliasNode* head);
void add_alias(AliasNode **head, char *name, char *command) ;

char *find_alias(AliasNode *head, char *name) ;
void gupta(char *filename, AliasNode **head);


#endif