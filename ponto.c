#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Ponto eletrônico para controle de funcionários - Universidade Positivo
//Luiz Felipe
//Luis Arthur

//Structs

//Struct para funcionarios
typedef struct {
    char nome[50];
    int matricula;
    char funcao[30];
    int horasTrabalhadas;
} Funcionario;

//Struct para data
typedef struct {
    int dia;
    int mes;
    int ano;
} Data;

//Struct principal para registro de ponto.
typedef struct {
    Funcionario funcionario;    //Struct funcionario
    Data data;                  //Struct data
    char entrada[6];
    char intervaloIni[6];
    char intervaloFim[6];
    char saida[6];
} RegistroPonto;

// Funcoes auxiliares para ordenar o vetor de funcionarios por horasTrabalhadas

// Funcao para trocar dois funcionarios de posicao durante a ordenacao
void swapFuncionario(Funcionario *a, Funcionario *b) {
    Funcionario tmp = *a;
    *a = *b;
    *b = tmp;
}

// Funcao de particao para o quicksort
int particaoFuncionarios(Funcionario vetor[], int inicio, int fim) {
    
    int pivo = vetor[(inicio + fim) / 2].horasTrabalhadas;
    int i = inicio;
    int j = fim;
    
    while (i <= j) {
        while (vetor[i].horasTrabalhadas > pivo) i++;
        while (vetor[j].horasTrabalhadas < pivo) j--;
        if (i <= j) {
            swapFuncionario(&vetor[i], &vetor[j]);
            i++;
            j--;
        }
    }
    return i;

}

// Funcao principal do quicksort
void quicksortFuncionarios(Funcionario vetor[], int inicio, int fim) {

    if (inicio >= fim) return; // Caso base
    int index = particaoFuncionarios(vetor, inicio, fim);
    if (inicio < index - 1)
        quicksortFuncionarios(vetor, inicio, index - 1);
    if (index < fim)
        quicksortFuncionarios(vetor, index, fim);

}

// Funcao para listar funcionarios ordenados por horas trabalhadas
void listarFuncionarios(Funcionario *vetor, int quantidade) { 

    printf("\n====== LISTA DE FUNCIONARIOS ======\n");
    
    // Verifica se ha funcionarios cadastrados
    if (quantidade == 0) {
        printf("Nenhum funcionario cadastrado.\n");
        printf("Pressione Enter para continuar...");
        while (getchar() != '\n'); 
        getchar(); 
        return;
    }

    // Criar uma copia do vetor para nao alterar a ordem original
    Funcionario *copia = malloc(quantidade * sizeof(Funcionario));
    if (copia == NULL) {
        printf("Erro de alocacao de memoria para ordenacao.\n");
        return;
    }
    memcpy(copia, vetor, quantidade * sizeof(Funcionario));

    // Ordena a copia (maior horas trabalhadas primeiro)
    quicksortFuncionarios(copia, 0, quantidade - 1);

    // Imprime a lista ordenada
    printf("\nFuncionarios ordenados por horas trabalhadas (maior para menor):\n");
    for (int i = 0; i < quantidade; i++) {
        printf("%d) Nome: %s | Matricula: %d | Funcao: %s | Horas trabalhadas: %d\n",
               i + 1,
               copia[i].nome,
               copia[i].matricula,
               copia[i].funcao,
               copia[i].horasTrabalhadas);
    }

    free(copia);

    printf("\nPressione Enter para continuar...");
    while (getchar() != '\n'); 
    getchar(); 

}

