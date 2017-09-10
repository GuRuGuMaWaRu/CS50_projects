#include <stdio.h>
#include <cs50.h>
#include <ctype.h>
#include <string.h>

int main(int argc, string argv[])
{

    // check if command line has only one argument
    if (argc != 2)
    {
        printf("You must enter exactly one argument to use as a cipher key.\n");
        return 1;
    }

    string keyword = argv[1];
    int keyword_length = strlen(keyword);


    // check if command line argument contains only alphabetical characters
    for (int j = 0; j < keyword_length; j++)
    {
        if (!isalpha(keyword[j]))
        {
            printf("The argument must contain only alphabetical characters.\n");
            return 1;
        }
    }

    // get plaintext
    printf("plaintext: ");
    string plain_text = get_string();
    int plain_text_length = strlen(plain_text);

    // prepare a container for enciphered text
    char enciphered_text[plain_text_length + 1];

    // create a counter to iterate through keyword characters
    int keyword_counter = 0;

    // encipher plaintext
    for (int i = 0; i < plain_text_length; i++)
    {
        char character = plain_text[i];

        if (isalpha(character))
        {
            char plain_text_range_start = isupper(character) ? 65 : 97;
            char keyword_character = keyword[keyword_counter % keyword_length];
            int counter = keyword_character - (isupper(keyword_character) ? 65 : 97);

            enciphered_text[i] = ((character - plain_text_range_start + counter) % 26) + plain_text_range_start;

            keyword_counter += 1;
        }
        else
        {
            enciphered_text[i] = character;
        }
    }

    enciphered_text[plain_text_length] = '\0';

    printf("ciphertext: %s\n", enciphered_text);

}
