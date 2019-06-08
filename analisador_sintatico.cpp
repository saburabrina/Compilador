#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <stack>
#include <utility>
#define ARQINV "ARQUIVO INVALIDO\n"
#define OK "OK\n"
#define MAX 512
#define MAXFILE 131072
using namespace std;
struct token {
	string valor;
	string categoria;
};
int linha=1;
int coluna=1;
int referencia=0;
bool no_lexical_errors=true;
vector<token> tokens;
void geracao_tokens(string termo, string tipo);
bool azAZ09(char a, bool letra, bool number);
void id(string termo);
void erro_lexico(int coluna);
void num(string termo);
void texto(string termo){
	string acc = "";
	int aux;
	while(acc.size() < MAX+1){
		if((unsigned) referencia == termo.size()){
	        if(coluna != 1) erro_lexico(coluna-1);
			else {
				linha--;
				erro_lexico(aux);
			}
			return ;
	    }
		if(termo[referencia] == '"'){
			geracao_tokens(acc, "TEXTO");
			coluna++; 
			referencia++;
			return ;
		}
		else {
			acc += termo[referencia];	
			if(acc.size() == MAX+1){
				if(coluna != 1) erro_lexico(coluna-1);
				else {
					linha--;
					erro_lexico(aux);
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
				erro_lexico(coluna-1);
				return ;
			}	
			coluna++;
			referencia++;
		}
		else if(termo[referencia] == 44){
			if (!virgula){
				acc += termo[referencia];
				if(acc.size() == MAX+1){
					erro_lexico(coluna-1);
					return ;
				}	
				coluna++;
				referencia++;
				virgula = true;
			}
			else{
				erro_lexico(coluna-1);
				return ;
			}
		}
		else if(azAZ09(termo[referencia], true, false)){
			erro_lexico(coluna-1);
			return ;
		}
		else {
			geracao_tokens(acc, "VALOR");
			return ;
		}
	}
}
void erro_lexico(int column){
	no_lexical_errors = false;
	cout << linha << " " << column << endl;
}
void id(string termo){
	string acc = "";
	while(acc.size() < MAX+1){
		if(azAZ09(termo[referencia], true, true)){
			acc += termo[referencia];
			if(acc.size() == MAX+1){
				erro_lexico(coluna-1);
				return ;
			}	
			coluna++;
			referencia++;
		}
		else {
			geracao_tokens(acc, "ID");
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
void geracao_tokens(string termo, string tipo){
	token novo;
	novo.valor = termo;
	novo.categoria = tipo;
	tokens.push_back(novo);
	cout << tipo << " ";
}
void analise_lexica(string termo){
	while((unsigned) referencia < termo.size()){
		if( termo[referencia] == '.' || termo[referencia] == ':' || termo[referencia] == ';' || termo[referencia] == '+' || termo[referencia] == '-' || termo[referencia] == '/' || termo[referencia] == '%' || termo[referencia] == '(' || termo[referencia] == ')' || termo[referencia] == '[' || termo[referencia] == ']' || termo[referencia] == '=' || termo[referencia] == '&' || termo[referencia] == '|' || termo[referencia] == '!' ){
			if(termo[referencia] == '/' || termo[referencia] == '%' ) geracao_tokens(string(1, termo[referencia]), "OP2");
			else if(termo[referencia] == '=') geracao_tokens(string(1, termo[referencia]), "OPR");
			else{
				geracao_tokens(string(1, termo[referencia]), string(1, termo[referencia]));
			}
			coluna++;
			referencia++;
    	}
		else if( termo[referencia] == '<'){
        	coluna++;
			referencia++;
        	if(termo[referencia] == '-'){ 
				geracao_tokens("<-", "<-");
				coluna++;
				referencia++;
        	}
        	else if(termo[referencia] == '='){ 
				geracao_tokens("<=", "OPR");
				coluna++;
				referencia++;
        	}
        	else if(termo[referencia] == '>'){ 
				geracao_tokens("<>", "OPR");
				coluna++;
				referencia++;
        	}
        	else geracao_tokens("<", "OPR");
      	}
		else if( termo[referencia] == '*'){
			coluna++;
			referencia++;

			if(termo[referencia] == '*'){ 
				geracao_tokens("**", "**");
				coluna++;
				referencia++;
			}
			else geracao_tokens("*", "OP2");
        }
		else if( termo[referencia] == '>'){
			coluna++;
			referencia++;
			if(termo[referencia] == '='){ 
				geracao_tokens(">=", "OPR");
				coluna++;
				referencia++;
			}
			else geracao_tokens(">", "OPR");
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
				if(termo.substr(referencia, 5) == "SENAO" && !azAZ09(termo[referencia+5], true, true)){
					geracao_tokens(termo.substr(referencia, 5), termo.substr(referencia, 5));
					coluna+=5;
					referencia+=5;
				} 
				else id(termo); 
			}
			else{
				geracao_tokens(termo.substr(referencia, 2), termo.substr(referencia, 2));
				coluna+=2;
				referencia+=2;
			} 
      	}
		else if(termo.substr(referencia, 3) == "FIM" || termo.substr(referencia, 3) == "VAR" || termo.substr(referencia, 3) == "VET"){
			if(azAZ09(termo[referencia+3], true, true)) id(termo);
			else{
				geracao_tokens(termo.substr(referencia, 3), termo.substr(referencia, 3));
				coluna+=3;
				referencia+=3;
			} 
		}
		else if(termo.substr(referencia, 4) == "ATEH" || termo.substr(referencia, 4) == "LEIA" || termo.substr(referencia, 4) == "PARE" || termo.substr(referencia, 4) == "PARA" || termo.substr(referencia, 4) == "NULO" || termo.substr(referencia, 4) == "REAL"){
			if(azAZ09(termo[referencia+4], true, true)) id(termo);
			else{
				if(termo.substr(referencia, 4) == "REAL") geracao_tokens(termo.substr(referencia, 4), "TIPO");
				else{
					geracao_tokens(termo.substr(referencia, 4), termo.substr(referencia, 4));
				}
				coluna+=4;
				referencia+=4;
			} 
		}
		else if(termo.substr(referencia, 6) == "FUNCAO" || termo.substr(referencia, 6) == "INICIO" || termo.substr(referencia, 6) == "RECEBA"){
			if(azAZ09(termo[referencia+6], true, true)) id(termo);
			else{
				geracao_tokens(termo.substr(referencia, 6), termo.substr(referencia, 6));
				coluna+=6;
				referencia+=6;
			} 
		}
		else if(termo.substr(referencia, 7) == "ESCREVA" || termo.substr(referencia, 7) == "INTEIRO" ){
			if(azAZ09(termo[referencia+7], true, true)) id(termo);
			else{
				if(termo.substr(referencia, 7) == "INTEIRO") geracao_tokens(termo.substr(referencia, 7), "TIPO");
				else{
					geracao_tokens(termo.substr(referencia, 7), "ESCREVA");
				}
				coluna+=7;
				referencia+=7;
			} 
		}
		else if(termo.substr(referencia, 8) == "ENQUANTO"){
			if(azAZ09(termo[referencia+8], true, true)) id(termo);
			else{
				geracao_tokens(termo.substr(referencia, 8), termo.substr(referencia, 8));
				coluna+=8;
				referencia+=8;
			} 
		}
		else if(azAZ09(termo[referencia], true, false)) id(termo);
		else if(azAZ09(termo[referencia], false, true)) num(termo);
		else{ 
			erro_lexico(coluna);
			coluna++;
			referencia++;
		}
	}
}
void analise_sintatica(){
	size_t estado = 0;
	map<string,string> lr_table[];
	stack<string> pilha;
	pair<string, size_t> reducoes[];
	string variavel = tokens[i].categoria;
	string transicao = (lr_table[estado])[variavel];
	int i = 0;
	while (i < tokens.size()){	
		if (transicao.at(0) == 's'){
			pilha.push(variavel); // empilha o token
			pilha.push(transicao.substr(1)); // empilha o novo estado
			i++; // incrementa o ponteiro para o token
			estado = stoi(transicao.substr(1)); // atualiza o estado
			variavel = tokens[i].categoria; // atualiza o token
			transicao = (lr_table[estado])[variavel]; // atualiza a transição
		} else if (transicao.at(0) == 'r'){
			size_t reducao = stoi(transicao.substr(1));
			size_t r = reducoes[reducao].second*2;
			for (int j = 0; (unsigned) i < r; ++i){
				pilha.pop(); // dá pop na pilha duas vezes o tamanho da transição
			}
			estado = stoi(pilha.top());
			variavel = reducoes[reducao].first;
			pilha.push(variavel);
			transicao = (lr_table[estado])[variavel];
		} else {
			pilha.push(transicao);
			estado = stoi(transicao);
			variavel = tokens[i].categoria;
			transicao = (lr_table[estado])[variavel];
		}
	}
}
int main (){
  	char termoaux[MAXFILE];
  	string termo = "";
  	fread (termoaux, 1, MAXFILE, stdin);
  	termo = string(termoaux);
	for(int i = 0; (unsigned) i < termo.size(); i++){
		if(termo[i] < 9 || (termo[i] > 10 && termo[i] < 32) || termo[i] > 126){
			cout << ARQINV;
			break;
		}
	}
	if((unsigned) i == termo.size()){
		analise_lexica(termo);
		if(no_lexical_errors){
			analise_sintatica();
		}
	}
}