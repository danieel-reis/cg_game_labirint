/* Responsavel por realizar a leitura do arquivo .OBJ e armazenar os dados lidos */

/*********************************************** BIBLIOTECAS ************************************************/
#include <GL/glut.h>
#include <stdio.h>
#include <string.h>

/******************************************** ESTRUTURA DE DADOS ********************************************/
typedef struct rgb /* RGB */
{
    GLfloat r, g, b; /* Cor */
} Rgb;

typedef struct mtl /* Arquivo .mtl */
{
    char newmtl[30];
    GLfloat Ns, Ni, d;
    Rgb Ka, Kd, Ks, Ke;
    struct mtl *ant, *prox; /* Referencia para o elemento anterior e o proximo cadastrado na lista */
} Mtl;

typedef struct v /* Vertice */
{
    GLfloat x, y, z; /* Coordenadas */
    struct v *ant, *prox; /* Referencia para o elemento anterior e o proximo cadastrado na lista */
} V;

typedef struct vt /* Coordenada de textura de um vertice */
{
    GLfloat x, y; /* Coordenadas */
    struct vt *ant, *prox; /* Referencia para o elemento anterior e o proximo cadastrado na lista */
} VT;

typedef struct vn /* Normal de um vertice */
{
    GLfloat x, y, z; /* Coordenadas */
    struct vn *ant, *prox; /* Referencia para o elemento anterior e o proximo cadastrado na lista */
} VN;

typedef struct f /* Face */
{
    GLint vi1, uvi1, ni1, vi2, uvi2, ni2, vi3, uvi3, ni3; /* Indice dos vertices, uv, e normal (x, y e z de cada) */
    char texture[30];
    struct f *ant, *prox; /* Referencia para o elemento anterior e o proximo cadastrado na lista */
} F;

typedef struct obj /* OBJ */
{
    V *vlistinit, *vlistend;
    VT *vtlistinit, *vtlistend;
    VN *vnlistinit, *vnlistend;
    F *flistinit, *flistend;
    Mtl *mtllistinit, *mtllistend;
} OBJ;

/************************************************* LISTAS ***************************************************/
void createOBJEmpty(OBJ* obj) /* Inicializa todas as listas */
{
    obj->vlistinit = (V*) malloc (sizeof (V));          /* Cria a celula e liga na referencia da primeira da lista */
    obj->vlistend = obj->vlistinit;                     /* No momento a ultima celula e igual a primeira */
    obj->vlistinit->prox = NULL;                        /* Anula a Referencia da proxima celula */
    obj->vlistinit->ant = NULL;                         /* Anula a Referencia da celula anterior */
    
    obj->vtlistinit = (VT*) malloc (sizeof (VT));       /* Cria a celula e liga na referencia da primeira da lista */
    obj->vtlistend = obj->vtlistinit;                   /* No momento a ultima celula e igual a primeira */
    obj->vtlistinit->prox = NULL;                       /* Anula a Referencia da proxima celula */
    obj->vtlistinit->ant = NULL;                        /* Anula a Referencia da celula anterior */
    
    obj->vnlistinit = (VN*) malloc (sizeof (VN));       /* Cria a celula e liga na referencia da primeira da lista */
    obj->vnlistend = obj->vnlistinit;                   /* No momento a ultima celula e igual a primeira */
    obj->vnlistinit->prox = NULL;                       /* Anula a Referencia da proxima celula */
    obj->vnlistinit->ant = NULL;                        /* Anula a Referencia da celula anterior */
    
    obj->flistinit = (F*) malloc (sizeof (F));          /* Cria a celula e liga na referencia da primeira da lista */
    obj->flistend = obj->flistinit;                     /* No momento a ultima celula e igual a primeira */
    obj->flistinit->prox = NULL;                        /* Anula a Referencia da proxima celula */
    obj->flistinit->ant = NULL;                         /* Anula a Referencia da celula anterior */
    
    obj->mtllistinit = (Mtl*) malloc (sizeof (Mtl));    /* Cria a celula e liga na referencia da primeira da lista */
    obj->mtllistend = obj->mtllistinit;                 /* No momento a ultima celula e igual a primeira */
    obj->mtllistinit->prox = NULL;                      /* Anula a Referencia da proxima celula */
    obj->mtllistinit->ant = NULL;                       /* Anula a Referencia da celula anterior */
}

