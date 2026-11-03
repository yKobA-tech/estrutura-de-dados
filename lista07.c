

#include <stdio.h>
#include <stdlib.h>
#include "ListaDinEncadDupla.h"

/* Definicao do tipo lista: oculta de quem usa a biblioteca.
   Com struct aluno de 48 bytes, sizeof(Elem) = 64 bytes em uma
   plataforma de 64 bits, contra 56 na lista encadeada simples. */
struct elemento {
    struct elemento *ant;
    struct aluno dados;
    struct elemento *prox;
};
typedef struct elemento Elem;

/* --- criacao e destruicao ---------------------------------- */

Lista* cria_lista(void) {
    Lista* li = (Lista*) malloc(sizeof(Lista));
    if (li != NULL)
        *li = NULL;
    return li;
}

void libera_lista(Lista* li) {
    if (li != NULL) {
        Elem* no;
        while ((*li) != NULL) {
            no = *li;
            *li = (*li)->prox;   /* avanca antes de liberar */
            free(no);
        }
        free(li);                /* libera o bloco do inicio */
    }
}

/* --- informacoes de estado --------------------------------- */

int tamanho_lista(Lista* li) {
    if (li == NULL)
        return 0;
    int cont = 0;
    Elem* no = *li;
    while (no != NULL) {
        cont++;
        no = no->prox;
    }
    return cont;
}

int lista_cheia(Lista* li) {
    (void) li;               /* parametro mantido por simetria com as demais */
    return 0;
}

int lista_vazia(Lista* li) {
    if (li == NULL)
        return 1;            /* lista invalida */
    if (*li == NULL)
        return 1;            /* nenhum elemento alocado */
    return 0;
}

/* --- insercao ---------------------------------------------- */

int insere_lista_inicio(Lista* li, struct aluno al) {
    if (li == NULL) return 0;
    Elem* no = (Elem*) malloc(sizeof(Elem));
    if (no == NULL) return 0;    /* sem memoria: a lista esta cheia */
    no->dados = al;
    no->prox = (*li);
    no->ant = NULL;
    if (*li != NULL)             /* lista nao vazia */
        (*li)->ant = no;         /* o antigo primeiro ganha antecessor */
    *li = no;
    return 1;
}

int insere_lista_final(Lista* li, struct aluno al) {
    if (li == NULL) return 0;
    Elem* no = (Elem*) malloc(sizeof(Elem));
    if (no == NULL) return 0;
    no->dados = al;
    no->prox = NULL;
    if ((*li) == NULL) {         /* lista vazia: insere inicio */
        no->ant = NULL;
        *li = no;
    } else {
        Elem* aux = *li;
        while (aux->prox != NULL) {
            aux = aux->prox;
        }
        aux->prox = no;
        no->ant = aux;
    }
    return 1;
}

int insere_lista_ordenada(Lista* li, struct aluno al) {
    if (li == NULL) return 0;
    Elem* no = (Elem*) malloc(sizeof(Elem));
    if (no == NULL) return 0;
    no->dados = al;
    if ((*li) == NULL) {         /* lista vazia: insere inicio */
        no->prox = NULL;
        no->ant = NULL;
        *li = no;
        return 1;
    }
    else {
        /* O livro declara ante sem valor inicial. A inicializacao com
           NULL nao altera o comportamento e elimina o aviso
           "may be used uninitialized" do gcc com -O2. */
        Elem *ante = NULL, *atual = *li;
        while (atual != NULL &&
               atual->dados.matricula < al.matricula) {
            ante = atual;
            atual = atual->prox;
        }
        if (atual == *li) {      /* insere inicio */
            no->ant = NULL;
            (*li)->ant = no;
            no->prox = (*li);
            *li = no;
        } else {                 /* insere no meio ou no final */
            no->prox = ante->prox;
            no->ant = ante;
            ante->prox = no;
            if (atual != NULL)   /* nao e o ultimo */
                atual->ant = no;
        }
        return 1;
    }
}

/* --- remocao ----------------------------------------------- */

int remove_lista_inicio(Lista* li) {
    if (li == NULL) return 0;
    if ((*li) == NULL) return 0;   /* lista vazia */
    Elem* no = *li;
    *li = no->prox;
    if (no->prox != NULL)          /* existe um seguinte */
        no->prox->ant = NULL;      /* ele passa a ser o primeiro */
    free(no);
    return 1;
}

