#include<stdio.h>
#include<malloc.h>
#pragma warning(disable:4996)
typedef struct ContBancar ContBancar;
struct ContBancar {
	char* nume_client;
	float sold;
	char* valuta_cont;
	char sex;
	int codBancar;
};
ContBancar citireDateDinFisier(FILE* f) {
	ContBancar cb;
	char buffer1[100];
	fscanf(f, "%s", buffer1);
	cb.nume_client = malloc(sizeof(char*) * (strlen(buffer1 + 1)));
	strcpy(cb.nume_client, buffer1);
	fscanf(f, "%f", &cb.sold);
	char buffer2[100];
	fscanf(f, "%s", buffer2);
	cb.valuta_cont = malloc(sizeof(char*) * (strlen(buffer2 + 1)));
	strcpy(cb.valuta_cont, buffer2);
	int var;
	fscanf(f, "%d", &var);
	if (var == 0)
	{
		cb.sex = 'M';
	}
	else
		if (var == 1)
		{
			cb.sex = 'F';
		}
		else
			cb.sex = 'N';
	fscanf(f, "%d", &cb.codBancar);
	return cb;
}
ContBancar* citireVectorConturi(FILE* f, int* nr) {
	fscanf(f, "%d", nr);
	ContBancar* conturi = malloc(sizeof(ContBancar) * (*nr));
	for (int i = 0; i < (*nr); i++) {
		conturi[i] = citireDateDinFisier(f);
	}
	return conturi;
}
ContBancar copiereCont(ContBancar cb) {
	ContBancar copie;
	copie.codBancar = cb.codBancar;
	copie.nume_client = malloc(sizeof(char) * (strlen(cb.nume_client)+1));
	strcpy(copie.nume_client, cb.nume_client);
	copie.sex = cb.sex;
	copie.sold = cb.sold;
	copie.valuta_cont = malloc(sizeof(char) * (strlen(cb.valuta_cont)+1));
	strcpy(copie.valuta_cont, cb.valuta_cont);
	return copie;
}
typedef struct Nod Nod;
struct Nod {
	ContBancar info;
	Nod* next;
};

Nod* inserareLaInceputLista(Nod* lista,ContBancar cont) {
	
	Nod* aux = malloc(sizeof(Nod));
	aux->info = copiereCont(cont);
	aux->next = lista;
	lista= aux;
	return aux;
}
void stergereLista(Nod* lista) {
	Nod* nod = lista;
	while (nod) {
		Nod* aux = nod->next;
		free(nod->info.nume_client);
		free(nod->info.valuta_cont);
		free(nod);
		nod = aux;
	}
	
}
typedef struct HashTable HashTable;
struct HashTable {
	int dimensiune;
	Nod** elemente;
};
int codHash(int dimensiune, const char* nume) {
	int cod = 0;
	for (int i = 0; i < strlen(nume); i++)
		cod = cod + nume[i];
	cod = cod % dimensiune;
	return cod;
}
HashTable creazaTabela(int dimensiune) {
	HashTable ht;
	ht.dimensiune = dimensiune;
	ht.elemente = malloc(sizeof(Nod) * dimensiune);
	for (int i = 0; i < dimensiune; i++) {
		ht.elemente[i] = NULL;
	}
	return ht;
}
HashTable insereazaElementeinHashTable(HashTable hashtable, ContBancar cb) {
	int cod = codHash(hashtable.dimensiune, cb.nume_client);
	if (hashtable.elemente[cod] == NULL) {
		hashtable.elemente[cod] = malloc(sizeof(Nod));
		hashtable.elemente[cod]->info = copiereCont(cb);
		hashtable.elemente[cod]->next = NULL;

	}
	else
	{
	 hashtable.elemente[cod]=inserareLaInceputLista(hashtable.elemente[cod], cb);
	}
	return hashtable;
}
void afisareContBancar(ContBancar cb) {
	printf("Nume:%s|| Sold:%5.2f|| Valuta:%s|| Sex:%c|| Cod:%d\n", cb.nume_client, cb.sold, cb.valuta_cont, cb.sex, cb.codBancar);
}
void afisareTabelaConturi(HashTable hashtable) {
	for (int i = 0; i < hashtable.dimensiune; i++) {
		Nod* aux = hashtable.elemente[i];
		while (aux != NULL) {
			afisareContBancar(aux->info);
			aux = aux->next;
		}

	}
}
int getNrElemente(HashTable hashtable, const char* value) {
	int nrElemente = 0;
	for (int i = 0; i < hashtable.dimensiune; i++) {
		Nod* aux = hashtable.elemente[i];
		while (aux) {
			if (strcmp(aux->info.valuta_cont, value) == 0)
			{
				nrElemente++;
			}
			aux = aux->next;
		}
	}
	return nrElemente;
}
ContBancar* conturiCuAceeasiValuta(const char* valuta,HashTable hashtable,int *nrElemente) {
	
	(*nrElemente) = getNrElemente(hashtable,valuta);
	int index = 0;
	ContBancar* c = malloc(sizeof(ContBancar) * (*nrElemente));
	for (int i = 0; i < hashtable.dimensiune; i++) {
		Nod* aux = hashtable.elemente[i];
		while (aux) {
			if (strcmp(aux->info.valuta_cont, valuta) == 0)
			{
				c[(index)] = copiereCont(aux->info);
				index++;
			}
			aux = aux->next;
		}
	}
	
	return c;
}
int* getClustere(HashTable hashtable, int* numClusters) {
	int* ClusterSizes = malloc(sizeof(int) * hashtable.dimensiune);
	int clusterIndex = 0;
	for (int i = 0; i < hashtable.dimensiune; i++) {
		Nod* aux = hashtable.elemente[i];
		int clusterSize=0;
		while (aux) {
			clusterSize++;
			aux = aux->next;
		}
		if (clusterSize > 1) {
			ClusterSizes[clusterIndex] = clusterSize;
			clusterIndex++;
		}
		else
		{
			ClusterSizes[clusterIndex] =0;
			clusterIndex++;
		}
	}
	*numClusters = clusterIndex;
	return ClusterSizes;
}
struct PerechiConturi {
	char* nume_client;
	float soldFinal;
};
struct PerechiConturi copiere(struct PerechiConturi pc) {

