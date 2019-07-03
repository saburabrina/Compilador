#include <iostream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


using namespace std;

int main(){
	FILE* saida = fopen("diff.txt", "w");
	int conta = 0;
	for (int num = 1; num<=1000; num++){
		string nome_arq = "teste/";
		string meu_arq = nome_arq;


		string n = "";

		if (num<10){
			n += num+'0';
			nome_arq += "000" + n;
			meu_arq += n;
		} else if (num<100){
			n += (num/10)+'0';
			n += (num%10)+'0';
			nome_arq += "00" + n;
			meu_arq += n;
		} else if (num<1000){
			n += (num/100)+'0';
			n += ((num%100)/10)+'0';
			n += (num%10)+'0';
			nome_arq += "0" +  n;
			meu_arq += n;
		} else {
			n = "1000";
			nome_arq += n;
			meu_arq += n;
		}

		nome_arq += ".out";
		
		FILE* correto = fopen(nome_arq.c_str(), "r");
		FILE* meu = fopen(meu_arq.c_str(), "r");

		char aux1[4];
		char aux2[4];

		fgets(aux1, 4, correto);
		fgets(aux2, 4, meu);

		if (aux1[0]!=aux2[0]){
			n+= '\n';
			fputs(n.c_str(), saida);
		} else {
			conta++;
		}

		fclose(correto);
		fclose(meu);
	}
	char auxi[7];
	itoa(conta, auxi, 10);
	string quant(auxi);
	quant += " corretos";
	fputs(quant.c_str(), saida);
	fclose(saida);
}