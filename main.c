#include <stdio.h>
#include <stdlib.h>
#define MAX_FB 100
#define DISK_SIZE 100
#include <string.h>

struct tenr{
  int id;
  char nom[21];
  int supp; // si supprime =1 , sinon =0
};

struct tbloc{
 struct tenr B[MAX_FB];
 int NE;
 int occup; // si occupe=> 1  si non 0
};

struct tblocChaine {
 struct tenr B [MAX_FB];
 int NE;
 int occup;
 struct tblocChaine *next;
};

struct tMetaD{
  char nomfichier[51];
  int tailleblocs;
  int taillenreg;
  int adrprebloc;
  char modeorgaglobale[51];
  char modeorgainterne[51];
};

struct MS {
    int nb;
    int nblibre;
    struct tbloc m[];
};
struct MSC{
    int nb;
    int nblibre;
    struct tblocChaine *tete;
};
typedef struct {
    struct tbloc block[DISK_SIZE]; // Array of blocks
    int freeBlocks;          // Number of free blocks
} Disk;


// fonction pour cration de blocs en mode contigue
void creatBlocContigue( struct tbloc *BLOC , int numenr){
   BLOC->occup = 1 ;
   BLOC->NE = 0 ;
  for (int i = 0 ; i < numenr ; i++){
    printf("give the id and the name of product number: %d\n" , i+1 );
    scanf(" give the id:");
    scanf("%d\n" , &BLOC->B[i].id);
    scanf("give the name:");
    scanf("%s\n", &BLOC->B[i].nom);
    BLOC->B[i].supp = 1 ; // existe
    BLOC->NE++;
  }
}
// fonction pour creation de blocs en mode chainee
struct tblocChaine *creatBlocChaine (int numenr){
   struct  tblocChaine *newBloc = (struct tblocChaine *)malloc(sizeof(struct tblocChaine));
     //if (*newBloc == NULL){}
     newBloc->occup = 1 ;
     newBloc->NE = 0;
     for (int i = 0 ; i < numenr ; i++ ){
        printf("give the id and the name of product number: %d\n" , i+1 );
        scanf(" give the id:");
        scanf("%d\n" , &newBloc->B[i].id);
        scanf("give the name:");
        scanf("%s\n", &newBloc->B[i].nom);
        newBloc->B[i].supp = 0 ; // existe
        newBloc->NE++;
     }
     newBloc->next = NULL;
     return newBloc ;

};

// fonction de triage des produits en mode contigue
void trierContigue(struct tbloc bloc ){
    struct tenr temp;

   for (int i = 0 ; i < bloc.NE - 1 ; i++){
    int indexMin = i ;
    for ( int j = i+1 ; j < bloc.NE ; j++){
        if (bloc.B[j].id < bloc.B[indexMin].id){
            indexMin = j ;
        }
    }
    if (indexMin != i){
        temp = bloc.B[i];
        bloc.B[i] = bloc.B[indexMin];
        bloc.B[indexMin] = temp ;
    }
   }
}

// fontcion de triage des produits en mode chainee
void trierChainee (struct tblocChaine bloc ){
struct tenr temp;

   for (int i = 0 ; i < bloc.NE - 1 ; i++){
    int indexMin = i ;
    for ( int j = i+1 ; j < bloc.NE ; j++){
        if (bloc.B[j].id < bloc.B[indexMin].id){
            indexMin = j ;
        }
    }
    if (indexMin != i){
        temp = bloc.B[i];
        bloc.B[i] = bloc.B[indexMin];
        bloc.B[indexMin] = temp ;
    }
   }

}

// creation de fichier metadonees
void creatMTfile (struct tMetaD *MT){
   char metaFile [56];
   snprintf(metaFile , sizeof(metaFile), "%s.meta" , MT->nomfichier);
   FILE *MTfile = fopen(metaFile, "w");
   fwrite(MT , sizeof(struct tMetaD), 1, metaFile);
   fclose(MTfile);
}

