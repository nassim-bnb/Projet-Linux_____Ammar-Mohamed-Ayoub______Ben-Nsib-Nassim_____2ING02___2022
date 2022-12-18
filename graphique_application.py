from tkinter import *
from tkinter.messagebox import *
from PIL import ImageTk, Image
from os import system,path
from time import sleep
from random import randint,choice
import string
import socket

chemin_base = "."
    
def demarrer_application(fenetre : Tk) -> Tk:
        fenetre.mainloop()
        
        
def handler_boutton_moins(etiquette_nombre_client : Label) -> None:
    def logique(event) -> None:
        nombre_client = int(etiquette_nombre_client.cget("text"))
        if(nombre_client > 1):
            etiquette_nombre_client.config(text=str(nombre_client-1))
            
    return logique
            
def handler_boutton_plus(etiquette_nombre_client : Label) -> None:
    def logique(event) -> None:
        nombre_client = int(etiquette_nombre_client.cget("text"))
        etiquette_nombre_client.config(text=str(nombre_client+1))
        
    return logique


def handler_choix_socket() -> None:
    def logique(event) -> None:
        global type_communication,type_socket
        type_communication = "SOCKET"
        type_socket = "TCP"
        fifo_radio_2.place(x=610,y=302)
        fifo_radio_1.place_forget()
        socket_radio_1.place(x=120,y=305)
        socket_radio_2.place_forget()
        socket_tcp_radio_1.place(x=200,y=405)
        socket_tcp_radio_2.place_forget()
        socket_udp_radio_2.place(x=360,y=405)
        socket_udp_radio_1.place_forget()
            
    return logique


def handler_choix_socket_tcp() -> None:
    def logique(event) -> None:
        global type_communication,type_socket
        type_communication = "SOCKET"
        type_socket = "TCP"
        fifo_radio_2.place(x=610,y=302)
        fifo_radio_1.place_forget()
        socket_radio_1.place(x=120,y=305)
        socket_radio_2.place_forget()
        socket_tcp_radio_1.place(x=200,y=405)
        socket_tcp_radio_2.place_forget()
        socket_udp_radio_2.place(x=360,y=405)
        socket_udp_radio_1.place_forget()
    
    return logique


def handler_choix_socket_udp() -> None:
    def logique(event) -> None:
        global type_communication,type_socket
        type_communication = "SOCKET"
        type_socket = "UDP"
        fifo_radio_2.place(x=610,y=302)
        fifo_radio_1.place_forget()
        socket_radio_1.place(x=120,y=305)
        socket_radio_2.place_forget()
        socket_tcp_radio_2.place(x=200,y=405)
        socket_tcp_radio_1.place_forget()
        socket_udp_radio_1.place(x=360,y=405)
        socket_udp_radio_2.place_forget()
            
    return logique
    
 
def handler_choix_fifo() -> None:
    def logique(event) -> None:
        global type_communication,type_socket
        type_communication = "FIFO"
        type_socket = None
        fifo_radio_1.place(x=610,y=302)
        fifo_radio_2.place_forget()
        socket_radio_2.place(x=120,y=305)
        socket_radio_1.place_forget()
        socket_tcp_radio_2.place(x=200,y=405)
        socket_tcp_radio_1.place_forget()
        socket_udp_radio_2.place(x=360,y=405)
        socket_udp_radio_1.place_forget()
            
    return logique
           
            
def handler_generer_nombre_client(etiquette_nombre_client : Label) -> None:
    def logique(event) -> None:
        etiquette_nombre_client.config(text=str(randint(3,30)))
            
    return logique


def handler_generer_port_serveur(champ_port_serveur : Entry) -> None:
    def logique(event) -> None:
        champ_port_serveur.delete(0,END)
        champ_port_serveur.insert(0,str(randint(1,65536)))
            
    return logique


def handler_generer_nom(champ : Entry) -> None:
    def logique(event) -> None:
        lettres = string.ascii_lowercase
        nom = ''.join(choice(lettres) for i in range(10))
        champ.delete(0,END)
        champ.insert(0,nom)
            
    return logique


def handler_verifier_port_serveur(champ : Entry) -> None:
    def logique(event) -> None:
        try:
            port = int(champ.get().strip())
            if(port < 1 or port > 65536 ):
                showerror(title="Erreur", message="Numero port invalide (Le numéro du port doit être entre 1..65536) !!")
                return
                
            s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            

            try:
                s.bind(("127.0.0.1", int(champ.get())))
                showinfo(title="Info",message="Ce port est valide et disponible")
                s.close()
            except socket.error as e:
                showerror(title="Erreur", message="Ce port est déjà utilisé !!")
                
        except Exception as e:
            showerror(title="Erreur", message="Numero port invalide !!")
            
    return logique


