import cs50
import sys

def main():
    # ensure there is exactly one command line argument
    if (len(sys.argv) != 2):
        print("You must enter exactly one argument to use as a cipher key.")
        return 1

    # ensure the provided keyword contains only alphabetical characters
    if (not sys.argv[1].isalpha()):
        print("Cipher keyword must be a word, not a number.")
        return 2

    # prepare variables
    keyword = sys.argv[1]
    keyword_length = len(keyword)

    # prepare a container for enciphered text
    enciphered_text = ""

    # create a counter to iterate through keyword characters
    keyword_counter = 0

    # get plaintext
    print("plaintext: ", end="");
    plain_text = cs50.get_string()
    plain_text_length = len(plain_text)

    # encipher text
    for character in plain_text:
        if (character.isalpha()):
            plain_text_range_start = 65 if character.isupper() else 97
            keyword_character = keyword[keyword_counter % keyword_length]
            counter = ord(keyword_character) - (65 if keyword_character.isupper() else 97)

            enciphered_text += chr(((ord(character) - plain_text_range_start + counter) % 26) + plain_text_range_start)

            keyword_counter += 1
        else:
            enciphered_text += character

    # print enciphered text
    print("ciphertext: {}".format(enciphered_text))

if __name__ == "__main__":
    main()
