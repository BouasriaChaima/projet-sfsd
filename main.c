#include <stdio.h>
#include <stdlib.h>
#define MAX_FB 100
#include<string.h>

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

<<<<<<< HEAD
struct tBuffer {
    struct tbloc b;
};


=======
>>>>>>> d4deacef09d54fe9605c5808f054a9c02edcde36
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
void renamefile(File *ms, File *f, char newname[]){
    struct tbloc


}
struct tMetaD{
  char nomfichier[51];
  int tailleblocs;
  int taillenreg;
  int adrprebloc;
  char modeorgaglobale[51];
  char modeorgainterne[51];
};
// recherche d'un enregistrement dans un fichier  en cas d'organisation contigu
void  rechercheenregistement(FILE *ms, FILE *f, int id,int adrs[2])){
   char orgaglobale[51];
    int nbrbloc=1;
     int adr = lirecharacteristique();//adresse du premier bloc
     int taille = lirecharacteristique();
   strcpy(orgaglobale,lirecharacteristique());//on verifie s'il s'agit d'un fichier contigu ou non
   if(strcmp(orgaglobale, "chainee")==0){
      struct tblocChaine buffer;
     char orgainterne[51];
  strcpy(orgainterne, lirecharacteristique());
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
        buffe=buffer->next;
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
        buffer
           }
   }
   }
   else if (strcmp(orgaglobale, "contigue")==0){
  struct tbloc buffer;
  char orgainterne[51];
  strcpy(orgainterne, lirecharacteristique());
  char orgainterne = lirecharacteristique();
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
//suppresion logique an cas d'organisation contigu
void supplogiccontigu(FILE *ms, FILE *f, int id){
    int adrs[2];
    struct tbloc buffer;
    n = adrs[0];
    m= adrs[1];
    recherchecontiguordonne(ms, f,id,adrs);
   int a = lirecharacteristique();//adresse du premier bloc
   fseek(ms, a*sizeof(buffer), SEEK_SET);
   fseek(ms, n*sizeof(buffer), SEEK_CUR);
   buffer.b[m-1].supp =1;

}

int main()
{

    return 0;
}
