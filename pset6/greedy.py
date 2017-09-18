import cs50

def main():
    print("O hai!")
    # input how much change is owned
    while(True):
        print("How much change is owed?")
        change_in_cents = round(cs50.get_float() * 100)
        if (change_in_cents > 0):
            break

    print("Total change: {}".format(change_in_cents))
    # convert dollars into cents
    # declare a variable to count coins
    # using modulus and divisions calculate required coins starting from the largest denomination

if __name__ == "__main__":
    main()
