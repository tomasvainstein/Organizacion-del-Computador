#include <stdlib.h>
#include "contador.h"
#include <stdio.h>

struct contador {
	int loads;
	int stores;
	int missR;
	int missW;
	int dirtyMissR;
	int dirtyMissW;
	int bytesRead;
	int bytesWritten;
	int accessTimeR;
	int accessTimeW;
};

contador_t* crearContador(void) {
	contador_t* contador = malloc(sizeof(contador_t));
	if (contador == NULL) return NULL;

	contador->loads = 0;
	contador->stores = 0;
	contador->missR = 0;
	contador->missW = 0;
	contador->dirtyMissR = 0;
	contador->dirtyMissW = 0;
	contador->bytesRead = 0;
	contador->bytesWritten = 0;
	contador->accessTimeR = 0;
	contador->accessTimeW = 0;

	return contador;
}

void destruirContador(contador_t* contador) {
	free(contador);
}

void incLoads(contador_t* contador) {
	contador->loads++;
}

void incStores(contador_t* contador) {
	contador->stores++;
}

void incMissR(contador_t* contador) {
	contador->missR++;
}

void incMissW(contador_t* contador) {
	contador->missW++;
}

void incDirtyMissW(contador_t* contador) {
	contador->dirtyMissW++;
}

void incDirtyMissR(contador_t* contador) {
	contador->dirtyMissR++;
}

void incBytesRead(contador_t* contador, int bytes) {
	contador->bytesRead += bytes;
}

void incBytesWritten(contador_t* contador, int bytes) {
	contador->bytesWritten += bytes;
}

void incAccessTimeR(contador_t* contador, int time) {
	contador->accessTimeR += time;
}

void incAccessTimeW(contador_t* contador, int time) {
	contador->accessTimeW += time;
}

int getLoads(contador_t* contador) {
	return contador->loads;
}

int getStores(contador_t* contador) {
	return contador->stores;
}

int getTotalAccesses(contador_t* contador) {
	return contador->loads + contador->stores;
}

int getMissR(contador_t* contador) {
	return contador->missR;
}

int getMissW(contador_t* contador) {
	return contador->missW;
}

int getTotalMisses(contador_t* contador) {
	return contador->missR + contador->missW;
}

int getDirtyMissW(contador_t* contador) {
	return contador->dirtyMissW;
}

int getDirtyMissR(contador_t* contador) {
	return contador->dirtyMissR;
}

int getBytesRead(contador_t* contador) {
	return contador->bytesRead;
}

int getBytesWritten(contador_t* contador) {
	return contador->bytesWritten;
}

int getAccessTimeR(contador_t* contador) {
	return contador->accessTimeR;
}

int getAccessTimeW(contador_t* contador) {
	return contador->accessTimeW;
}

float getTotalMissRate(contador_t* contador) {
	float totalMisses = getTotalMisses(contador);
	float totalAccesses = getTotalAccesses(contador);

	return totalMisses / totalAccesses;
}