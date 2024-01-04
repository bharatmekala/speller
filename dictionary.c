#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <stdio.h>
#include <cs50.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

int counter = 0;

// TODO: Choose number of buckets in hash table
const unsigned int N = 100000;

// Hash table
node *table[N];

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    char *test = malloc((strlen(word) + 1) * sizeof(char));

    for (int i = 0; i < strlen(word); i++)
    {
        test[i] = tolower(word[i]);
    }

    test[strlen(word)] = '\0';

    int h = hash(test) % N;
    node *cur = table[h];

    while (cur != NULL)
    {
        if (strcasecmp(cur->word, test) == 0)
        {
            free(test);
            return true;
        }
        cur = cur->next;
    }

    free(test);
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    unsigned int hash = 5381;
    int c;

    while ((c = *word++))
    {
        hash = ((hash << 5) + hash) + c; // hash = hash * 33 + c
    }

    return hash % N;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    for (int i = 0; i < N; i++)
    {
        table[i] = NULL;
    }

    FILE *inpt = fopen(dictionary, "r");
    if (inpt == NULL)
    {
        return false;
    }

    char *word = malloc((LENGTH + 1) * sizeof(char));
    if (word == NULL)
    {
        fclose(inpt);
        return false;
    }

    int result = fscanf(inpt, "%s", word);

    while (result != EOF)
    {
        int h = hash(word) % N;
        node *new_node = malloc(sizeof(node));
        if (new_node == NULL)
        {
            fclose(inpt);
            free(word);
            return false;
        }

        strcpy(new_node->word, word);
        new_node->next = NULL;

        if (table[h] == NULL)
        {
            table[h] = new_node;
            counter++;
        }
        else
        {
            node *cur = table[h];
            table[h] = new_node;
            new_node->next = cur;
            counter++;
        }

        result = fscanf(inpt, "%s", word);
    }

    free(word);
    fclose(inpt);
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    return counter;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    for (int i = 0; i < N; i++)
    {
        node *cur = table[i];

        while (cur != NULL)
        {
            node *tmp = cur->next;
            free(cur);
            cur = tmp;
        }
    }

    return true;
}
