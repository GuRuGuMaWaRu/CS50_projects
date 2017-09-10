/**
 * Implements a dictionary's functionality.
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "dictionary.h"

// define struct node
/**
 * NODE
 *
 * The NODE structure contains information about whether
 * this is a word and the next children (characters).
 */

typedef struct node
{
    bool is_word;
    struct node *children[27];
}
node;

// define root (first) node
node *root = NULL;

/**
 * Returns alphabet position for a character (including apostrophe)
 */
int get_alphabet_position(const char character)
{
    if (character == '\'')
    {
        return 26;
    }
    else if (isupper(character))
    {
        return character - 65;
    }
    return character - 97;
}

/**
 * Presets new node children to NULL
 */
void preset_to_null(node *new_node)
{
    for (int i = 0; i < 27; i++)
        new_node->children[i] = NULL;
}

/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char *word)
{
    // check input argument
    if (word == NULL)
    {
        fprintf(stderr, "Check function received NULL instead of a word!\n");
    }

    // set cursor pointer that will always point at current position in our tie structure
    node *cursor = root;
    // set helper variables
    char current_char;
    int char_position;

    for (int i = 0, len = strlen(word); i < len; i++)
    {
        // initialize helper variables
        current_char = word[i];
        char_position = get_alphabet_position(current_char);

        // there is no such character hence there is no such word in our dictionary
        if (cursor->children[char_position] == NULL)
        {
            return false;
        }

        // we found current character and move on to the next one
        cursor = cursor->children[char_position];
    }

    // check if the word we found exists in our dictionary
    if (cursor->is_word)
    {
        return true;
    }

    // there is no such word in our dictionary
    return false;
}

/**
 * Loads dictionary into memory. Returns true if successful else false.
 */
bool load(const char *dictionary)
{
    // try to open dictionary
    FILE *fp = fopen(dictionary, "r");
    if (fp == NULL)
    {
        fprintf(stderr, "Could not open dictionary with path: %s\n", dictionary);
        return false;
    }

    // allocate memory for the root (first) node
    root = malloc(sizeof(node));
    preset_to_null(root);
    // variable to store current word
    char word[LENGTH+1];
    // variable to store current child
    int child;
    // node variables
    node *current = NULL;
    node *new_node = NULL;

    // loop through all words in dictionary
    while (fscanf(fp, "%s", word) != EOF)
    {
        // make root the current node
        current = root;

        // loop through every character of a word & place it into memory
        for (int i = 0, len = strlen(word); i < len; i++)
        {
            // check if the current word is too long
            if (strlen(word) > LENGTH)
            {
                fprintf(stderr, "Error! Dictionary contains at least one word over 45 characters long.\n");
                return false;
            }

            child = get_alphabet_position(word[i]);

            // check if the required child node already exists
            if (current->children[child] == NULL)
            {
                new_node = malloc(sizeof(node));
                preset_to_null(new_node);
                // check if there is enough memory
                if (new_node == NULL)
                {
                    fprintf(stderr, "There is not enough memory.\n");
                    return false;
                }

                current->children[child] = new_node;
            }

            current = current->children[child];
        }
        current->is_word = true;
    }

    // close dictionary file
    fclose(fp);

    return true;
}

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void)
{
    // TODO
    return 0;
}

/**
 * Unload helper
 */
bool unload_helper(node *current_node)
{
    for (int i = 0; i < 27; i++)
    {
        if (current_node->children[i] != NULL)
        {
            unload_helper(current_node->children[i]);
        }
    }

    free(current_node);
    return true;
}

/**
 * Unloads dictionary from memory. Returns true if successful else false.
 */
bool unload(void)
{
    unload_helper(root);
    return true;
}
