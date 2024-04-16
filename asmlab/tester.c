#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include <stdbool.h>

#include "lib.h"

#define RUN(filename, action) pfile=fopen(filename,"a"); action; fclose(pfile);
#define NL(filename) pfile=fopen(filename,"a"); fprintf(pfile,"\n"); fclose(pfile);

char *filename_1 =  "salida.propios.caso1.txt";
char *filename_2 =  "salida.propios.caso2.txt";
void test_1(char* filename);
void test_2(char* filename);

void test_int(char* filename);
void test_string(char* filename);
void test_array(char* filename);
void test_list(char* filename);
void test_card(char* filename);
void test_game(char* filename);

int main() {

    remove(filename_1);
    remove(filename_2);

    srand(12345);
    test_int(filename_1);
    test_string(filename_1);
    test_array(filename_1);
    test_list(filename_1);
    test_card(filename_1);
    test_game(filename_2);
    return 0;
}

char* randomString(uint32_t l) {
    // 32 a 126 = caracteres validos
    char* s = malloc(l+1);
    for(uint32_t i=0; i<(l+1); i++)
       s[i] = (char)(33+(rand()%(126-33)));
    s[l] = 0;
    return s;
}

char* randomHexString(uint32_t l) {
    char* s = malloc(l+1);
    for(uint32_t i=0; i<(l+1); i++) {
        do {
            s[i] = (char)(rand()%256);
        } while (s[i]==0);
    }
    s[l] = 0;
    return s;
}

char* strings[10] = {"aa","bb","dd","ff","00","zz","cc","ee","gg","hh"};