/* Insere no fim da lista */
void insertV(OBJ *obj, V v)
{
    obj->vlistend->prox = (V*) malloc (sizeof (V));  /* Cria a celula e liga na referencia da ultima da lista */
    obj->vlistend->prox->ant = obj->vlistend;        /* Liga o anterior da nova celula a ultima celula da lista */
    obj->vlistend = obj->vlistend->prox;             /* Atualiza a referencia de ultima celula da lista */
    obj->vlistend->prox = NULL;                      /* Liga a referencia de proximo da ultima celula pra nulo */
    obj->vlistend->x = v.x;                          /* Insere os dados lidos */
    obj->vlistend->y = v.y;                          /* Insere os dados lidos */
    obj->vlistend->z = v.z;                          /* Insere os dados lidos */
}

/* Insere no fim da lista */
void insertVT(OBJ *obj, VT vt)
{
    obj->vtlistend->prox = (VT*) malloc (sizeof (VT));  /* Cria a celula e liga na referencia da ultima da lista */
    obj->vtlistend->prox->ant = obj->vtlistend;         /* Liga o anterior da nova celula a ultima celula da lista */
    obj->vtlistend = obj->vtlistend->prox;              /* Atualiza a referencia de ultima celula da lista */
    obj->vtlistend->prox = NULL;                        /* Liga a referencia de proximo da ultima celula pra nulo */
    obj->vtlistend->x = vt.x;                           /* Insere os dados lidos */
    obj->vtlistend->y = vt.y;                           /* Insere os dados lidos */
}

/* Insere no fim da lista */
void insertVN(OBJ *obj, VN vn)
{
    obj->vnlistend->prox = (VN*) malloc (sizeof (VN));  /* Cria a celula e liga na referencia da ultima da lista */
    obj->vnlistend->prox->ant = obj->vnlistend;         /* Liga o anterior da nova celula a ultima celula da lista */
    obj->vnlistend = obj->vnlistend->prox;              /* Atualiza a referencia de ultima celula da lista */
    obj->vnlistend->prox = NULL;                        /* Liga a referencia de proximo da ultima celula pra nulo */
    obj->vnlistend->x = vn.x;                           /* Insere os dados lidos */
    obj->vnlistend->y = vn.y;                           /* Insere os dados lidos */
    obj->vnlistend->z = vn.z;                           /* Insere os dados lidos */
}

/* Insere no fim da lista */
void insertF(OBJ *obj, F f)
{
    obj->flistend->prox = (F*) malloc (sizeof (F));  /* Cria a celula e liga na referencia da ultima da lista */
    obj->flistend->prox->ant = obj->flistend;        /* Liga o anterior da nova celula a ultima celula da lista */
    obj->flistend = obj->flistend->prox;             /* Atualiza a referencia de ultima celula da lista */
    obj->flistend->prox = NULL;                      /* Liga a referencia de proximo da ultima celula pra nulo */
    obj->flistend->vi1 = f.vi1;                      /* Insere os dados lidos */
    obj->flistend->uvi1 = f.uvi1;                    /* Insere os dados lidos */
    obj->flistend->ni1 = f.ni1;                      /* Insere os dados lidos */
    obj->flistend->vi2 = f.vi2;                      /* Insere os dados lidos */
    obj->flistend->uvi2 = f.uvi2;                    /* Insere os dados lidos */
    obj->flistend->ni2 = f.ni2;                      /* Insere os dados lidos */
    obj->flistend->vi3 = f.vi3;                      /* Insere os dados lidos */
    obj->flistend->uvi3 = f.uvi3;                    /* Insere os dados lidos */
    obj->flistend->ni3 = f.ni3;                      /* Insere os dados lidos */
    strcpy(obj->flistend->texture, f.texture);       /* Insere os dados lidos */
}

/* Insere no fim da lista */
void insertMTL(OBJ *obj, Mtl mtl)
{   
    obj->mtllistend->prox = (Mtl*) malloc (sizeof (Mtl));   /* Cria a celula e liga na referencia da ultima da lista */
    obj->mtllistend->prox->ant = obj->mtllistend;           /* Liga o anterior da nova celula a ultima celula da lista */
    obj->mtllistend = obj->mtllistend->prox;                /* Atualiza a referencia de ultima celula da lista */
    obj->mtllistend->prox = NULL;                           /* Liga a referencia de proximo da ultima celula pra nulo */
    strcpy(obj->mtllistend->newmtl, mtl.newmtl);            /* Insere os dados lidos */
    obj->mtllistend->Ns = mtl.Ns;                           /* Insere os dados lidos */
    obj->mtllistend->Ni = mtl.Ni;                           /* Insere os dados lidos */
    obj->mtllistend->d = mtl.d;                             /* Insere os dados lidos */
    obj->mtllistend->Ka = mtl.Ka;                           /* Insere os dados lidos */
    obj->mtllistend->Kd = mtl.Kd;                           /* Insere os dados lidos */
    obj->mtllistend->Ks = mtl.Ks;                           /* Insere os dados lidos */
    obj->mtllistend->Ke = mtl.Ke;                           /* Insere os dados lidos */
}

