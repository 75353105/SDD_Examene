
#include<stdio.h>
#include<malloc.h>
#pragma warning (disable:4996)
typedef struct Rezervare Rezervare;

struct Rezervare {
	unsigned int idRezervare;
	char* denumireHotel;
	unsigned char nrCamereRezervate;
	char* numeClient;
	float sumaDePlata;
};
Rezervare initializareRezrvare(unsigned int idRezervare,
const char* denumireHotel,
unsigned char nrCamereRezervate,
const char* numeClient,
float sumaDePlata) {
	struct Rezervare rezervare;
	rezervare.idRezervare = idRezervare;
	rezervare.denumireHotel = malloc(sizeof(char) * strlen(denumireHotel + 1));
	strcpy(rezervare.denumireHotel, denumireHotel);
	rezervare.nrCamereRezervate = nrCamereRezervate;
	rezervare.numeClient = malloc(sizeof(char) * strlen(numeClient + 1));
	strcpy(rezervare.numeClient, numeClient);
	rezervare.sumaDePlata = sumaDePlata;
	return rezervare;
}
Rezervare deepCopy(Rezervare r) {
	return initializareRezrvare(r.idRezervare, r.denumireHotel, r.nrCamereRezervate, r.numeClient, r.sumaDePlata);
}
Rezervare citireDateDinFisierText(FILE *f) {
	Rezervare r;
	fscanf(f, "%u", &r.idRezervare);
	char buffer[100];
	fscanf(f, "%s", &buffer);
	r.denumireHotel = malloc(sizeof(char*) * (strlen(buffer + 1)));
	strcpy(r.denumireHotel, buffer);
	char s;
	fscanf(f, "%hhu", &s);
	r.nrCamereRezervate = s;
	char inlocuitor[100];
	fscanf(f, "%s", &inlocuitor);
	r.numeClient = malloc(sizeof(char*) * (strlen(inlocuitor + 1)));
	strcpy(r.numeClient, inlocuitor);
	fscanf(f, "%f", &r.sumaDePlata);
	return r;
}
Rezervare* citireVectorRezervari(FILE* f,int *nr) {
	fscanf(f, "%d", nr);
	Rezervare* vectorRezervari = malloc(sizeof(Rezervare) * (*nr));
	
	for (int i = 0; i < (*nr); i++) {
		vectorRezervari[i] = citireDateDinFisierText(f);
	}
	return vectorRezervari;
}

typedef struct NodArbore NodArbore;
struct NodArbore {
	struct Rezervare info;
	struct NodArbore* left;
	struct NodArbore* right;
};

void inserareInArbore(NodArbore** radacina, Rezervare rezervare) {

	if (*radacina) {
		if (rezervare.idRezervare > (*radacina)->info.idRezervare) {
			inserareInArbore(&(*radacina)->right, rezervare);
		}
		else
			if (rezervare.idRezervare < (*radacina)->info.idRezervare) {
				inserareInArbore(&(*radacina)->left, rezervare);
			}
			
	}
	else
	{
		NodArbore* nou = malloc(sizeof(NodArbore));
		nou->left = NULL;
		nou->right = NULL;
		nou->info = rezervare;
		(*radacina) = nou;
	}
}
void AfisareRezervare(struct Rezervare rezervare) {
	printf("Rezervare cu codul %u a fost efectuata de clientul %s\n", rezervare.idRezervare, rezervare.numeClient);
}
void ParcurgerePreOrdine(NodArbore* radacina) {
	if (radacina){
		AfisareRezervare(radacina->info);
	ParcurgerePreOrdine(radacina->left);
	ParcurgerePreOrdine(radacina->right);
	}
}
void ParcurgereInOrdine(NodArbore* radacina) {
	if (radacina) {
		ParcurgereInOrdine(radacina->left);
		AfisareRezervare(radacina->info);
		ParcurgereInOrdine(radacina->right);
	}
}
void VectorNiveluri(int *niveluri, NodArbore* radacina,int i) {
	if (radacina) {
		niveluri[i] += 1;
		VectorNiveluri(niveluri, radacina->left, i + 1);
		VectorNiveluri(niveluri, radacina->right, i + 1);
	}
}
float valoareaRezervarilor(NodArbore* radacina, const char* NumeClient) {
	if (radacina) {
		float valoare = 0;
		if (strcmp(radacina->info.numeClient, NumeClient) == 0) {
			valoare += radacina->info.sumaDePlata;
		}
		valoare += valoareaRezervarilor(radacina->left, NumeClient);
		valoare += valoareaRezervarilor(radacina->right, NumeClient);
		return valoare;
	}
	return 0;
}
int calculInaltime(NodArbore* radacina) {
	if (radacina) {
		int inaltimeS = calculInaltime(radacina->left);
		int inaltimeD = calculInaltime(radacina->right);
		return 1 + (inaltimeD > inaltimeS ? inaltimeD : inaltimeS);
	}
	else
		return 0;
}
void stergereArbore(NodArbore* radacina) {
	if (radacina) {
		stergereArbore(radacina->left);
		stergereArbore(radacina->right);
		free(radacina->info.denumireHotel);
		free(radacina->info.numeClient);
		free(radacina);
	}
}

void stergereNodMinim(NodArbore** radacina) {
	if (*radacina)
	{
		if ((* radacina)->left != NULL) {
			stergereNodMinim(&(*radacina)->left);
		}
		else{

		NodArbore* aux = *radacina;
		*radacina = (*radacina)->right;
		free(aux->info.denumireHotel);
		free(aux->info.numeClient);
		free(aux);
	}
}
}


void main() {
	
	
	FILE* fisier;
	fisier = fopen("rezervari.txt", "r");
	int nr;
	Rezervare* vectorRezervari = NULL;
	vectorRezervari = citireVectorRezervari(fisier, &nr);
	NodArbore* ArboreBinar = NULL;
	for (int i = 0; i < nr; i++) {
		inserareInArbore(&ArboreBinar, vectorRezervari[i]);
	}
	ParcurgerePreOrdine(ArboreBinar);
	printf("\n");
	float suma =valoareaRezervarilor(ArboreBinar, "Zamfirescu");
	printf("%5.2f\n", suma);

	stergereNodMinim(&ArboreBinar);
	printf("\n");
	ParcurgerePreOrdine(ArboreBinar);


	int inaltimeMax = calculInaltime(ArboreBinar);
	int* niveluri = malloc(sizeof(int*) * inaltimeMax);
	int j = 0;
	for (int i = 0; i < inaltimeMax; i++) {
		niveluri[i] = 0;
	}
	VectorNiveluri(niveluri, ArboreBinar, j);
	for (int i = 0; i < inaltimeMax; i++) {
		printf("%d ", niveluri[i]);
	}
	free(niveluri);
	stergereArbore(ArboreBinar);
}