void test_array(char* filename) {
    FILE* pfile;
    array_t* a;
    void* data;

    RUN(filename, fprintf(pfile, "== Array ==\n");) NL(filename)

    // Agregar
    RUN(filename, fprintf(pfile, "Agregar\n");) NL(filename)
    a = arrayNew(TypeString, 20);
    RUN(filename, fprintf(pfile, "%i - ", arrayGetSize(a));) RUN(filename, arrayPrint(a,pfile);) NL(filename)
    arrayAddLast(a,strings[0]);
    RUN(filename, fprintf(pfile, "%i - ", arrayGetSize(a));) RUN(filename, arrayPrint(a,pfile);) NL(filename)
    arrayAddLast(a,strings[1]);
    RUN(filename, fprintf(pfile, "%i - ", arrayGetSize(a));) RUN(filename, arrayPrint(a,pfile);) NL(filename)
    arrayAddLast(a,strings[2]);
    RUN(filename, fprintf(pfile, "%i - ", arrayGetSize(a));) RUN(filename, arrayPrint(a,pfile);) NL(filename)
    NL(filename)
    // Get
    RUN(filename, fprintf(pfile, "Get\n");) NL(filename)
    RUN(filename, fprintf(pfile, "%i - ", arrayGetSize(a));) RUN(filename, arrayPrint(a,pfile);) NL(filename)
    data = arrayGet(a,0);
    RUN(filename, fprintf(pfile, "%s\n", (char*)data);)
    data = arrayGet(a,1);
    RUN(filename, fprintf(pfile, "%s\n", (char*)data);)
    data = arrayGet(a,2);
    RUN(filename, fprintf(pfile, "%s\n", (char*)data);)
    data = arrayGet(a,3);
    RUN(filename, fprintf(pfile, "%s\n", (char*)data);)
    NL(filename)

    // Quitar
    RUN(filename, fprintf(pfile, "Quitar\n");) NL(filename)
    data = arrayRemove(a,0);
    RUN(filename, fprintf(pfile, "%i - ", arrayGetSize(a));) RUN(filename, arrayPrint(a,pfile);) RUN(filename, fprintf(pfile, " - %s\n", (char*)data);)
    strDelete((char*)data);
    data = arrayRemove(a,1);
    RUN(filename, fprintf(pfile, "%i - ", arrayGetSize(a));) RUN(filename, arrayPrint(a,pfile);) RUN(filename, fprintf(pfile, " - %s\n", (char*)data);)
    strDelete((char*)data);
    data = arrayRemove(a,0);
    RUN(filename, fprintf(pfile, "%i - ", arrayGetSize(a));) RUN(filename, arrayPrint(a,pfile);) RUN(filename, fprintf(pfile, " - %s\n", (char*)data);)
    strDelete((char*)data);
    NL(filename)
    // Swap
    RUN(filename, fprintf(pfile, "Swap\n");) NL(filename)
    arrayAddLast(a,strings[0]); arrayAddLast(a,strings[1]); arrayAddLast(a,strings[2]);
    RUN(filename, fprintf(pfile, "%i - ", arrayGetSize(a));) RUN(filename, arrayPrint(a,pfile);) NL(filename)
    arraySwap(a, 0, 0);
    RUN(filename, fprintf(pfile, "%i - ", arrayGetSize(a));) RUN(filename, arrayPrint(a,pfile);) NL(filename)
    arraySwap(a, 1, 1);
    RUN(filename, fprintf(pfile, "%i - ", arrayGetSize(a));) RUN(filename, arrayPrint(a,pfile);) NL(filename)
    arraySwap(a, 2, 2);
    RUN(filename, fprintf(pfile, "%i - ", arrayGetSize(a));) RUN(filename, arrayPrint(a,pfile);) NL(filename)
    arraySwap(a, 0, 2);
    RUN(filename, fprintf(pfile, "%i - ", arrayGetSize(a));) RUN(filename, arrayPrint(a,pfile);) NL(filename)
    arraySwap(a, 2, 0);
    RUN(filename, fprintf(pfile, "%i - ", arrayGetSize(a));) RUN(filename, arrayPrint(a,pfile);) NL(filename)
    arraySwap(a, 0, 1);
    arraySwap(a, 1, 2);
    RUN(filename, fprintf(pfile, "%i - ", arrayGetSize(a));) RUN(filename, arrayPrint(a,pfile);) NL(filename)
    NL(filename)
    // Agregar de mas
    RUN(filename, fprintf(pfile, "Agregar de mas\n");) NL(filename)
    RUN(filename, fprintf(pfile, "%i - ", arrayGetSize(a));) RUN(filename, arrayPrint(a,pfile);) NL(filename)
    for(int j=0; j<3; j++) {
        for(int i=0; i<10; i++) {
            arrayAddLast(a,strings[i]);
            RUN(filename, fprintf(pfile, "%i - ", arrayGetSize(a));) RUN(filename, arrayPrint(a,pfile);) NL(filename)
        }
    }
    NL(filename)
    // Quitar de mas
    RUN(filename, fprintf(pfile, "Quitar de mas\n");) NL(filename)
    RUN(filename, fprintf(pfile, "%i - ", arrayGetSize(a));) RUN(filename, arrayPrint(a,pfile);) NL(filename)
    for(int j=0; j<3; j++) {
        for(int i=0; i<10; i++) {
            data = arrayRemove(a,i);
            RUN(filename, fprintf(pfile, "%i - ", arrayGetSize(a));) RUN(filename, arrayPrint(a,pfile);) RUN(filename, fprintf(pfile, " - %s\n", (char*)data);)
            strDelete((char*)data);
        }
    }
    data = arrayRemove(a,0);
    RUN(filename, fprintf(pfile, "%i - ", arrayGetSize(a));) RUN(filename, arrayPrint(a,pfile);) RUN(filename, fprintf(pfile, " - %s\n", (char*)data);)
    strDelete((char*)data);
    data = arrayRemove(a,0);
    RUN(filename, fprintf(pfile, "%i - ", arrayGetSize(a));) RUN(filename, arrayPrint(a,pfile);) RUN(filename, fprintf(pfile, " - %s\n", (char*)data);)
    strDelete((char*)data);
    data = arrayRemove(a,10);
    RUN(filename, fprintf(pfile, "%i - ", arrayGetSize(a));) RUN(filename, arrayPrint(a,pfile);) RUN(filename, fprintf(pfile, " - %s\n", (char*)data);)
    strDelete((char*)data);
    NL(filename)
    // Delete
    RUN(filename, fprintf(pfile, "Delete\n");) NL(filename)
    RUN(filename, fprintf(pfile, "%i - ", arrayGetSize(a));) RUN(filename, arrayPrint(a,pfile);) NL(filename)
    for(int i=0; i<10; i++) {
        arrayAddLast(a,strings[i]);
    }
    RUN(filename, fprintf(pfile, "%i - ", arrayGetSize(a));) RUN(filename, arrayPrint(a,pfile);) NL(filename)
    arrayDelete(a);
    NL(filename)
}

