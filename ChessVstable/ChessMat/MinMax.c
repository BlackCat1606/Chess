#define MAX 1
#define MIN -1
#define INFINI 10000
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
int tailleBDD=0 ;
int intCmp=0;
char save[2000]="";
int colonne=1;
typedef struct
{
    int taille ;
    char cmd_bdd[100][4];
} ligne ;
ligne BDD[1000];
ligne BDD2[500];
void lireFichier(char *cmd)
{

    FILE *fichier = fopen("book.txt","r");
    int i=0, j=0, k=0 ;
    char c=fgetc(fichier);
    while (c!=EOF)
    {
        if(c!='\n')
        {


            if(c!=' ')
            {

                BDD[i].cmd_bdd[j][k] =c;
                k++;
                if(k==4)
                {
                    j++;
                    k=0;
                }

            }

        }
        else
        {
            BDD[i].taille = j;
            i++;
            j=0;
        }
        c=fgetc(fichier);
        tailleBDD=i;
    }
    fclose(fichier);
int r = 0;
int ind=0;
for(r=0;r<tailleBDD;r++)
{
    char chaine[5]="";
    int t=0;
    for(t=0;t<4;t++)
    {
        chaine[t] = BDD[r].cmd_bdd[0][t];
    }
    printf("la commande est %d %s \n",r,cmd);
  if(strcmp(cmd,chaine)==0)
  {
      BDD2[ind].taille = BDD[r].taille;
      int j=0;
      for(j=0;j<BDD[r].taille;j++)
      {
          int k=0;
          for (k=0;k<=4;k++)
          {
             BDD2[ind].cmd_bdd[j][k] = BDD[r].cmd_bdd[j][k];

          }
         // printf("hello %s \n",BDD2[ind].cmd_bdd[j]);
      }
        ind++;

  }

}
tailleBDD = ind;
printf("tailleBDD %d \n",tailleBDD);
    //printf("%d\n",i);
}

void copier( struct configuration *c1, struct configuration *c2 ) /// Copie la configuration c1 dans c2
{
    int i, j;

    for (i=0; i<8; i++)
        for (j=0; j<8; j++)
            c2->board[i][j] = c1->board[i][j];
    for(i=0; i<4; i++)
        c2->roque[i] = c1->roque[i];
    c2->turn = c1->turn;
    c2->val = c1->val;
}

int egal(char c1[8][8], char c2[8][8] ) /// Teste si les conf c1 et c2 sont égales
{
    int i, j;

    for (i=0; i<8; i++)
        for (j=0; j<8; j++)
            if (c1[i][j] != c2[i][j]) return 0;
    return 1;
}

int feuille( struct configuration conf, int *cout ) /// Teste si conf représente une fin de partie et retourne dans 'cout' la valeur associée
{
    *cout = 0;
    int i=0,j=0,pasroiN=0,pasroiB=0;

    for (i=0; i<8; i++)
    {
        for (j=0; j<8; j++)
        {
            if(conf.board[i][j]== 'R')
            {
                pasroiB=1;
            }
        }
    }
    // Si victoire pour les Blancs cout = +10000
    for (i=0; i<8; i++)
    {
        for (j=0; j<8; j++)
        {
            if(conf.board[i][j]== -'R')
            {
                pasroiN=1;
            }
        }
    }
    if(!pasroiB)
    {
        *cout = -10000;
        return 1;
    }
    if(!pasroiN)
    {
        *cout = +10000;
        return 1;
    }
    // Sinon ce n'est pas une config feuille
    return 0;
}

