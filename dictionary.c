// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
} node;

// Choose number of buckets in hash table
const unsigned int BUCKETS = 143107; // smallest prime number greater than size of dict/large,
                                     // trying to target 1.0 load factor.

// Hash table
node *table[BUCKETS];

// Word count global
unsigned int word_count = 0;

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    unsigned int hash_value = hash(word);

    // Check for node at given hash value in hash table. Unoccoupied locations are initialized to
    // NULL as "table" is a global var.
    node *bucket = table[hash_value];

    while (bucket != NULL)
    {
        if (strcasecmp(bucket->word, word) == 0)
        {
            return true;
        }
        // if initial node's "word" value isn't equal to paramater of check(), check for the next
        // node in the linked list.
        bucket = bucket->next;
    }
    return false;
}

// Hashes word to a number
unsigned int hash(const char *buffer)
{
    long long big_prime = 2147483647; // ceiling (highest [32 bit / 4 byte] prime)
    int small_prime = 31;             // base (smallest prime greater than size of character set)

    long long hash = 0;

    for (int i = 0; buffer[i] != '\0'; i++)
    {
        // *** polynomial rolling hash ***
        // https://en.wikipedia.org/wiki/Horner%27s_method (remember the Accordion :p)
        // https://en.wikipedia.org/wiki/Rabin%E2%80%93Karp_algorithm
        // handle apostrophe to avoid negative number.
        if (buffer[i] == '\'')
        {
            hash = (small_prime * hash + 27) % big_prime;
        }
        // normalize so character set is mapped to 1 - 26, buffer[i] never being larger than small_prime reduces colisions.
        else
        {
            hash = (small_prime * hash + (tolower(buffer[i]) - 'a' + 1)) % big_prime;
        }
    }
    unsigned int hash_value = hash % BUCKETS;

    return hash_value;
}

// helper funciton to make code prettier, handle counting dict size.
node *_create_node()
{
    node *word_node_loc = malloc(sizeof(node));
    // handle malloc failure
    if (word_node_loc == NULL)
    {
        printf("Failed to allocate memory for node.\n");
        return word_node_loc;
    }
    // set "next" to a known value
    word_node_loc->next = NULL;
    // increment global counter.
    word_count++;

    return word_node_loc;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    char word_buffer[LENGTH];
    FILE *dict = fopen(dictionary, "r");
    // handle fopen err
    if (dict == NULL)
    {
        printf("Err: Unable to open file %s.", dictionary);
        return false;
    }
    while (fscanf(dict, "%s", word_buffer) == 1)
    {
        // find location in hash table for new word
        unsigned int hash_value = hash(word_buffer);

        // *** prepend node ***
        node *new_node = _create_node();
        if (new_node == NULL)
        {
            printf("Failed to allocate memory for node.\n");
            fclose(dict);
            return false;
        }
        // copy the values from word_buffer into new_node's word member.
        strcpy(new_node->word, word_buffer);
        // set new_node's next value to the current head of the list.
        new_node->next = table[hash_value];
        // overwrite the head of the list's address in the hash table with the a pointer to the new
        // node.
        table[hash_value] = new_node;
    }
    fclose(dict);
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    // global var which is incremented when _create_node is called.
    return word_count;
}

// Recusively frees nodes in given bucket
void _free(node *node)
{
    // handle empty bucket
    if (node == NULL)
    {
        return;
    }
    // base case; next == NULL
    else if (node->next == NULL)
    {
        free(node);
        return;
    }
    // recursive case; if next, traverse list, free self.
    else
    {
        _free(node->next);
        free(node);
    }
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    for (int i = 0; i < BUCKETS; i++)
    {
        _free(table[i]);
    }
    return true;
}
