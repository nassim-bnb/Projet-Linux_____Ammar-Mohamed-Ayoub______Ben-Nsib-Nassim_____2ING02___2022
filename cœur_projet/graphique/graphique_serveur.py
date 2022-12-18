from tkinter import *
import time
from tkinter.ttk import *

fenetre = Tk()
fenetre.title("Serveur")
fenetre.geometry("800x500")
fenetre.resizable(0,0)



v = Scrollbar(fenetre, orient='vertical')
v.pack(side=RIGHT, fill='y')

texte = Text(fenetre, font=("Georgia, 12"), yscrollcommand=v.set)


with open(f"./build/temp/trace_communicarion_serveur.txt", "r") as file:
    trace_communicarion = "".join(file.readlines())


texte.insert(END, trace_communicarion)

v.config(command=texte.yview)
texte.pack()

fenetre.mainloop()