	struct PerechiConturi copie;
	copie.soldFinal = pc.soldFinal;
	copie.nume_client = malloc(sizeof(char) * (strlen(pc.nume_client)+1));
	strcpy(copie.nume_client, pc.nume_client);
	return copie;

}
struct PerechiConturi* push_Perechi(struct PerechiConturi* pc, ContBancar cb, int* nrElemente) {
	(*nrElemente)++;
	struct PerechiConturi* copie = malloc(sizeof(struct PerechiConturi) * (*nrElemente));
	for (int i = 0; i < (*nrElemente) - 1; i++)
		copie[i] =copiere(pc[i]);
	copie[(*nrElemente) - 1].nume_client = malloc(sizeof(char) * (strlen(cb.nume_client) + 1));
	strcpy(copie[(*nrElemente) - 1].nume_client, cb.nume_client);
	copie[(*nrElemente) - 1].soldFinal = cb.sold;
	if (pc != NULL){
		for (int i = 0; i < (*nrElemente)-1; i++)
			free(pc[i].nume_client);
}
	free(pc);
	return copie;

}
struct PerechiConturi* vectorDePerechi(ContBancar* cb,int nrEl, int *n) {
	int nrElemente = 0;
	struct PerechiConturi* pc=NULL;
	pc=push_Perechi(pc, cb[0], &nrElemente);
	(*n)++;
	for (int i = 1; i < nrEl; i++) {
		int ok = 1;
		for (int j = 0; j < nrElemente; j++) {
			if (strcmp(cb[i].nume_client, pc[j].nume_client) == 0)
			{
				pc[j].soldFinal += cb[i].sold;
				ok = 0;
			}
		}
		if (ok == 1) {
			pc=push_Perechi(pc, cb[i], &nrElemente);
			(*n)++;
		}
	}
	
	return pc;
}
void StergereTabela(HashTable *hashtable) {
	for (int i = 0; i < hashtable[i].dimensiune; i++)
	{
		Nod* aux = hashtable[i].elemente;
		stergereLista(aux);
	}
}
void main() {
	FILE* fisier;
	fisier = fopen("Conturi.txt", "r");
	int nr = 0;
	ContBancar* conturi = citireVectorConturi(fisier, &nr);
	HashTable TabelaDispersie = creazaTabela(nr);
	for (int i = 0; i < nr; i++) {
		TabelaDispersie = insereazaElementeinHashTable(TabelaDispersie, conturi[i]);
	}
	afisareTabelaConturi(TabelaDispersie);
	printf("\n");
	int nrEl = 0;
	ContBancar* conturiAceeasiValuta = conturiCuAceeasiValuta("USD", TabelaDispersie,&nrEl);
	for (int i = 0; i < nrEl; i++) {
		afisareContBancar(conturiAceeasiValuta[i]);
	}
	printf("\nVom afisa numarul de clustere pentru codurile unde avem coliziune.\n");
	int nrClustere = 0;
	int* clustere = getClustere(TabelaDispersie, &nrClustere);
	for (int i = 0; i < nrClustere; i++) {
		printf("%d ", clustere[i]);
	}
	printf("\n");
	int n = 0;
	struct PerechiConturi* pc = vectorDePerechi(conturiAceeasiValuta, nrEl,&n);
	for (int i = 0; i < n; i++) {
		printf("%s......%5.2f\n", pc[i].nume_client, pc[i].soldFinal);
		
	}
	for (int i = 0; i < n; i++) {
		free(pc[i].nume_client);
	}
	free(pc);
	for (int i = 0; i < nrEl; i++) {
		free(conturiAceeasiValuta[i].nume_client);
		free(conturiAceeasiValuta[i].valuta_cont);
		
	}
	free(conturiAceeasiValuta);
	
	for (int i = 0; i < nr; i++)
	{
		free(conturi[i].nume_client);
		free(conturi[i].valuta_cont);
	}
	free(conturi);
	StergereTabela(&TabelaDispersie);
}