def handler_creer_tube_nomme(champ1 : Entry,champ2 : Entry) -> None:
    def logique(event) -> None:
        nom = champ1.get().strip()
        if(len(nom)==0):
            showerror(title="Erreur", message="Champ vide !!")
        elif(champ1.get().strip() == champ2.get().strip()):
            showerror(title="Erreur", message="Le nom du tube de la question et celui du tube de la réponse ne doivent pas être identiques.")
        elif(path.exists(f"./build/temp/{nom}")):
            showerror(title="Erreur", message="Ce tube existe déjà")
        else:
            system(f"mkfifo ./build/temp/{nom}")
            showinfo(title="Info", message="Le tube est créé avec succès")
            
            
    return logique


def handler_demarrer_simulation(champ_port_serveur : Entry,etiquette_nombre_client : Label,champ_nom_tube_questions : Entry,champ_nom_tube_reponses : Entry):

    def logique(event):
        global type_communication,type_socket
        port_serveur = 0
        nombre_client = int(etiquette_nombre_client.cget("text"))
        nom_tube_questions = f"./build/temp/{champ_nom_tube_questions.get().strip()}"
        nom_tube_reponses = f"./build/temp/{champ_nom_tube_reponses.get().strip()}"
        
        if(type_communication=="SOCKET"):
            try:
                port_serveur = int(champ_port_serveur.get().strip())
                if(port_serveur < 1 or port_serveur > 65536 ):
                    showerror(title="Erreur", message="Numero port invalide (Le numéro du port doit être entre 1..65536) !!")
                    return
                    
                s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
                

                try:
                    s.bind(("127.0.0.1", port_serveur))
                    s.close()
                except socket.error as e:
                    showerror(title="Erreur", message="Ce port est déjà utilisé !!")
                    return
                    
            except Exception as e:
                showerror(title="Erreur", message="Numero port invalide !!")
                return
        else:
            tube_reponses = champ_nom_tube_reponses.get().strip()
            tube_questions = champ_nom_tube_questions.get().strip()
            if(len(tube_questions)==0):
                showerror(title="Erreur", message="Champ du nom de tube questions est vide !!")
                return
            elif(len(tube_reponses)==0):
                showerror(title="Erreur", message="Champ du nom de tube réponses est vide !!")
                return
            elif(tube_reponses == tube_questions):
                showerror(title="Erreur", message="Le nom du tube de la questions et celui du tube de la réponse ne doivent pas être identiques.")
                return
            elif(not path.exists(f"./build/temp/{tube_questions}")):
                showerror(title="Erreur", message="Le tube de questions n'existe pas")
                return
            elif(not path.exists(f"./build/temp/{tube_reponses}")):
                showerror(title="Erreur", message="Le tube de réponses n'existe pas")
                return

        if(type_communication == "SOCKET" and type_socket == "TCP"):
            chemin_client = f"./{chemin_base}/build/client-TCP"
            chemin_serveur = f"./{chemin_base}/build/serveur-TCP"
            commande_serveur = f"{chemin_serveur} {nombre_client} {port_serveur} > ./build/temp/trace_communicarion_serveur.txt &"
            system(f"{commande_serveur}")
            
            sleep(1)
            for numero_client in range(1,nombre_client+1):
                commande_client = f"{chemin_client} {numero_client} {port_serveur} > ./build/temp/trace_communicarion_client_{numero_client}.txt &"
                system(f"{commande_client}")
                sleep(0.025)
                system(f"python3 ./cœur_projet/graphique/graphique_client.py {numero_client} &")
            
            system(f"python3 ./cœur_projet/graphique/graphique_serveur.py &")
            
        elif(type_communication == "SOCKET" and type_socket == "UDP"):
            chemin_client = f"./{chemin_base}/build/client-UDP"
            chemin_serveur = f"./{chemin_base}/build/serveur-UDP"
            commande_serveur = f"{chemin_serveur} {nombre_client} {port_serveur} > ./build/temp/trace_communicarion_serveur.txt &"
            system(f"{commande_serveur}")
            
            sleep(1)
            for numero_client in range(1,nombre_client+1):
                commande_client = f"{chemin_client} {numero_client} {port_serveur} > ./build/temp/trace_communicarion_client_{numero_client}.txt &"
                system(f"{commande_client}")
                sleep(0.025)
                system(f"python3 ./cœur_projet/graphique/graphique_client.py {numero_client} &")
            
            system(f"python3 ./cœur_projet/graphique/graphique_serveur.py &")
        else:
            chemin_client = f"./{chemin_base}/build/client-FIFO"
            chemin_serveur = f"./{chemin_base}/build/serveur-FIFO"
            commande_serveur = f"{chemin_serveur} {nombre_client} {nom_tube_questions} {nom_tube_reponses} > ./build/temp/trace_communicarion_serveur.txt &"
            system(f"{commande_serveur}")
            
            sleep(1)
            for numero_client in range(1,nombre_client+1):
                commande_client = f"{chemin_client} {numero_client} {nom_tube_questions} {nom_tube_reponses} > ./build/temp/trace_communicarion_client_{numero_client}.txt &"
                system(f"{commande_client}")
                sleep(10)
                system(f"python3 ./cœur_projet/graphique/graphique_client.py {numero_client} &")
                print("hani 5rejt")
            system(f"python3 ./cœur_projet/graphique/graphique_serveur.py &")

    return logique