/* Imprime o OBJ */
void printOBJ(OBJ obj)
{
    V *vaux;     /* Cria uma celula auxiliar para percorrer a lista */
    VT *vtaux;   /* Cria uma celula auxiliar para percorrer a lista */
    VN *vnaux;   /* Cria uma celula auxiliar para percorrer a lista */
    F *faux;     /* Cria uma celula auxiliar para percorrer a lista */
    Mtl *mtlaux; /* Cria uma celula auxiliar para percorrer a lista */
    
    printf("\nImpressao OBJ\n");
    
    /* Percorre a lista da primeira a ultima celula */
    for(vaux = obj.vlistinit->prox; vaux != NULL; vaux = vaux->prox)
        printf("\tv %.6f %.6f %.6f\n", vaux->x, vaux->y, vaux->z);
    for(vtaux = obj.vtlistinit->prox; vtaux != NULL; vtaux = vtaux->prox)
        printf("\tvt %.6f %.6f\n", vtaux->x, vtaux->y);
    for(vnaux = obj.vnlistinit->prox; vnaux != NULL; vnaux = vnaux->prox)
        printf("\tvn %.6f %.6f %.6f\n", vnaux->x, vnaux->y, vnaux->z);
    for(faux = obj.flistinit->prox; faux != NULL; faux = faux->prox)
        printf("\tf %d/%d/%d %d/%d/%d %d/%d/%d\t%s\n", faux->vi1, faux->uvi1, faux->ni1, faux->vi2, faux->uvi2, faux->ni2, faux->vi3, faux->uvi3, faux->ni3, faux->texture);
    for(mtlaux = obj.mtllistinit->prox; mtlaux != NULL; mtlaux = mtlaux->prox)
    {
        printf("\tmtl\n\t\tNs %.6f", mtlaux->Ns);
        printf("\n\t\tKa %.6f %.6f %.6f", mtlaux->Ka.r, mtlaux->Ka.g, mtlaux->Ka.b);
        printf("\n\t\tKd %.6f %.6f %.6f", mtlaux->Kd.r, mtlaux->Kd.g, mtlaux->Kd.b);
        printf("\n\t\tKs %.6f %.6f %.6f", mtlaux->Ks.r, mtlaux->Ks.g, mtlaux->Ks.b);
        printf("\n\t\tKe %.6f %.6f %.6f", mtlaux->Ke.r, mtlaux->Ke.g, mtlaux->Ke.b);
        printf("\n\t\tNi %.6f\n\t\td %.6f\n", mtlaux->Ni, mtlaux->d);
    }
    printf("\n");
}