void test_list(char* filename) {
    FILE* pfile;
    list_t *a, *b;
    void* data;

    RUN(filename, fprintf(pfile, "== List ==\n");) NL(filename)

    // Agregar
    RUN(filename, fprintf(pfile, "Agregar\n");) NL(filename)
    a = listNew(TypeString);
    RUN(filename, fprintf(pfile, "%i - ", listGetSize(a));) RUN(filename, listPrint(a,pfile);) NL(filename)
    listAddLast(a,strings[0]);
    RUN(filename, fprintf(pfile, "%i - ", listGetSize(a));) RUN(filename, listPrint(a,pfile);) NL(filename)
    listAddLast(a,strings[1]);
    RUN(filename, fprintf(pfile, "%i - ", listGetSize(a));) RUN(filename, listPrint(a,pfile);) NL(filename)
    listAddLast(a,strings[2]);
    RUN(filename, fprintf(pfile, "%i - ", listGetSize(a));) RUN(filename, listPrint(a,pfile);) NL(filename)
    
    b = listNew(TypeInt);
    RUN(filename, fprintf(pfile, "%i - ", listGetSize(b));) RUN(filename, listPrint(b,pfile);) NL(filename)
    
    for(int i = 0; i < 20; i++){
        listAddFirst(b,&i);
        RUN(filename, fprintf(pfile, "%i - ", listGetSize(b));) RUN(filename, listPrint(b,pfile);) NL(filename)
    }
    NL(filename)
    // Get
    RUN(filename, fprintf(pfile, "Get\n");) NL(filename)
    RUN(filename, fprintf(pfile, "%i - ", listGetSize(a));) RUN(filename, listPrint(a,pfile);) NL(filename)
    data = listGet(a,0);
    RUN(filename, fprintf(pfile, "%s\n", (char*)data);)
    data = listGet(a,1);
    RUN(filename, fprintf(pfile, "%s\n", (char*)data);)
    data = listGet(a,2);
    RUN(filename, fprintf(pfile, "%s\n", (char*)data);)
    data = listGet(a,3);
    RUN(filename, fprintf(pfile, "%s\n", (char*)data);)
    NL(filename)

    // Quitar
    RUN(filename, fprintf(pfile, "Quitar\n");) NL(filename)
    data = listRemove(a,0);
    RUN(filename, fprintf(pfile, "%i - ", listGetSize(a));) RUN(filename, listPrint(a,pfile);) RUN(filename, fprintf(pfile, " - %s\n", (char*)data);)
    strDelete((char*)data);
    data = listRemove(a,1);
    RUN(filename, fprintf(pfile, "%i - ", listGetSize(a));) RUN(filename, listPrint(a,pfile);) RUN(filename, fprintf(pfile, " - %s\n", (char*)data);)
    strDelete((char*)data);
    data = listRemove(a,0);
    RUN(filename, fprintf(pfile, "%i - ", listGetSize(a));) RUN(filename, listPrint(a,pfile);) RUN(filename, fprintf(pfile, " - %s\n", (char*)data);)
    strDelete((char*)data);
    NL(filename)
    // Swap
    RUN(filename, fprintf(pfile, "Swap\n");) NL(filename)
    listAddLast(a,strings[0]); listAddLast(a,strings[1]); listAddLast(a,strings[2]);
    RUN(filename, fprintf(pfile, "%i - ", listGetSize(a));) RUN(filename, listPrint(a,pfile);) NL(filename)
    listSwap(a, 0, 0);
    RUN(filename, fprintf(pfile, "%i - ", listGetSize(a));) RUN(filename, listPrint(a,pfile);) NL(filename)
    listSwap(a, 1, 1);
    RUN(filename, fprintf(pfile, "%i - ", listGetSize(a));) RUN(filename, listPrint(a,pfile);) NL(filename)
    listSwap(a, 2, 2);
    RUN(filename, fprintf(pfile, "%i - ", listGetSize(a));) RUN(filename, listPrint(a,pfile);) NL(filename)
    listSwap(a, 0, 2);
    RUN(filename, fprintf(pfile, "%i - ", listGetSize(a));) RUN(filename, listPrint(a,pfile);) NL(filename)
    listSwap(a, 2, 0);
    RUN(filename, fprintf(pfile, "%i - ", listGetSize(a));) RUN(filename, listPrint(a,pfile);) NL(filename)
    listSwap(a, 0, 1);
    listSwap(a, 1, 2);
    RUN(filename, fprintf(pfile, "%i - ", listGetSize(a));) RUN(filename, listPrint(a,pfile);) NL(filename)
    NL(filename)
    // Agregar de mas
    RUN(filename, fprintf(pfile, "Agregar 2\n");) NL(filename)
    RUN(filename, fprintf(pfile, "%i - ", listGetSize(a));) RUN(filename, listPrint(a,pfile);) NL(filename)
    for(int j=0; j<3; j++) {
        for(int i=0; i<10; i++) {
            listAddLast(a,strings[i]);
            RUN(filename, fprintf(pfile, "%i - ", listGetSize(a));) RUN(filename, listPrint(a,pfile);) NL(filename)
        }
    }
    NL(filename)
    // Quitar de mas
    RUN(filename, fprintf(pfile, "Quitar de mas\n");) NL(filename)
    RUN(filename, fprintf(pfile, "%i - ", listGetSize(a));) RUN(filename, listPrint(a,pfile);) NL(filename)
    for(int j=0; j<3; j++) {
        for(int i=0; i<10; i++) {
            data = listRemove(a,i);
            RUN(filename, fprintf(pfile, "%i - ", listGetSize(a));) RUN(filename, listPrint(a,pfile);) RUN(filename, fprintf(pfile, " - %s\n", (char*)data);)
            strDelete((char*)data);
        }
    }
    
    for(int i=0; i<10; i++) {
        data = listRemove(a,0);
        RUN(filename, fprintf(pfile, "%i - ", listGetSize(a));) RUN(filename, listPrint(a,pfile);) RUN(filename, fprintf(pfile, " - %s\n", (char*)data);)
        strDelete((char*)data);
    }
    NL(filename)
    // Delete
    RUN(filename, fprintf(pfile, "Delete\n");) NL(filename)
    RUN(filename, fprintf(pfile, "%i - ", listGetSize(a));) RUN(filename, listPrint(a,pfile);) NL(filename)
    for(int i=0; i<10; i++) {
        listAddLast(a,strings[i]);
    }
    RUN(filename, fprintf(pfile, "%i - ", listGetSize(a));) RUN(filename, listPrint(a,pfile);) NL(filename)
    listDelete(a);
    listDelete(b);
    NL(filename)
}

