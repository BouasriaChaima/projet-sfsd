#include <stdio.h>
#include <stdlib.h>
#define MAX_FB 100
#include<string.h>

struct tenr{
  int id;
  char nom[21];
  int supp;
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


struct tBuffer {
    struct tbloc b;
};


struct MS {
    int nb;
    int nblibre;
    struct tbloc m[];
};


// fonction pour cration de blocs en mode contigue
void creatBlocContigue( struct tbloc *BLOC){
   BLOC->occup = 1 ;
   BLOC->NE = 0 ;
  for (int i = 0 ; i < MAX_FB -1; i++){
    printf("give the id and the name of product number: %d\n" , i+1 );
    scanf("%d\n" , &BLOC->B[i].id);
    scanf("%s\n", &BLOC->B[i].nom);
    BLOC->B[i].supp = 1 ; // existe
    BLOC->NE++;
  }
}
// fonction pour creation de blocs en mode chainee
struct tblocChaine *creatBlocChaine (){
   struct  tblocChaine *newBloc = (struct tblocChaine *)malloc(sizeof(struct tblocChaine));
     //if (*newBloc == NULL){}
     newBloc->occup = 1 ;
     newBloc->NE = 0;
     for (int i = 0 ; i < MAX_FB ; i++ ){
        printf("give the id and the name of product number: %d\n" , i+1 );
        scanf("%d\n" , &newBloc->B[i].id);
        scanf("%s\n", &newBloc->B[i].nom);
        newBloc->B[i].supp = 1 ; // existe
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
 // creation du fichier meta donnees associer
  char metaFichierMT [53];
  snprintf(metaFicherMT, sizeof(metaFicherMT), "MT%s", nomFichier);
  FILE *metaFichier = fopen(metaFichier , "w");
  fwrite(&MT , sizeof(struct tMetaD) , 1 , metaFichier);
  fclose(metaFichier);
}





//function to rename a fike

/*void renommerfichier(FILE *f, char newname[]){

}
// recherche d'un fichier en cas d'organisation contigu
void recherchecontiguordonne(FILE *ms, FILE *f, int id)){
  struct tbloc buffer;

}
//suppresion logique an cas d'organisation contigu
void supplogiccontigu(FILE *f, int id){

}*/

int main()
{

    return 0;
}