// fonction pour creation des fichiers
void CreeFichier (char nomFichier [51] , int nbrEnreg , int choixGlobale , int choixIntern){ //contigue = 0 , chainee = 1 , trier = 0 , non trier = 1
  //File *file ;
  int nbrBloc ;
  struct tbloc BlocContigue ;
  struct tblocChaine *headBlocChainee = NULL ;
  struct tblocChaine *currentBlocChainee = NULL;
  struct tMetaD MT;

  strncpy(MT.nomfichier, nomFichier, sizeof(MT.nomfichier));

   // create the file
   FILE *file = fopen(nomFichier , "w");
   nbrBloc = (nbrEnreg + MAX_FB -1) / MAX_FB;
   int nbrEdernierBloc = nbrEnreg % MAX_FB;

   MT.taillenreg = nbrEnreg;
   MT.tailleblocs = nbrBloc ;

    if ( choixGlobale == 0 ){
    // le mode globale = contigue
    strncpy(MT.modeorgaglobale , "contigue" , sizeof(MT.modeorgaglobale));
        for (int i = 0 ; i < nbrBloc ; i++ ){
            printf("creation of bloc %d (contigue)\n" , i+1);
            if (i != nbrBloc -1){
                creatBlocContigue(&BlocContigue, MAX_FB);
            }else {
            creatBlocContigue(&BlocContigue, nbrEdernierBloc);
            }
            fwrite(&BlocContigue , sizeof(struct tbloc),1 , file);
            if (i == 0) {
                MT.adrprebloc = (long)&BlocContigue;
            }
            if (choixIntern == 0){
                // choix interne : tier
                strncpy(MT.modeorgainterne , "trier" , sizeof(MT.modeorgainterne));
                trierContigue(BlocContigue);
            } else {
              strncpy(MT.modeorgainterne , "non trier" , sizeof(MT.modeorgainterne));
            }
        }
    } else if (choixGlobale == 1){
      // le mode globale = chainee
      strncpy(MT.modeorgaglobale , "chainee" , sizeof(MT.modeorgaglobale));
      struct tblocChaine *newBloc = NULL;
      for (int i = 0 ; i < nbrBloc ; i++){
      printf("cration of bloc %d (chainee)\n" , i+1);
      // mode FIFO
      if (i != nbrBloc-1){
        struct tblocChaine *newBloc = creatBlocChaine(MAX_FB);
      } else {
         struct tblocChaine *newBloc = creatBlocChaine(nbrEdernierBloc);
      }
       if (i == 0){
           MT.adrprebloc = (long)newBloc;
       }
        if (choixIntern == 0 ){
            // choix interne = trier
            strncpy(MT.modeorgainterne , "trier" , sizeof(MT.modeorgainterne));
            trierChainee(*newBloc);
        } else {
          strncpy(MT.modeorgainterne , "non trier" , sizeof(MT.modeorgainterne));
        }
       if (headBlocChainee == NULL){
        headBlocChainee = newBloc ;
        currentBlocChainee = newBloc;
       } else {
         currentBlocChainee->next = newBloc;
         currentBlocChainee = newBloc;
       }
       //currentBlocChainee =  newBloc;
       fwrite(newBloc , sizeof( struct tblocChaine), 1 ,file);
    }
    }
    fclose(file);
   creatMTfile(&MT);
}

// fonction lireMT
 void lireMT (FILE *f , int nc , void* result ){
   rewind(f);
   struct tMetaD MT;
   fread(&MT , sizeof( struct tMetaD) , 1 , f);
   switch(nc){
     case 1 : //fileName
      strcpy((char*)result , MT.nomfichier);
      break ;
     case 2 : // tailleBloc
        *(int*)result = MT.tailleblocs;
        break;
     case 3 : // tailleEnre
        *(int*)result = MT.taillenreg;
        break;
     case 4 : // adressePrBloc
        *(int*)result = MT.adrprebloc;
        break;
     case 5 : // modeGlobal
        strcpy((char*)result , MT.modeorgaglobale);
        break;
     case 6 : // modeIntern
        strcpy((char*)result , MT.modeorgainterne);
        break;
   }
 }

