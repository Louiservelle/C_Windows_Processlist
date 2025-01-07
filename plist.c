#include <stdio.h>

//strcomp pour les différents cas possible -h process -d
int main(int argc, char *argv[]) {

    //Comparer les arguments
    if (strcmp(argv[1], "-h") == 0) {
        Gethelp();
    } else if (strcmp(argv[1], "-d") == 0) {
        GetThreadsFromPid();
    } else if (argc == 2) {
      char *process = argv[1];
      printf("%s\n", process);
      GetProcessDetails(process);
    }
    else{
        GetAllCurrentProcess();
        printf("Argument inconnu : %s\n", argv[1]);
    }


    return 0;
}

// Fonction pour l'aide
int Gethelp(){

  printf("%s", "get help");

}

//Fonction pour Get tous les process
int GetAllCurrentProcess(){

  printf("%s", "Get All process");

}

//Fonction pour Get un process particulier
int GetProcessDetails(process){


}

//Fonction pour Get un thread depuis un PID
int GetThreadsFromPid(){

  printf("%s", "Get threads from pid");

}