/****************************************** LEITURA DO ARQUIVO .OBJ ******************************************/
/* Leitura dos dados do arquivo obj */
void loadOBJ(char *filenameOBJ, char *filenameMTL, OBJ* obj)
{   
    FILE *file = fopen(filenameOBJ, "r");
    if (file == NULL)
    {
        printf("Nao foi possivel abrir o arquivo %s\n", filenameOBJ);
        exit(1); /* Sair */
    }
    
    /* Declaracao dos tipos de dados utilizados */
    V v;
    VT vt;
    VN vn;
    F f;
    Mtl mtl;
    Rgb rgb;
    
    char line[50]; /* Linha lida */
    char delimiter_space[] = " "; /* Caractere delimitador espaco */
    char delimiter_barra[] = "/"; /* Caractere delimitador barra */
    char *info; /* Ponteiro para armazenar as informacoes lidas */
    
    char type[10];      /* Tipo da linha lida */
    char texture[30];   /* Textura */
    
    printf("Importando arquivo %s\n", filenameOBJ);
    
    while(fgets(line, sizeof(line)-1, file) != NULL) /* Loop para ler cada linha do arquivo enquanto houver linhas */
    {       
        info = strtok(line, delimiter_space); /* info recebe a primeira linha antes do primeiro delimitador */
        strcpy(type, info); /* Copia o tipo */
        
        if (strcmp(type, "usemtl") == 0) /* Se o tipo for "usemtl" - textura */
        {
            info = strtok(NULL, delimiter_space);
            strcpy(texture, info);
//             printf("\t%s\n", texture);
        }
        else if (strcmp(type, "v") == 0) /* Se o tipo for "v" - vertices */
        {
            info = strtok(NULL, delimiter_space); /* Primeiro valor -> x */
            v.x = atof(info); /* Copia e converte pra float */
            info = strtok(NULL, delimiter_space); /* Segundo valor -> y */
            v.y = atof(info); /* Copia e converte pra float */
            info = strtok(NULL, delimiter_space); /* Terceiro valor -> z */
            v.z = atof(info); /* Copia e converte pra float */
            
//            printf("\t%s %f %f %f\n", type, v.x, v.y, v.z);
            insertV(obj, v); /* Insere na lista */
        }
        else if (strcmp(type, "vt") == 0) /* Se o tipo for "vt" */
        {
            info = strtok(NULL, delimiter_space); /* Primeiro valor -> x */
            vt.x = atof(info); /* Copia e converte pra float */
            info = strtok(NULL, delimiter_space); /* Segundo valor -> y */
            vt.y = atof(info); /* Copia e converte pra float */
                
//            printf("\t%s %f %f\n", type, vt.x, vt.y);
            insertVT(obj, vt); /* Insere na lista */
        }
        else if (strcmp(type, "vn") == 0) /* Se o tipo for "vn" -> normal */
        {
            info = strtok(NULL, delimiter_space); /* Primeiro valor -> x */
            vn.x = atof(info); /* Copia e converte pra float */
            info = strtok(NULL, delimiter_space); /* Segundo valor -> y */
            vn.y = atof(info); /* Copia e converte pra float */
            info = strtok(NULL, delimiter_space); /* Terceiro valor -> z */
            vn.z = atof(info); /* Copia e converte pra float */
                
//            printf("\t%s %f %f %f\n", type, vn.x, vn.y, vn.z);
            insertVN(obj, vn); /* Insere na lista */
        }
        else if (strcmp(type, "f") == 0) /* Se o tipo for "f" */
        {
            info = strtok(NULL, delimiter_barra); /* Primeiro valor */
            f.vi1 = atoi(info); /* Copia e converte pra inteiro */
            info = strtok(NULL, delimiter_barra); /* Segundo valor */
            f.uvi1 = atoi(info); /* Copia e converte pra inteiro */
            info = strtok(NULL, delimiter_space); /* Terceiro valor */
            f.ni1 = atoi(info); /* Copia e converte pra inteiro */
            
            info = strtok(NULL, delimiter_barra); /* Quarto valor */
            f.vi2 = atoi(info); /* Copia e converte pra inteiro */
            info = strtok(NULL, delimiter_barra); /* Quinto valor */
            f.uvi2 = atoi(info); /* Copia e converte pra inteiro */
            info = strtok(NULL, delimiter_space); /* Sexto valor */
            f.ni2 = atoi(info); /* Copia e converte pra inteiro */
            
            info = strtok(NULL, delimiter_barra); /* Setimo valor */
            f.vi3 = atoi(info); /* Copia e converte pra inteiro */
            info = strtok(NULL, delimiter_barra); /* Oitavo valor */
            f.uvi3 = atoi(info); /* Copia e converte pra inteiro */
            info = strtok(NULL, delimiter_space); /* Nono valor */
            f.ni3 = atoi(info); /* Copia e converte pra inteiro */
            
            strcpy(f.texture, texture); /* Textura */
            
//            printf("\t%s %d/%d/%d %d/%d/%d %d/%d/%d\n", type, f.vi1, f.uvi1, f.ni1, f.vi2, f.uvi2, f.ni2, f.vi3, f.uvi3, f.ni3);
            insertF(obj, f); /* Insere na lista */
        }
    }
        
    fclose(file); /* Fecha o arquivo */
    
    file = fopen(filenameMTL, "r");
    if (file == NULL)
    {
        printf("Nao foi possivel abrir o arquivo %s\n", filenameMTL);
        exit(1); /* Sair */
    }
    
    printf("Importando arquivo %s\n", filenameMTL);
    
    while(fgets(line, sizeof(line)-1, file) != NULL) /* Loop para ler cada linha do arquivo enquanto houver linhas */
    {       
        info = strtok(line, delimiter_space); /* info recebe a primeira linha antes do primeiro delimitador */
        strcpy(type, info); /* Copia o tipo */
        
        if (strcmp(type, "newmtl") == 0) /* Se o tipo for "newmtl" - textura */
        {
            info = strtok(NULL, delimiter_space);
            strcpy(mtl.newmtl, info);
        }
        else if (strcmp(type, "Ns") == 0) /* Se o tipo for "Ns" */
        {
            info = strtok(NULL, delimiter_space);
            mtl.Ns = atof(info); /* Copia e converte pra float */
        }
        else if (strcmp(type, "Ka") == 0) /* Se o tipo for "Ka" */
        {
            info = strtok(NULL, delimiter_space);
            rgb.r = atof(info); /* Copia e converte pra float */
            info = strtok(NULL, delimiter_space);
            rgb.g = atof(info); /* Copia e converte pra float */
            info = strtok(NULL, delimiter_space);
            rgb.b = atof(info); /* Copia e converte pra float */
            
            mtl.Ka = rgb;
        }
        else if (strcmp(type, "Kd") == 0) /* Se o tipo for "Kd" */
        {
            info = strtok(NULL, delimiter_space);
            rgb.r = atof(info); /* Copia e converte pra float */
            info = strtok(NULL, delimiter_space);
            rgb.g = atof(info); /* Copia e converte pra float */
            info = strtok(NULL, delimiter_space);
            rgb.b = atof(info); /* Copia e converte pra float */
            
            mtl.Kd = rgb;
        }
        else if (strcmp(type, "Ks") == 0) /* Se o tipo for "Ks" */
        {
            info = strtok(NULL, delimiter_space);
            rgb.r = atof(info); /* Copia e converte pra float */
            info = strtok(NULL, delimiter_space);
            rgb.g = atof(info); /* Copia e converte pra float */
            info = strtok(NULL, delimiter_space);
            rgb.b = atof(info); /* Copia e converte pra float */
            
            mtl.Ks = rgb;
        }
        else if (strcmp(type, "Ke") == 0) /* Se o tipo for "Ke" */
        {
            info = strtok(NULL, delimiter_space);
            rgb.r = atof(info); /* Copia e converte pra float */
            info = strtok(NULL, delimiter_space);
            rgb.g = atof(info); /* Copia e converte pra float */
            info = strtok(NULL, delimiter_space);
            rgb.b = atof(info); /* Copia e converte pra float */
            
            mtl.Ke = rgb;
        }
        else if (strcmp(type, "Ni") == 0) /* Se o tipo for "Ni" */
        {
            info = strtok(NULL, delimiter_space);
            mtl.Ni = atof(info); /* Copia e converte pra float */
        }
        else if (strcmp(type, "d") == 0) /* Se o tipo for "d" */
        {
            info = strtok(NULL, delimiter_space);
            mtl.d = atof(info); /* Copia e converte pra float */
            
            insertMTL(obj, mtl); /* Insere na lista */
        }
    }
    
    fclose(file); /* Fecha o arquivo */
}