void test_int(char* filename) {
    FILE* pfile;
    int32_t ia = 13, ib = 0, ic = -20;
    int32_t *a = &ia;
    int32_t *b = &ib;
    int32_t *c = &ic;

    RUN(filename, fprintf(pfile, "== Int ==\n");) NL(filename)

    // Clonar
    RUN(filename, fprintf(pfile, "Clonar\n");) NL(filename)
    int32_t* ac = intClone(a);
    assert(a != ac); assert(*a == *ac);
    RUN(filename, intPrint(ac,pfile);) NL(filename)
    int32_t* bc = intClone(b);
    assert(b != bc); assert(*b == *bc);
    RUN(filename, intPrint(bc,pfile);) NL(filename)
    int32_t* cc = intClone(c);
    assert(c != cc); assert(*c == *cc);
    RUN(filename, intPrint(cc,pfile);) NL(filename)
    NL(filename)

    //Borrar

    intDelete(ac);
    intDelete(bc);
    intDelete(cc);

    //Comparar
    RUN(filename, fprintf(pfile, "Comparar\n");) NL(filename)
    RUN(filename, fprintf(pfile,"intCmp(%d,%d) --> %d\n", *a, *a, intCmp(a,a));)
    RUN(filename, fprintf(pfile,"intCmp(%d,%d) --> %d\n", *b, *b, intCmp(b,b));)
    RUN(filename, fprintf(pfile,"intCmp(%d,%d) --> %d\n", *b, *a, intCmp(b,a));)
    RUN(filename, fprintf(pfile,"intCmp(%d,%d) --> %d\n", *a, *b, intCmp(a,b));)
    RUN(filename, fprintf(pfile,"intCmp(%d,%d) --> %d\n", *c, *a, intCmp(c,a));)
    RUN(filename, fprintf(pfile,"intCmp(%d,%d) --> %d\n", *b, *c, intCmp(b,c));)
    RUN(filename, fprintf(pfile,"intCmp(%d,%d) --> %d\n", *c, *b, intCmp(c,b));)
    NL(filename)

}
	
