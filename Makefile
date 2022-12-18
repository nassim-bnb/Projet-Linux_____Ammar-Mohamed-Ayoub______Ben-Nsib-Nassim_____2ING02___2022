chemin_communication = ./cœur_projet/communication_inter-processus
chemin_tcp = ${chemin_communication}/socket/protocole-TCP
chemin_udp = ${chemin_communication}/socket/protocole-UDP
chemin_fifo = ${chemin_communication}/tube_nomme
chemin_construction = ./build

drapeau_debut : supprimer_trace_construction compiler_fichiers
	@echo "========================================================================================================="
	@echo "Projet est prêt"
	@echo "========================================================================================================="
	@echo ""
	

supprimer_trace_construction : 
	@echo "---------------------------------------------------------------------------------------------------------"
	@echo "[DEBUT] : Suppression de la trace de l'ancienne construction"
	@echo "---------------------------------------------------------------------------------------------------------"
	@echo ""
	@mkdir -p build/temp
	rmdir -p build/temp
	@echo ""
	@echo "---------------------------------------------------------------------------------------------------------"
	@echo "[FIN] : Suppression de la trace de l'ancienne construction"
	@echo "---------------------------------------------------------------------------------------------------------"
	@echo "\n"


compiler_fichiers : 
	@echo "---------------------------------------------------------------------------------------------------------"
	@echo "[DENUT] : Compilation de fichiers"
	@echo "---------------------------------------------------------------------------------------------------------"
	@echo ""
	@mkdir -p build/temp
	gcc -o ${chemin_construction}/client-TCP ${chemin_tcp}/client-TCP.c
	gcc -o ${chemin_construction}/client-UDP ${chemin_udp}/client-UDP.c
	gcc -o ${chemin_construction}/client-FIFO ${chemin_fifo}/client-FIFO.c
	gcc -o ${chemin_construction}/serveur-TCP ${chemin_tcp}/serveur-TCP.c
	gcc -o ${chemin_construction}/serveur-UDP ${chemin_udp}/serveur-UDP.c
	gcc -o ${chemin_construction}/serveur-FIFO ${chemin_fifo}/serveur-FIFO.c
	@echo ""
	@echo "---------------------------------------------------------------------------------------------------------"
	@echo "[FIN] : Compilation de fichiers"
	@echo "---------------------------------------------------------------------------------------------------------"
	@echo "\n"