//function to rename a file
void renommerfichier(char *nomfichier, char *nvnom) {
    if (rename(nomfichier, nvnom) == 0) {
        printf("File renomme\n");
    } else {
        perror("Error \n");
    }
}

// recherche d'un enregistrement dans un fichier
void  recherchenregistement(FILE *ms, FILE *f, int id,int adrs[2]){
   char orgaglobale[51];
    char orgainterne[51];
    int numbloc=1;
    int adr ;
    lireMT(f,4,adr);//adresse du premier bloc
    int taille;//numero des blocs dans le fichiers
    lireMT(f, 2,taille);
    lireMT(f, 5 ,orgaglobale);//on verifie s'il s'agit d'un fichier contigu ou non
    lireMT(f,6,orgainterne);
              if(strcmp(orgaglobale, "chainee")==0){
                    struct tblocChaine buffer;
                    fread(&buffer, sizeof(buffer), 1, ms);
                    for(int i=0;i < adr;i++){
                   // buffer = buffer.next;
                           }
                         //verification s'il est ordonne
                      if(strcmp(orgainterne, "trier")==0){// recherche dichotomique
                           for(int i=0;i<taille;i++){
                             fread(&buffer, sizeof(buffer), 1, ms);
                            int fin = buffer.NE -1;
                            int debut=0;
                              while (debut<=fin) {
                                int milieu = (fin+debut)/2;
                                int n= buffer.NE-1;
                                if (buffer.B[milieu].id==id) {
                                   adrs[0]= numbloc;
                                   adrs[1]= milieu +1;
                                 printf("Bloc:%d, Enregistement:%d", adrs[0], adrs[1]);
                                  }
                              else if (buffer.B[milieu].id < id) {
                              debut = milieu + 1; // On cherche dans la moitié droite
                                   }
                                 else if(buffer.B[milieu].id> id){
                                  fin = milieu-1;
                                   }
                            }
                          numbloc++;
                        buffer=buffer.next;
                     }
  }
    //recherche sequentielle
           else if(strcmp(orgainterne, "non trier")==0){
                for (int i=0;i<taille;i++){
                fread(&buffer, sizeof(buffer), 1, ms);
                int j=0;
               while(j<buffer.NE){
                   if(buffer.B[j].id == id){
                   adrs[0]= numbloc;
                   adrs[1]= j +1;
                  printf("Bloc:%d, Enregistement:%d", adrs[0], adrs[1];);
                     }
                  }
           numbloc++;
         buffer= buffer->next;
           }
   }
   }
         else if (strcmp(orgaglobale, "contigue")==0){
              struct tbloc buffer;
             fseek(ms, adr*sizeof(buffer), SEEK_SET);
                //verification s'il est ordonne
                  // recherche dichotomique
                 if(strcmp(orgainterne, "trier")==0){
                   for(int i=0;i<taille;i++){
                  fread(&buffer, sizeof(buffer), 1, ms);
                  int fin = buffer.NE -1;
                 int debut=0;
                while (debut<=fin) {
               int milieu = (fin+debut)/2;
                int n= buffer.NE-1;
                if (buffer.B[milieu].id==id) {
                          adrs[0]= numbloc;
                           adrs[1]= milieu +1;
                           printf("Bloc:%d, Enregistement:%d", adrs[0], adrs[1];);
                  }
                  else if (buffer.B[milieu].id < id) {
                  debut = milieu + 1; // On cherche dans la moitié droite
                 }
                    else(buffer.B[milieu].id>id) {
                    fin = milieu-1;
                  }
                    }
        numbloc++;
                     }
  }
    //recherche sequentielle
          else if(strcmp(orgainterne, "non trier")==0){
               for (int i=0;i<taille;i++){
                fread(&buffer, sizeof(buffer), 1, ms);
                int j=0;
               while(j<buffer.NE){
                   if(buffer.B[j].id == id){
                   adrs[0]= numbloc;
                   adrs[1]= j +1;
                  printf("Bloc:%d, Enregistement:%d", adrs[0], adrs[1];);
            }
        }
        numbloc++;
    }
  }

}
}
//suppression logique
void supprlogique(FILE *ms, FILE *f, int id){
    int adrs[2];
     int a ;
   char orgaglobale[51];
   int n= adrs[0];
   int m=adrs[1];
   lireMT(f,4,a);
   lireMT(f,5,orgaglobale);
    recherchenregistrement(ms, f,id,adrs); // recupiration de l'adresse d'enregistrement qu'on veut supprimer
                if(strcmp(orgaglobale, "contigue")==0){// verifie s'il est contigue
                struct tbloc buffer;
            fseek(ms, a*sizeof(buffer), SEEK_SET);
             fseek(ms, n*sizeof(buffer), SEEK_CUR);
             fread(&buffer, sizeof(buffer), 1, ms);
             buffer.b[m-1].supp =1;// supp =1 , il est supprime
                }
         else if(strcmp(orgaglobale,"chainee")==0){//fichier des blocs chainee
            struct tblocChaine buffer;
             fread(&buffer, sizeof(buffer), 1, ms);
            for(int i=0;i=<adr;i++){
              buffer=buffer->next;//parcourir tous les blocs comme une liste chainee jusqu'on arrive a l'adresse
             }
             for(int i=0;i<adrs[0];i++){
                buffer = buffer.next;
             }
             fread(&buffer, sizeof(buffer), 1, ms);
             buffer.B[m-1].supp = 1;
         }
}

