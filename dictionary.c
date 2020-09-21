// Implements a dictionary's functionality

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
const unsigned int N = 34259;

// Hash table
node *table[N];

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    // TODO
    int LEN = strlen(word);
    char w[LEN + 1];
    for (int i = 0; i < LEN; ++i)
    {
        w[i] = tolower(word[i]);
    }
    w[LEN] = '\0';

    int idx = hash(w);
    node *cur = table[idx];

    while (cur != NULL)
    {
        if (strcmp(cur->word, w) == 0)
        {
            return true;
        }
        cur = cur->next;
    }

    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // TODO
    unsigned int hsh = 0;
    for (int i = 0; word[i]; ++i)
    {
        hsh = (hsh * 26 + word[i]) % N;
    }
    return hsh;
}

int cnt_word = 0;

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    // TODO
    FILE *file = fopen(dictionary, "r");
    if (file == NULL)
    {
        return false;
    }
    for (int i = 0; i < N; i++)
    {
        table[i] = NULL;
    }
    char word[LENGTH + 1];
    while (fscanf(file, "%s", word) != EOF)
    {
        ++cnt_word;
        int idx = hash(word);

        node *newNode = (node *)malloc(sizeof(node));
        strcpy(newNode->word, word);
        newNode->next = table[idx];
        table[idx] = newNode;
    }
    fclose(file);
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    // TODO
    return cnt_word;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    // TODO
    for (int i = 0; i < N; ++i)
    {
        node *cur = table[i];
        node *tmp = cur;
        table[i] = NULL;
        while (cur != NULL)
        {
            cur = cur->next;
            free(tmp);
            tmp = cur;
        }
    }
    return true;
}
