/************************************
 * MUHAMED REFATI                   *
 * MATRICOLA: VR458240              *
 * DATA: 22/01/2022                 *
 * PROGETTO DI SYSTEM CALLS         *
 *                                  *
 * FILE MULINOSERVER.C              *
 * **********************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <unistd.h>
#include <signal.h>
#include <sys/resource.h>
#include <sys/time.h>
#include <sys/ipc.h>
#include <sys/signal.h>
#include <sys/types.h>
#include <time.h>

#define SIZEMEM 2048
#define SIZECAMPO 49
#define SIZEFILL 30
#define SIZEFF 300
#define SIZEGIOCO 12
#define KSEM 101
#define KSS 102
#define SIZESIG 3

//area variabili globali
int i;
char fillnot='+';
int keep_running = 1;

//creo le variabili per la shared memory
int shmid, ret, gio, signaling, clo;
key_t key = 100;
key_t keysig = 10; //DEVO CAMBIARLO
key_t keyg = 104;

struct mypidfather{
    int numpid;
    int numclient[4];
}mypidfather;

//area dichiarazione funzioni
void stamparighe();
int checkvittoria(char (*campo)[SIZECAMPO], int giocate);
void reset_it(char (*campo)[SIZECAMPO]);
void my_handler(int num);
void destroy_mysem();
void destroy_myshm();
void getmypid(int *passme);
void my_happy_ending();
void killher();

//-------------------area funzioni
void my_happy_ending(){
    printf("\n\t\tEhi, uno dei client è uscito! La partita finisce in pareggio!\n");
    printf("\n\t\t...goodbye!\n");
    int y = getpid();
    kill(y, SIGKILL);
}

void destroy_myshm(){
    shmctl(shmid, IPC_RMID, NULL);
    shmctl(gio, IPC_RMID, NULL);
    shmctl(signaling, IPC_RMID, NULL);
}

void destroy_mysem(){
    semctl(ret, 1, IPC_RMID, NULL);
    semctl(clo, 1, IPC_RMID, NULL);
}

void getmypid(int *passme){
    struct mypidfather pidtopass;
    pidtopass.numclient[0] = passme[1];
    pidtopass.numclient[1] = passme[2];
}

void my_handler(int num){
    struct mypidfather pidtopass;
    if(i!=1){
    keep_running = 1;
    printf("\n\t\tDevi premere CTRL C due volte per poter uscire \n");
    i++;
    fflush(stdout);
    }else{
        fflush(stdout);
        keep_running = 0;
        int y = getpid(); 
        kill(pidtopass.numclient[0], SIGKILL);
        kill(pidtopass.numclient[1], SIGKILL);
        destroy_myshm();
        destroy_mysem();
        kill(y, SIGKILL);
    }
}

void stamparighe(){
    printf("\n\t\t----------------------------------\n");
}

void reset_it(char (*campo)[SIZECAMPO]){
    
    //riempio del char scelto le caselle del campo che mi interessano
    sprintf(campo[0], "%c", 'a');
    sprintf(campo[3], "%c", 'b');
    sprintf(campo[6], "%c", 'c');
    sprintf(campo[8], "%c", 'd');
    sprintf(campo[10], "%c", 'e');
    sprintf(campo[12], "%c", '#');
    sprintf(campo[16], "%c", 'f');
    sprintf(campo[17], "%c", 'g');
    sprintf(campo[18], "%c", 'h');
    sprintf(campo[21], "%c", 'i');
    sprintf(campo[22], "%c", 'j');
    sprintf(campo[23], "%c", 'k');
    sprintf(campo[25], "%c", 'l');
    sprintf(campo[26], "%c", 'm');
    sprintf(campo[27], "%c", 'n');
    sprintf(campo[30], "%c", 'y');
    sprintf(campo[31], "%c", 'p');
    sprintf(campo[32], "%c", 'q');
    sprintf(campo[36], "%c", 'r');
    sprintf(campo[38], "%c", 's');
    sprintf(campo[40], "%c", 't');
    sprintf(campo[42], "%c", 'u');
    sprintf(campo[45], "%c", 'v');
    sprintf(campo[48], "%c", 'z');

    //riempio le altre di un valore a caso per escluderle dalla scelta
    sprintf(campo[1], "%c", fillnot);
    sprintf(campo[2], "%c", fillnot);
    sprintf(campo[4], "%c", fillnot);
    sprintf(campo[5], "%c", fillnot);
    sprintf(campo[7], "%c", fillnot);
    sprintf(campo[9], "%c", fillnot);
    sprintf(campo[11], "%c", fillnot);
    sprintf(campo[13], "%c", fillnot);
    sprintf(campo[14], "%c", fillnot);
    sprintf(campo[15], "%c", fillnot);
    sprintf(campo[19], "%c", fillnot);
    sprintf(campo[20], "%c", fillnot);
    sprintf(campo[24], "%c", fillnot);
    sprintf(campo[28], "%c", fillnot);
    sprintf(campo[29], "%c", fillnot);
    sprintf(campo[33], "%c", fillnot);
    sprintf(campo[34], "%c", fillnot);
    sprintf(campo[35], "%c", fillnot);
    sprintf(campo[37], "%c", fillnot);
    sprintf(campo[39], "%c", fillnot);
    sprintf(campo[41], "%c", fillnot);
    sprintf(campo[43], "%c", fillnot);
    sprintf(campo[44], "%c", fillnot);
    sprintf(campo[46], "%c", fillnot);
    sprintf(campo[47], "%c", fillnot);
}

int checkvittoria(char (*campo)[SIZECAMPO], int giocate){
    int cc = 0;
    
    //confronto per righe
    int a = strcmp(campo[0], campo[3]);
    int b = strcmp(campo[3], campo[6]);
    int c = strcmp(campo[8], campo[10]);
    int d = strcmp(campo[10], campo[12]);
    int e = strcmp(campo[16], campo[17]);
    int f = strcmp(campo[17], campo[18]);
    int g = strcmp(campo[21], campo[22]);
    int h = strcmp(campo[22], campo[23]);
    int i = strcmp(campo[25], campo[26]);
    int j = strcmp(campo[26], campo[27]);
    int k = strcmp(campo[30], campo[31]);
    int l = strcmp(campo[31], campo[32]);
    int m = strcmp(campo[36], campo[38]);
    int n = strcmp(campo[38], campo[40]);
    int o = strcmp(campo[42], campo[45]);
    int p = strcmp(campo[45], campo[48]);

    //confronto per colonne
    int aa = strcmp(campo[0], campo[21]);
    int bb = strcmp(campo[21], campo[42]);
    int ccc = strcmp(campo[8], campo[22]);
    int dd = strcmp(campo[22], campo[36]);
    int ee = strcmp(campo[16], campo[23]);
    int ff = strcmp(campo[23], campo[30]);
    int gg = strcmp(campo[3], campo[10]);
    int hh = strcmp(campo[10], campo[17]);
    int ii = strcmp(campo[31], campo[38]);
    int jj = strcmp(campo[38], campo[45]);
    int kk = strcmp(campo[18], campo[25]);
    int ll = strcmp(campo[25], campo[32]);
    int mm = strcmp(campo[12], campo[26]);
    int nn = strcmp(campo[26], campo[40]);
    int oo = strcmp(campo[6], campo[27]);
    int pp = strcmp(campo[27], campo[48]);

    //confronto in diagonale
    int duno = strcmp(campo[0], campo[8]);
    int dduno = strcmp(campo[8], campo[16]);
    int ddue = strcmp(campo[6], campo[12]);
    int dddue = strcmp(campo[12], campo[18]);
    int dtre = strcmp(campo[30], campo[36]);
    int ddtre = strcmp(campo[36], campo[42]);
    int dquattro = strcmp(campo[32], campo[40]);
    int ddquattro = strcmp(campo[40], campo[48]);

    if ((a==0 && b == 0) || (c==0 && d==0) || (e==0 && f==0) || (i==0 && j==0) || (k==0 && l==0) || (m==0 && n==0) || (o == 0 && p==0)){
        cc = 1; //controllo se ho fatto tris sulle righe
        return cc;
    } else if ((aa==0 && bb == 0) || (ccc==0 && dd==0) || (ee==0 && ff==0) || (ii==0 && jj==0) || (kk==0 && ll==0) || (mm==0 && nn==0) || (oo == 0 && pp==0)){
        cc = 1; //controllo se ho fatto tris sulle colonne
        return cc;
    } else if ((duno == 0 && dduno == 0) || (ddue == 0 && dddue == 0) || (dtre == 0 && ddtre == 0) || (dquattro == 0 && ddquattro == 0)){
        cc = 1; //controllo se ho fatto tris in diagonale
        return cc;
    }

    if (giocate == 0 && cc == 0){
        cc = 2;
        return cc;
    }
    return cc;
}
//---------------------------fine area funzioni

int main(int argc, char *argv[]){

system("clear");
signal(SIGINT, my_handler);
signal(SIGKILL, my_happy_ending);

while (keep_running){
    printf("\n\t\t---------- M U L I N O -----------");
    
    int z;
    char (*campo)[SIZECAMPO];
    if (argc == 3) {
         z=strcmp(argv[1], argv[2]);
        if (z!=0){    //controllo che le pedine inserite siano 2, 3 se considero lo spazio in mezzo
            printf("\n\n\t\tPEDINE INSERITE: %s e %s", argv[1], argv[2]);
            stamparighe();
            printf("\n\t\tIN ATTESA DEI GIOCATORI...");
            stamparighe();
            }
        else{
            printf("\n\n\tValori identici inseriti da terminale.\n\n\tEseguire di nuovo il programma inserendo due pedine diverse a tua scelta.\n\n\tEsempio: ./MulinoServer X O.\n\n\tMi dispiace, purtroppo devo uscire\n\n\t\t\t\t...arrivederci\n");
            exit(1); // qua devo mettere anche le fx che eliminano la memoria condivisa e i semafori
        }
    } else {
        printf("\n\n\tNessun valore inserito da terminale.\n\n\tEseguire di nuovo il programma inserendo due pedine a tua scelta.\n\n\tEsempio: ./MulinoServer X O.\n\n\tMi dispiace, purtroppo devo uscire\n\n\t\t\t\t...arrivederci\n");
        exit(1); // qua devo mettere anche le fx che eliminano la memoria condivisa e i semafori
    }

    shmid = shmget (key, SIZEMEM*sizeof(char), IPC_CREAT | IPC_EXCL | 0666);
    if (shmid < 0){
        perror("\n\t\tErorre! Ho fallito nella creazione della memoria: ");
        _exit(1);
    }

    //attacco il campo alla memoria e inizializzo ogni cella a zero -> mi servirà per la strcmp dopo
    campo = shmat(shmid, NULL, 0);
    if (campo == (void *) -1){
        perror("\n\t\tErrore nell'attacco della shared memory. SHMAT: ");
        _exit(1);
    }

    gio = shmget (keyg, SIZEGIOCO*sizeof(int), IPC_CREAT | IPC_EXCL | 0666);
    if (gio < 0){
        perror("\n\t\t Errore nella creazione della shared memory. SHMGET: ");
        _exit(1);
    }
    
    int *giocate = shmat(gio, NULL, 0);
    if (giocate == (void *) -1){
        perror("\n\t\tErrore nell'attacco della shared memory. SHMAT: ");
        _exit(1);
    }

    int *mypid = shmat(shmid, NULL, 0);
    if (mypid == (void *)-1){
        perror("\n\t\tErrore nell'attacco della shared memory: SHMAT: ");
        _exit(1);
    }

    mypid[1] = getpid();

    char (* fillargv) [SIZEFILL] = shmat(shmid, NULL, 0); //passo qua i valori di argv[1] e argv[2] che poi passerò ai giocatori come pedine
    if (fillargv == (void *)-1) {
        perror("\n\t\tErrore nell'attacco della shared memory. SHMAT: ");
        _exit(1);
    }
 
    char (* fillname) [SIZEFF] = shmat(shmid, NULL, 0); //la uso per ricevere i nomi inseriti
    if (fillname == (void *)-1){
        perror("\n\t\tErrore nell'attacco della shared memory. riga 66 SHMAT: ");
        _exit(1);
    }

    signaling = shmget (keysig, SIZESIG*sizeof(int), IPC_CREAT | IPC_EXCL | 0666); //la uso per passare il valore dei semafori
    int *passme = shmat(signaling, NULL, 0);

    if (passme == (void *) -1){
        perror("\n\t\tErrore nell'attacco della memoria condivisa. riga 75 SHMAT: ");
        _exit(1);
    }

    //copio le pedine inserite da user input
    int h, h_h;
    h = strlen(argv[1]);
    h_h = strlen(argv[2]);
    
    memcpy(fillargv[1], argv[1], h*sizeof(char)); //passo la pedina 1
    memcpy(fillargv[2], argv[2], h_h*sizeof(char)); //passo la pedina 2

    //imposto il semaforo
    ret = semget (KSEM, 1, IPC_CREAT | IPC_EXCL | 0666);
    clo = semget (KSS, 1, IPC_CREAT | IPC_EXCL | 0666);

    if (ret < 0){
        perror("\n\t\tErrore nell'esecuzione del semaforo. SEMGET: ");
        _exit(1);
    }

    if (clo < 0){
        perror("\n\t\tErrore nell'esecuzione del semaforo. SEMGET: ");
        _exit(1);
    }

    struct sembuf aum;
    aum.sem_num = 0;
    aum.sem_op = 1;
    aum.sem_flg = 0;

    struct sembuf dim;
    dim.sem_num = 0;
    dim.sem_op = -1;
    dim.sem_flg = 0;

    dim.sem_op = -2;
    //metto il semaforo del server a -2 così i client devono connettersi due volte per alzarlo
    semop(ret, &dim, 1); //passa all'id del semaforo il valore 1 di op[0] cioè -2
    int *semv = shmat(signaling, NULL, 0);
    int checkv;
    printf("\n\t\tI due giocatori %s e %s sono entrati\n\t\tcon rispettivamente le pedine %s e %s", fillname[1], fillname[2], fillargv[1], fillargv[2]);
    printf("\n\t\tIn attesa che venga fatta una mossa...");
    stamparighe();

    if(!passme[0]){
        semop(clo, &aum, 1); //sblocco il primo processo
    }else{
        semop(clo, &aum, 1); //sblocco il secondo processo
    }
    getmypid(passme);
    dim.sem_op = -1;
    semop(ret, &dim, 1);

    if(!passme[0]){
        semop(clo, &aum,1);
    }else{
        semop(clo, &aum, 1);
    }
    while(giocate){
        int choiceof;
        checkv = checkvittoria(campo, *giocate);
        if (checkv == 1){
            printf("\n\t\tCOMPLIMENTI, HAI VINTO LA PARTITA.\n");
            printf("\n\t\tVuoi iniziare una nuova partita?\n\t\t1. SI\n\t\t2. NO");
            scanf("%i", &choiceof);
            if (choiceof == 1){
                *giocate = 12;
                reset_it(campo);
            } else{
                destroy_mysem();
                destroy_myshm();
                exit(1);
            }
        } else if (checkv == 0) {
        giocate--;
        semop(ret, &dim, 1);
        semop(clo, &aum, 1);
        }else if (checkv == 2 && *giocate == 0){
            printf("\n\t\tNESSUNO DEI GIOCATORI HA VINTO. PAREGGIO.\n");
            printf("\n\t\tVuoi iniziare una nuova partita?\n\t\t1. SI\n\t\t2. NO, voglio uscire\n");
            printf("\n\t\tSCELGA 1 O 2: ");
            scanf("%i", &choiceof);
            if (choiceof == 1){
                *giocate = 12;
                reset_it(campo); //RIPORTO IL CAMPO CON I VALORI INIZIALI
                
                semop(ret, &aum, SEM_UNDO); //riporto sem a valori di partenza
                semop(clo, &dim, SEM_UNDO); //riporto sem a valori di partenza
            } else{
                destroy_mysem();
                destroy_myshm();
                exit(1);
            }
        }
    }
    
    printf("\n\n\n");
}
    
    //in caso di exit improvvise
    
    destroy_mysem();
    destroy_myshm();

    return 0;
}