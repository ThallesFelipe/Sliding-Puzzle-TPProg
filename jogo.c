/*
    Feito por Thalles Felipe.

        Este foi meu primeiro trabalho prático que fiz na minha graduação de Ciência da Computação na Universidade Federal de Ouro Preto.
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define ANSI_RESET "\x1b[0m"
#define ANSI_BOLD "\x1b[1m"
#define ANSI_COLOR_RED "\x1b[31m"
#define ANSI_COLOR_GREEN "\x1b[32m"
#define ANSI_COLOR_YELLOW "\x1b[33m"
#define ANSI_BG_COLOR_GREEN "\x1b[42m"
#define HORIZONTAL_LINE "==============="
#define VERTICAL_LINE "|"

#define BOLD(string) ANSI_BOLD string ANSI_RESET
#define RED(string) ANSI_COLOR_RED string ANSI_RESET
#define GREEN(string) ANSI_COLOR_GREEN string ANSI_RESET
#define YELLOW(string) ANSI_COLOR_YELLOW string ANSI_RESET
#define BG_GREEN(string) ANSI_BG_COLOR_GREEN string ANSI_RESET

#define LINCOL 3

int main()
{
    int jogadas = 0;

    int **tabuleiro = calloc(LINCOL, sizeof(int *));
    for (int i = 0; i < LINCOL; i++)
        tabuleiro[i] = calloc(LINCOL, sizeof(int));

    if (tabuleiro == NULL)
    {
        printf("Erro ao alocar memória para o tabuleiro.\n");
        exit(1);
    }

    int opcao;

    do
    {
        opcao = menu(tabuleiro, jogadas);
    } while (opcao != 0);

    return 0;
}

void liberarMatriz(int **tabuleiro)
{
    if (tabuleiro == NULL)
        return;

    for (int i = 0; i < LINCOL; i++)
    {
        free(tabuleiro[i]);
        tabuleiro[i] = NULL;
    }

    free(tabuleiro);
}

void imprimirTabuleiro(int **tabuleiro)
{
    for (int i = 0; i < LINCOL; i++)
    {
        for (int j = 0; j < LINCOL; j++)
        {
            printf(" %2d ", tabuleiro[i][j]);
            if (j != LINCOL - 1)
                printf("%s", VERTICAL_LINE);
        }
        printf("\n");

        if (i != LINCOL - 1)
            printf("%s\n", HORIZONTAL_LINE);
    }
    printf("\n");
}

int menu(int **tabuleiro, int jogadas)
{
    while (1)
    {
        printf(BOLD("\nBem vindo ao Quebra-Cabeças\n\n"));

        printf(RED("0. Sair do Jogo"));
        printf("\n1. Começar um novo jogo\n2. Continuar o jogo atual\n3. Continuar um jogo salvo\n4. Instruções do jogo\n\n");
        printf("Escolha a opção (digite “sair” em qualquer lugar para sair do jogo): ");

        char opcao[20];
        scanf(" %s", opcao);
        int op = atoi(opcao);

        if (strcmp(opcao, "voltar") == 0)
            continue;
        else if (strcmp(opcao, "sair") == 0)
        {
            liberarMatriz(tabuleiro);
            exit(0);
        }

        int numeros, nivel;

        switch (op)
        {
        case 0:
            liberarMatriz(tabuleiro);
            exit(0);
            break;
        case 1:
            numeros = 1;
            nivel = 0;

            jogadas = 0;
            for (int i = 0; i < LINCOL; i++)
            {
                for (int j = 0; j < LINCOL; j++)
                {
                    tabuleiro[i][j] = numeros;
                    numeros++;
                }
            }
            tabuleiro[LINCOL - 1][LINCOL - 1] = 0;

            printf("\nTabuleiro ordenado!\n");
            imprimirTabuleiro(tabuleiro);

            while (1)
            {
                printf("Digite o nível de dificuldade para embaralhar, " BOLD(GREEN("fácil (1)")) ", " BOLD(YELLOW("médio (2)")) ", ou " BOLD(RED("difícil (3)")) ": ");
                scanf(" %s", opcao);

                if (strcmp(opcao, "voltar") == 0)
                    break;
                else if (strcmp(opcao, "sair") == 0)
                {
                    liberarMatriz(tabuleiro);
                    exit(0);
                }
                else
                {
                    nivel = atoi(opcao);
                    if (nivel >= 1 && nivel <= 3)
                        break;
                    else
                        printf(BOLD(RED("\nDificuldade inválida. Insira uma dificuldade válida, fácil (1), médio (2) ou difícil (3): ")));
                }
            }

            if (nivel >= 1 && nivel <= 3)
            {
                embaralhar(tabuleiro, nivel);
                printf("Tabuleiro embaralhado. Vamos começar o jogo!\n\n");
                imprimirTabuleiro(tabuleiro);

                int resultadoJogo;
                do
                {
                    resultadoJogo = jogar(tabuleiro, jogadas);
                } while (resultadoJogo != 0);
            }
            break;

        case 2:
            if (tabuleiro[LINCOL - 1][LINCOL - 1] == 0)
                printf("Primeiro inicialize um jogo ou carregue um jogo previamente salvo.\n\n");
            else
            {
                imprimirTabuleiro(tabuleiro);
                int resultadoJogo;
                do
                {
                    resultadoJogo = jogar(tabuleiro, jogadas);
                } while (resultadoJogo != 0);
            }
            break;
        case 3:
            carregarJogoSalvo(tabuleiro, &jogadas);
            break;
        case 4:
            printf("Para iniciar um novo jogo, durante o menu inicial digite '1'.\n");
            printf("Para voltar ao menu inicial em qualquer momento, digite 'voltar'.\n");
            printf("Durante o jogo, digite o número da peça que você deseja mover.\n");
            printf("Para salvar o jogo em um arquivo, digite 'salvar' seguido pelo nome do arquivo.\n");
            break;
        default:
            printf(BOLD(RED("Comando inválido.")) " Tente novamente.\n");
        }
    }

    return 1;
}

int trocarPecas(int **tabuleiro, int n)
{
    int i = 0, j = 0, x = 0, y = 0;

    if (n >= 1 && n <= 8)
    {
        for (int a = 0; a < LINCOL; a++)
        {
            for (int b = 0; b < LINCOL; b++)
            {
                if (tabuleiro[a][b] == n)
                {
                    i = a;
                    j = b;
                }
                else if (tabuleiro[a][b] == 0)
                {
                    x = a;
                    y = b;
                }
            }
        }

        if ((abs(i - x) == 1 && j == y) || (abs(j - y) == 1 && i == x))
        {
            int aux = tabuleiro[x][y];
            tabuleiro[x][y] = tabuleiro[i][j];
            tabuleiro[i][j] = aux;
            return 1;
        }
    }

    return 0;
}

void trocar(int **tabuleiro, int x1, int y1, int x2, int y2)
{
    int aux = tabuleiro[x1][y1];
    tabuleiro[x1][y1] = tabuleiro[x2][y2];
    tabuleiro[x2][y2] = aux;
}

void embaralhar(int **tabuleiro, int dificuldade)
{

    srand(time(NULL));
    int aux, embaralhadas;

    if (dificuldade == 1)
        embaralhadas = 20;
    else if (dificuldade == 2)
        embaralhadas = 40;
    else
        embaralhadas = 80;

    for (int k = 0; k < embaralhadas; k++)
    {
        int x, y;
        for (int m = 0; m < LINCOL; m++)
        {
            for (int n = 0; n < LINCOL; n++)
            {
                if (tabuleiro[m][n] == 0)
                {
                    x = m;
                    y = n;
                }
            }
        }
        int randomNum = rand() % 4;

        if (x > 0 && randomNum == 0)
            trocar(tabuleiro, x, y, x - 1, y);
        else if (x < LINCOL - 1 && randomNum == 1)
            trocar(tabuleiro, x, y, x + 1, y);
        else if (y > 0 && randomNum == 2)
            trocar(tabuleiro, x, y, x, y - 1);
        else if (y < LINCOL - 1 && randomNum == 3)
            trocar(tabuleiro, x, y, x, y + 1);
    }
}

int verificar(int **tabuleiro)
{
    int tabuleiroPadrao[LINCOL][LINCOL] = {{1, 2, 3}, {4, 5, 6}, {7, 8, 0}};

    for (int i = 0; i < LINCOL; i++)
        for (int j = 0; j < LINCOL; j++)
            if (tabuleiro[i][j] != tabuleiroPadrao[i][j])
                return 0;

    return 1;
}

void exibirMensagemVitoria(int **tabuleiro, int jogadas)
{
    printf("\n");
    printf("\033[1m\033[42m***WINNER!***\033[0m\n");
    printf("\n");

    imprimirTabuleiro(tabuleiro);

    printf("\033[1m\033[33m\nParabéns!\033[0m você completou o quebra-cabeça com %d movimento(s).\n", jogadas);
    liberarMatriz(tabuleiro);
    exit(0);
}

int jogar(int **tabuleiro, int jogadas)
{
    char input[20];
    int retorno = 0;

    do
    {
        if (verificar(tabuleiro) == 1)
            exibirMensagemVitoria(tabuleiro, jogadas);
        else
        {
            printf("Digite um comando ou número da peça que quer mover: ");
            scanf("%s", input);
            printf("\n");

            if (strcmp(input, "sair") == 0)
            {
                printf("Saiu!\n");
                liberarMatriz(tabuleiro);
                exit(0);
            }
            else if (strcmp(input, "voltar") == 0)
                menu(tabuleiro, jogadas);
            else if (strcmp(input, "salvar") == 0)
                salvarJogo(tabuleiro, jogadas);
            else
            {
                int number = atoi(input);

                if (number != 0)
                {
                    if (trocarPecas(tabuleiro, number) == 1)
                    {
                        imprimirTabuleiro(tabuleiro);
                        jogadas++;
                        printf("Quantidade de jogadas até agora: %d\n", jogadas);
                        retorno = 1;
                    }
                    else
                        printf("Não é possível mover a peça: %d\n", number);
                }
                else
                    printf("Entrada inválida. Digite um número válido ou um comando.\n");
            }
        }
    } while (retorno != 0);

    return retorno;
}

int endsWith(const char *str, const char *suffix)
{
    size_t str_len = strlen(str);
    size_t suffix_len = strlen(suffix);

    if (suffix_len > str_len)
        return 0;

    return strncmp(str + str_len - suffix_len, suffix, suffix_len) == 0;
}

void salvarJogo(int **tabuleiro, int jogadas)
{
    char nomeDoArq[100];

    do
    {
        printf("Digite o nome do arquivo para salvar o jogo (deve terminar com .txt): ");
        scanf("%99s", nomeDoArq);
    } while (!(endsWith(nomeDoArq, ".txt")));

    FILE *file = fopen(nomeDoArq, "w");
    if (file == NULL)
    {
        printf("Erro ao abrir o arquivo para escrita.\n");
        return;
    }

    fprintf(file, "%d\n", LINCOL);

    for (int i = 0; i < LINCOL; i++)
    {
        for (int j = 0; j < LINCOL; j++)
            fprintf(file, "%d ", tabuleiro[i][j]);
        fprintf(file, "\n");
    }

    fprintf(file, "%d\n", jogadas);
    fclose(file);

    printf("Arquivo %s salvo com sucesso!\n", nomeDoArq);
}

int carregarJogoSalvo(int **tabuleiro, int *jogadas)
{
    char nomeDoArq[100];
    int tamanhoMatriz = LINCOL;

    do
    {
        printf("Digite o nome do arquivo para carregar o jogo: ");
        scanf(" %s", nomeDoArq);

        if (strcmp(nomeDoArq, "voltar") == 0)
            return 0;
        else if (strcmp(nomeDoArq, "sair") == 0)
            exit(0);
        else if (!endsWith(nomeDoArq, ".txt"))
            printf("Tipo de arquivo inválido. Use arquivos com a extensão .txt.\n");
    } while (!endsWith(nomeDoArq, ".txt"));

    FILE *file = fopen(nomeDoArq, "r");
    if (file == NULL)
    {
        printf("Não foi possível abrir o arquivo. Verifique se o nome está correto ou se o arquivo existe.\n");
        return 0;
    }

    int matrizCopiadora[LINCOL][LINCOL];

    if (fscanf(file, "%d\n", &tamanhoMatriz) != 1 || tamanhoMatriz != LINCOL)
    {
        printf("Arquivo inválido. A matriz salva deve ter dimensões %dx%d.\n", LINCOL, LINCOL);
        fclose(file);
        return 0;
    }

    for (int i = 0; i < LINCOL; i++)
    {
        for (int j = 0; j < LINCOL; j++)
        {
            if (fscanf(file, "%d", &matrizCopiadora[i][j]) != 1)
            {
                printf("Arquivo inválido. Erro ao ler a matriz.\n");
                fclose(file);
                return 0;
            }
        }
        fscanf(file, "\n");
    }

    if (fscanf(file, "%d\n", jogadas) != 1)
    {
        printf("Arquivo inválido. Erro ao ler a quantidade de jogadas.\n");
        fclose(file);
        return 0;
    }

    fclose(file);

    for (int i = 0; i < LINCOL; i++)
        for (int j = 0; j < LINCOL; j++)
            tabuleiro[i][j] = matrizCopiadora[i][j];

    imprimirTabuleiro(tabuleiro);
    jogar(tabuleiro, *jogadas);

    return 1;
}