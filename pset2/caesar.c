#include <stdio.h>
#include <cs50.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

char get_ciphered_char(char character, int first_char, int counter);

int main(int argc, string argv[])
{
    // check if there is a command-line argument
    if (argc != 2)
    {
        printf("Please provide one command-line integer argument!\n");
        return 1;
    }

    // convert command-line argument into an integer
    int counter = atoi(argv[1]);

    // get text from user
    printf("plaintext: ");
    string plain_text = get_string();
    char cipher_text [strlen(plain_text)];

    // cryptify every character of the provided text
    for (int i = 0, len = strlen(plain_text); i < len; i++)
    {
        char character = plain_text[i];

        if (isalpha(character))
        {
            if (isupper(character))
            {
                cipher_text[i] = get_ciphered_char(character, 65, counter);
            }
            else
            {
                cipher_text[i] = get_ciphered_char(character, 97, counter);
            }
        }
        else
        {
            cipher_text[i] = character;
        }
    }

    printf("ciphertext: %s", cipher_text);
    printf("\n");

    return 0;
}

char get_ciphered_char(char character, int first_char, int counter)
{
    return ((character - first_char + counter) % 26) + first_char;
}