// suppression physique
void supprphysique(FILE *ms, FILE *f, int id){
    int add [2];
    recherchenregistement(ms, f, id,add[2]);//recupiration de l'adresse
    char orgaglobale[51];
    lireMT(f,5,orgaglobale);
    int taille;
    lireMT(f,2,taille);
    int adr;
    lireMT(f,4,adr);
          if(strcmp(orgaglobale, "contigue")==0){// verifiant de mode d'organisation globale
                struct tbloc buffer;
              fseek(ms, adr*sizeof(buffer),SEEK_SET);
              fseek(ms, add[0]*sizeof(buffer), SEEK_CUR);//passer directement a l'dresse du bloc
              fread(&buffer, sizeof(buffer), 1, ms);
              for(int i=add[1];i<buffer.NE;i++){
                 buffer.B[i]=buffer.B[i+1];//decalage des enregistrements
              }
        }
        else if(strcmp(orgaglobale, "chainee")==0){
            struct tblocChaine buffer;
             fread(&buffer, sizeof(buffer), 1, ms);
                for(int i=0;i=<adr;i++){
                buffer=buffer->next;// passer par les liens de chainage
             }
             for(int i=0;i<adr[0];i++){
                buffer=buffer->next;
             }
               fread(&buffer, sizeof(buffer), 1, ms);
             for(int i=adr[1];i<buffer.NE;i++){
                buffer.B[i]=buffer.B[i+1];//decalage
             }
        }
}

