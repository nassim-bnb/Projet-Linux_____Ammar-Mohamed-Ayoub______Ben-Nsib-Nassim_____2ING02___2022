from tkinter import *
from tkinter.ttk import *
from sys import argv

numero = argv[1]

fenetre = Tk()
fenetre.title(f"Client-{numero}")
fenetre.geometry("800x500")
fenetre.resizable(0,0)


v = Scrollbar(fenetre, orient='vertical')
v.pack(side=RIGHT, fill='y')

texte = Text(fenetre, font=("Georgia, 12"), yscrollcommand=v.set)


with open(f"./build/temp/trace_communicarion_client_{numero}.txt", "r") as file:
    trace_communicarion = "".join(file.readlines())


texte.insert(END, trace_communicarion)

v.config(command=texte.yview)
texte.pack()

fenetre.mainloop()