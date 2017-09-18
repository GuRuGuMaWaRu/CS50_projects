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
    string_keyword = sys.argv[1]
    keyword_length = len(string_keyword)

    # get plaintext
    print("plaintext: ", end="");
    plain_text = cs50.get_string()
    plain_text_length = len(plain_text)

if __name__ == "__main__":
    main()