// Funcao para cadastrar funcionarios
void cadastroFuncionario(Funcionario **vetor, int *quantidade) { 
    
    printf("\n===== CADASTRO DE FUNCIONARIO =====");

    // Loop para cadastrar varios funcionarios
    char continuar; 
    do {

        // Realoca memoria para mais um funcionario
        Funcionario *temp = realloc(*vetor, (*quantidade + 1) * sizeof(Funcionario)); 
        if (temp == NULL) { 
            printf("Erro de alocacao de memoria para funcionario!\n");
            return; 
        }
        *vetor = temp; 
        
        // Coleta os dados do novo funcionario
        printf("\nCadastro de Funcionario ID %d:\n", *quantidade + 1);
        printf("Nome: ");
        scanf(" %49[^\n]", (*vetor)[*quantidade].nome);
        printf("Matricula: ");
        scanf("%d", &(*vetor)[*quantidade].matricula);
        printf("Funcao: ");
        scanf(" %29[^\n]", (*vetor)[*quantidade].funcao);
        (*vetor)[*quantidade].horasTrabalhadas = 0; // Inicializa as horas trabalhadas como 0
        (*quantidade)++; 

        // Mostra os dados cadastrados
        printf("\nMostrando dados cadastrados:\n");
        printf("Nome: %s\n", (*vetor)[*quantidade - 1].nome);
        printf("Matricula: %d\n", (*vetor)[*quantidade - 1].matricula);
        printf("Funcao: %s\n", (*vetor)[*quantidade - 1].funcao);

        printf("\nDeseja cadastrar outro funcionario?  b(s/n): ");
        scanf(" %c", &continuar);

    } while (continuar == 's' || continuar == 'S');
    
}

