import random
import os
from termcolor import cprint
from tkinter import *
from PIL import ImageTk, Image

class Player:

    def __init__(self, name):
        self.hand = []
        self.secretavailable = True
        self.tradeoffavailable = True
        self.giftavailable = True
        self.competitionavailable = True
        self.geishagifts = [0, 0, 0, 0, 0, 0, 0]
        self.geishalove = [0, 0, 0, 0, 0, 0, 0, 0]
        self.secretcard = 0
        self.score = 0
        self.name = name

class Hanamikoji():

    def __init__(self, master=None):
        # super().__init__(master)
        # self.master = master
        # self.pack()

        # self.hi_there = Button(self)
        # self.hi_there["text"] = "Hello World\n(click me)"
        # self.hi_there["command"] = self.deal
        # self.hi_there.pack(side="top")

        # self.quit = Button(self, text="QUIT", fg="red",
        #                       command=self.master.destroy)
        # self.quit.pack(side="bottom")

        # img = Image.open("Resources/Geisha1.jpeg")
        # imago = ImageTk.PhotoImage(img)
        # self.geishaOne = Label(image=imago)
        # self.geishaOne.image = imago
        # self.geishaOne.pack
        # self.img = ImageTk.PhotoImage(Image.open("Resources/Geisha1.jpeg"))
        # geishaOne.create_image(image=self.img)

    
        self.playerhand = []
        self.deck = [1, 1, 2, 2, 3, 3, 4, 4, 4, 5, 5, 5, 6, 6, 6, 6, 7, 7, 7, 7, 7]
        random.shuffle(self.deck)
        self.player_one = Player("Player 1")
        self.player_two = Player("Player 2")

    def draw(self, player):
        player.hand.append(self.deck.pop())
        player.hand.sort()

    def deal(self):
        for i in range(6):
            self.draw(self.player_one)
            self.draw(self.player_two)
        self.player_one.hand.sort()
        self.player_two.hand.sort()

    def turn(self, player):
        os.system("clear")
        self.draw(player)
        self.table()
        print(player.name, "turn, your cards:")
        self.displayhand(player)
        print("What would you like to do? Write the corresponding number!")
        if player.secretavailable:
            print("1. Secret")
        if player.tradeoffavailable:
            print("2. Tradeoff")
        if player.giftavailable:
            print("3. Gift")
        if player.competitionavailable:
            print("4. Competition")
        while True:
            action = int(input())
            if action == 1 and player.secretavailable:
                self.secret(player)
                break
            elif action == 2 and player.tradeoffavailable:
                self.tradeoff(player)
                break
            elif action == 3 and player.giftavailable:
                self.gift(player)
                break
            elif action == 4 and player.competitionavailable:
                self.competition(player)
                break
    
    def card(self, code):
        if code == 1: cprint("flute 2", on_color='on_cyan', end='')
        if code == 2: cprint("fan 2", on_color='on_red', end='')
        if code == 3: cprint("scroll 2", on_color='on_yellow', end='')
        if code == 4: cprint("parasol 3", on_color='on_blue', end='')
        if code == 5: cprint("guitar 3", on_color='on_grey', end='')
        if code == 6: cprint("tea 4", on_color='on_green', end='')
        if code == 7: cprint("flower 5", on_color='on_magenta', end='')
        
    def secret(self, player):
        player.secretavailable = False
        print("Which card would you like to set aside?")
        while True:
            inp = int(input())
            if inp > len(player.hand):
                print ("You don't have that many cards in your hand!")
            else:
                break
        player.secretcard = player.hand.pop(inp - 1)

    def tradeoff(self, player):
        player.tradeoffavailable = False
        print ("Which cards would you like to discard?")
        while True:
            cone = int(input())
            ctwo = int(input())
            if cone > len(player.hand) or ctwo > len(player.hand) or cone == ctwo:
                print("Wrong input, repeat both cards!")
            else:
                break
        for ele in sorted([cone-1, ctwo-1], reverse = True):
            del player.hand[ele] 
        
    def gift(self, player):
        player.giftavailable = False
        print("Which cards would you like to offer?")
        while True:
            cone = int(input())
            ctwo = int(input())
            cthree = int(input())
            if cone > len(player.hand) or ctwo > len(player.hand) or cthree > len(player.hand) or cone == ctwo or cone == cthree or ctwo == cthree:
                print("Wrong input, repeat all three cards!")
            else:
                break
        os.system("clear")
        self.table()
        print(player.name, "played gift!")
        self.displayhand(self.other(player))
        print("\n The cards offered:")
        print("1.", end='')
        self.card(player.hand[cone-1])
        print("\n2.", end='')
        self.card(player.hand[ctwo-1])
        print("\n3.", end='')
        self.card(player.hand[cthree-1])
        print("\nWhich card do you choose?")
        while True:
            choice = int(input())
            if choice > 3 or choice < 1:
                print("Wrong input, repeat your choice!")
            else:
                break
        if choice == 1:
            player.geishagifts[player.hand[cone-1]-1] += 1
        else:
            self.other(player).geishagifts[player.hand[cone-1]-1] += 1
        if choice == 2:
            player.geishagifts[player.hand[ctwo-1]-1] += 1
        else:
            self.other(player).geishagifts[player.hand[ctwo-1]-1] += 1
        if choice == 3:
            player.geishagifts[player.hand[cthree-1]-1] += 1
        else:
            self.other(player).geishagifts[player.hand[cthree-1]-1] += 1
        for ele in sorted([cone-1, ctwo-1, cthree-1], reverse = True):
            del player.hand[ele] 
    
    def competition(self, player):
        player.competitionavailable = False
        print ("Which cards should be in the first pair?")
        while True:
            cone = int(input())
            ctwo = int(input())
            if cone > len(player.hand) or ctwo > len(player.hand) or cone == ctwo:
                print("Wrong input! Which cards should be in the first pair?")
            else:
                break
        print ("Which cards should be in the second pair?")
        while True:
            cthree = int(input())
            cfour = int(input())
            if cthree > len(player.hand) or cfour > len(player.hand) or cthree == cone or cthree == ctwo or cthree == cfour or cfour == cone or cfour == ctwo:
                print("Wrong input! Which cards should be in the second pair?")
            else:
                break
        os.system("clear")
        self.table()
        print("The opponent played competition!")
        print("1.", end='')
        self.card(player.hand[cone - 1])
        print(" & ", end ='')
        self.card(player.hand[ctwo - 1])
        print("\n2.",end='')
        self.card(player.hand[cthree - 1])
        print(" & ", end='')
        self.card(player.hand[cfour - 1])
        print("\nWhich pair do you choose?")
        while True:
            choice = int(input())
            if not (choice == 1 or choice == 2):
                print("Wrong input! Which pair do you choose?")
            else:
                break
        if choice == 1:
            self.other(player).geishagifts[player.hand[cone - 1]-1] += 1
            self.other(player).geishagifts[player.hand[ctwo - 1]-1] += 1
            player.geishagifts[player.hand[cthree - 1]-1] += 1
            player.geishagifts[player.hand[cfour - 1]-1] += 1
        else:
            player.geishagifts[player.hand[cone - 1]-1] += 1
            player.geishagifts[player.hand[ctwo - 1]-1] += 1
            self.other(player).geishagifts[player.hand[cthree - 1]-1] += 1
            self.other(player).geishagifts[player.hand[cfour - 1]-1] += 1
        for ele in sorted([cone-1, ctwo-1, cthree-1, cfour-1], reverse = True):
            del player.hand[ele] 
    
    def other(self, player):
        if player.name == "Player 1":
            return self.player_two
        if player.name == "Player 2":
            return self.player_one
    
    def table(self):
        pass

    def endgame(self):
        os.system("clear")
        pass

    def displayhand(self, player):
        cardnumber = 0
        for card in player.hand:
            cardnumber += 1
            print(str(cardnumber) + ".", end='')
            self.card(card)
            print()

class Application(Frame):
    def __init__(self, master=None):
        super().__init__(master)
        self.master = master
        self.pack()

        self.hi_there = Button(self)
        self.hi_there["text"] = "Hello World\n(click me)"
        self.hi_there["command"] = self.say_hi
        self.hi_there.pack(side="top")

        self.quit = Button(self, text="QUIT", fg="red",
                              command=self.master.destroy)
        self.quit.pack(side="bottom")

    def say_hi(self):
        print("hi there, everyone!")


if __name__ == '__main__':

    root = Tk()
    root.title("Hanamikoji")
    root.configure(width = 1000, height = 800)
    # oreg = Application(master=root)
    # oreg.mainloop()
    game = Hanamikoji()
    game.deal
    # game.mainloop()

    for i in range(4):
        game.turn(game.player_one)
        game.turn(game.player_two)
    game.endgame()
        
        