//insertion d'un enregistrement
void insertion(FILE *ms, FILE *f){
    rewind(ms);
    struct tenr newenr;
    int a, taille;
    lireCaracteristique(f,4,a);//recupiration d'adresse du premier bloc
    lireMT(f,2,taille);// recupiration de la taille du fichier en blocs
    printf("Entrez le ID du nouveau enregistrement :\n");
    printf("ID : ");
    scanf("%d", &newenr.id);
    printf("Nom : ");
    scanf(" %[^\n]", newenr.nom);
    char orgaglobale[51];
    char orgainterne [51];
    lireMT(f,6,orgainterne);
     lireMt(f,5,orgaglobale);
        if(strcmp(orgaglobale, "contigue")==0){//mode d'organisation globale
                struct tbloc buffer;
            if(strcmp(orgainterne, "non trier")==0){
              fread(&buffer, sizeof(buffer), 1, ms);
               int adrdernier = (a+taille)-1;
              fseek(ms, adrdernier*sizeof(buffer), SEEK_SET);//passer directement au dernier bloc
                    if(buffer.NE < MAX_FB){//en voir si le bloc est charge completement
                  buffer.B[buffer.NE].id = newenr.id;// si non on ajoute a la fin
                 strcpy(buffer.B[buffer.NE].nom, newenr.nom);
                 buffer.B[buffer.NE].supp=0;
                  buffer.NE++;
                  fseek(ms, -sizeof(buffer), SEEK_CUR);
                  write(&buffer, sizeof(buffer), 1, ms);
                 MAJMeta(f, 2, n+1);
                          }
                   else{// si oui on alloue un nouveau bloc
                     struct tbloc buffer2;
                     creatBlocContigue(buffer2, 1);
                       buffer2.B[0].id = newenr.id;// si non on ajoute a la fin
                     strcpy(buffer2.B[0].nom, newenr.nom);
                     buffer2.B[0].supp=0;
                   fseek(ms, (aDernierBloc+1)*sizeof(buffer), SEEK_SET);
                    fwrite(&buffer, sizeof(buffer), 1, ms);
                  MAJMeta(f, 2, n+1);
                  MAJMeta(f, 3, taille+1);//mise a jour de la taille du fichier
                  }
                 }
        else {   int numbloc=1;// cas du fichier trier
                for(int i=0;i<taille;i++){
                    fread(&buffer, sizeof(buffer), 1, ms);
                    int debut = 0;
                    int fin = buffer.NE;
                   while (debut <= fin) {// on fait la recherche dichotomique
                   int milieu = (debut + fin) / 2;
                   if (buffer.B[milieu] < ) {
                  debut  = milieu + 1;
                   } else {
                 fin = milieu - 1;// l'adresse d'insertion se trouve au debut
                 }
              }
              numbloc++;
                }
                rewind(ms);// on retourne au debut
                fseek(ms , (numbloc+a)*sizeof(buffer),SEEK_SET);
                fread( &buffer, sizeof(buffer), 1, ms);
                if(buffer.NE<MAX_FB){// on verifie si le bloc est plein
                    for(int i=buffer.NE;i>debut;i--){//decalage des enre apres l'adresse d'insertion
                        buffer.B[i]=buffer.B[i-1];
                    }
                    buffer.B[debut].id = newenr.id;
                  strcpy(buffer.B[debut].nom, newenr.nom);
                  buffer.B[debut].supp=0;
                  buffer.NE++;
                  fseek(ms, -sizeof(buffer), SEEK_CUR);
                  write(&buffer, sizeof(buffer), 1, ms);
                  MAJMeta(f, 2, n+1);
                     }
                     else{// s'il est plein on alloue un nouveau bloc
                            struct tbloc buffer2;
                    creatBlocContigue(buffer2, 1);
                    buffer2.B[0]=buffer.B[buffer.NE];// on met le dernier element du bloc plein ds le nv bloc
                    for(int i=buffer.NE;i>debut;i--){
                        buffer.B[i]=buffer.B[i-1];
                    }
                    buffer.B[debut].id = newenr.id;
                    strcpy(buffer.B[debut].nom, newenr.nom);
                    buffer.B[debut].supp=0;
                  fseek(ms, -sizeof(buffer), SEEK_CUR);
                  write(&buffer, sizeof(buffer), 1, ms);
                  MAJMeta(f, 2, n+1);//mise a jour du taille du fichier
                     }

              }
        }
          else if(strcmp(orgaglobale, "chainee")==0){
            struct tblocChaine buffer, buffer2;
               if(strcmp(orgainterne, "non trier")==0){// on verifie s'il est non trier
                     fread(&buffer, sizeof(buffer),1,ms);
                     for(int i =0;i<a;i++){
                        buffer = buffer.next;
                     }
                     for(int i =0;i<taille;i++){
                        buffer = buffer.next;
                     }
                     fread(&buffer, sizeof(buffer),1,ms);// on insere a la fin
                     if(buffer.NE<MAX_FB){// verifiant si le dernier bloc est plein
                           buffer.B[buffer.NE].id = newenr.id;
                          strcpy(buffer.B[buffer.NE].nom, newenr.nom);
                          buffer.B[buffer.NE].supp=0;
                          buffer.NE++;
                          write(&buffer, sizeof(buffer), 1, ms);
                          MAJMeta(f, 2, n+1);
                     }
                     else{
                      struct tblocChaine  nbloc = creatBlocChaine(1);// si le dernier bloc est plein on alloue un nv bloc
                      nbloc.NE=1;
                      nbloc.B[0].id = newenr.id;
                      strcpy(buffer.B[buffer.NE].nom, newenr.nom);
                      nbloc.occup=1;
                     buffer.next=nbolc;
                    taille ++;
                    MAJMeta(f, 2, taille);//mise a jour de la taille du fichier
                     }

            }
            else if (strcmp(orgainterne, "trier")==0){// cas du fichier des blocs chainees tries
                 for(int i=0;i<taille;i++){//recherche dichotomique
                           int numbloc=1;//pour compter le num de bloc ou on va inserer
                             fread(&buffer, sizeof(buffer), 1, ms);
                            int fin = buffer.NE -1;
                             int debut=0;
                              while (debut<=fin) {
                                int milieu = (fin+debut)/2;
                                int n= buffer.NE-1;
                                if (buffer.B[milieu].id < id) {
                              debut = milieu + 1; // l'adresse d'insertion se trouve au "debut"
                                   }
                                 else if(buffer.B[milieu].id> id){
                                  fin = milieu-1;
                                   }
                            }
                             numbloc++;
                             buffer=buffer.next;
                        }
                        rewind(ms);// on retourne au debut
                  for(int i=0;i<a;i++){
                    buffer = buffer.next;
                  }
                   for(int i=0;i<numbloc;i++){
                    buffer = buffer.next;//parcourir jusqu'au bloc qui contient l'adresse d'insertion
                  }
                  if(buffer.NE<MAX_FB){// si le bloc n'est pas plein
                  for(int i=buffer.NE;i>debut;i--){
                    buffer.B[i]=buffer.B[i-1];// on decale
                  }
                  buffer.B[debut].id = newenr.id;
                  strcpy(buffer.B[debut].nom, newenr.nom);// et on insere
                  buffer.B[debut].supp=0;
                  buffer.NE++;
                }
                else{
                     struct tblocChaine  nbloc = creatBlocChaine(1);// s'il est plein pn alloue un nv bloc
                     nbloc.B[0].id = buffer[buffer.NE].id;// on met le dernier element du bloc ds le nv bloc
                  strcpy(nbloc.B[0].nom, buffer.B[buffer.NE].nom);
                     nbloc.B[0].supp=0;
                     buffer.next=nbloc//met a jour le chainage
                    for(int i=buffer.NE;i>debut;i--){
                    buffer.B[i]=buffer.B[i-1];// puis on decale
                  }
                  buffer.B[debut].id = newenr.id;
                  strcpy(buffer.B[debut].nom, newenr.nom);// et on insere
                  buffer.B[debut].supp=0;
                }
                }
          }
}