void marcarPonto(   Funcionario *vetorfuncionarios, int quantidadeFuncionarios,
                    Data **vetorData, int *quantidadeData,
                    RegistroPonto **vetorregistros, int *quantidadeRegistros) {

    printf("\n======== MARCACAO DE PONTO ========\n");

    // Coleta a data atual
    int tmpdia, tmpmes, tmpano;
    printf("Qual o dia de hoje? (dd mm aaaa): ");
    scanf("%d %d %d", &tmpdia, &tmpmes, &tmpano);
    int dataEncontrada = 0; 
    int posicaoData = -1; 
    for (int i = 0; i < *quantidadeData; i++) { // Loop para verificar se a data ja existe no vetor
        if ((*vetorData)[i].dia == tmpdia &&
            (*vetorData)[i].mes == tmpmes &&
            (*vetorData)[i].ano == tmpano) {
            dataEncontrada = 1;
            posicaoData = i;
            break;
        }
    }
    
    // Se a data nao foi encontrada, adiciona uma nova data ao vetor
    if (!dataEncontrada) { 

        // Aloca memoria para a nova data
        Data *tempData = realloc(*vetorData, (*quantidadeData + 1) * sizeof(Data));
        if (tempData == NULL) {
            printf("Erro de alocacao de memoria para data!\n");
            return;
        }
        *vetorData = tempData;

        // Adiciona os valores da nova data
        (*vetorData)[*quantidadeData].dia = tmpdia;  
        (*vetorData)[*quantidadeData].mes = tmpmes;
        (*vetorData)[*quantidadeData].ano = tmpano;
        posicaoData = *quantidadeData;
        (*quantidadeData)++;
    }

    // Loop para marcar ponto para varios funcionarios
    char continuar;
    do {
        
        // Busca o funcionario pela matricula
        int matriculaBusca;
        int encontrado = 0;
        int posicaoFuncionario = -1;
        while (1) {
            printf("\nDigite a matricula do funcionario: ");
            scanf("%d", &matriculaBusca);
            for (int i = 0; i < quantidadeFuncionarios; i++) { // Loop para buscar a matricula no vetor de funcionarios
                if (vetorfuncionarios[i].matricula == matriculaBusca) {
                    encontrado = 1;
                    posicaoFuncionario = i;}
                }
        
            if (encontrado) {
                printf("Funcionario encontrado: %s\n", vetorfuncionarios[posicaoFuncionario].nome); 
                break;
            } else {
                printf("Funcionario com matricula %d nao encontrado. Tente novamente\n", matriculaBusca);
            }
        }

        // Verifica se o funcionario ja tem um registro de ponto para a data atual
        int registroExistente = 0;
        for (int i = 0; i < *quantidadeRegistros; i++) {
            if ((*vetorregistros)[i].funcionario.matricula == matriculaBusca &&
                (*vetorregistros)[i].data.dia == tmpdia &&
                (*vetorregistros)[i].data.mes == tmpmes &&
                (*vetorregistros)[i].data.ano == tmpano) {
                registroExistente = 1;
                break;
            }
        }

        // Se nao existir, cria um novo registro de ponto
        if (!registroExistente) {
            RegistroPonto *tempRegistro = realloc(*vetorregistros, (*quantidadeRegistros + 1) * sizeof(RegistroPonto));
            if (tempRegistro == NULL) {
                printf("Erro de alocacao de memoria para registro de ponto!\n");
                return;
            }
            *vetorregistros = tempRegistro;
            RegistroPonto *novoRegistro = &(*vetorregistros)[*quantidadeRegistros];
            novoRegistro->funcionario = vetorfuncionarios[posicaoFuncionario];
            novoRegistro->data.dia = tmpdia;
            novoRegistro->data.mes = tmpmes;
            novoRegistro->data.ano = tmpano;
            novoRegistro->entrada[0] = '\0';
            novoRegistro->intervaloIni[0] = '\0';
            novoRegistro->intervaloFim[0] = '\0';
            novoRegistro->saida[0] = '\0';
            (*quantidadeRegistros)++;
        }
        
        // Marca o ponto
        char temphorario[6];
        char tipo [20];
        printf("Digite o horario atual (hh:mm): ");
        scanf(" %5s", temphorario);
        for (int i = 0; i < *quantidadeRegistros; i++) {
            if ((*vetorregistros)[i].funcionario.matricula == matriculaBusca &&
                (*vetorregistros)[i].data.dia == tmpdia &&
                (*vetorregistros)[i].data.mes == tmpmes &&
                (*vetorregistros)[i].data.ano == tmpano) {
                
                // Verifica qual horario deve ser marcado
                int horaE, minutoE, horaEI, minutoEI, horaSI, minutoSI, horaS, minutoS;
                if ((*vetorregistros)[i].entrada[0] == '\0') {
                    strcpy((*vetorregistros)[i].entrada, temphorario);
                    strcpy(tipo, "entrada");
                    sscanf(temphorario, "%d:%d", &horaE, &minutoE);
                } else if ((*vetorregistros)[i].intervaloIni[0] == '\0') {
                    strcpy((*vetorregistros)[i].intervaloIni, temphorario);
                    strcpy(tipo, "inicio do intervalo");
                    sscanf(temphorario, "%d:%d", &horaEI, &minutoEI);
                } else if ((*vetorregistros)[i].intervaloFim[0] == '\0') {
                    strcpy((*vetorregistros)[i].intervaloFim, temphorario);
                    strcpy(tipo, "fim do intervalo");
                    sscanf(temphorario, "%d:%d", &horaSI, &minutoSI);
                } else if ((*vetorregistros)[i].saida[0] == '\0') {
                    strcpy((*vetorregistros)[i].saida, temphorario);
                    strcpy(tipo, "saida");
                    sscanf(temphorario, "%d:%d", &horaS, &minutoS);
                    
                    // Calcula as horas trabalhadas
                    int totalMinutos = (horaS * 60 + minutoS) - (horaE * 60 + minutoE) - ((horaSI * 60 + minutoSI) - (horaEI * 60 + minutoEI));
                    float horasTrabalhadas = (float)totalMinutos / 60.0;
                    int horasArredondadas = (int)(horasTrabalhadas + 0.5); 
                    vetorfuncionarios[posicaoFuncionario].horasTrabalhadas += horasArredondadas;
                    printf("Horas trabalhadas hoje para %s: %d horas\n", vetorfuncionarios[posicaoFuncionario].nome, horasArredondadas);
                } else {
                    printf("Todos os horarios ja foram marcados para este funcionario hoje.\n");
                    tipo[0] = '\0'; 
                }
            }
        }

        // Mostra o registro atualizado
        if (tipo[0] != '\0'){
            printf("\nRegistro de %s atualizado para %s em %d/%d/%d: %s \n", vetorfuncionarios[posicaoFuncionario].nome, tipo, tmpdia, tmpmes, tmpano, temphorario);
        }

        printf("Deseja marcar ponto para outro funcionario? (s/n): ");
        scanf(" %c", &continuar);

    } while (continuar == 's' || continuar == 'S');
}

    // Funcao para buscar registros de ponto
