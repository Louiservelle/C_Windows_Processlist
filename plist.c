#include <windows.h>
#include <tlhelp32.h> // Nécessaire pour utiliser CreateToolhelp32Snapshot et Process32*
#include <stdio.h>
// Remove later if possible
#include <string.h> // Utilise pour savoir si une substring est contenu dans une string

//strcomp pour les différents cas possible -h process -d
int main(int argc, char *argv[]) {

  //Comparer les arguments
	// S'il y a 1 seule argument : commande de l'executable
	if (argc == 1) {
		GetAllCurrentProcess();
	}
	// S'il y a + de 1 argument : commande de l'executable + options
	else{
		if (strcmp(argv[1], "-h") == 0) {
        Gethelp();
		} 
		else if (strcmp(argv[1], "-d") == 0) {
			GetThreadsFromPid(argv[2]);
		} 
		else if (argc == 2) {
		  char *process = argv[1];
		  printf("%s\n", process);
		  GetProcessDetails(process);
		}
		else {
			GetAllCurrentProcess();
			printf("Argument inconnu : %s\n", argv[1]);
		}
	}
    
    return 0;
}

// Fonction pour l'aide
int Gethelp(){
	printf("\n");
  printf("%s", "HELP : plist.exe\n");
  printf("----------------------------------------\n");
  printf("%-12s %-2s %s\n", "Parameter", "|", "Description");
  printf("----------------------------------------\n");
  printf("%-12s %-2s %s\n", "no-argument", ":", "list currents process");
  printf("%-12s %-2s %s\n", "-h", ":", "show this help");
  printf("%-12s %-2s %s\n", "-d <pid>", ":", "Show thread detail.");
  printf("%-12s %-2s %s\n", "<name>", ":", "Show information about processes that begin with the name specified.");
  printf("----------------------------------------\n");
  return 0;
}

//Fonction pour Get tous les process
int GetAllCurrentProcess(){

	printf("\n%s", "Currents process ::\n");

	// Creation du snapshot des processus
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnapshot == INVALID_HANDLE_VALUE) {
		printf("Impossible de créer le snapshot des processus.\n");
		return 1;
	}

	PROCESSENTRY32 pe32;
	pe32.dwSize = sizeof(PROCESSENTRY32);
	// taille (en octets) de la structure PROCESSENTRY32, necessaire pour Process32First et Process32Next

	// Reccuperation du premier processus
	if (!Process32First(hSnapshot, &pe32)) {
		printf("Impossible d'obtenir les processus.\n");
		CloseHandle(hSnapshot);
		return 1;
	}

	// Si on parvient a recuperer le premier processus
  // Affichage des en-tetes avec format tableau
  printf("-----------------------------------------\n");
  printf("%-28s %-10s %-10s\n", "Name", "Pid", "Thd");
  printf("------------------------------------------\n");
	
	// Lister tous les processus (on part du premier puis les suivants jusqu'a ce que Process32Next soit FALSE)
	do {
		printf("%-28s %-10u %-10u\n", pe32.szExeFile, pe32.th32ProcessID, pe32.cntThreads);
		// TO DO : Add Get GetProcessInfo(pid) to get more informations
		GetProcessInfos(pe32.th32ProcessID);
	} while (Process32Next(hSnapshot, &pe32));

	// Fermer le handle du snapshot
	CloseHandle(hSnapshot);

	return 0;
}

//Fonction pour Get un process particulier (depuis son nom) !! NEED refacto !! (same as GetAllCurrentProcess)
int GetProcessDetails(char *process){
	printf("\nProcess %s ::\n", process);

	// Creation du snapshot des processus
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnapshot == INVALID_HANDLE_VALUE) {
		printf("Impossible de créer le snapshot des processus.\n");
		return 1;
	}

	PROCESSENTRY32 pe32;
	pe32.dwSize = sizeof(PROCESSENTRY32);
	// taille (en octets) de la structure PROCESSENTRY32, necessaire pour Process32First et Process32Next

	// Reccuperation du premier processus
	if (!Process32First(hSnapshot, &pe32)) {
		printf("Impossible d'obtenir les processus.\n");
		CloseHandle(hSnapshot);
		return 1;
	}

	// Si on parvient a recuperer le premier processus
  // Affichage des en-tetes avec format tableau
  printf("-----------------------------------------\n");
  printf("%-28s %-10s %-10s\n", "Name", "Pid", "Thd");
  printf("------------------------------------------\n");
	
	// Lister tous les processus (on part du premier puis les suivants jusqu'a ce que Process32Next soit FALSE)
	do {
		// Si la string en argument est contenu dans le nom du process, on l'affiche
		if(strstr(pe32.szExeFile, process) != NULL){
			printf("%-28s %-10u %-10u\n", pe32.szExeFile, pe32.th32ProcessID, pe32.cntThreads);
		}
	} while (Process32Next(hSnapshot, &pe32));

	// Fermer le handle du snapshot
	CloseHandle(hSnapshot);

	return 0;
}

//Fonction pour Get un thread depuis un PID
int GetThreadsFromPid(DWORD processID){
	GetProcessInfos(processID);
  printf("%s", "Get threads from pid");
  return 0;
}


int GetProcessInfos(DWORD processID){
	HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION,FALSE ,processID);
	if(hProcess == NULL){
		// Certains process ne peuvent s'ouvrir. Problème de droits ?
		return 1;
	}
	else{
		// DEBUG
		printf("DEBUG : GetProcessInfos()\n");
		return 0;
	}

	return 0;
}