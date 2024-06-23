#include<stdio.h>
#include<malloc.h>
#pragma warning(disable:4996)
typedef struct Programare Programare;

struct Programare {
	char* cod;
	int zi;
	int luna;
	int an;
	int ora;
	char* nume_solicitant;
	int termenEliberare;
	char* locatie;
	int durata;
};
typedef struct NodLdi NodLdi;
 struct NodLdi {
	Programare info;
	NodLdi* next;
	NodLdi* prev;
};
 typedef struct ListaDubla ListaDubla;

 struct ListaDubla {
	 NodLdi* first;
	 NodLdi* last;
 };

 Programare citireProgramareDinFisier(FILE* fisier ) {
	 Programare programare;
	 char aux[100];
	 fscanf(fisier, "%s", &aux);
	 programare.cod = malloc(sizeof(char) * (strlen(aux) + 1));
	 strcpy(programare.cod, aux);
	 fscanf(fisier, "%d", &programare.zi);
	 fscanf(fisier, "%d", &programare.luna);
	 fscanf(fisier, "%d", &programare.an);
	 fscanf(fisier, "%d", &programare.ora);
	 char aux2[100];
	 fscanf(fisier, "%s", &aux2);
	 programare.nume_solicitant = malloc(sizeof(char) * (strlen(aux2) + 1));
	 strcpy(programare.nume_solicitant, aux2);
	 fscanf(fisier, "%d", &programare.termenEliberare);
	 char aux1[100];
	 fscanf(fisier, "%s", &aux1);
	 programare.locatie = malloc(sizeof(char) * (strlen(aux) + 1));
	 strcpy(programare.locatie, aux1);
	 fscanf(fisier, "%d", &programare.durata);

	 return programare;
 }
 Programare initializareProgram(const char* cod,
	 int zi,
	 int luna,
	 int an,
	 int ora,
	 const char* nume_solicitant,
	 int termenEliberare,
	 const char* locatie,
	 int durata) {
	 Programare p;
	 p.an = an;
	 p.luna = luna;
	 p.zi = zi;
	 p.durata = durata;
	 p.termenEliberare = termenEliberare;
	 p.ora = ora;
	 p.cod = malloc(sizeof(char) * (strlen(cod) + 1));
	 strcpy(p.cod, cod);
	 p.nume_solicitant = malloc(sizeof(char) * (strlen(nume_solicitant) + 1));
	 strpcy(p.nume_solicitant, nume_solicitant);
	 p.locatie = malloc(sizeof(char) * (strlen(locatie) + 1));
	 strpcy(p.locatie, locatie);

	 return p;
 }
 Programare copiereProgramare(Programare p) {
	 return initializareProgram(p.cod, p.zi, p.luna, p.an, p.ora, p.nume_solicitant, p.termenEliberare, p.locatie, p.durata);
 }
 void inserareLaInceput(ListaDubla* ld, Programare p) {
	 NodLdi* nod;
	 nod = malloc(sizeof(NodLdi));
	 nod->info = copiereProgramare(p);
	 nod->prev = NULL;
	 nod->next = ld->first;
	 if (ld->first) {
		 ld->first->prev = nod;
		 ld->first = nod;
	 }
	 else
	 {
		 ld->first = nod;
		 ld->last = nod;
	 }
 }
 void inserareLaFinal(ListaDubla* ld, Programare p) {
	 NodLdi* nod;
	 nod = malloc(sizeof(NodLdi));
	 nod->info = copiereProgramare(p);
	 nod->next = NULL;
	 nod->prev = ld->last;
	 if (ld->last) {
		 ld->last->next = nod;
		 ld->last = nod;
	 }
	 else
	 {
		 ld->first = nod;
		 ld->last = nod;
	 }
 }

 int nrMaximDeProgramari(ListaDubla* ld) {
	 int nrProgramari[12];
	 for (int i = 0; i < 12; i++) {
		 nrProgramari[i] = 0;
	 }
	 NodLdi* aux = ld->first;
	 while (aux != NULL) {
		 nrProgramari[aux->info.luna]++;
		 aux = aux->next;
	 }
	 int max = 0;
	 for (int i = 0; i < 12; i++) {
		 if (max < nrProgramari[i])
			 max = nrProgramari[i];
	 }
	 return max;
 }

 void main()
 {

 }