/********************************************* BUSCA POR INDICE **********************************************/
V getV(OBJ obj, int indice)
{
    int contador = 1;
    V *vaux;     /* Cria uma celula auxiliar para percorrer a lista */
    
    /* Percorre a lista da primeira a ultima celula */
    for(vaux = obj.vlistinit->prox; vaux != NULL; vaux = vaux->prox, contador++)
    {
        if (contador == indice)
        {
            return *vaux;
        }
    }
    return *vaux;
}

VT getVT(OBJ obj, int indice)
{
    int contador = 1;
    VT *vtaux;   /* Cria uma celula auxiliar para percorrer a lista */
    
    /* Percorre a lista da primeira a ultima celula */
    for(vtaux = obj.vtlistinit->prox; vtaux != NULL; vtaux = vtaux->prox, contador++)
    {
        if (contador == indice)
        {
            return *vtaux;
        }
    }
    return *vtaux;
}

VN getVN(OBJ obj, int indice)
{
    int contador = 1;
    VN *vnaux;   /* Cria uma celula auxiliar para percorrer a lista */

    /* Percorre a lista da primeira a ultima celula */
    for(vnaux = obj.vnlistinit->prox; vnaux != NULL; vnaux = vnaux->prox, contador++)
    {
        if (contador == indice)
        {
            return *vnaux;
        }
    }
    return *vnaux;
}

Mtl getMtl(OBJ obj, char name[])
{
    Mtl *mtlaux;   /* Cria uma celula auxiliar para percorrer a lista */

    /* Percorre a lista da primeira a ultima celula */
    for(mtlaux = obj.mtllistinit->prox; mtlaux != NULL; mtlaux = mtlaux->prox)
    {
        if (strcmp(name, mtlaux->newmtl) == 0)
        {
            return *mtlaux;
        }
    }
    return *mtlaux;
}

int searchMtl(OBJ obj, char name[])
{
    int contador = 1;
    Mtl *mtlaux;   /* Cria uma celula auxiliar para percorrer a lista */

    /* Percorre a lista da primeira a ultima celula */
    for(mtlaux = obj.mtllistinit->prox; mtlaux != NULL; mtlaux = mtlaux->prox)
    {
        if (strcmp(name, mtlaux->newmtl) == 0)
        {
            return contador;
        }
    }
    return -1;
}
