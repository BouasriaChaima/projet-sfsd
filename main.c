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
struct tBuffer {
    struct tbloc b;
};
struct MS {
    struct tbloc m[];
    int nb;
    int nblibre;
};
int main()
{

    return 0;
}
