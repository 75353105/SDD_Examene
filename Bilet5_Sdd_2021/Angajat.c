#include<stdio.h>
#include<malloc.h>
#pragma warning(disable:4996)
typedef struct Angajat Angajat;
struct Angajat {
	unsigned int cod_departament;
	char* nume_angajat;
	char sex;
	int varsta;
	float salariu;
	int ore_lucrate;
};
Angajat citireDateDinFisier(FILE* f) {
	Angajat ang;
	fscanf(f, "%u", &ang.cod_departament);
	char buffer[100];
	fscanf(f, "%s", buffer);
	ang.nume_angajat = malloc(sizeof(char) * (strlen(buffer) + 1));
	strcpy(ang.nume_angajat, buffer);
	int val=0;
	fscanf(f, "%d", &val);
	if (val == 0) {
		ang.sex = 'F';
	}
	else
		if (val == 1) {
			ang.sex = 'M';
		}
		else
			ang.sex = 'N';
	fscanf(f, "%d", &ang.varsta);
	fscanf(f, "%f", &ang.salariu);
	fscanf(f, "%d", &ang.ore_lucrate);
	return ang;
}
Angajat copiereAngajat(Angajat a) {
	Angajat copie;
	copie.cod_departament = a.cod_departament;
	copie.nume_angajat = malloc(sizeof(char) * (strlen(a.nume_angajat) + 1));
	strcpy(copie.nume_angajat, a.nume_angajat);
	copie.ore_lucrate = a.ore_lucrate;
	copie.salariu = a.salariu;
	copie.sex = a.sex;
	copie.varsta = a.varsta;
	return copie;
}
Angajat* creareVectorAngajati(FILE* f, int* nr) {
	fscanf(f, "%d", nr);
	Angajat* vectorAngajati = malloc(sizeof(Angajat) * (*nr));
	for (int i = 0; i < (*nr); i++) {
		vectorAngajati[i] = citireDateDinFisier(f);
	}
	return vectorAngajati;
}
void AfiseazaUnAngajat(Angajat angajat) {
	printf("Cod Departament:%u || Nume:%s || Sex:%c || Varsta:%d || Salariu:%5.2f || Ore lucrate: %d\n",
		angajat.cod_departament,angajat.nume_angajat,angajat.sex,angajat.varsta,angajat.salariu,angajat.ore_lucrate);
}
Angajat* push_Vector(Angajat* angajati, Angajat ang, int* nrElemente) {
	(*nrElemente)++;
	Angajat* copie = malloc(sizeof(Angajat) * (*nrElemente));
	for (int i = 0; i < (*nrElemente) - 1; i++) {
		copie[i] = copiereAngajat(angajati[i]);
	}
	copie[(*nrElemente)-1] = copiereAngajat(ang);
	return copie;
}
void afiseazaVectorAngajati(Angajat* angajati, int nr) {
	for (int i = 0; i < nr; i++) {
		AfiseazaUnAngajat(angajati[i]);
	}
}
Angajat** creareMatriceAngajati(Angajat*angajati,int **dimensiune, int nrStructuri, int nrCategorii) {
	Angajat** matriceAngajati;
	matriceAngajati = malloc(sizeof(Angajat*) * nrCategorii);
	*dimensiune = malloc(sizeof(int) * nrCategorii);
	for (int i = 0; i < nrCategorii; i++) {
		matriceAngajati[i] = NULL;
		(*dimensiune)[i] =0;
	}
	for (int i = 0; i < nrStructuri; i++) {
		int index = angajati[i].cod_departament;
		matriceAngajati[index-1] = push_Vector(matriceAngajati[index-1], angajati[i],&((*dimensiune)[index-1]));
	}
	return matriceAngajati;
}
void afiseazaMatriceaDeAngajati(Angajat** matriceAngajati,int* dimensiuni, int nrCategorii ) {
	for (int i = 0; i < nrCategorii; i++)
		afiseazaVectorAngajati(matriceAngajati[i], dimensiuni[i]);
}
int cautaNrAngajatiPesteLimita(Angajat **matriceAngajati,int*dimensiuni,int nrCategorii, float valoare) {
	int nrAngajati = 0;
	for (int i = 0; i <nrCategorii ; i++) {
		for (int j = 0; j < dimensiuni[i]; j++) {
			if (matriceAngajati[i][j].salariu > valoare) {
				nrAngajati++;
			}
		}
	}
	return nrAngajati;
}
Angajat* cautaAngajatiiPesteLimita(Angajat** matriceAngajati,int*dimensiuni, int nrCategorii, float valoare,int *nrAngajati) {
	(*nrAngajati) = cautaNrAngajatiPesteLimita(matriceAngajati,dimensiuni,nrCategorii,valoare);
	Angajat* angajatiPesteLimita = malloc(sizeof(Angajat) * (*nrAngajati));
	int index = 0;
	for (int i = 0; i < nrCategorii; i++) {
		for (int j = 0; j < dimensiuni[i]; j++)
		{
			if (matriceAngajati[i][j].salariu > valoare)
			{
				angajatiPesteLimita[index] = matriceAngajati[i][j];
				index++;
			}
		}
	}
	return angajatiPesteLimita;
}

