import cs50

def main():
    # request pyramid height
    while(True):
        print("Please input height of the pyramid: ", end="")
        input_value = cs50.get_int()
        if (input_value > 0 and input_value < 23):
            break

    # print pyramid
    for i in range(1, input_value + 1):
        # print empty space
        print(" " * (input_value - i), end="")
        # print pyramid level
        print("#" * i)

if __name__ == "__main__":
    main()