def creer_interface(fenetre : Tk,etiquette_nombre_client : Label,champ_port_serveur : Entry,champ_nom_tube_reponses : Entry) -> None:
    # Configuration de l'interface
    fenetre.geometry("865x705")
    fenetre.title("Application Principale")
    fenetre.configure(bg="white")
    fenetre.resizable(0,0)
    
    # Ajout des composants graphiques
    etiquette_nombre_client = Label(text="3", bg="white", fg="black",font=("Arial",13),pady=0)
    etiquette_nombre_client.place(x=380,y=155)
    
    champ_port_serveur = Entry(borderwidth=0,highlightthickness=0,font=("Arial",12),width=17)
    champ_port_serveur.place(x=95,y=520)
    
    champ_nom_tube_questions = Entry(borderwidth=0,highlightthickness=0,font=("Arial",12),bg="white",width=17)
    champ_nom_tube_questions.place(x=520,y=415) 
    
    champ_nom_tube_reponses = Entry(borderwidth=0,highlightthickness=0,font=("Arial",12),bg="white",width=17)
    champ_nom_tube_reponses.place(x=523,y=520)
    
    boutton_moins = Label(text=" - ", bg="white", fg="black",font=("Arial",13),padx=0)
    boutton_moins.bind('<Button-1>', handler_boutton_moins(etiquette_nombre_client))
    boutton_moins.place(x=348,y=155)
    
    boutton_jouer_test = Label(text=" Démarrer la simulation ", bg="white", fg="black",font=("Arial",16),padx=0)
    boutton_jouer_test.bind('<Button-1>', handler_demarrer_simulation(champ_port_serveur,etiquette_nombre_client,champ_nom_tube_questions,champ_nom_tube_reponses))
    boutton_jouer_test.place(x=345,y=630)
    
    boutton_plus = Label(text=" + ", bg="white", fg="black",font=("Arial",12),pady=0)
    boutton_plus.bind('<Button-1>', handler_boutton_plus(etiquette_nombre_client))
    boutton_plus.place(x=527,y=155)
    
    boutton_generer_nombre_client = Label(text="Générer ", bg="white", fg="black",font=("Arial",13),pady=0)
    boutton_generer_nombre_client.bind('<Button-1>', handler_generer_nombre_client(etiquette_nombre_client))
    boutton_generer_nombre_client.place(x=412,y=195)
    
    boutton_generer_port_serveur = Label(text="Générer ", bg="white", fg="black",font=("Arial",13),pady=0)
    boutton_generer_port_serveur.bind('<Button-1>', handler_generer_port_serveur(champ_port_serveur))
    boutton_generer_port_serveur.place(x=265,y=520)
    
    boutton_generer_nom_tube_reponses = Label(text="Générer ", bg="white", fg="black",font=("Arial",13),pady=0)
    boutton_generer_nom_tube_reponses.bind('<Button-1>', handler_generer_nom(champ_nom_tube_reponses))
    boutton_generer_nom_tube_reponses.place(x=690,y=519)
    
    boutton_generer_nom_tube_questions = Label(text="Générer", bg="white", fg="black",font=("Arial",13),pady=0)
    boutton_generer_nom_tube_questions.bind('<Button-1>', handler_generer_nom(champ_nom_tube_questions))
    boutton_generer_nom_tube_questions.place(x=690,y=413) 
    
    boutton_verifier_port_serveur = Label(text="Vérifier", bg="white", fg="black",font=("Arial",13),pady=0)
    boutton_verifier_port_serveur.bind('<Button-1>', handler_verifier_port_serveur(champ_port_serveur))
    boutton_verifier_port_serveur.place(x=349,y=520)
    
    boutton_nom_tube_questions = Label(text="Créer   ", bg="white", fg="black",font=("Arial",13),pady=0)
    boutton_nom_tube_questions.bind('<Button-1>', handler_creer_tube_nomme(champ_nom_tube_questions,champ_nom_tube_reponses))
    boutton_nom_tube_questions.place(x=775,y=413)
    
    boutton_nom_tube_reponses = Label(text="Créer   ", bg="white", fg="black",font=("Arial",13),pady=0)
    boutton_nom_tube_reponses.bind('<Button-1>', handler_creer_tube_nomme(champ_nom_tube_reponses,champ_nom_tube_questions))
    boutton_nom_tube_reponses.place(x=775,y=519)
    
    
    