int remove_lista_final(Lista* li) {
    if (li == NULL) return 0;
    if ((*li) == NULL) return 0;   /* lista vazia */
    Elem* no = *li;
    while (no->prox != NULL)
        no = no->prox;
    if (no->ant == NULL)           /* primeiro e unico */
        *li = no->prox;
    else
        no->ant->prox = NULL;      /* penultimo obtido pelo proprio no */
    free(no);
    return 1;
}

int remove_lista(Lista* li, int mat) {
    if (li == NULL) return 0;
    if ((*li) == NULL) return 0;   /* lista vazia */
    Elem* no = *li;
    while (no != NULL && no->dados.matricula != mat)
        no = no->prox;
    if (no == NULL)                /* chave ausente */
        return 0;
    if (no->ant == NULL)           /* remove o primeiro */
        *li = no->prox;
    else
        no->ant->prox = no->prox;
    if (no->prox != NULL)          /* nao e o ultimo */
        no->prox->ant = no->ant;
    free(no);
    return 1;
}

/* --- busca ------------------------------------------------- */

int busca_lista_pos(Lista* li, int pos, struct aluno *al) {
    if (li == NULL || pos <= 0)
        return 0;
    Elem *no = *li;
    int i = 1;
    while (no != NULL && i < pos) {
        no = no->prox;
        i++;
    }
    if (no == NULL)
        return 0;
    else {
        *al = no->dados;
        return 1;
    }
}

int busca_lista_mat(Lista* li, int mat, struct aluno *al) {
    if (li == NULL)
        return 0;
    Elem *no = *li;
    while (no != NULL && no->dados.matricula != mat)
        no = no->prox;
    if (no == NULL)
        return 0;
    else {
        *al = no->dados;
        return 1;
    }
}

/*Funções criadas a partir da Lista 07*/

void imprime_lista_inversa(Lista *li){
    if(li == NULL || *li == NULL) {
        printf("Lista Vazia");
        return;
    }
    Elem *no = *li;
    while(no->prox != NULL){
        no = no->prox;
    }
    while (no != NULL){
        printf("%d ",no->dados.matricula);
        no = no->ant;
    }
    return;
}
/*A função possui complexidade O(n) por que passa por dois laços while que não estão encadeados,
por isso, ela não é O(n²)*/

int lista_consistente(Lista *li){
    if(li == NULL) return 0;

    if(*li == NULL) return 1;

    Elem *no = *li;

    if(no->ant == NULL){
        while(no->prox != NULL){
            if( no->prox->ant != no) return 0;
            no = no->prox;
        }
    }
    else return 0;

    return 1;
}

/*Questão 3:
a) A função não trata do caso de ter somente um dado ou a matricula procurada ser o primeiro dado,
fazendo com que aponte o ponteiro ant(NULL) para o no->prox, ocasionando um erro crítico
b) NULL <-33-> <-16-> NULL
c) */

int remove_lista(Lista* li, int mat){
    if(li == NULL) return 0;
    if((*li) == NULL) return 0;
    Elem* no = *li;
    while(no != NULL && no->dados.matricula != mat)
        no = no->prox;
    if(no == NULL) return 0;

    if(no == *li){
        if(no->prox != NULL){
            no->prox->ant = NULL;
            *li = no->prox;
        }
        else *li = NULL;

        free(no);
        return 1;

    }
    if(no->prox == NULL){
        no->ant->prox = NULL;
        free(no);
        return 1;
    }
    no->ant->prox = no->prox;
    no->prox->ant = no->ant;
    free(no);
    return 1;
}

int remove_lista_pos(Lista *li, int pos){
    if(lista_consistente(li)){
        int cont = 1;
        Elem* no = *li;
        while (cont < pos && no != NULL){
            no = no->prox;
            cont++;
        }
        if(no == NULL) return 0;

        if(no == *li){
             if(no->prox != NULL){
            no->prox->ant = NULL;
            *li = no->prox;
        }
        else *li = NULL;

        free(no);
        return 1;
        }

        if(no->prox == NULL){
            no->ant->prox = NULL;
            free(no);
            return 1;
        }
        no->ant->prox = no->prox;
        no->prox->ant = no->ant;
        free(no);
        return 1;
    }  
    return 0;
}
/*O custo para essa função é O(n), pois temos apenas um laço while que corre n vezes a partir do valor de pos*/

