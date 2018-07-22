//--------------------------------------------------------------
// NOMES: Guilherme Oliveira Goularte && Leonardo Victor Moreira de Souza
//--------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <malloc.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>



// ######### ESCREVA O NROUSP DO PRIMEIRO INTEGRANTE AQUI
char* nroUSP1() {
    return("10387748");
}



// ######### ESCREVA O NROUSP DO SEGUNDO INTEGRANTE AQUI (OU DEIXE COM ZERO)
char* nroUSP2() {
    return("10284409");
}



// no da arvore
typedef struct estr {
        int chave;
        struct estr *esq;
	struct estr *dir;
} NO;

int maiorNaEsq (NO* p){//essa função retorna o maior nó de uma ABB (aquele mais a direita),
    while (p->dir){      //a função será utilizada para achar os nós mais a direita de uma sub-árvore
        p = p->dir;
    }
    return p->chave;
}

int menorNaDir (NO* p){//essa função faz o mesmo que sua antecessora, mas retorna o menor
    while (p->esq){    //ambas são funções auxiliares para encontrar os valores mais a esquerda e mais a direita
        p = p->esq;
    }
    return p->chave;
}

NO* baixoPraCima (NO* p, NO** pai, NO* sent){//essa função faz a checagem de baixo para cima
    if (p){
        NO* a = baixoPraCima(p->esq, pai, sent);//joga a recursão para a esquerda
        if (a != sent){//caso o "a" retorne sentila significa q o filho esquerdo é nulo
            if (a){// caso o retorno seja valido e diferente de sentinela, significa que a função achou um nó inválido
                if (!*pai)//a função testa se existe um pai, caso não exista o nó atual é o pai
                    *pai = p;
                return a;
            }
            int aux = maiorNaEsq(p->esq);//a função irpa então testar se o nó atual está errado
            if (p->chave < aux)//caso a chave do nó atual seja menor que a maior chave na sub-árvore enraizada em seu filho esquerdo,
                return p;//esse nó está errado e, por tanto, será retornado na função
        }
        NO* b = baixoPraCima(p->dir, pai, sent);//joga a recursão para a direita
        if (b != sent){//caso o "b" retorne sentila significa q o filho direito é nulo
            if (b){// caso o retorno seja valido e diferente de sentinela, significa que a função achou um nó inválido
                if (!*pai)//a função testa se existe um pai, caso não exista o nó atual é o pai
                    *pai = p;
                return b;
            }
            int aux = menorNaDir(p->dir);//a função irpa então testar se o nó atual está errado
            if (p->chave > aux)//caso a chave do nó atual seja maior que a menor chave na sub-árvore enraizada em seu filho direito,
                return p;//esse nó está errado e, por tanto, será retornado na função
        }
        return NULL;//se os dois filhos forem nulos (o nó é uma folha), ou não entrar em nenhum dos if's (esse nó não é inválido),
                    //a funsão retornará NULL
    }
    return sent;//caso o nó em questão seja nulo, a função retorna um sentinela passado incialmente como parâmetro
}

NO* cimaPraBaixo (NO* p, int maior, int menor, NO** pai){
    if (p){
        if (p->chave > maior || p->chave < menor)
            return p;
        NO* aux = cimaPraBaixo(p->esq, p->chave, menor, pai);
        if (aux){
            if (!*pai)
                *pai = p;
            return aux;
        }
        aux = cimaPraBaixo(p->dir, maior, p->chave, pai);
        if (aux){
            if (!*pai)
                *pai = p;
            return aux;
        }
    }
    return NULL;
}

bool teste (NO* p, int max, int min){
    if (p){
        if (p->chave > max || p->chave < min)
            return false;
        return teste(p->dir, max, p->chave) && teste(p->esq, p->chave, min);
    }
    return true;
}

void reinserir (NO* p, NO* raiz){
    NO* atual = raiz;
    NO* ant = NULL;
    while (atual){
        ant = atual;
        if (p->chave > atual->chave)
            atual = atual->dir;
        else
            atual = atual->esq;
    }
    if (p->chave > ant->chave)
        ant->dir = p;
    else
        ant->esq = p;
}

NO* copia (NO*p){
    if (p){
        NO* novo = (NO*) malloc(sizeof(NO));
        novo->chave = p->chave;
        novo->dir = copia(p->dir);
        novo->esq = copia(p->esq);
        return novo;
    }
    return NULL;
}

void arrumar (NO* p, NO* pai, NO** raiz){
    NO* ant = NULL;
    NO* esq = p->esq;
    if (esq){
        while (esq->dir){
            ant = esq;
            esq = esq->dir;
        }
        if (ant && esq->esq){
            ant->dir = esq->esq;
            esq->esq = p->esq;
        }else if (ant && !esq->esq){
            ant->dir = NULL;
            esq->esq = p->esq;
        }

        esq->dir = p->dir;
        if (pai){
            if (pai->dir == p)
                pai->dir = esq;
            else
                pai->esq = esq;

        }else
            *raiz = esq;
    }else{
        NO* dir = p->dir;
        if (dir){
            while (dir->esq){
                ant = dir;
                dir = dir->esq;
            }
            if (ant && dir->dir){
                ant->esq = dir->dir;
                dir->dir = p->dir;
            }else if (ant && !dir->dir){
                ant->esq = NULL;
                dir->dir = p->dir;
            }
            dir->esq = p->esq;
            if (pai){
                if (pai->dir == p)
                    pai->dir = dir;
                else
                    pai->esq = dir;
            }else
                *raiz = dir;
        }else {
            if (pai->dir == p)
                pai->dir = NULL;
            else
                pai->esq = NULL;
        }
    }
    p->esq = NULL;
    p->dir = NULL;
    reinserir(p, *raiz);
}

void organizar(NO* *raiz);





//------------------------------------------
// O EP consiste em implementar esta funcao
//------------------------------------------
void organizar(NO* *raiz) {
    NO* aux = copia(*raiz);
    NO* sent = (NO*) malloc(sizeof(NO));
    NO* pai = NULL;
    NO* p = baixoPraCima(aux, &pai, sent);
    arrumar (p, pai, &aux);
    int a = INFINITY;
    if (teste(aux, a, -a))
        *raiz = aux;
    else{
        pai = NULL;
        p = cimaPraBaixo(*raiz, a, -a, &pai);
        arrumar(p,pai, raiz);
    }
}



//---------------------------------------------------------
// use main() para fazer chamadas de teste ao seu programa
//---------------------------------------------------------
int main() {

	NO* arv = NULL;


	// serao realizadas chamadas como esta:
	organizar(&arv);
}

// por favor nao inclua nenhum código abaixo da função main()