int minmax_ab( struct configuration conf, int mode, int niv, int alpha, int beta ) /// MinMax avec elagage alpha-beta
{
    int n, i, score, score2;
    struct configuration T[100];

    if ( feuille(conf, &score) )
        return score;

    if ( niv == 0 )
        return evaluation( conf );

    if ( mode == MAX )
    {

        n=cout_possible(T, conf, MAX);
        score = alpha;
        for ( i=0; i<n; i++ )
        {
            score2 = minmax_ab( T[i], MIN, niv-1, score, beta );
            if (score2 > score) score = score2;
            if (score > beta)
            {
                return beta;
            }
        }
    }
    else    // mode == MIN
    {
        n=cout_possible(T, conf, MIN);
        score = beta;
        for ( i=0; i<n; i++ )
        {
            score2 = minmax_ab( T[i], MAX, niv-1, alpha, score );
            if (score2 < score) score = score2;
            if (score < alpha)
            {
                return alpha;
            }
        }
    }
    if ( score == +INFINI ) score = +10000;
    if ( score == -INFINI ) score = -10000;
    return score;
}

int minmax( struct configuration conf, int mode, int niv) /// MinMax sans elagage alpha-beta
{
    int n, i, score, score2;
    struct configuration T[100];

    if ( feuille(conf, &score) )
        return score;

    if ( niv == 0 )
        return evaluation( conf );
    if ( mode == MAX )
    {
        score = -INFINI;
    }
    else
    {
        score = +INFINI;
    }
    if ( mode == MAX )
    {
        n = cout_possible( T, conf, MAX);
        for ( i=0; i<n; i++ )
        {
            score2 = minmax( T[i], MIN, niv-1);
            if (score2 > score) score = score2;
        }
    }
    else
    {
        n=cout_possible(T, conf, MIN);
        for ( i=0; i<n; i++ )
        {
            score2 = minmax( T[i], MAX, niv-1);
            if (score2 < score) score = score2;
        }
    }
    if ( score == +INFINI ) score = +10000;
    if ( score == -INFINI ) score = -10000;
    return score;
}