void test_string(char* filename) {
     FILE* pfile;
     char* a = "Omega 4";
     char* b = "Palaven";
     char* c = "Feros";
     char* n = "";

    RUN(filename, fprintf(pfile, "== String ==\n");) NL(filename)

    // Clonar
    RUN(filename, fprintf(pfile, "Clonar\n");) NL(filename)
    char* ac = strClone(a);
    assert(a != ac); assert(strCmp(a,ac) == 0);
    RUN(filename, fprintf(pfile, "%i - ", strLen(ac));) RUN(filename, strPrint(ac,pfile);) NL(filename)
    char* nc = strClone(n);
    assert(n != nc); assert(strCmp(n,nc) == 0);
    RUN(filename, fprintf(pfile, "%i - ", strLen(nc));) RUN(filename, strPrint(nc,pfile);) NL(filename)
    NL(filename)

    //Borrar
    strDelete(ac);
    strDelete(nc);

    //Comparar
    RUN(filename, fprintf(pfile, "Comparar\n");) NL(filename)
    RUN(filename, fprintf(pfile,"strCmp(%s,%s) --> %d\n", a, a, strCmp(a,a));)
    RUN(filename, fprintf(pfile,"strCmp(%s,%s) --> %d\n", n, n, strCmp(n,n));)
    RUN(filename, fprintf(pfile,"strCmp(%s,%s) --> %d\n", b, a, strCmp(b,a));)
    RUN(filename, fprintf(pfile,"strCmp(%s,%s) --> %d\n", a, b, strCmp(a,b));)
    RUN(filename, fprintf(pfile,"strCmp(%s,%s) --> %d\n", c, a, strCmp(c,a));)
    RUN(filename, fprintf(pfile,"strCmp(%s,%s) --> %d\n", b, c, strCmp(b,c));)
    RUN(filename, fprintf(pfile,"strCmp(%s,%s) --> %d\n", c, b, strCmp(c,b));)
   
    char* texts[9] = {"sar","23","taaa","tbb","tix", "taaab", "taa0", "tbb", ""};
    for(int i=0; i<9; i++) {
        RUN(filename, fprintf(pfile,"strLen(%s) --> %d\n",texts[i],strLen(texts[i])))
        for(int j=0; j<9; j++) {
            RUN(filename, fprintf(pfile,"strCmp(%s,%s) --> %d\n",texts[i],texts[j],strCmp(texts[i],texts[j])))   
        }
    }
    NL(filename)
 }

