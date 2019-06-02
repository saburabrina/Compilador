#include <iostream>
#include <string>
#include <vector>

#define ARQINV "ARQUIVO INVALIDO\n"
#define OK "OK\n"
#define MAX 512
#define MAXFILE 524289

using namespace std;

struct token {
  string valor;
  string categoria;
};

int linha = 1;
int coluna = 1;
int referencia = 0;
bool atehentaotudookay = true;
vector<token> tokens;

void estadozero(string termo);
void armazenamento(string termo, string tipo);
bool azAZ09(char a, bool letra, bool number);
void id(string termo);
void erro(int coluna);
void num(string termo);
void texto(string termo);

void texto(string termo){
	string acc = "";
	int aux;
	while(acc.size() < MAX+1){
		if((unsigned) referencia == termo.size()){
	        if(coluna != 1) erro(coluna-1);
			else {
				linha--;
				erro(aux);
			}
			return ;
	    }

		if(termo[referencia] == '"'){
			armazenamento(acc, "TEXTO");
			coluna++; 
			referencia++;
			return ;
		}
		else {
			acc += termo[referencia];
	
			if(acc.size() == MAX+1){
				if(coluna != 1) erro(coluna-1);
				else {
					linha--;
					erro(aux);
					linha++;
				}
				return ;
			}
			
			if(termo[referencia] == 10){
				aux = coluna;
				coluna = 1;
				linha++;
			}
			else {
				coluna++;
			}
			referencia++;
		}
	}
}

void num(string termo){
	string acc = "";
	bool virgula = false;
	while(acc.size() < MAX+1){
		if(azAZ09(termo[referencia], false, true)){
			acc += termo[referencia];
			
			if(acc.size() == MAX+1){
				erro(coluna-1);
				return ;
			}	
			coluna++;
			referencia++;
		}
		else if(termo[referencia] == 44){
			if (!virgula){
				acc += termo[referencia];

				if(acc.size() == MAX+1){
					erro(coluna-1);
					return ;
				}	
				coluna++;
				referencia++;
				virgula = true;
			}
			else{
				erro(coluna-1);
				return ;
			}
		}
		else if(azAZ09(termo[referencia], true, false)){
			erro(coluna-1);
			return ;
		}
		else {
			armazenamento(acc, "VALOR");
			return ;
		}
	}
}

void erro(int column){
	atehentaotudookay = false;
	cout << linha << " " << column << endl;
}

void id(string termo){
	string acc = "";
	while(acc.size() < MAX+1){
		if(azAZ09(termo[referencia], true, true)){
			acc += termo[referencia];
			if(acc.size() == MAX+1){
				erro(coluna-1);
				return ;
			}	
			coluna++;
			referencia++;
		}
		else {
			armazenamento(acc, "ID");
			return ;
		}
	}
}

bool azAZ09(char a, bool letra, bool number){
	if(letra && a > 64 && a < 91) return true;
	if(letra && a > 96 && a < 123) return true;
	if(number && a > 47 && a < 58) return true;

	return false;
}

void armazenamento(string termo, string tipo){
	token novo;
	novo.valor = termo;
	novo.categoria = tipo;

	tokens.push_back(novo);
}

