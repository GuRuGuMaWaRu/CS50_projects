import cs50
import sys

def main():
    # ensure that exactly one argument is used
    if (len(sys.argv) != 2):
        print("You must enter exactly one argument to use as a cipher key.")
        return 1

    # prepare variables
    string_keyword = sys.argv[1]
    keyword_length = len(string_keyword)



if __name__ == "__main__":
    main()