void test_card(char* filename){
     FILE* pfile;

    RUN(filename, fprintf(pfile, "== Card ==\n");) NL(filename)

    RUN(filename, fprintf(pfile, "Nueva\n");) NL(filename)
    int32_t v = 1;
    int32_t *uno = &v;
    card_t *ancho = cardNew("espada",uno);
    assert(intCmp(cardGetNumber(ancho),uno) == 0);
    assert(strCmp(cardGetSuit(ancho),"espada") == 0);
    assert(listGetSize(cardGetStacked(ancho)) == 0);
    
    RUN(filename, cardPrint(ancho,pfile);) NL(filename)
    RUN(filename, fprintf(pfile, "Comparar\n");) NL(filename)

    card_t *clone = cardClone(ancho);
    assert(cardCmp(clone, ancho) == 0);
    cardDelete(ancho);
    cardDelete(clone);

    char *suits[] = {"espada", "basto", "copa", "oro"};
    array_t *mazoArray = arrayNew(TypeCard, 48);
    list_t *mazoList = listNew(TypeCard);

    for (int i=0; i<4; ++i){
        for(int32_t j=1; j<13; ++j){
            card_t *carta = cardNew(suits[i], &j);
            arrayAddLast(mazoArray, (void*) carta);
            listAddLast(mazoList, (void*) carta);
            RUN(filename, arrayPrint(mazoArray,pfile);) NL(filename)
            RUN(filename, listPrint(mazoList,pfile);) NL(filename)
            cardDelete(carta);
        }
    }
    NL(filename)

    for (uint8_t i=0; i<48; ++i){
        card_t *cardi = (card_t*) arrayGet(mazoArray, i);
        for(uint8_t j=0; j<48; ++j){
            card_t *cardj = (card_t*) arrayGet(mazoArray, j);
            RUN(filename, cardPrint(cardi, pfile);)
            RUN(filename, fprintf(pfile, " cmp "))
            RUN(filename, cardPrint(cardj, pfile);)
            RUN(filename, fprintf(pfile, " -> %d", cardCmp(cardi, cardj));) NL(filename)            
        }
    }

    NL(filename)

    RUN(filename, fprintf(pfile, "Apilando\n");) NL(filename)

    card_t *nuevoancho = cardNew("espada",uno);

    for(int32_t j=1; j<13; ++j){
        card_t *carta = arrayRemove(mazoArray, j);
        cardAddStacked(nuevoancho, carta);
        RUN(filename, cardPrint(nuevoancho,pfile);) NL(filename)
        cardDelete(carta);
    }

    arrayDelete(mazoArray);
    NL(filename)

    for(int32_t j=13; j<26; ++j){
        card_t *carta = listRemove(mazoList, j);
        cardAddStacked(nuevoancho, carta);
        RUN(filename, cardPrint(nuevoancho,pfile);) NL(filename)
        cardDelete(carta);
    }

    listDelete(mazoList);

    list_t *stacked = cardGetStacked(nuevoancho);
    RUN(filename, cardPrint(nuevoancho,pfile);) NL(filename)

    while(listGetSize(stacked) != 0){

        card_t *carta = listRemove(stacked, 0);
        RUN(filename, cardPrint(carta,pfile);) RUN(filename, cardPrint(nuevoancho,pfile);) NL(filename)
        cardDelete(carta);
    }
    RUN(filename, cardPrint(nuevoancho,pfile);) NL(filename)
    cardDelete(nuevoancho);
    NL(filename)
    
}

#define TESTING
#include "game.c"

void test_game(char* filename) {
    FILE* pfile;
    array_t* a;
    void* data;

    game_t* game;
    
    for(int i=0; i<10; i++) {
        RUN(filename, fprintf(pfile, "== Game Array ==\n");) NL(filename)
        game = buildGameWithArray(i);
        RUN(filename, playGame(game, pfile);) NL(filename)
        gameDelete(game);

        RUN(filename, fprintf(pfile, "== Game List ==\n");) NL(filename)    
        game = buildGameWithList(i);
        RUN(filename, playGame(game, pfile);) NL(filename)
        gameDelete(game);
    }
}