// fonction pour lire tous les enregistrements
void lirenregistrements(FILE *ms, FILE *f){
     char orgaglobale[51];
      int taille;
      int a;
     lireMT(f,2,taille);
     lireMT(f,4,a);
     lireMT(f,5,orgaglobale);
      if(strcmp(orgaglobale, "contigue")==0){
         struct tbloc buffer;
         fseek(ms, a*sizeof(buffer), SEEK_SET);
          for(int i=0; i<taille; i++){
          fread(&buffer, sizeof(buffer), 1, ms);
          int j = 0;
         while(j<buffer.NE)
        {
            printf("ID: %d\nNom: %s\n", buffer.B[j].id, buffer.B[j].nom);
            j++;
        }
        printf("Nombre d'enregistrements: %d\n", buffer.NE);
      }
      }
      else if (strcmp(orgaglobale, "chainee")==0){
              struct tblocChaine buffer;
             fread(&buffer, sizeof(buffer), 1, ms);
                for(int i=0;i=<adr;i++){
                buffer=buffer->next;
             }
             for(int i=0;i<taille;i++){
                fread(&buffer, sizeof(buffer), 1, ms);
                int j=0;
                while(j<buffer.NE){
               printf("ID: %d\nNom: %s\n", buffer.B[j].id, buffer.B[j].nom);
                j++;
                }
                printf("Nombre d'enregistrements: %d\n", buffer.NE);
                buffer = buffer->next;
      }
      }

  //fonction de compactage(proposition authomatique en cas d'espace insuffisant
void compactageMS(FILE *ms){
      int nbrblocs=lireMS;
      struct ms bufferms;
       fread(&bufferms, sizeof(bufferms),1,ms);
      for(int i=0;i<nbrblocs;i++){
        if(bufferms.m[i].occup=0){
            bufferms.m[i]=bufferms.m[i+1];
        }
      }
      }

// fonction de chargement du fichier en MS
void chargementfichier(FILE *ms, FILE *f, FILE *MT){
    rewind(ms);//placer a la tete
    char orgaglobale;
    int nbrblocs;
    lireMT(f,2,nbrblocs);
    lireMT(f,5,orgaglobale);
    if(strcmp(orgaglobale, "contigue")==0){
        struct MS bufferms;
        struct tbloc buffer;
       fread(&bufferms, sizeof(bufferms), 1,ms);
        if(bufferms.nblibre>nbrblocs){
            int adrs=0;// si on a suffisamment d'espace
            for(int j=0;j<bufferms.nb;j++){
            if(bufferms.m[j].occup==1){// parcourir la MS jusqu'a trouver le premier bloc libre
            adrs++;
                }
            }
            int bcons=0;
            for(int j=adrs;j<(bufferms.nb-adrs);j++){
                if(bufferms.m[j].occup==0){//compter le num des bloc consecutif libre
                bcons++;
            }
            }
            if(bcons>nbrblocs){//si le num des blocs consecutifs vide est < a l'espace du fichier
            for(int i=0;i<nbrblocs;i++){
             fread(&buffer, sizeof(buffer),1,ms);
                bufferms.m[adrs+i]=buffer;
               }
            }
            else{
                printf("faire le compactage");
                compactageMS(ms);// si on a pas des blocs consecutifs libres suffisants on fait le compactage
                for(int j=0;j<bufferms.nb;j++){
              if(bufferms.m[j].occup==1){// mm chose parcourir pour trouver la nouvelle adresse du pre bloc libre
              adrs++;
                }
            }
                for(int i=0;i<nbrblocs;i++){
             fread(&buffer, sizeof(buffer),1,ms);
                bufferms.m[adrs+i]=buffer;
               }// et on fait le chatgement
            }
        }
            else{
                printf("on doit faire le compactage");
                compactageMS(ms);// s'il n'ya pas suffisamment des blocs libres on fait juste le compactage
                // main on charge rien
            }
        }
        else if (strcmp(orgaglobale, "chainee")==0){
        struct MSC bufferms;
        struct tbloc buffer;
       fread(&bufferms, sizeof(bufferms), 1,ms);
       if(bufferms.nblibre>nbrblocs){// si ona suffisamment d'espace
             fread(&buffer, sizeof(buffer),1,ms);
             for(int i=0;i<bufferms.nblibre;i++){
             if(bufferms.tete.occup==1){// parcourir les pointeurs jusqu'on arrive a un bloc libre
            bufferms.tete=bufferms.tete.next;
                }
                bufferms.tete=buffer;// on met le premier bloc du fichier dans ce pointeur
                // et it va pointer vert ;es autres blocs du fichier
             }
        }
        else{
            printf("espace insuffisant");// le compactage sert a rien ici
        }
       }

        }

  // Function to initialize the disk (MS structure)
void initializeDisk(MS *disk, int nbBlocks) {
    disk->nb = nbBlocks;
    disk->nblibre = nbBlocks; // all blocks are free fr now

    for (int i = 0; i < nbBlocks; i++) {
        disk->m[i].occup = 0;  // mark blocks as free (pas occupied)
        disk->m[i].NE = 0;     // no entries in the block yet
        for (int j = 0; j < MAX_FB; j++) {
            disk->m[i].B[j].id = -1;  // Initialize all entries with -1 (meaning empty)
            memset(disk->m[i].B[j].nom, 0, 21); // Clear name
            disk->m[i].B[j].supp = 0;  // Not deleted
        }
    }
    printf("Disk initialized with %d blocks.\n", nbBlocks);
}

// Function to simulate adding data to a block
void addDataToBlock(MS *disk, int blockIndex, const tenr *entry) {
    if (blockIndex >= 0 && blockIndex < disk->nb && disk->m[blockIndex].occup == 0 && disk->m[blockIndex].NE < MAX_FB) {
        disk->m[blockIndex].B[disk->m[blockIndex].NE] = *entry; // add entry to the block
        disk->m[blockIndex].NE++;  // increment the number of entries
        disk->m[blockIndex].occup = 1; // mark block as occupied
        disk->nblibre--; // decrease the free block count
    } else {
        printf("Error: Cannot add data to block %d.\n", blockIndex);
    }
}

// Function to defragment the disk
void defragmentDisk(MS *disk) {
    int writeIndex = 0; // tracks where to move used blocks

    for (int readIndex = 0; readIndex < disk->nb; readIndex++) {
        if (disk->m[readIndex].occup == 1) {
            // move the block if needed
            if (readIndex != writeIndex) {
                // copy entries to the write position
                disk->m[writeIndex] = disk->m[readIndex];

                // mark the old position as free
                disk->m[readIndex].occup = 0;
                disk->m[readIndex].NE = 0;  // Reset the number of entries
                for (int j = 0; j < MAX_FB; j++) {
                    disk->m[readIndex].B[j].id = -1;  // set ID to -1 (empty)
                    memset(disk->m[readIndex].B[j].nom, 0, 21); // Clear the name
                    disk->m[readIndex].B[j].supp = 0;  // Set 'supp' to 0 (not deleted)
                }
            }
            writeIndex++;
        }
    }

    printf("Disk defragmentation complete. All used blocks are now contiguous.\n");
}

// Function to clear all data in the disk (scc memory)
void clearDisk(MS *disk) {
    for (int i = 0; i < disk->nb; i++) {
        disk->m[i].occup = 0;  // Mark the block as free (not occupied)
        disk->m[i].NE = 0;     // clear all entries in the block

        // clear all entries in the block
        for (int j = 0; j < MAX_FB; j++) {
            disk->m[i].B[j].id = -1;  // Set ID to -1 (empty)
            memset(disk->m[i].B[j].nom, 0, 21); // Clear the name
            disk->m[i].B[j].supp = 0;  // set 'supp' to 0 (not deleted)
        }
    }
    disk->nblibre = disk->nb; // Reset the free block counter
    printf("Disk has been cleared. All blocks are now free.\n");
}

// Function to display the status of the disk
void displayDiskStatus(MS *disk) {
    printf("Disk Status:\n");
    printf("Free Blocks: %d\n", disk->nblibre);
    printf("Used Blocks: %d\n", disk->nb - disk->nblibre);
    printf("Blocks:\n");

    for (int i = 0; i < disk->nb; i++) {
        printf("Block %d: %s\n", i, disk->m[i].occup ? "Used" : "Free");
        if (disk->m[i].occup) {
            for (int j = 0; j < disk->m[i].NE; j++) {
                printf("  Entry %d: ID = %d, Name = %s\n", j, disk->m[i].B[j].id, disk->m[i].B[j].nom);
            }
        }
    }
}


int main(){

MS myDisk;
    int nbBlocks = 10;  // ex: 10 blocks
    // Initialize the disk
    initializeDisk(&myDisk, nbBlocks);

    // simulate adding data
    tenr entry1 = {1, "File 1", 0};
    tenr entry2 = {2, "File 2", 0};
    tenr entry3 = {3, "File 3", 0};

    addDataToBlock(&myDisk, 0, &entry1);
    addDataToBlock(&myDisk, 2, &entry2);
    addDataToBlock(&myDisk, 5, &entry3);

    printf("\nBefore Defragmentation:\n");
    displayDiskStatus(&myDisk);

    // Defragment the disk
    defragmentDisk(&myDisk);

    printf("\nAfter Defragmentation:\n");
    displayDiskStatus(&myDisk);

    return 0;
}

