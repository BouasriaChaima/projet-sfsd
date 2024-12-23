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
typedef struct {
    struct tbloc block[DISK_SIZE]; // Array of blocks
    int freeBlocks;          // Number of free blocks
} Disk;


// fonction pour cration de blocs en mode contigue
void creatBlocContigue( struct tbloc BLOC int numenr){
   BLOC->occup = 1 ;
   BLOC->NE = 0 ;
  for (int i = 0 ; i < numenr -1; i++){
    printf("give the id and the name of product number: %d\n" , i+1 );
    scanf("%d\n" , &BLOC->B[i].id);
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
        scanf("%d\n" , &newBloc->B[i].id);
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

   printf("give the name of the file \n:");
   scanf("%s\n" , &nomFichier);
   strncpy(MT.nomfichier, nomFichier, sizeof(MT.nomfichier));

   // create the file
   FILE *file = fopen(nomFichier , "w");

   printf ("give the number of records \n:");
   scanf("%d\n" , &nbrEnreg);
   nbrBloc = nbrEnreg / MAX_FB;
   MT.taillenreg = nbrEnreg;
   MT.tailleblocs = nbrBloc ;


   printf ("give the mode of global file creation\n : ");
   scanf("%d\n" , & choixGlobale);

    printf ("give the mode of intern file creation\n : ");
    scanf("%d\n" , & choixIntern);

    if ( choixGlobale == 0 ){
    // le mode globale = contigue
    strncpy(MT.modeorgaglobale , "contigue" , sizeof(MT.modeorgaglobale));
        for (int i = 0 ; i < nbrBloc - 1 ; i++ ){
            printf("creation of bloc %d (contigue)\n" , i+1);
            creatBlocContigue(&BlocContigue);
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
      for (int i = 0 ; i < nbrBloc - 1; i++){
      printf("cration of bloc %d (chainee)\n" , i+1);
      // mode FIFO
       struct tblocChaine *newBloc = creatBlocChaine();
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
                              debut = milieu + 1; // On cherche dans la moiti� droite
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
                  debut = milieu + 1; // On cherche dans la moiti� droite
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

int main(){






    return 0;
}

