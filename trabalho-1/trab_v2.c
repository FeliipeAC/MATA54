#include <stdio.h>
#include <conio.h>
#include <string.h>
#define TAMANHO_ARQUIVO 11

typedef struct
{
    int key;
    char name[21];
    int age;
    int pos;
    int status;
} registro;

// Função para adição de registros no arquivo
void add_reg (FILE *arq, registro reg_s, int index_l) {
    registro reg_aux;
    /* Posiciona ponteiro do arquivo de registros de acordo com o índice fornecido */ 
    fseek(arq, sizeof(registro)*index_l, SEEK_SET);

    /* Verifica disponibilidade da posição. Se resposta da leitura do arquivo na posição 
    apontada for igual a 1, então a posição já foi explorada */
    if((fread(&reg_aux, sizeof(registro), 1, arq)) == 1) {
        /* Se 'status' do registro for igual a 0, então a posição já foi utilizada, 
        mas no momento está livre */
        if (reg_aux.status == 0) {
            fseek(arq, sizeof(registro)*index_l, SEEK_SET);
            reg_s.pos = index_l;
            reg_s.status = 1;
            fwrite(&reg_s, sizeof(registro), 1, arq);
            
        }
        /* Se 'status' diferente de 0, posição ocupada */
        else
            printf("%s %d\n", "posicao ocupada:", index_l);
            
    }
    /* Se diferente de 1, posição ainda não explorada */
    else {
        fseek(arq, sizeof(registro)*index_l, SEEK_SET);
        reg_s.pos = index_l;
        reg_s.status = 1;
        fwrite(&reg_s, sizeof(registro), 1, arq);

    }  
    
}

// Função de consulta de registro no arquivo
void consult_reg(FILE *arq, int index_c) {
    registro reg_consult;

    /* Posiciona ponteiro na posição requerida e faz a leitura */
    fseek(arq, sizeof(registro)*index_c, SEEK_SET);
    

    /* Ler o arquivo na posição direcionada e verifica. Se a resposta for 0, então 
    posição ainda não explorada */
    if(fread(&reg_consult, sizeof(registro), 1, arq) == 0)
        printf("%s %d\n", "posicao vazia:", index_c);

    /* Se diferente de 0, posição já utilizada. Verifica se atualmente está livre ou ocupada */   
    else {
        fseek(arq, sizeof(registro)*index_c, SEEK_SET);
        fread(&reg_consult, sizeof(registro), 1, arq);

        if (reg_consult.status == 1)
            printf("%s %d %s %d\n","chave:", reg_consult.key, reg_consult.name, reg_consult.age );
        else 
            printf("%s %d\n", "posicao vazia:", index_c);
    }
}

// Função para remoção de registro do arquivo 
void remove_reg(FILE *arq, int index_r) {
    registro reg_rem;

    /* Posiciona ponteiro na posição requerida e faz a leitura */
    fseek(arq, sizeof(registro)*index_r, SEEK_SET);
    
    
    /* Ler o arquivo na posição requerida, se a resposta for diferente de 1 então a 
    posição não foi descoberta ainda */
    if((fread(&reg_rem, sizeof(registro), 1, arq)) != 1)
        printf("%s %d\n", "posicao ja estava vazia:", index_r);

    /* Se igual a 1, posição já explorada. Verifica se atualmente está livre ou ocupada */ 
    else {
        fseek(arq, sizeof(registro)*index_r, SEEK_SET);
        fread(&reg_rem, sizeof(registro), 1, arq);
        
        if (reg_rem.status == 0)
            printf("%s %d\n", "posicao ja estava vazia:", reg_rem.pos);
        else {
            reg_rem.status = 0;
            fseek(arq, sizeof(registro)*index_r, SEEK_SET);
            fwrite(&reg_rem, sizeof(registro), 1, arq);
        }
    }
}

// Função para impressão dos registros do arquivo
void print_reg(FILE *arq) {
    
    registro reg_print; 
    int i;
        rewind(arq);
        
        for (i = 0; i < TAMANHO_ARQUIVO; i++){
            fread(&reg_print, sizeof(registro), 1, arq);
            if(ferror(arq)){
                printf("%d: %s\n", i, "vazio");
            }
            else if (feof(arq)) {
                printf("%d: %s\n", i, "vazio");
            }
            else {
                fseek(arq, sizeof(registro)*i, SEEK_SET);
                fread(&reg_print, sizeof(registro), 1, arq);
                if (reg_print.status == 1)
                    printf("%d: %d %s %d\n", reg_print.pos, reg_print.key, reg_print.name, reg_print.age);               
                else
                    printf("%d: %s\n", i, "vazio"); 
            }
        }
    
}

int main(void)
{
    // Arquivo de registros
    FILE *arq; 

    // Referência a estrutura Registro
    registro reg;

    int index;
    char op;

    // Abre o arquivo para leitura e escrita
    arq = fopen("registros2.dat", "r+b");

    // Se o arquivo não existir cria no modo leitura e escrita
    if (arq == NULL) {
        arq = fopen("registros2.dat", "w+b");
    }


    while (op != 'e') {
        switch(op)
        {
            case 'i':
                scanf("%d", &reg.key);
                fflush(stdin);
                gets(reg.name);
                scanf("%d", &reg.age);
                scanf("%d", &index);
                add_reg(arq, reg, index);
                
                break;
            case 'r':
                scanf("%d", &index);
                remove_reg(arq, index);
                break;
            case 'c':
                scanf("%d", &index);
                consult_reg(arq, index);
                break;
            case 'p':
                print_reg(arq);
                break;
        }
        scanf("%c", &op);
    }

    // Fecha o arquivo
    fclose(arq);
    return 0;
}