void estadozero(string termo){
	while((unsigned) referencia < termo.size()){
		if( termo[referencia] == '.' || termo[referencia] == ':' || termo[referencia] == ';' || termo[referencia] == '+' || termo[referencia] == '-' || termo[referencia] == '/' || termo[referencia] == '%' || termo[referencia] == '(' || termo[referencia] == ')' || termo[referencia] == '[' || termo[referencia] == ']' || termo[referencia] == '=' || termo[referencia] == '&' || termo[referencia] == '|' || termo[referencia] == '!' ){
			if(termo[referencia] == '/' || termo[referencia] == '%' ){
				armazenamento(string(1, termo[referencia]), "OP2");
			}
			else if(termo[referencia] == '=') {
				armazenamento(string(1, termo[referencia]), "OPR");
			}
			else{
				armazenamento(string(1, termo[referencia]), string(1, termo[referencia]));
			}
			coluna++;
			referencia++;
    	}

		else if( termo[referencia] == '<'){
        	coluna++;
			referencia++;

        	if(termo[referencia] == '-'){ 
				armazenamento("<-", "<-");
				coluna++;
				referencia++;
        	}
        	else if(termo[referencia] == '='){ 
				armazenamento("<=", "OPR");
				coluna++;
				referencia++;
        	}
        	else if(termo[referencia] == '>'){ 
				armazenamento("<>", "OPR");
				coluna++;
				referencia++;
        	}
        	else armazenamento("<", "OPR");
      	}

		else if( termo[referencia] == '*'){
			coluna++;
			referencia++;

			if(termo[referencia] == '*'){ 
				armazenamento("**", "**");
				coluna++;
				referencia++;
			}
			else armazenamento("*", "OP2");
        }
      
		else if( termo[referencia] == '>'){
			coluna++;
			referencia++;

			if(termo[referencia] == '='){ 
				armazenamento(">=", "OPR");
				coluna++;
				referencia++;
			}
			else armazenamento(">", "OPR");
		}

		else if( termo[referencia] == 9 || termo[referencia] == 32){
			coluna++;
			referencia++;
		}

		else if(termo[referencia] == 10){
			linha++;
			coluna = 1;
			referencia++;
		}
    
		else if (termo[referencia] == '"'){
			coluna++;
			referencia++;
			texto(termo);
		}

      	else if (termo.substr(referencia, 2) == "DE" || termo.substr(referencia, 2) == "SE"){
			if(azAZ09(termo[referencia+2], true, true)){
				id(termo);
			}
			else{
				armazenamento(termo.substr(referencia, 2), termo.substr(referencia, 2));
				coluna+=2;
				referencia+=2;
			} 
      	}

		else if(termo.substr(referencia, 3) == "FIM" || termo.substr(referencia, 3) == "VAR" || termo.substr(referencia, 3) == "VET"){
			if(azAZ09(termo[referencia+3], true, true)){
				id(termo);
			}
			else{
				armazenamento(termo.substr(referencia, 3), termo.substr(referencia, 3));
				coluna+=3;
				referencia+=3;
			} 
		}

		else if(termo.substr(referencia, 4) == "ATEH" || termo.substr(referencia, 4) == "LEIA" || termo.substr(referencia, 4) == "PARE" || termo.substr(referencia, 4) == "PARA" || termo.substr(referencia, 4) == "NULO" || termo.substr(referencia, 4) == "REAL"){
			if(azAZ09(termo[referencia+4], true, true)){
				id(termo);
			}
			else{
				if(termo.substr(referencia, 4) == "REAL"){
					armazenamento(termo.substr(referencia, 4), "TIPO");
				}
				else{
					armazenamento(termo.substr(referencia, 4), termo.substr(referencia, 4));
				}
				
				coluna+=4;
				referencia+=4;
			} 
		}

		else if(termo.substr(referencia, 5) == "SENAO"){
			if(azAZ09(termo[referencia+5], true, true)){
				id(termo);
			}
			else{
				armazenamento(termo.substr(referencia, 5), termo.substr(referencia, 5));
				coluna+=5;
				referencia+=5;
			} 
		}

		else if(termo.substr(referencia, 6) == "FUNCAO" || termo.substr(referencia, 6) == "INICIO" || termo.substr(referencia, 6) == "RECEBA"){
			if(azAZ09(termo[referencia+6], true, true)){
				id(termo);
			}
			else{
				armazenamento(termo.substr(referencia, 6), termo.substr(referencia, 6));
				coluna+=6;
				referencia+=6;
			} 
		}

		else if(termo.substr(referencia, 7) == "ESCREVA" || termo.substr(referencia, 7) == "INTEIRO" ){
			if(azAZ09(termo[referencia+7], true, true)){
				id(termo);
			}
			else{
				if(termo.substr(referencia, 7) == "INTEIRO"){
					armazenamento(termo.substr(referencia, 7), "TIPO");
				}
				else{
					armazenamento(termo.substr(referencia, 7), "ESCREVA");
				}
				coluna+=7;
				referencia+=7;
			} 
		}

		else if(termo.substr(referencia, 8) == "ENQUANTO"){
			if(azAZ09(termo[referencia+8], true, true)){
				id(termo);
			}
			else{
				armazenamento(termo.substr(referencia, 8), termo.substr(referencia, 8));
				coluna+=8;
				referencia+=8;
			} 
		}

		else if(azAZ09(termo[referencia], true, false)){
			id(termo);
		}

		else if(azAZ09(termo[referencia], false, true)){
			num(termo);
		}
		else{ 
			erro(coluna);
			//cout << "erro caractere n permitido\n";
			coluna++;
			referencia++;
		}
	}
}

int main (){

  	char termoaux[MAXFILE];
  	string termo = "";

  	fread (termoaux, 1, MAXFILE, stdin);
  	termo = string(termoaux);
  
	int i;
	for(i = 0; (unsigned) i < termo.size(); i++){
		if(termo[i] < 9 || (termo[i] > 10 && termo[i] < 32) || termo[i] > 126){
			cout << ARQINV;
			break;
		}
	}

	if((unsigned)i == termo.size()){
		estadozero(termo);
		if(atehentaotudookay){
			cout << OK;
		}
	}
}
