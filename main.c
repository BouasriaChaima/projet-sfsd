#include <stdio.h>
#include <stdlib.h>
#define MAX_FB 100
struct tenr{
  int id;
  char nom[21];
  int supp;
};
struct tbloc{
 struct tenr B [MAX_FB];
 int NE;
 int occup;
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
    struct tbloc m[];
    int nb;
    int nblibre;
};
//function to rename a fike

void renommerfichier(FILE *f, char newname[]){

}
// recherche d'un fichier en cas d'organisation contigu
void recherchecontiguordonne(FILE *ms, FILE *f, int id)){
  struct tbloc buffer;

}
//suppresion logique an cas d'organisation contigu
void supplogiccontigu(FILE *f, int id){

}
int main()
{

    return 0;
}
