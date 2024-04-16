#include "lib.h"

funcCmp_t* getCompareFunction(type_t t) {
    switch (t) {
        case TypeInt:      return (funcCmp_t*)&intCmp; break;
        case TypeString:   return (funcCmp_t*)&strCmp; break;
        case TypeCard:     return (funcCmp_t*)&cardCmp; break;
        default: break;
    }
    return 0;
}
funcClone_t* getCloneFunction(type_t t) {
    switch (t) {
        case TypeInt:      return (funcClone_t*)&intClone; break;
        case TypeString:   return (funcClone_t*)&strClone; break;
        case TypeCard:     return (funcClone_t*)&cardClone; break;
        default: break;
    }
    return 0;
}
funcDelete_t* getDeleteFunction(type_t t) {
    switch (t) {
        case TypeInt:      return (funcDelete_t*)&intDelete; break;
        case TypeString:   return (funcDelete_t*)&strDelete; break;
        case TypeCard:     return (funcDelete_t*)&cardDelete; break;
        default: break;
    }
    return 0;
}
funcPrint_t* getPrintFunction(type_t t) {
    switch (t) {
        case TypeInt:      return (funcPrint_t*)&intPrint; break;
        case TypeString:   return (funcPrint_t*)&strPrint; break;
        case TypeCard:     return (funcPrint_t*)&cardPrint; break;
        default: break;
    }
    return 0;
}


/** Int **/

int32_t intCmp(int32_t* a, int32_t* b){
    if(!a || !b){
        return 0;
    }
    if(*a == *b)
        return 0;
    else if(*a < *b)
        return 1;
    else
        return -1;
}

void intDelete(int32_t* a){
    if(!a){
        return;
    }
    free(a);
}


void intPrint(int32_t* a, FILE* pFile){
    fprintf(pFile, "%d", *a);
}

/* String */

char* strClone(char* a) {
    if (a == NULL)
        return NULL;
    
    uint32_t len = strLen(a);
    char* copia = malloc(len+1);

    if (!copia)
        return NULL;
    
    for (uint32_t i = 0; i < len; i++)
        copia[i] = a[i];
    
    copia[len] = '\0';
    return copia;
}

void strPrint(char* a, FILE* pFile) {
    if (!(*a))
        fprintf(pFile, "%s", "NULL");
    fprintf(pFile, "%s", a);
}


/* Card */

card_t* cardNew(char* suit, int32_t* number) {
    card_t* card = malloc(sizeof(card_t));
    if (!card)
        return NULL;

    card->suit = strClone(suit);
    if (!card->suit) {
        free(card);
        return NULL;
    }

    card->number = intClone(number);
    if (!card->number) {
        free(card->suit);
        free(card);
        return NULL;
    }

    card->stacked = listNew(3);
    if (!card->stacked) {
        free(card->number);
        free(card->suit);
        free(card);
        return NULL;
    }
    return card;
}

char* cardGetSuit(card_t* c) {
    if(!c)
        return NULL;
    return c->suit;
}

int32_t* cardGetNumber(card_t* c) {
    if(!c)
        return NULL;
    return c->number;
}

list_t* cardGetStacked(card_t* c) {
    if(!c)
        return NULL;
    return c->stacked;
}


void cardPrint(card_t* c, FILE* pFile) {
    fprintf(pFile, "{%s-%d-", c->suit, *(c->number));
    listPrint(c->stacked, pFile);
    fprintf(pFile, "}");
}


/* Array */

array_t* arrayNew(type_t t, uint8_t capacity) {
    array_t* arr = malloc(sizeof(array_t)); // Solicitar memoria para la estructura del array
    if (!arr)
        return NULL;
    
    arr->type = t;
    arr->size = 0;
    arr->capacity = capacity;
    arr->data = malloc(capacity * sizeof(void*)); // Solicitar memoria para el almacenamiento de datos
    if(!(arr->data)){
        free(arr);
        return NULL;
    }
        
    return arr;
}

void* arrayRemove(array_t* a, uint8_t i) {
    if (!a)
        return NULL;

    // Si la posición indicada está fuera del rango, retornar 0
    if (i >= a->size) 
        return 0;

    // Puntero al elemento que se va a eliminar
    void* removed_elem = a->data[i];
    //void* data = a->data[i];
    //free(data);

    // Mover todos los elementos siguientes una posición hacia la izquierda
    for (int n = i; n < (a->size - 1); n++) {
        a->data[n] = a->data[n+1];
    }
    // Actualizar el tamaño del arreglo
    a->size--;

    return removed_elem;
}

void  arraySwap(array_t* a, uint8_t i, uint8_t j) {
    if (!a)
        return;
    // Verifica si los índices i y j están dentro de los límites del arreglo
    if (i >= a->size || j >= a->size) {
        return;
    }
    // Intercambia los valores de los elementos i y j
    void* temp = a->data[i];
    a->data[i] = a->data[j];
    a->data[j] = temp;
}

void arrayDelete(array_t* arr) {
    if (!arr)
        return;

    funcDelete_t* deleteFunc = getDeleteFunction(arr->type);

    for (uint8_t i = 0; i < arr->size; i++) {
        if (deleteFunc != NULL && arr->data[i] != NULL) {
            deleteFunc(arr->data[i]); // Llamar a la función de eliminación correspondiente
        }
    }

    free(arr->data); // Liberar la memoria del arreglo de datos
    free(arr); // Liberar la memoria de la estructura del array
}


