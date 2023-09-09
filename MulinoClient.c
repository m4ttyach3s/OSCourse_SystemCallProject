/************************************
 * MUHAMED REFATI                   *
 * MATRICOLA: VR458240              *
 * DATA: 22/01/2022                 *
 * PROGETTO DI SYSTEM CALLS         *
 *                                  *
 * FILE MULINOCLIENT.C              *
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

//area per la dichiarazione delle funzioni
void stamparighe();
void fillit(char (*campo)[SIZECAMPO]);
void printthat (char (*campo)[SIZECAMPO]);
void chooseme(char (*campo)[SIZECAMPO], char fillargv[]);
void sostituiscimi(int pedinax, int pedinay, char (*campo)[SIZECAMPO], char fillargv[]);
void my_hanlder(int num);
void getmypid(int *mypid, int *passme);
void killher();

//area var globali
char fillnot = '+';
int v;
int shmid, ret, gio, signaling, clo;

//area struct
struct mypidfather{
    int numpid;
    int numclient[4];
}mypidfather;

//----------------area funzioni
void destroy_myshm(){
    shmctl(shmid, IPC_RMID, NULL);
    shmctl(gio, IPC_RMID, NULL);
    shmctl(signaling, IPC_RMID, NULL);
}

void destroy_mysem(){
    semctl(ret, 1, IPC_RMID, NULL);
    semctl(clo, 1, IPC_RMID, NULL);
}

void killher(){
    struct mypidfather pidtopass;
    kill(pidtopass.numpid, SIGKILL);
    destroy_mysem();
    destroy_myshm();
    kill(pidtopass.numclient[0], SIGKILL);
    kill(pidtopass.numclient[1], SIGKILL);
}

void getmypid(int *mypid, int *passme){
    struct mypidfather pidtopass;
    pidtopass.numpid = *mypid;
    pidtopass.numclient[0] = passme[1];
    pidtopass.numclient[1] = passme[2];
}

void my_handler(int num){
    struct mypidfather pidtopass;
    printf("\n\t\tHai premuto CTRL+C...\nIn attesa di chiudere i processi!\nLa partita e\' terminata.\n");
    int y = getpid();
    killher(); //chiudo il server e l'altro client
    kill(y, SIGKILL);
}

void stamparighe(){
    printf("\n\t\t----------------------------------\n");
}

void sostituiscimi(int pedinax, int pedinay, char (*campo)[SIZECAMPO], char fillargv[]){
    v = (pedinay*7)+pedinax;

    if ( (strcmp(campo[v], fillargv))!=0){
        sprintf(campo[v], "%s", fillargv);
    }
}

void chooseme(char (*campo)[SIZECAMPO], char fillargv[]){

    int pedinax, pedinay, comparethem, compareargv;
    char useless;
    //devo controllare se la casella sia occupata o meno
    printf("\n\t\t Si possono selezionare solo le caselle con simboli e lettere e senza la presenza di pedine.\n");
    printf("\t\tScegli la COLONNA e la RIGA disponibile in cui vuoi inserire la pedina: ");
    scanf("%i%c%i", &pedinax, &useless, &pedinay);
    v = (pedinay*7)+pedinax; //srotolo la matrice
    comparethem = strcmp(campo[v], &fillnot);
    compareargv = strcmp(campo[v], fillargv);

    if ((pedinax < 0 || pedinax>7) && (pedinay <0 || pedinay > 7) && ((pedinax >='a' && pedinax<='z') || (pedinax>='A' && pedinax <= 'Z') && (pedinay >='a' && pedinay<='z') || (pedinay>='A' && pedinay <= 'Z'))){
        printf("\n\t\tVALORE ERRATO.\n");
        do{
        printf("\t\tScegli la COLONNA e la RIGA disponibile in cui vuoi inserire la pedina: ");
        scanf("%i%c%i", &pedinax, &useless, &pedinay);
        } while((pedinax < 0 || pedinax>7) && (pedinay <0 || pedinay > 7) && ((pedinax >='a' && pedinax<='z') || (pedinax>='A' && pedinax <= 'Z') && (pedinay >='a' && pedinay<='z') || (pedinay>='A' && pedinay <= 'Z')));
    }else if((pedinax >= 0 || pedinax <=6) && (pedinay >=0 || pedinay <= 6) && (comparethem == 0 || compareargv == 0)){
        printf("\n\t\tCASELLA NON OCCUPABILE.\n");
        do{
        printf("\t\tScegli la COLONNA e la RIGA disponibile in cui vuoi inserire la pedina: ");
        scanf("%i%c%i", &pedinax, &useless, &pedinay);
        v = (pedinay*7)+pedinax;
        comparethem = strcmp(campo[v], &fillnot);
        }while(comparethem==0);
    }
    
    sostituiscimi(pedinax, pedinay, campo, fillargv);
}

void printthat (char (*campo)[SIZECAMPO]){
    printf("\t\t\t\tC  O  L  O  N  N  E\n");
    printf("\t\t    |  0  |  1  |  2  |  3  |  4  |  5  |  6  |\n");
    printf("\t\t  _   _________________________________________\n");
    printf("\t\t    |     |     |     |     |     |     |     |\n");
    printf("\t\t  0 |  %s  |     |     |  %s  |     |     |  %s  |\n", campo[0], campo[3], campo[6]);
    printf("\t\t  _ |_____|_____|_____|_____|_____|_____|_____|\n");
    printf("\t\t    |     |     |     |     |     |     |     |\n");
    printf("\t\t  1 |     |  %s  |     |  %s  |     |  %s  |     |\n", campo[8], campo[10], campo[12]);
    printf("\t\tR _ |_____|_____|_____|_____|_____|_____|_____|\n");
    printf("\t\t    |     |     |     |     |     |     |     |\n");
    printf("\t\tI 2 |     |     |  %s  |  %s  |  %s  |     |     |\n", campo[16], campo[17], campo[18]);
    printf("\t\t  _ |_____|_____|_____|_____|_____|_____|_____|\n");
    printf("\t\tG   |     |     |     |     |     |     |     |\n");
    printf("\t\t  3 |  %s  |  %s  |  %s  |     |  %s  |  %s  |  %s  |\n", campo[21], campo[22], campo[23], campo[25], campo[26], campo[27]);
    printf("\t\tH _ |_____|_____|_____|_____|_____|_____|_____|\n");
    printf("\t\t    |     |     |     |     |     |     |     |\n");
    printf("\t\tE 4 |     |     |  %s  |  %s  |  %s  |     |     |\n", campo[30], campo[31], campo[32]);
    printf("\t\t  _ |_____|_____|_____|_____|_____|_____|_____|\n");
    printf("\t\t    |     |     |     |     |     |     |     |\n");
    printf("\t\t  5 |     |  %s  |     |  %s  |     |  %s  |     |\n", campo[36], campo[38], campo[40]);
    printf("\t\t  _ |_____|_____|_____|_____|_____|_____|_____|\n");
    printf("\t\t    |     |     |     |     |     |     |     |\n");
    printf("\t\t  6 |  %s  |     |     |  %s  |     |     |  %s  |\n", campo[42], campo[45], campo[48]);
    printf("\t\t  _ |_____|_____|_____|_____|_____|_____|_____|\n");
}

void fillit(char (*campo)[SIZECAMPO]){
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
//-------------------------fine area funzioni


int main(int argc, char * argv[]){

    system("clear");
    
    key_t key = 100;
    key_t keysig = 10;
    key_t keyg = 104;
    char (*campo) [SIZECAMPO];

    char *ciao;
    ciao = (void *) signal(2, my_handler);

    printf("\n\t\t---------- PLAYGROUND -----------");

    if (argc != 1){
        printf("\n\n\t\tUTENTE %s LETTO CORRETTAMENTE\n", argv[1]);
        stamparighe();
    }
    else{
        printf("\n\n\tNessun valore inserito da terminale o valore errato.\n\n\tEseguire di nuovo il programma inserendo il tuo nome o un nome a tua scelta.\n\n\tEsempio: ./MulinoClient CHIARA.\n\n\tMi dispiace, purtroppo devo uscire\n\n\t\t\t\t...arrivederci\n");
        exit(1);
    }

    shmid = shmget(key, SIZEMEM*sizeof(char), IPC_EXCL | 0666);
    if (shmid < 0){
        perror("\n\t\tErorre! Ho fallito nella creazione della memoria: ");
        _exit(1);
    }

    campo = shmat(shmid, NULL, 0);
    if (campo == (void *) -1){
        perror("\n\t\tErrore nell'attacco della shared memory. SHMAT: ");
        _exit(1);
    }

    //ho attaccato il campo, ora lo riempio
    fillit(campo);

    int gio = shmget (keyg, SIZEGIOCO*sizeof(int), IPC_EXCL | 0666);
    if (gio < 0){
        perror("\n\t\t Errore nella creazione della shared memory. SHMGET: ");
        _exit(1);
    }
    
    int *giocate = shmat(gio, NULL, 0);
    if (giocate == (void *) -1){
        perror("\n\t\tErrore nell'attacco della shared memory. SHMAT: ");
        _exit(1);
    }

    char (* fillargv) [SIZEFILL] = shmat(shmid, NULL, 0); //passo qua i valori di argv[1] e argv[2] che poi passerò ai giocatori come pedine
    if (fillargv == (void *)-1) {
        perror("\n\t\tErrore nell'attacco della shared memory. SHMAT: ");
        _exit(1);
    }

    char (* fillname) [SIZEFF] = shmat(shmid, NULL, 0); //la uso per passare i nomi inseriti
    if (fillname == (void *)-1){
        perror("\n\t\tErrore nell'attacco della shared memory. SHMAT: ");
        _exit(1);
    }

    signaling = shmget (keysig, SIZESIG*sizeof(int), IPC_EXCL | 0666); //la uso per passare il valore del PID dei semafori
    int *passme = shmat(signaling, NULL, 0);

    if (passme == (void *) -1){
        perror("\n\t\tErrore nell'attacco della memoria condivisa. SHMAT: ");
        _exit(1);
    }

    int *mypid = shmat(shmid, NULL, 0);
    if (mypid == (void *)-1){
        perror("\n\t\tErrore nell'attacco della shared memory. SHMAT: ");
        _exit(1);
    }

    printf("\n\t\tPEDINE INSERITE DAL SERVER: %s e %s\n", fillargv[1], fillargv[2]);
    stamparighe();

    printf("\n\t\tIN ATTESA DI CONNESSIONE AL SERVER");
    stamparighe();

    int compareme;
    compareme = strcmp (fillname[1], fillname[2]);
    if (compareme == 0){ //se le pos sono uguali, quindi sono entrambe vuote
        int h;
        h = strlen(argv[1]);
        memcpy(fillname[1], argv[1], h*sizeof(char)); //copia nella pos1 dell'array il contenuto di argv[1] (primo nome)
    }
    else{                               //altrimenti copialo nella pos2 dell'array
        int h;
        h = strlen(argv[1]);
        memcpy(fillname[2], argv[1], h*sizeof(char));
    }

    ret = semget(KSEM, 1, IPC_EXCL | 0666);
    clo = semget (KSS, 1, IPC_EXCL | 0666);

    if (ret < 0){
        perror("\n\t\tErrore nella creazione del semaforo. SEMGET: ");
        _exit(1);
    }

    if (clo < 0){
        perror("\n\t\tErrore nella creazione del semaforo. SEMGET: ");
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

    semop(ret, &aum, 1); //qui ho il semaforo a +1 così che alzerà due volte il server. quando il server arriverà a 0, legge quello che ha sotto

    if (!passme[0]){
        passme[0] = 1;
        printf("\n\t\tCONNESSIONE AL SERVER AVVENUTA.");
        printf("\n\n\tPRIMO GIOCATORE %s ENTRATO. TI SPETTA LA PEDINA %s", fillname[1], fillargv[1]);
        printf("\n\n\t\tIN ATTESA DEL SECONDO GIOCATORE\n");
        passme[1] = getpid();
        stamparighe();
    } else{
        passme[0] = 0;
        printf("\n\t\tCONNESSIONE AL SERVER AVVENUTA.");
        printf("\n\n\tSECONDO GIOCATORE %s ENTRATO. TI SPETTA LA PEDINA %s", fillname[2], fillargv[2]);
        stamparighe();
        passme[2] = getpid();
    }

    getmypid(&mypid[1], passme);
    
    *giocate = 12; //ho 24 caselle, ogni giocatore può fare al massimo 12 mosse

    while(giocate){
        if(!passme[0]){
            semop(clo, &dim, 1); //gioca il primo giocatore
            printthat(campo);
            chooseme(campo, fillargv[1]);
            printthat(campo);
        }
        else{
            semop(clo, &dim, 1); //gioca il secondo giocatore
            printthat(campo);
            chooseme(campo, fillargv[2]);
            printthat(campo);
        }

        //attivo il server x controllare la vittoria
        semop(ret, &aum, 1);

        if (!passme[0]){
            semop(clo, &dim, 1);    //abbasso il primo così controllo se ha vinto
        } else{
            semop(clo, &dim, 1);     //abbasso il secondo così controllo se ha vinto
        }
        semop(ret, &aum, 1);        //alzo il server per continuare la partita o dichiarare il vincitore
    }
       
    return 0;
}