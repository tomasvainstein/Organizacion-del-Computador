#include <stdlib.h>

typedef struct contador contador_t;

contador_t* crearContador(void);

void destruirContador(contador_t* contador);

void incLoads(contador_t* contador);

void incStores(contador_t* contador);

void incMissR(contador_t* contador);

void incMissW(contador_t* contador);

void incDirtyMissW(contador_t* contador);

void incDirtyMissR(contador_t* contador);

void incBytesRead(contador_t* contador, int bytes);

void incBytesWritten(contador_t* contador, int bytes);

void incAccessTimeR(contador_t* contador, int time);

void incAccessTimeW(contador_t* contador, int time);

int getLoads(contador_t* contador);

int getStores(contador_t* contador);

int getTotalAccesses(contador_t* contador);

int getMissR(contador_t* contador);

int getMissW(contador_t* contador);

int getTotalMisses(contador_t* contador);

int getDirtyMissW(contador_t* contador);

int getDirtyMissR(contador_t* contador);

int getBytesRead(contador_t* contador);

int getBytesWritten(contador_t* contador);

int getAccessTimeR(contador_t* contador);

int getAccessTimeW(contador_t* contador);

float getTotalMissRate(contador_t* contador);