# Champs de textes
etiquette_nombre_client = None
champ_port_serveur = None
champ_nom_tube_questions = None
champ_nom_tube_reponses = None

type_communication = "SOCKET"
type_socket = "TCP"

# Fênetre
fenetre = Tk()

# L'ajout de 'limage de l'interface
frame = Frame(fenetre, width=865, height=705)
frame.pack()
frame.place(anchor='center', relx=0.5, rely=0.5)
img = ImageTk.PhotoImage(Image.open("./cœur_projet/graphique/ressources/application.png"))
label = Label(frame, image = img)
label.pack()

# L'ajout des images de radio
socket_radio_img_2 = ImageTk.PhotoImage(Image.open("./cœur_projet/graphique/ressources/boutton_radio_2.png"))
socket_radio_2 = Label(fenetre, image = socket_radio_img_2,borderwidth=0)
socket_radio_2.place(x=120,y=305)
socket_radio_2.bind('<Button-1>', handler_choix_socket())
socket_radio_img_1 = ImageTk.PhotoImage(Image.open("./cœur_projet/graphique/ressources/boutton_radio_1.png"))
socket_radio_1 = Label(fenetre, image = socket_radio_img_1,borderwidth=0)
socket_radio_1.place(x=120,y=305)

socket_tcp_radio_img_2 = ImageTk.PhotoImage(Image.open("./cœur_projet/graphique/ressources/boutton_radio_2.png"))
socket_tcp_radio_2 = Label(fenetre, image = socket_tcp_radio_img_2,borderwidth=0)
socket_tcp_radio_2.place(x=200,y=405)
socket_tcp_radio_2.bind('<Button-1>', handler_choix_socket_tcp())
socket_tcp_radio_img_1 = ImageTk.PhotoImage(Image.open("./cœur_projet/graphique/ressources/boutton_radio_1.png"))
socket_tcp_radio_1 = Label(fenetre, image = socket_tcp_radio_img_1,borderwidth=0)
socket_tcp_radio_1.place(x=200,y=405)

socket_udp_radio_img_1 = ImageTk.PhotoImage(Image.open("./cœur_projet/graphique/ressources/boutton_radio_1.png"))
socket_udp_radio_1 = Label(fenetre, image = socket_udp_radio_img_1,borderwidth=0)
socket_udp_radio_1.place(x=360,y=405)
socket_udp_radio_img_2 = ImageTk.PhotoImage(Image.open("./cœur_projet/graphique/ressources/boutton_radio_2.png"))
socket_udp_radio_2 = Label(fenetre, image = socket_udp_radio_img_2,borderwidth=0)
socket_udp_radio_2.place(x=360,y=405)
socket_udp_radio_2.bind('<Button-1>', handler_choix_socket_udp())

fifo_radio_im_1 = ImageTk.PhotoImage(Image.open("./cœur_projet/graphique/ressources/boutton_radio_1.png"))
fifo_radio_1 = Label(fenetre, image = fifo_radio_im_1,borderwidth=0)
fifo_radio_1.place(x=610,y=302)
fifo_radio_img_2 = ImageTk.PhotoImage(Image.open("./cœur_projet/graphique/ressources/boutton_radio_2.png"))
fifo_radio_2 = Label(fenetre, image = socket_udp_radio_img_2,borderwidth=0)
fifo_radio_2.place(x=610,y=302)
fifo_radio_2.bind('<Button-1>', handler_choix_fifo())


# Création de l'interface
creer_interface(fenetre,etiquette_nombre_client,champ_port_serveur,champ_nom_tube_reponses)

# Lancement de l'application
demarrer_application(fenetre=fenetre)


