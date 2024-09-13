#include "alias.h"
#define MAX_LINE_LENGTH 256
// Function to add an alias to the linked list
void add_alias(AliasNode **head, char *name, char *command)
{
    AliasNode *temp = (AliasNode *)malloc(sizeof(AliasNode));
    if (temp == NULL)
    {
        printf("Failed to allocate memory");
        exit(EXIT_FAILURE);
    }
    strcpy(temp->alias_name, name);
    strcpy(temp->alias_command, command);
    temp->next = *head;
    *head = temp;
}

// Function to search for an alias in the linked list
char *find_alias(AliasNode *head, char *name)
{
    AliasNode *current = head;
    while (current != NULL)
    {
        if (strcmp(current->alias_name, name) == 0)
        {
            return current->alias_command;
        }
        current = current->next;
    }
    return NULL; // Return NULL if alias not found
}


void print(AliasNode *head)
{
    while (head != NULL)
    {
        printf("%s %s", head->alias_command, head->alias_name);
        head = head->next;
    }
}