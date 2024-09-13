#include "alias.h"
#define MAX_LINE_LENGTH 256
// Function to add an alias to the linked list
void add_alias(AliasNode **head, char *name, char *command) {
    AliasNode *new_node = (AliasNode *)malloc(sizeof(AliasNode));
    if (new_node == NULL) {
        perror("Failed to allocate memory");
        exit(EXIT_FAILURE);
    }
    strcpy(new_node->alias_name, name);
    strcpy(new_node->alias_command, command);
    new_node->next = *head;
    *head = new_node;
}

// Function to search for an alias in the linked list
char *find_alias(AliasNode *head, char *name) {
    AliasNode *current = head;
    while (current != NULL) {
        if (strcmp(current->alias_name, name) == 0) {
            return current->alias_command;
        }
        current = current->next;
    }
    return NULL;  // Return NULL if alias not found
}

void gupta(char *filename, AliasNode **head)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        perror("Failed to open file");
        return;
    }

    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), file))
    {
        // Check if the line starts with "alias"
        if (strncmp(line, "alias", 5) == 0)
        {
            // Parse the alias name and its command
            char alias_name[MAX_LINE_LENGTH];
            char alias_command[MAX_LINE_LENGTH];

            // Extract the alias name and command from the line
            sscanf(line, "alias %[^=]=\'%[^\']\'", alias_name, alias_command);

            // Add the alias to the linked list
            add_alias(head, alias_name, alias_command);
        }
    }

    fclose(file);
}

void print(AliasNode* head)
{
    while(head!=NULL)
    {
        printf("%s %s",head->alias_command,head->alias_name);
        head=head->next;
    }
}