int move_para_inicio(Lista *li, int mat){
    if(lista_consistente(li)){
        Elem* no = *li;
        while(no != NULL && no->dados.matricula != mat)
        no = no->prox;
        if(no == NULL) return 0;

        if(no == *li) return 1;

        if(no->prox == NULL){
            no->ant->prox = NULL;
            no->prox = *li;
            no->ant = NULL;
            no->prox->ant = no;
            *li = no;
            return 1;
        }

        no->ant->prox = no->prox;
        no->prox->ant = no->ant;
        no->prox = *li;
        no->ant = NULL;
        no->prox->ant = no;
        *li = no;
        return 1;
    }
    return 0;
}
/*Quando o elemento está no meio: São escritos 5 campos de ponteiro de nós:
no->ant->prox = no->prox; (ponteiro prox do antecessor)

no->prox->ant = no->ant; (ponteiro ant do sucessor)

no->prox = *li; (ponteiro prox do próprio nó)

no->ant = NULL; (ponteiro ant do próprio nó)

no->prox->ant = no; (ponteiro ant do antigo primeiro elemento)

Quando o elemento é o último: São escritos 4 campos de ponteiro de nós:
no->ant->prox = NULL; (ponteiro prox do antecessor, que passa a ser o novo fim)

no->prox = *li; (ponteiro prox do próprio nó)

no->ant = NULL; (ponteiro ant do próprio nó)

no->prox->ant = no; (ponteiro ant do antigo primeiro elemento)*/

int remove_todos(Lista *li, int mat){
    if(lista_consistente(li)){
        Elem *no = *li;
        int cont = 0;
        while(no != NULL){
            Elem * no_prox = no->prox;
            if(no->dados.matricula == mat){
               if(no->prox != NULL) no->prox->ant = no->ant;
                if(no->ant != NULL) no->ant->prox = no->prox;
                if(no == *li) *li = no->prox;
                free(no);
                cont++;
            }
            no = no_prox;
        }
        return cont;
    }
    return 0;
}

void inverte_lista(Lista *li){
    if(li != NULL && lista_consistente(li) && *li != NULL){
        Elem* atual = *li;
        Elem* temp = NULL;
        Elem* ultimo = NULL;
        
        while(atual != NULL){
            temp = atual->ant;
            atual->ant = atual->prox;
            atual->prox = temp;
            
            ultimo = atual;         
            atual = atual->ant;      
        }
        
        *li = ultimo; 
    }
}
/*Porque numa lista duplamente encadeada cada nó possui ligações diretas para ambos os lados (ant e prox). 
Ao inverter estes dois campos em todos os nós da lista, invertemos simultaneamente a direção de todas as conexões. 
Como a estrutura física dos nós se mantém intacta (sem necessidade de alocações ou libertações de memória), 
basta atualizar o ponteiro principal da lista (*li) para que passe a apontar para o elemento que estava originalmente no fim.

O(N) (linear), pois percorre cada um dos N elementos da lista exatamente uma vez para inverter os seus ponteiros.*/

int eh_palindromo(Lista *li) {
    if (li == NULL || !lista_consistente(li)) return 0;
    if (*li == NULL) return 1;

    Elem *inicio = *li;
    Elem *fim = *li;

    while (fim->prox != NULL) {
        fim = fim->prox;
    }

    while (inicio != fim && inicio->ant != fim) {
        if (inicio->dados.matricula != fim->dados.matricula) {
            return 0;
        }
        inicio = inicio->prox;
        fim = fim->ant;
    }

    return 1;
}
/*O(N) (linear), pois percorremos a lista uma vez para encontrar o fim e, no pior caso, 
os dois ponteiros cruzam-se a meio, visitando coletivamente todos os N nós.

Numa lista simplesmente encadeada, cada nó tem apenas o ponteiro prox (aponta para a frente). 
Não conseguimos caminhar de trás para a frente (ant), o que tornaria impossível recuar a partir do final 
sem percorrer a lista inteira repetidas vezes ou sem utilizar memória auxiliar (como uma pilha ou um vetor). 
Já na lista duplamente encadeada, temos acesso direto ao ponteiro ant, permitindo avançar do início e recuar do fim de forma eficiente.*/