void  arrayPrint(array_t* a, FILE* pFile) {
    if (!a)
        return;

    fprintf(pFile, "[");
    for (uint8_t i = 0; i < (a->size); i++) {
        if (i > 0) {
            fprintf(pFile, ",");
        }
        switch (a->type)
        {
        case TypeInt:
            intPrint(a->data[i], pFile);
            break;
        case TypeString:
            strPrint(a->data[i], pFile);
            break;
        case TypeCard:
            cardPrint(a->data[i], pFile);
            break;
        
        default:
            break;
        }
    }
    fprintf(pFile, "]");
}

/* Lista */

listElem_t* newListElement(void* data, type_t type)
{
    listElem_t* newElement = malloc(sizeof(listElem_t));

    if (!newElement)
        return NULL;

    funcClone_t* cloneFunc = getCloneFunction(type);

    newElement->data = cloneFunc(data);
    newElement->next = NULL;
    newElement->prev = NULL;

    return newElement;
}

bool emptyList(list_t* l)
{
    return (!l || l->size == 0);
}

void listAddLast(list_t* l, void* data)
{
    if (!l)
        return;

    listElem_t* insertedElement = newListElement(data, l->type);

    if (!insertedElement)
        return;

    insertedElement->prev = l->last;
    if (emptyList(l))
        l->first = insertedElement;
    else
        l->last->next= insertedElement;
    l->last = insertedElement;

    l->size++;
}

void* listRemove(list_t* l, uint8_t i){
    if (!l || emptyList(l))
		return 0;

	if (l->size <= i)
		return 0;	 
	
	listElem_t *auxListElem = l->first;
	void *data = l->first->data;
	
	if (i == 0) {
		l->first = l->first->next;
        if (l->first)
            l->first->prev = NULL;
        else
            l->last = NULL;
		free(auxListElem);
	} else {
		for (uint8_t p = 0; p < i-1; p++)
			auxListElem = auxListElem->next;

		listElem_t *removedElement = auxListElem->next;
		auxListElem->next = removedElement->next;
        if (auxListElem->next)
            auxListElem->next->prev = auxListElem;
        else
            l->last = auxListElem;
		data = removedElement->data;

		free(removedElement);
	}
	l->size--;
	return data;
}

void  listSwap(list_t* l, uint8_t i, uint8_t j) {
    if (l->size <= i || l->size <= j)
		return;	
    
    listElem_t *iListElem = l->first;
    listElem_t *jListElem = l->first;

    for (uint8_t p = 0; p < i; p++)
        iListElem = iListElem->next;
    for (uint8_t p = 0; p < j; p++)
        jListElem = jListElem->next;

    void *auxData = iListElem->data;
    iListElem->data = jListElem->data;
    jListElem->data = auxData;
}

list_t* listClone(list_t* l) {
    if (!l)
		return NULL;

    list_t *newList = listNew(l->type);

	if (!newList)
		return NULL;

    listElem_t *auxListElem = l->first;

    while(auxListElem) {
        listAddLast(newList,auxListElem->data);

        auxListElem = auxListElem->next;
    }

	return newList;
}

void listDelete(list_t* l) {
    if (!l)
        return;

    funcDelete_t* deleteFunc = getDeleteFunction(l->type);

    while (l->first) {
        listElem_t* auxListElem = l->first->next;

        if (deleteFunc && l->first->data)
            deleteFunc(l->first->data);

        free(l->first);
        l->first = auxListElem;
        l->size--;
    }
    free(l);
}


void listPrint(list_t* l, FILE* pFile) {
    fprintf(pFile, "[");
    
    listElem_t *auxListElem = l->first;
    funcPrint_t* printFunction = getPrintFunction(l->type);
    
    for (uint8_t i = 0; i < l->size; i++) {
        if (i > 0) 
            fprintf(pFile, ",");
    
        printFunction(auxListElem->data, pFile);
        auxListElem = auxListElem->next;
    }
    fprintf(pFile, "]");
}

/* Game */

game_t* gameNew(void* cardDeck, funcGet_t* funcGet, funcRemove_t* funcRemove, funcSize_t* funcSize, funcPrint_t* funcPrint, funcDelete_t* funcDelete) {
    game_t* game = (game_t*)malloc(sizeof(game_t));
    game->cardDeck = cardDeck;
    game->funcGet = funcGet;
    game->funcRemove = funcRemove;
    game->funcSize = funcSize;
    game->funcPrint = funcPrint;
    game->funcDelete = funcDelete;
    return game;
}
int gamePlayStep(game_t* g) {
    int applied = 0;
    uint8_t i = 0;
    while(applied == 0 && i+2 < g->funcSize(g->cardDeck)) {
        card_t* a = g->funcGet(g->cardDeck,i);
        card_t* b = g->funcGet(g->cardDeck,i+1);
        card_t* c = g->funcGet(g->cardDeck,i+2);
        if( strCmp(cardGetSuit(a), cardGetSuit(c)) == 0 || intCmp(cardGetNumber(a), cardGetNumber(c)) == 0 ) {
            card_t* removed = g->funcRemove(g->cardDeck,i);
            cardAddStacked(b,removed);
            cardDelete(removed);
            applied = 1;
        }
        i++;
    }
    return applied;
}
uint8_t gameGetCardDeckSize(game_t* g) {
    return g->funcSize(g->cardDeck);
}
void gameDelete(game_t* g) {
    g->funcDelete(g->cardDeck);
    free(g);
}
void gamePrint(game_t* g, FILE* pFile) {
    g->funcPrint(g->cardDeck, pFile);
}
