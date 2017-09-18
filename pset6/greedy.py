import cs50

def main():
    print("O hai!")

    # get amount of change and convert it into cents
    while(True):
        print("How much change is owed?")
        change_in_cents = round(cs50.get_float() * 100)
        if (change_in_cents > 0):
            break

    # declare variable for the number of coins
    change_given = 0

    # find out the number of returned coins
    if(change_in_cents % 25 >= 0):
        change_given += change_in_cents // 25
        change_in_cents = change_in_cents % 25
      
    if(change_in_cents % 10 >= 0):
        change_given += change_in_cents // 10
        change_in_cents = change_in_cents % 10

    if(change_in_cents % 5 >= 0):
        change_given += change_in_cents // 5
        change_in_cents = change_in_cents % 5

    if(change_in_cents % 1 >= 0):
        change_given += change_in_cents // 1
        change_in_cents = change_in_cents % 1

    # print total number of returned coins
    print(change_given)


if __name__ == "__main__":
    main()