void buscarPonto(RegistroPonto *vetorRegistros, int quantidadeRegistros) {

    printf("\n===== BUSCA REGISTRO DE PONTO =====\n"); 

    do {
    printf("Deseja buscar por (1) Matricula ou (2) Data? ");
    int escolha;
    scanf("%d", &escolha);
    if (escolha == 1) {
        int matriculaBusca;
        int encontrado = 0;
        printf("Digite a matricula do funcionario: ");
        scanf("%d", &matriculaBusca);
        printf("Registros de ponto para o funcionario com matricula %d:\n", matriculaBusca);
        for (int i = 0; i < quantidadeRegistros; i++) {
            if (vetorRegistros[i].funcionario.matricula == matriculaBusca) {
                printf("Data: %02d/%02d/%04d | Entrada: %s | Inicio Intervalo: %s | Fim Intervalo: %s | Saida: %s\n",
                       vetorRegistros[i].data.dia,
                       vetorRegistros[i].data.mes,
                       vetorRegistros[i].data.ano,
                       vetorRegistros[i].entrada,
                       vetorRegistros[i].intervaloIni,
                       vetorRegistros[i].intervaloFim,
                       vetorRegistros[i].saida);
                encontrado = 1;
            }
        }
        if (!encontrado) {
            printf("Nenhum registro de ponto encontrado para a matricula %d.\n", matriculaBusca);
        }
    
    } 
    
    else if (escolha == 2) {
        int diaBusca, mesBusca, anoBusca;
        int encontrado = 0;
        printf("Digite a data (dd mm aaaa): ");
        scanf("%d %d %d", &diaBusca, &mesBusca, &anoBusca);
        printf("Registros de ponto para a data %02d/%02d/%04d:\n", diaBusca, mesBusca, anoBusca);
        for (int i = 0; i < quantidadeRegistros; i++) {
            if (vetorRegistros[i].data.dia == diaBusca &&
                vetorRegistros[i].data.mes == mesBusca &&
                vetorRegistros[i].data.ano == anoBusca) {
                printf("Funcionario: %s | Matricula: %d | Entrada: %s | Inicio Intervalo: %s | Fim Intervalo: %s | Saida: %s\n",
                       vetorRegistros[i].funcionario.nome,
                       vetorRegistros[i].funcionario.matricula,
                       vetorRegistros[i].entrada,
                       vetorRegistros[i].intervaloIni,
                       vetorRegistros[i].intervaloFim,
                       vetorRegistros[i].saida);
                encontrado = 1;
            }
        }
        if (!encontrado) {
            printf("Nenhum registro de ponto encontrado para a data %02d/%02d/%04d.\n", diaBusca, mesBusca, anoBusca);
        }

    } else {
        printf("Opcao invalida.\n");
    }
    char continuar;
    printf("Deseja realizar outra busca? (s/n): ");
    scanf(" %c", &continuar);
    if (continuar != 's' && continuar != 'S') {
        break;
    } 
    } while (1);
}

int main() {
    
    Funcionario *funcionarios = NULL; //Começa toda zerada
    int quantidadeFuncionarios = 0; //Quantidade inicial de funcionarios
    
    Data *data = NULL;  //Vetor de datas
    int quantidadeData = 0; //Quantidade inicial de datas

    RegistroPonto *registros = NULL; //Vetor de registros de ponto
    int quantidadeRegistros = 0; //Quantidade inicial de registros de ponto

    int opcao;
    do {
        printf("\n=========== MENU INICIO ===========\n");
        printf("1 - Marcar ponto\n");
        printf("2 - Cadastrar funcionario\n");
        printf("3 - Horas trabalhadas\n");
        printf("4 - Buscar ponto\n");
        printf("0 - Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                marcarPonto(funcionarios, quantidadeFuncionarios, &data, &quantidadeData, &registros, &quantidadeRegistros);
                break;
            case 2:
                cadastroFuncionario(&funcionarios, &quantidadeFuncionarios);
                break;
            case 3:
                listarFuncionarios(funcionarios, quantidadeFuncionarios);
                break;
            case 4:
                buscarPonto(registros, quantidadeRegistros);
                break;
            case 0:
                printf("\nEncerrando o programa...\n");
                break;
            default:
                printf("\nOpcao invalida!\n");
        }
    } while (opcao != 0);

    free(funcionarios);
    free(data);
    free(registros);

    return 0;
}