typedef struct Coada Coada;
struct Coada {
	Angajat info;
	Coada* next;
};
Coada* inserareLaInceput(Coada* coada, Angajat angajat) {
	Coada* aux = malloc(sizeof(Coada));
	aux->info = copiereAngajat(angajat);
	aux->next = coada;
	return aux;
}
void stergeCoada(Coada* coada) {
	Coada* c1 = coada;
	while (c1 != NULL) {
		Coada* c2 = c1->next;
		free(c1->info.nume_angajat);
		free(c1);
		c1 = c2;
	}
}
void enqueue(Coada** coada,Angajat a )
{
	Coada* aux = malloc(sizeof(Coada));
	aux->info = copiereAngajat(a);
	aux->next = NULL;
	if (*coada == NULL) {
		*coada = aux;
		return;
	}
	Coada* iterator = *coada;
	while (iterator->next != NULL) {
		iterator = iterator->next;
	}
	iterator->next = aux;
}
Angajat dequeue(Coada** coada)
{
	if (*coada != NULL) {
		Angajat extras =copiereAngajat((*coada)->info);
		Coada* aux = *coada;
		(*coada) = (*coada)->next;
		free(aux);
		return extras;
	}
}
Coada* getAngajatiPeDepartament(Angajat** matriceAngajati, int* dimensiuni, int nrCategorii, int cod_primit) {
	Coada* coada = NULL;
	for (int i = 0; i < nrCategorii; i++)
	{
		for (int j = 0; j < dimensiuni[i]; j++)
		{
			if (matriceAngajati[i][j].cod_departament == cod_primit)
				enqueue(&coada, matriceAngajati[i][j]);
		}
	}
	return coada;
}
void afisareElementeCoada(Coada** coada) {
	Coada* temp = NULL;
	while (*coada) {
		Angajat a = dequeue(coada);
		AfiseazaUnAngajat(a);
		enqueue(&temp, a);
	}
	*coada = temp;
}
void schimbaAngajatii(Angajat* a, Angajat* b) {
	Angajat temp = *a;
	*a = *b;
	*b = temp;
}
Coada* angajatiiCuCeleMaiMiciSalarii(Coada** coada) {
	int schimbat;
	Coada* ptr1 = NULL;
	Coada* ptr2 = NULL;
	if (*coada == NULL) {
		return;
	}else
		do {
			schimbat = 0;
			ptr1 = *coada;
			while (ptr1->next != ptr2)
			{
				if (ptr1->info.salariu > ptr1->next->info.salariu) {
					schimbaAngajatii(&ptr1->info, &ptr1->next->info);
					schimbat = 1;
				}
				ptr1 = ptr1->next;
			}
			ptr2 = ptr1;
		} while (schimbat);
		Coada* coadaReturnata = NULL;
		ptr1 = *coada;
		int i = 0;
		while (ptr1 != NULL && i < 3) {
			enqueue(&coadaReturnata, ptr1->info);
			ptr1 = ptr1->next;
			i++;
		}

		return coadaReturnata;
		
}
void main()
{
	int nr = 0;
	FILE* fisier;
	fisier =fopen ("Angajati.txt", "r");
	Angajat* angajati = creareVectorAngajati(fisier, &nr);
	/*for (int i = 0; i < nr; i++)
	{
		AfiseazaUnAngajat(angajati[i]);
	}*/
	unsigned int* VectorDepartamente=NULL;
	int nrDepartamente = 0;
	//VectorDepartamente[0] = angajati[0].cod_departament;
	for (int i = 0; i < nr; i++) {
		int ok = 1;
		for (int j = 0; j < nrDepartamente; j++) {
			if (angajati[i].cod_departament == VectorDepartamente[j])
			{
				ok = 0;
				break;
			}
		}
		if (ok == 1)
		{
			VectorDepartamente = realloc(VectorDepartamente, sizeof(unsigned int) * (nrDepartamente + 1));
			VectorDepartamente[nrDepartamente] = angajati[i].cod_departament;
			nrDepartamente++;
		}
	}
	for (int i = 0; i < nrDepartamente; i++) {
		printf("%d ", VectorDepartamente[i]);
	}
	printf("\n");
	int* dimensiuni;
	Angajat** matriceAngajati = creareMatriceAngajati(angajati,&dimensiuni , nr, nrDepartamente);
	afiseazaMatriceaDeAngajati(matriceAngajati, dimensiuni, nrDepartamente);
	printf("\n Afisam angajatii care au salariu mai mare de 5000:\n");
	int nr2 = 0;
	Angajat* angajatiSalariuMare = cautaAngajatiiPesteLimita(matriceAngajati, dimensiuni, nrDepartamente, 5000, &nr2);
	afiseazaVectorAngajati(angajatiSalariuMare, nr2);
	printf("\n Afisam angajatii din departamentul 2 in coada:\n");
	Coada* coadaAngajati = NULL;
	coadaAngajati = getAngajatiPeDepartament(matriceAngajati, dimensiuni, nrDepartamente, 2);
	afisareElementeCoada(&coadaAngajati);
	Coada* coadaAngajati2 = NULL;
	coadaAngajati2 = angajatiiCuCeleMaiMiciSalarii(&coadaAngajati);
	afisareElementeCoada(&coadaAngajati2);

	stergeCoada(coadaAngajati2);
	stergeCoada(coadaAngajati);
	for (int i = 0; i < nr2; i++) {
		free(angajatiSalariuMare[i].nume_angajat);
	}
	free(angajatiSalariuMare);

	for (int i = 0; i < nrDepartamente; i++)
	{
		for (int j = 0; j < dimensiuni[i]; i++){
			matriceAngajati[i][j].nume_angajat = NULL;
			free(matriceAngajati[i][j].nume_angajat);
	}
		matriceAngajati[i] = NULL;
		free(matriceAngajati[i]);
	}
	matriceAngajati = NULL;
	free(matriceAngajati);

	free(VectorDepartamente);
	for (int i = 0; i < nr2; i++) {
		free(angajati[i].nume_angajat);
	}
	free(angajati);

}