#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<malloc.h>
typedef struct Proiect Proiect;
struct Proiect {
	unsigned int codProiect;
	char* titluProiect;
	char* beneficiar;
	unsigned char nrExecutanti;
	float BugetAlocat;
};
Proiect citireStructuraDinFisier(FILE* f) {
	Proiect p;
	fscanf(f, "%u", &p.codProiect);
	char buffer1[100];
	fscanf(f, "%s", &buffer1);
	p.titluProiect = malloc(sizeof(char*) * strlen(buffer1 + 1));
	strcpy(p.titluProiect, buffer1);
	char bugger2[100];
	fscanf(f, "%s", &bugger2);
	p.beneficiar = malloc(sizeof(char*) * strlen(bugger2 + 1));
	strcpy(p.beneficiar, bugger2);
	fscanf(f, "%hhu", &p.nrExecutanti);
	fscanf(f, "%f", &p.BugetAlocat);
	return p;
}
Proiect* citireVectorDeProiecte(FILE* f, int* nr) {
	fscanf(f, "%d", nr);
	struct Proiect* proiecte;
	proiecte = malloc(sizeof(Proiect) * (*nr));
	for (int i = 0; i < (*nr); i++) {
		proiecte[i] = citireStructuraDinFisier(f);
	}
	return proiecte;
}
void AifsareProiect(Proiect p) {
	printf("De proiectul %u beneficiaza %s.\n", p.codProiect, p.beneficiar);
}
typedef struct NodTabela NodTabela;
struct NodTabela {
	Proiect info;
	NodTabela* next;
};
void inserareLaFinal(NodTabela** lista,Proiect p) {
	NodTabela* nod = malloc(sizeof(NodTabela));
	nod->info = p;
	nod->next = NULL;
	if (*lista != NULL) {
		NodTabela* aux = (*lista);
		while (aux->next) {
			aux = aux->next;
		}
		aux->next = nod;
	}
	else
	{
		(*lista) = nod;
	}
}
void stergereLista(NodTabela** lista) {
	NodTabela* aux = *lista;
	while (aux) {
		free(aux->info.beneficiar);
		free(aux->info.titluProiect);
		NodTabela* temp = aux->next;
		free(aux);
		aux = temp;
	}
	*lista = NULL;
}
void afisareLista(NodTabela* lista) {
	NodTabela* aux = lista;
	while (aux) {
		AifsareProiect(aux->info);
		aux = aux->next;
	}
}
typedef struct HashTable HashTable;
struct HashTable {
	NodTabela** noduri;
	int dimensiune;
};
unsigned int codHash(const char* beneficiar, int dimensiune) {
	int suma = 0;
	for (int i = 0; i < strlen(beneficiar); i++)
		suma = suma + beneficiar[i];
	suma = suma % dimensiune;
	return suma;
}
struct HashTable initializareHashTable(int dimensiune) {
	struct HashTable ht;
	ht.dimensiune = dimensiune;
	ht.noduri = malloc(sizeof(NodTabela*) * dimensiune);
	for (int i = 0; i < dimensiune; i++) {
		ht.noduri[i] = NULL;
	}
	return ht;
}
void inserareinHashTable(HashTable hashtable,Proiect proiect) {
	unsigned int hashcode = codHash(proiect.beneficiar, hashtable.dimensiune);
	if (hashtable.noduri[hashcode] == NULL) {
		inserareLaFinal(&hashtable.noduri[hashcode], proiect);
	}
	else
	{
		inserareLaFinal(&hashtable.noduri[hashcode], proiect);
	}
}
void afisareElementeHashTable(HashTable hashtable) {
	for (int i = 0; i < hashtable.dimensiune; i++) {
		NodTabela* aux = hashtable.noduri[i];
		while (aux) {
			AifsareProiect(aux->info);
			aux = aux->next;
		}
	}
}
float TotalInvestitii(HashTable hashtable, const char* beneficiar) {
	float valoare = 0;
	for (int i = 0; i < hashtable.dimensiune; i++) {
		NodTabela* aux = hashtable.noduri[i];
		while (aux)
		{
			if (strcmp(aux->info.beneficiar, beneficiar) == 0)
				valoare = valoare + aux->info.BugetAlocat;
			aux = aux->next;
		}
	}
	return valoare;
}
void schimbaNumele(HashTable* hashtable, const char* beneficiarVechi, const char* beneficiarNou) {
	for (int i = 0; i < (*hashtable).dimensiune; i++) {
		NodTabela* aux = (*hashtable).noduri[i];
		while (aux)
		{
			if (strcmp(aux->info.beneficiar, beneficiarVechi) == 0)
			{
				free(aux->info.beneficiar);
				aux->info.beneficiar = malloc(sizeof(char*) * strlen(beneficiarNou + 1));
				strcpy(aux->info.beneficiar, beneficiarNou);
			}
			aux = aux->next;
		}
	}
}
NodTabela* proiectePreaScumpe(HashTable hashtable, float prag) {
	NodTabela* lista = malloc(sizeof(NodTabela));
	lista = NULL;
	for (int i = 0; i < hashtable.dimensiune; i++) {
		NodTabela* aux = hashtable.noduri[i];
		while (aux) {
			if (aux->info.BugetAlocat > prag)
				inserareLaFinal(&lista, aux->info);
			aux = aux->next;
		}
	}
	return lista;
}
void stergeretabela(HashTable* ht) {
	for (int i = 0; i < ht->dimensiune; i++) {
		stergereLista(&ht->noduri[i]);

	}
	ht->dimensiune = 0;
	ht = NULL;
	free(ht);
	
	
}
void main() {
	FILE* fisier;
	fisier = fopen("Proiecte.txt", "r");
	int nr = 0;
	Proiect* vectorProiecte = citireVectorDeProiecte(fisier,&nr );
	HashTable tabelaDispersie = initializareHashTable(nr);
	for (int i = 0; i < nr; i++) {
		inserareinHashTable(tabelaDispersie, vectorProiecte[i]);
	}
	afisareElementeHashTable(tabelaDispersie);
	printf("\n");
	float suma = TotalInvestitii(tabelaDispersie, "Ionescu");
	printf("%5.2f", suma);
	printf("\nAfisarea tabelei de dispersie dupa schimbare de nume:\n");
	schimbaNumele(&tabelaDispersie, "Ciucea", "Pavelescu");
	afisareElementeHashTable(tabelaDispersie);
	printf("\nLista cu proiecte care depasesc pragul:\n");
	NodTabela* lista = proiectePreaScumpe(tabelaDispersie, 900);
	afisareLista(lista);
	stergeretabela(&tabelaDispersie);
}