void getCMD(configuration board1, configuration board2, char *cmd[5]) /// Conclure la comande du mouvement à partire du déplacement éffectué par la machine
{
    int i, j, x, y;
    bool get = false;

    if((board1.board[0][0] == 'T') && (board1.board[0][4] == 'R') && (board2.board[0][0] == 0) && (board2.board[0][4] == 0))
    {
        cmd[0] = 'e';
        cmd[1] = '1';
        cmd[2] = 'c';
        cmd[3] = '1';
        get = true;
    }
    if((board1.board[7][0] == -'T') && (board1.board[7][4] == -'R') && (board2.board[7][0] == 0) && (board2.board[7][4] == 0))
    {
        cmd[0] = 'e';
        cmd[1] = '8';
        cmd[2] = 'c';
        cmd[3] = '8';
        get = true;
    }
    if((board1.board[0][7] == 'T') && (board1.board[0][4] == 'R') && (board2.board[0][7] == 0) && (board2.board[0][4] == 0))
    {
        cmd[0] = 'e';
        cmd[1] = '1';
        cmd[2] = 'g';
        cmd[3] = '1';
        get = true;
    }
    if((board1.board[7][7] == -'T') && (board1.board[7][4] == -'R') && (board2.board[7][7] == 0) && (board2.board[7][4] == 0))
    {
        cmd[0] = 'e';
        cmd[1] = '8';
        cmd[2] = 'g';
        cmd[3] = '8';
        get = true;
    }
    if(!get)
    {
        for(i = 0 ; i <= 7 ; i++)
        {
            for(j = 0 ; j <= 7 ; j++)
            {
                if((board1.board[i][j] != 0) && (board2.board[i][j] == 0))
                {
                    x = i;
                    y = j;
                    cmd[1] = i + '1';
                    cmd[0] = j + 'a';
                }
            }
        }
        for(i = 0 ; i <= 7 ; i++)
        {
            for(j = 0 ; j <= 7 ; j++)
            {
                if((board1.board[i][j] != board2.board[i][j]) && ((x != i) || (y != j)))
                {
                    cmd[3] = i + '1';
                    cmd[2] = j + 'a';
                }
            }
        }
    }
}
bool lecture = true;
bool comparerLigne(int *i,int taille,char *mem,char *cmd)
{ bool found = false;
  int *indice = *i;
  char s[2000]="";
  strcpy(s,mem);
  strcat(s,cmd);
  strcat(s," ");
  char cmp[2000]="";
    while(*i<tailleBDD && !found)
    {
       // printf("la ligne est %d",*i);
        strcpy(cmp,"");
        int k=0,t=0;
        for(k=0;k<colonne;k++)
        {
            //printf("here1 \n");
            for(t=0;t<4;t++)
            {
                printf("%c \n", BDD2[*i].cmd_bdd[k][t]);
                cmp[intCmp] = BDD2[*i].cmd_bdd[k][t];
                intCmp++;
            }
            cmp[intCmp]=' ';
            intCmp++;

        }
            printf("S est %s\n",s);
            printf("CMP est %s\n",cmp);

        if(strcmp(s,cmp)==0)
        {
            printf("Comparaiosn Réusitte \n");
            //system("pause");
            found = true;
        }
        else
        {
        *i=*i+1;
       // *sortir = true;
        intCmp=0;
        }

    }
    intCmp = 0;
    return found;
}
void MachineIA() /// La fonction qui permet à la machine de prondre la bonne desision de deplacement
{
    int n, m, i, j, score, cout, hauteur;
    struct configuration T[100];
    char cmd[5];
    bool sortir = false;
    int x1, y1, x2, y2;
    bool deplacer = false;
    bool fin = false;
    int choix;
    int cpt = 0;
    int nb_etape = 0 ;
    //bool lecture = true ;

    configuration board;
    configuration echequier;


    printf("Veuillez-vous entré la profandeur de l'arbre:");
    scanf("%d", &hauteur);
    printf("\n\n");

    while((choix!=1)&&(choix!=2))
    {
        printf("MinMax Avec Elagage : 1\n");
        printf("MinMax Sans Elagage : 2\n");
        scanf("%d", &choix);
    }
    printf("\n\n");

    //hauteur=2;

    init(&board);
    affich(board);

    while(!fin)
    {


 turn:   dupliquerConfiguration(board, &echequier);
        if(roiEchec(board))
        {
            printf("Echec !");
        }

        printf("\n- Tour des blancs\n");
        while(!deplacer)
        {
            printf("Entrée votre déplacement: ");
            scanf("%s", cmd);

            if(strcmp(cmd, "0-0-0") == 0)
            {
                deplacer = grandRock(&board);
            }
            else
            {
                if(strcmp(cmd, "0-0") == 0)
                {
                    deplacer = petitRock(&board);
                }
                else
                {
                    x1 = cmd[1] - '1' + 1;
                    y1 = cmd[0] - 'a' + 1;
                    x2 = cmd[3] - '1' + 1;
                    y2 = cmd[2] - 'a' + 1;
                    deplacer = deplacerBlanc(&board, x1, y1, x2, y2);
                    if(deplacer && (x2 == 8))
                    {

                        transformerPiant(&board, x2 - 1, y2 - 1);
                    }
                }
            }
            if(deplacer)
            {
                board.turn--;
                if(roiEchec(board))
                {
                    deplacer = false;
                    dupliquerConfiguration(echequier, &board);
                }
                else
                {
                    board.turn++;
                }
                cpt++;
            }
            if(!deplacer)
            {
                printf("Déplacement impossible!\n");
            }
        }
        affich(board);
        deplacer=false;
        printf("\n- Tour des noirs\n");

        /** tour de la machine : ***/
        if(cpt==1){lireFichier(cmd);}
if(lecture)
{
 // lire la BDD}

            int iBDD=0,jBDD=0,kBDD=0;
            char couBDD[5];
            couBDD[4]='\0';
            bool trouv=false,found=false;bool stop=false;
            int ligne=0;

             while(iBDD<tailleBDD && !stop)
            {
                found = comparerLigne(&iBDD,tailleBDD,save,cmd);


                for (jBDD=colonne-1; jBDD< colonne/*BDD[iBDD].taille*/; jBDD++)
                {
                    if(colonne==BDD2[iBDD].taille || iBDD == tailleBDD )
                    {
                    lecture = false;
                    printf("ma taille %d",tailleBDD);
                    goto maxy ;
                    }
                    for(kBDD=0; kBDD<4; kBDD++)
                    {
                        //printf("%c", BDD[iBDD].cmd_bdd[jBDD][kBDD]);
                        couBDD[kBDD]=BDD2[iBDD].cmd_bdd[jBDD][kBDD];
                    }

                    if (strcmp(cmd,couBDD)== 0 && found)
                     {
                        colonne+=2;
                        strcat(save,cmd);
                        strcat(save," ");
                        printf("la chaine sauve : %s \n",save);
                        //////fonction ///////////
                        char coupMachine[5];
                        for(kBDD=0; kBDD<4; kBDD++)
                        {
                            coupMachine[kBDD]=BDD2[iBDD].cmd_bdd[jBDD+1][kBDD];
                        }
                        printf("Coup Machine : %s\n",coupMachine);
                        strcat(save,coupMachine);
                        strcat(save," ");
                        printf("nouveau save %s \n",save);
                        system("pause");

                        ////////////////////////////
                        dupliquerConfiguration(board, &echequier);
                        if(roiEchec(board))
                        {
                            printf("Echec !");
                        }
       //                 printf("\n- Tour des noirs \n");
                        while(!deplacer)
                        {

                            if(strcmp(coupMachine, "0-0-0") == 0)
                            {
                                 printf("\n- GrandRock \n");
                                deplacer = grandRock(&board);

                            }
                            else
                            {
                                if(strcmp(coupMachine, "0-0") == 0)
                                {
                                     printf("\n- petitRock \n");
                                    deplacer = petitRock(&board);


                                }
                                else
                                {
                                    x1 = coupMachine[1] - '1' + 1;
                                    y1 = coupMachine[0] - 'a' + 1;
                                    x2 = coupMachine[3] - '1' + 1;
                                    y2 = coupMachine[2] - 'a' + 1;
                                    deplacer = deplacerNoir(&board, x1, y1, x2, y2);
                                    if(deplacer && (x2 == 1))
                                    {
                                         printf("\n- Transfo \n");
                                        transformerPiant(&board, x2 - 1, y2 - 1);
                                    }
                                }
                            }
                            if(deplacer)
                            {
                                board.turn--;
                                if(roiEchec(board))
                                {
                                    deplacer = false;
                                     printf("\n- RoisEchec \n");
                                    dupliquerConfiguration(echequier, &board);
                                }
                                else
                                {
                                    board.turn++;
                                }
                            }
                            if(!deplacer)
                            {
                                printf("Déplacement impossible noir!\n");
                                goto maxy;

                            }
                        }

          affich(board);

                    }
                    else
                    {
                         //lecture = false;
                    deplacer=false;
                    stop = true;goto turn;
                    }
                }
            }
}
else{
          maxy: printf("\n MIN MAX \n");
            n=cout_possible( T, board, MIN );
            score = +INFINI;
            j = -1;
            for (i=0; i<n; i++)
            {
                if(choix==1)
                {
                    cout = minmax_ab( T[i], MAX, hauteur,-INFINI, +INFINI);
                }
                else
                {
                    cout = minmax( T[i], MAX, hauteur);
                }
                if ( cout < score )
                {
                    score = cout;
                    j = i;
                }
            }
            if ( j != -1 )
            {
                copier( &T[j], &board );
                board.val = score;
            }
            deplacer = false;
            affich(board);
            m=cout_possible( T, board, MAX );
            if((n == 0) || (m == 0))
            {
                fin = true;
            }
            printf("\nn:%d         m:%d\n",n,m);

}
    }
    printf("Echec est mat !");
}
