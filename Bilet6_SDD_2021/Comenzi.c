#include<stdio.h>
#include<malloc.h>
#pragma warning(disable:4996)
typedef struct ComandaMancare ComandaMancare;
struct ComandaMancare {
	int id_comanda;
	int cod_client;
	int timp_livrare;
	char* locatie;
	char* nume_client;
	float pret;
	float bacsis;
};
ComandaMancare copiereComanda(ComandaMancare cm) {
	ComandaMancare copie;
	copie.bacsis = cm.bacsis;
	copie.cod_client = cm.cod_client;
	copie.id_comanda = cm.id_comanda;
	copie.locatie = malloc(sizeof(char) * (strlen(cm.locatie) + 1));
	strcpy(copie.locatie, cm.locatie);
	copie.nume_client = malloc(sizeof(char) * (strlen(cm.nume_client) + 1));
	strcpy(copie.nume_client, cm.nume_client);
	copie.pret = cm.pret;
	copie.timp_livrare = cm.timp_livrare;
	return copie;
}
ComandaMancare citireDateDinFisier(FILE* f) {
	ComandaMancare cm;
	fscanf(f, "%d", &cm.id_comanda);
	fscanf(f, "%d", &cm.cod_client);
	fscanf(f, "%d", &cm.timp_livrare);
	char buffer1[100];
	fscanf(f, "%s", buffer1);
	cm.locatie = malloc(sizeof(char) * (strlen(buffer1) + 1));
	strcpy(cm.locatie, buffer1);
	char buffer2[100];
	fscanf(f, "%s", buffer2);
	cm.nume_client= malloc(sizeof(char) * (strlen(buffer2) + 1));
	strcpy(cm.nume_client, buffer2);
	fscanf(f, "%f", &cm.pret);
	fscanf(f, "%f", &cm.bacsis);
	return cm;
}
ComandaMancare* citireVectorDeComenzi(FILE* f, int* nr) {
	fscanf(f, "%d", nr);
	ComandaMancare* vectorComenzi = malloc(sizeof(ComandaMancare) * (*nr));
	for (int i = 0; i < (*nr); i++) {
		vectorComenzi[i] = citireDateDinFisier(f);
	}
	return vectorComenzi;
}
void afisareComandaMancare(ComandaMancare cm) {
	printf("id_comanda:%d ||cod_client:%d ||timp_livrare:%d ||locatie:%s ||nume_client:%s || pret:%5.2f|| bacsis:%5.2f\n",
		cm.id_comanda, cm.cod_client, cm.timp_livrare, cm.locatie, cm.nume_client, cm.pret, cm.bacsis);
}
typedef struct NodArbore NodArbore;
struct NodArbore {
	ComandaMancare info;
	NodArbore* left;
	NodArbore* right;
};
NodArbore* inserareComandaInArbore(NodArbore* radacina,ComandaMancare cm) {
	if (radacina) {
		if (radacina->info.id_comanda < cm.id_comanda) {
			radacina->right = inserareComandaInArbore(radacina->right, cm);
		}
		else
			if (radacina->info.id_comanda > cm.id_comanda) {
				radacina->left = inserareComandaInArbore(radacina->left, cm);
			}
	}
	else
	{
		radacina = malloc(sizeof(NodArbore));
		radacina->info = copiereComanda(cm);
		radacina->left = NULL;
		radacina->right = NULL;
	}
	return radacina;
}
void afisareInOrdine(NodArbore* radacina) {
	if (radacina) {
		afisareInOrdine(radacina->left);
		afisareComandaMancare(radacina->info);
		afisareInOrdine(radacina->right);
	}
}
ComandaMancare* cautareComenziDupaTimp(NodArbore* radacina,ComandaMancare**cm, int timp_livrare, int *nr) {
	if (radacina) {
		
		if (radacina->info.timp_livrare > timp_livrare) {
			(*nr)++;
			*cm = realloc(*cm, sizeof(ComandaMancare) * (*nr));
			(*cm)[(*nr)-1] = copiereComanda(radacina->info);
		
		}
		*cm = cautareComenziDupaTimp(radacina->left,cm, timp_livrare,nr);
		*cm = cautareComenziDupaTimp(radacina->right,cm, timp_livrare,nr);
		
	}
	return *cm;
}
typedef struct Heap Heap;
struct Heap {
	int dimensiune;
	int numarElemente;
	ComandaMancare* comenzi;
};
Heap initializareHeap(int dimensiune) {
	Heap heap;
	heap.dimensiune = dimensiune;
	heap.numarElemente = 0;
	heap.comenzi = malloc(sizeof(ComandaMancare) * dimensiune);
	return heap;
}
Heap adaugareElementeInHeap(NodArbore* radacina, Heap heap) {
	if (radacina) {
		
			heap.comenzi[heap.numarElemente] = copiereComanda(radacina->info);
			heap.numarElemente++;
		
		heap = adaugareElementeInHeap(radacina->left, heap);
		heap = adaugareElementeInHeap(radacina->right, heap);
	}
	return heap;
}
void filtrareHeap(Heap *heap, int pozitie) {
	int pozS = 2 * pozitie + 1;
	int pozD = 2 * pozitie + 2;
	int pozMax = pozitie;

	if (pozS < heap->numarElemente && heap->comenzi[pozMax].timp_livrare < heap->comenzi[pozS].timp_livrare) {
		pozMax = pozS;
	}
	if (pozD < heap->numarElemente && heap->comenzi[pozMax].timp_livrare < heap->comenzi[pozD].timp_livrare) {
		pozMax = pozD;
	}
	if (pozMax != pozitie) {
		ComandaMancare aux = heap->comenzi[pozMax];
		heap->comenzi[pozMax] = heap->comenzi[pozitie];
		heap->comenzi[pozitie] = aux;
		filtrareHeap(heap, pozMax);
	}
}
void afisareHeap(Heap h)
{
	for (int i = 0; i < h.numarElemente; i++)
	{
		afisareComandaMancare(h.comenzi[i]);
	}
}
void main() {
	FILE* fisier;
	fisier = fopen("Comenzi.txt", "r");
	int nr = 0;
	ComandaMancare* comenzi = citireVectorDeComenzi(fisier, &nr);
	NodArbore* radacina=NULL;
	for (int i = 0; i < nr; i++)
	{
		radacina = inserareComandaInArbore(radacina, comenzi[i]);
	}
	afisareInOrdine(radacina);
	printf("Afisam comenzile care vin in mai mult de 40 de minute\n");
	int nr2 = 0;
	ComandaMancare* comenziDurataLunga = NULL;
		comenziDurataLunga = cautareComenziDupaTimp(radacina,&comenziDurataLunga, 40,&nr2);
	printf("\n");
	for (int i = 0; i < nr2; i++) {
		afisareComandaMancare(comenziDurataLunga[i]);
	}
	printf("\n Afisam elementele dintr-un HEAP in care prioritatea este data de timpul de livrare\n");
	Heap structuraHeap = initializareHeap(nr);
	structuraHeap = adaugareElementeInHeap(radacina, structuraHeap);

	for (int i = (structuraHeap.numarElemente-2) / 2; i >= 0; i--)
	{
		filtrareHeap(&structuraHeap, i);
	}
	afisareHeap(structuraHeap);
}