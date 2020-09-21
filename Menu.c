#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <stdlib.h>
#include <conio.h>
#include<locale.h>


#define CIMA 72
#define BAIXO 80
#define ENTER 13
#define SAIDA 27

#define NOME 4
#define DRAW_AREA_X 60
#define DRAW_AREA_Y 26
#define POSICAO_BIRD 10
#define POSICAO_DO_CANO_inicio 49
#define POSICAO_DO_CANO_fim 59
#define TAM_BURACO 3
#define COR_CANO 57
#define TEMPOCAIR 100

#define BRANCO 255
#define AMARELO 239
#define CINZA 127
#define AMARELO_QUEIMADO 111
#define CINZA_FORTE 143
#define AMARELO 239
#define ROSA 223
#define ROXO 95
#define RED 79
#define AGUA 63
#define VERDE 47
#define AZUL 32

typedef struct str_jogador
{
    char nome[NOME];
    int pontuacao;
} TIPO_jogador;

typedef struct str_cano
{
    int posicaox;
    int posicaoX;
    int coluna_cima;
    int coluna_baixo;
    int tela;
} TIPO_cano;
typedef struct str_inimigo
{
    int posicaox;
    int posicaoy;
} TIPO_inimigo;
typedef struct str_tiro
{
    int posicaox;
    int posicaoy;
} TIPO_tiro;


float posy = POSICAO_BIRD; /// definição da variavel responsavel pela posição do passaro
TIPO_jogador jogadores[3];


int ocorreu_colisao_mod(int posy, TIPO_cano cano, CHAR_INFO buf[][DRAW_AREA_X], TIPO_tiro tiro)
///checa se houve colisão com as bordas, e executa uma "tela de game over"
{
    int i,j;
    int stop;

    if (posy>25 || posy<0)///verifica se ocorreu colisão com o teto e com o chão
    {
        printf("\ngame over");/// se sim, game-over
        return 1;
    }
    else if(buf[(int)posy][POSICAO_BIRD].Attributes != 67) ///verifica se ocorreu colisão com o chão
    {
        printf("\ngame over");
        return 1; /// se sim, game-over
    }
    else
    {
        return 0; /// continua o jogo
    }
}

int ocorreu_colisao(int posy, TIPO_cano cano, CHAR_INFO buf[][DRAW_AREA_X] )
///checa se houve colisão com as bordas, e executa uma "tela de game over"
{
    int i,j;
    int stop;

    if (posy>25 || posy<0)
    {
        printf("\ngame over");
        return 1;
    }
    else if(buf[(int)posy][POSICAO_BIRD].Attributes != 47)
    {
        buf[(int)posy][POSICAO_BIRD].Char.AsciiChar = '@';
        buf[(int)posy][POSICAO_BIRD].Attributes = 67;
        return 0;
    }
    else
    {
        printf("\ngame over");
        return 1;
    }
}

void
draw_quads(HANDLE console, CHAR_INFO buf[][DRAW_AREA_X], TIPO_cano cano)
{
    // tipo CHAR_INFO define uma struct com um caractere e atributos de estilo, e.g,
    // cor da fonte e cor de fundo

    COORD buf_size = {DRAW_AREA_X, DRAW_AREA_Y};
    COORD initial = {0, 0};
    SMALL_RECT area_size = {0, 0, DRAW_AREA_X, DRAW_AREA_Y};

    // inicializa buffer com linhas de tamanho 60 e colunas de tamanho 26


    WriteConsoleOutput(console, (CHAR_INFO *) buf, buf_size, initial, &area_size);


    // escreve 'buf' direto no console, na area comecando em 'initial'
    // e de tamanho 'area_size'
}


///* Escreve a string msg posicionando o primeiro caractere nas coordenadas (x, y),
///* a posicao (0, 0) esta no canto superior esquerdo da tela do terminal


void clear_win(CHAR_INFO buf[][DRAW_AREA_X])
///limpa o buf para fazer a mudanças dos objetos
{
    int i, j;
    for(i=0; i< DRAW_AREA_Y; i++)
        for(j=0; j< DRAW_AREA_X; j++)
        {
            buf[i][j].Char.AsciiChar = ' ';
            buf[i][j].Attributes = 78; ///preenchendo cada espaço com a cor vermelha
        }
}


void imprime_cano_na_tela(TIPO_cano cano, CHAR_INFO buf[][DRAW_AREA_X])
///função que faz a criação do cano
{
    int i,j,coluna_cima=0;

    cano.coluna_cima =  cano.coluna_baixo - TAM_BURACO; /// calcula o tamanho que a coluna tem que ter para que haja um vão de 3 espaços

    if(cano.coluna_cima < 0)///verifica se o valor da coluna de baixo é menor que 0
    {///caso seja
        cano.coluna_cima = 0; /// coluna cima recebe 0
        cano.coluna_baixo = 3;///e coluna baixo recebe 3
    }

    for(i=cano.posicaoX; i>=cano.posicaox ; i--)///desenha a coluna de baixo
        for(j=DRAW_AREA_Y; j>cano.coluna_baixo; j--)
        {
            buf[j][i].Char.AsciiChar = '#';
            buf[j][i].Attributes = 47;
        }
    for(i=cano.posicaoX; i>= cano.posicaox; i--)///desenha a coluna de cima
        for(j=0; j<=cano.coluna_cima; j++)
        {
            buf[j][i].Char.AsciiChar = '#';
            buf[j][i].Attributes = 47;
        }
}
void barra_lateral(CHAR_INFO buf[][DRAW_AREA_X])
///função que mostra a barra preta lateral para mostrar as informações
{
    int i,j;

    for(i=48; i<DRAW_AREA_X; i++) ///desenha a barra preta
        for(j=0; j<DRAW_AREA_Y; j++)
        {
            buf[j][i].Char.AsciiChar = ' ';
            buf[j][i].Attributes = 8;
        }
}

void atualiza_tela(HANDLE console_out, CHAR_INFO buf[][DRAW_AREA_X], TIPO_cano cano,int pontuacao)
///atualiza tela
{

    ///função que valida se o passaro não colidiu com o cano
    if(buf[(int)posy][POSICAO_BIRD].Attributes != 47)
    {   ///caso não
        buf[(int)posy][POSICAO_BIRD].Char.AsciiChar = '@';///desloca o passaro na tela
        buf[(int)posy][POSICAO_BIRD].Attributes = 67;
    }





    clear_win(buf); /// chamamento da função que limpa a tela
    buf[(int)posy][POSICAO_BIRD].Char.AsciiChar = '@'; ///imprime o passaro para a nova posição
    buf[(int)posy][POSICAO_BIRD].Attributes = 67;
    imprime_cano_na_tela(cano, buf);///chamamento da função que imprime os canos
    barra_lateral(buf);///chamamento da função que imprime a barra lateral



    ///função que faz o dezenho na tela
    draw_quads(console_out, buf, cano); /// chamamento da função que imprime o jogo
    draw_text(console_out, pontuacao, 50, 3); ///chamamento da função que imprime as informações
}
void atualiza_tela_mod(HANDLE console_out, CHAR_INFO buf[][DRAW_AREA_X], TIPO_cano cano,int pontuacao, TIPO_inimigo inimigo, TIPO_tiro tiro)
///atualiza tela da versão modificada
{

    ///função que limpa a tela
    clear_win(buf);
    ///função que valida se o passaro não colidiu com o cano
    if(buf[(int)posy][POSICAO_BIRD].Attributes != 47)
    {
        buf[(int)posy][POSICAO_BIRD].Char.AsciiChar = '@';
        buf[(int)posy][POSICAO_BIRD].Attributes = 67;
    }


    if(buf[tiro.posicaoy][tiro.posicaox].Attributes != AMARELO && buf[tiro.posicaoy][tiro.posicaox].Char.AsciiChar != '8'&&
       buf[inimigo.posicaoy][inimigo.posicaox-1].Attributes != 7 && buf[tiro.posicaoy][tiro.posicaox].Char.AsciiChar != '>')
    {
        buf[tiro.posicaoy][tiro.posicaox].Char.AsciiChar = '>';
        buf[tiro.posicaoy][tiro.posicaox].Attributes = 7;
    }


    else
    {
        buf[inimigo.posicaoy-1][inimigo.posicaox-1].Char.AsciiChar = ' ';
        buf[inimigo.posicaoy-1][inimigo.posicaox-1].Attributes = 67;
        buf[tiro.posicaoy][tiro.posicaox].Char.AsciiChar = ' ';
        buf[tiro.posicaoy][tiro.posicaox].Attributes = 67;
    }

    ///desenha o personagem na nova posição
    buf[(int)posy][POSICAO_BIRD].Char.AsciiChar = '@';
    buf[(int)posy][POSICAO_BIRD].Attributes = 67;
    ///desenha o inimigo na nova posição
    buf[inimigo.posicaoy-1][inimigo.posicaox-1].Char.AsciiChar = '8';
    buf[inimigo.posicaoy-1][inimigo.posicaox-1].Attributes = 207;

    imprime_cano_na_tela(cano, buf);
    barra_lateral(buf);

    ///função que faz o dezenho na tela
    draw_quads(console_out, buf, cano);/// chamamento da função que imprime o jogo

    draw_text(console_out, pontuacao, 50, 3);///chamamento da função que imprime as informações
}


float move_bird(float posy, float *velocidade)
///função que faz a gravidade agir sobre o passaro
{
    *velocidade = *velocidade * 1.1; ///calcula a velocidade
    posy =  posy + *velocidade;///soma a velocidade a posição do passaro

    return posy; ///retorna a nova posição
}

int aleatorio(TIPO_cano cano, CHAR_INFO buf[][DRAW_AREA_X])
///função que gera o numero aleatorio
{
    int i, j, coluna_baixo;
    cano.coluna_baixo = clock()%14 +1;

    return cano.coluna_baixo;
}



int faz_pontuacao(TIPO_cano cano, int *pontuacao)
///função que imcrementa a pontuação
{
    if(cano.posicaoX == POSICAO_BIRD)
    {
        (*pontuacao)++;
    }
    return *pontuacao; ///retorna a nova pontuação
}

draw_text(HANDLE output, int pontuacao, int x, int y)
///função que imprime as informações na tela
{
    setlocale(LC_ALL, "Portuguese");
    COORD coord = {(SHORT)48, (SHORT)0};

    SetConsoleCursorPosition(output, coord);
    printf("Pontuacao %d", pontuacao);
}

void grava_tresmaiores()
///função que grava os tres maiores valores e o nome de cada um deles
{
    TIPO_jogador jogador[3];
    FILE *arq2;
    char auxl[NOME];

    if(!(arq2 = fopen("ranqueV1.bin", "wb"))) ///verifica se abriu
        printf("Falha ao abrir o arquivo para gravação"); /// se ocorreu falha
    else///se não
    {
        if(!fwrite(jogadores, sizeof(TIPO_jogador), 3, arq2))///escreve
        {
            printf("erro");///se deu falha
        }
    }

    fclose(arq2);///fecha o arquivo
}

void salvadadosV1(int pontuacaoV1)
///salva a pontuação do jogo normal
{
    int pontuacaoOriginal;
    FILE *arquivo_para_salvarV1;


    if (!(arquivo_para_salvarV1 = fopen("ranqueV1.bin", "rb")))///verifica se abriu
    {
        printf("Erro ao ler arquivo"); /// se ocorreu falha
    }
    else
    {
        if(!fread(&pontuacaoOriginal, sizeof(int), 1, arquivo_para_salvarV1))
        {
            printf("Erro ao ler pontuacao");
        }
        fclose(arquivo_para_salvarV1);
    }

    if(pontuacaoOriginal < pontuacaoV1)
    {
        if(!(arquivo_para_salvarV1 = fopen("ranqueV1.bin", "w+b")))
        {
            printf("Erro ao abrir o arquivo para escrever");

        }
        else
        {
            if(!(fwrite(&pontuacaoV1, sizeof(int), 1, arquivo_para_salvarV1)))
                printf("Erro ao escrever no arquivo");

            fclose(arquivo_para_salvarV1);
        }
    }
}

int entra_jogo(CHAR_INFO buf[][DRAW_AREA_X])
///função que da inicio ao jogo
{
    // TIPO_jogador jogador;
    // jogador.pontuacao = 0;
    TIPO_cano cano1;
    HANDLE console_out;
    console_out = GetStdHandle(STD_OUTPUT_HANDLE);
    int c = 0;
    char aperta;
    float aceleracao = 0.1;

    int pontuacao = 0;
    int coluna_baixo = 0;
    char nome[NOME];

    cano1.posicaox = POSICAO_DO_CANO_inicio; ///inicio do cano
    cano1.posicaoX = POSICAO_DO_CANO_fim;///fim do cano

    cano1.coluna_baixo = aleatorio(cano1, buf); ///adiciona um valor aleatorio a coluna baixo
    atualiza_tela(console_out, buf, cano1, pontuacao);///imprime a tela
    printf("aperte qualquer botão para jogar");
    scanf(" %c", &aperta);
    while(!ocorreu_colisao((int)posy,  cano1, buf))///começa o loop e verifica se ocorreu colisão
    {
        faz_pontuacao(cano1,&pontuacao); /// faz a pontuação do jogador
        Sleep(TEMPOCAIR); /// para o programa
        cano1.posicaox -= 1; ///decrementa o inicio do cano
        cano1.posicaoX -= 1;///decrementa o fim do cano

        if(_kbhit())///testa se algum botão foi precionado
        {
            c = _getch();
            if (c== ' ') /// verifica se o retorno da função getch é igual a espaço
            {
                posy -=1;///se for, faz o pulo
                aceleracao = 0.1;/// e "zera" a aceleração
            }
        }
        else
        {
            posy = move_bird(posy, &aceleracao); ///caso contrario faz a "gravidade"
        }
        if(cano1.posicaox < 0)
            ///verifica se cano inicio é menor que 0
        {
            cano1.posicaox = 0; ///se sim, mantem o cano em zero
        }
        if(cano1.posicaoX == 0)
        ///verifica se cano fim é menor que 0
        {
            cano1.posicaox = POSICAO_DO_CANO_inicio;   ///zera cano inicio
            cano1.posicaoX = POSICAO_DO_CANO_fim;       ///zera cano fim

            cano1.coluna_baixo = aleatorio(cano1, buf); /// adiciona um valor aleatorio a coluna
        }

        atualiza_tela(console_out, buf, cano1, pontuacao);///atualiza a tela
    }
    salvadadosV1(pontuacao); ///chama a função para salvamento
    return pontuacao; ///retorna a pontuação
}

int entra_jogo_mod(CHAR_INFO buf[][DRAW_AREA_X])
///função que da inicio ao jogo
{
    // TIPO_jogador jogador;
    // jogador.pontuacao = 0;
    TIPO_inimigo inimigo;
    TIPO_cano cano1;
    TIPO_tiro tiro;
    HANDLE console_out;
    console_out = GetStdHandle(STD_OUTPUT_HANDLE);
    int c = 0;
    float aceleracao = 0.4;
    int pontuacao = 0;
    int coluna_baixo = 0;
    char nome[NOME];

    cano1.posicaox = POSICAO_DO_CANO_inicio;///inicio do cano
    cano1.posicaoX = POSICAO_DO_CANO_fim;///fim do cano

    inimigo.posicaox = POSICAO_DO_CANO_fim-1;///    adiciona o inimigo na tela
    inimigo.posicaoy = aleatorio(cano1, buf);///     adiciona um valor aleatorio referente a altura

    cano1.coluna_baixo = aleatorio(cano1, buf); /// adiciona um valor aleatorio referente a o cano baixo
    tiro.posicaox = 53; ///inicia o tiro na posiçãox
    tiro.posicaoy = 2; ///inicia o tiro na posiçãoy
    atualiza_tela_mod(console_out, buf, cano1, pontuacao, inimigo, tiro);
    while(!ocorreu_colisao_mod((int)posy,  cano1, buf, tiro))
    {
        faz_pontuacao(cano1,&pontuacao);
        Sleep(TEMPOCAIR);
        cano1.posicaox -= 1;
        cano1.posicaoX -= 1;
        if(cano1.posicaoX > 13 && inimigo.posicaox>48)
        {
            inimigo.posicaox = POSICAO_DO_CANO_fim;
        }


        if(cano1.posicaoX == 13)///libera o inimigo para se movimentar

        {
            inimigo.posicaox = POSICAO_DO_CANO_fim-1;
            inimigo.posicaoy = aleatorio(cano1, buf);
        }

        if(inimigo.posicaox <=POSICAO_DO_CANO_fim)
        {
            inimigo.posicaox -= 1;  ///movimenta o inimigo
        }

        if(_kbhit())
        {
            c = _getch();
            if (c == ' ')
            {
                posy -=1;
                aceleracao = 0.1;
            }
            if(c == 'w')
            {
               // printf("fdasklfkj");
                tiro.posicaox = POSICAO_BIRD + 1; ///adiciona a posição do passaro a posição x do tiro
                tiro.posicaoy = (int)posy; ///adiciona a posição do passaro a posição y do tiro
            }


        }
        else
        {
            posy = move_bird(posy, &aceleracao);
            tiro.posicaox +=1;
        }

        if (buf[tiro.posicaoy][tiro.posicaox+1].Char.AsciiChar == '8')///limpa o inimigos e o tiro
        {
            inimigo.posicaox = POSICAO_DO_CANO_fim;
            tiro.posicaox = POSICAO_DO_CANO_fim;
        }
        if(buf[tiro.posicaoy][tiro.posicaox+1].Char.AsciiChar == '#') ///limpa o tiro caso acerte o cano
        {
            tiro.posicaox = POSICAO_DO_CANO_fim;
        }

        ///valida se o cano mais perto do ponto 0 é menor que zero
        if(cano1.posicaox < 0)
        {
            cano1.posicaox = 0;
        }


        if(cano1.posicaoX == 0)
        {
            cano1.posicaox = POSICAO_DO_CANO_inicio;
            cano1.posicaoX = POSICAO_DO_CANO_fim;

            cano1.coluna_baixo = aleatorio(cano1, buf);
        }

        if(cano1.posicaox >= POSICAO_DO_CANO_inicio)
        {
            cano1.posicaoX -= 1;
        }

        if(tiro.posicaox > DRAW_AREA_X)
        {
            tiro.posicaox = 53;
        }

        atualiza_tela_mod(console_out, buf, cano1, pontuacao, inimigo, tiro);
    }
    printf("\ndigite uma sigla que lhe represente");
    gets(nome);
    grava_tresmaiores(pontuacao, nome);///chama a função para o salvamento dos tres maiores records
    return pontuacao; ///retorna pontuação
}

void imprime_jogadores()///imprime todos os jogadores salvos
{
    int i;

    for(i=0; i<3; i++)
    {
        printf("\nnome :");
        puts(jogadores[i].nome);
        printf("\npontuação: %d",jogadores[i].pontuacao);
    }
}

void le_tresmaiores()
///função que le os tres maiores records
{
    TIPO_jogador jogador;
    FILE *arq2;



    if (!(arq2 = fopen("ranqueV1.bin", "r+")))
    {
        printf("Erro na criacao");
    }
    else
    {
        while(!feof(arq2))
        {
            if(!(fread(jogadores, sizeof(TIPO_jogador), 3, arq2)))
            {
                printf("Não foi possivel ler");
            }

        }
        fclose(arq2);
    }
}

int jogo_normal()
///função que chama a função para iniciar o jogo normal
{
    setlocale(LC_ALL, "Portuguese");
    TIPO_jogador jogador;
    jogador.pontuacao = 0;
    system("mode 60, 26");
    CHAR_INFO buf[DRAW_AREA_Y][DRAW_AREA_X];
    int posicao;
    char pergunta1;
    char pergunta2;

    do
    {
        jogador.pontuacao = entra_jogo(buf);
        le_tresmaiores();
        do
        {
            printf("\nsua pontuacao e: %d\n", jogador.pontuacao);
            printf("Deseja recomeçar o jogo?\n");
            fflush(stdin);
            scanf("%c", &pergunta1);
        }
        while(pergunta1!='s' || pergunta1!='n');
        do
        {
            printf("Deseja recomeçar ou salvar sua pontuacao?");
            fflush(stdin);
            scanf("%c", &pergunta2);
        }
        while(pergunta2!='s' || pergunta2!='n');
    }
    while(pergunta1 == 's' || pergunta1 != 'n');


    return 0;
}

int jogo_modificado()
{
    TIPO_jogador jogador;
    jogador.pontuacao = 0;
    setlocale(LC_ALL, "Portuguese");
    system("mode 60, 26");
    CHAR_INFO buf[DRAW_AREA_Y][DRAW_AREA_X];
    int posicao;
    char pergunta1;
    char pergunta2;
    int flag=0;
    int sair = 0;

    do
    {
        jogador.pontuacao = entra_jogo_mod(buf);
        le_tresmaiores();
        do
        {
            printf("\nsua pontuacao e: %d\n", jogador.pontuacao);
            printf("Deseja recomeçar o jogo?\n");
            fflush(stdin);
            scanf("%c", &pergunta1);
        }
        while(pergunta1!='s' || pergunta1!='n');
        do
        {
            printf("Deseja recomeçar ou salvar sua pontuacao?");
            fflush(stdin);
            scanf("%c", &pergunta2);
        }
        while(pergunta2!='s' || pergunta2!='n');
    }
    while(pergunta1 == 's');


    return 0;
}

/* desliga buffering em stdout para que mensagens sejam imprimidas na
 * funcao draw_text */


void gotoxymenu(int coluna, int linha, char opcao[10]);
void menu();

///////////////////////////////////////////////////////////////////////////////////
void gotoxymenu(int coluna, int linha, char opcao[10])
{
    HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD position = {coluna,linha}; ///função para otulização do gotoxy
    ///Recupera um identificador para o dispositivo padrão especificado (entrada padrão, saída padrão, ou erro padrão).
    SetConsoleCursorPosition(hStdout,position);
    ///Para colocarmos o cursor numa dada posição da janela função do Windows.h
    printf("%s", opcao);
}
///variaveis
char tecla;
int pcimacoluna;
int pbaixolinha = 5;
///////////////////////////////////////////////////////////////////////////////////
void menu() ///função para o desenho do menu e das opções
{

    system("cls"); ///limpa janela
    system("color 0E"); ///definição das cores de texto (D) e fundo (0)

    int i;
    int j;

    ///i=coluna
    ///j=linha

    ///desenho do quadrado que envolve o menu
    for (i=35; i<120; i++)

        gotoxymenu(i,2,"°");

    for (i=35; i<120; i++)

        gotoxymenu(i,21,"°");

    for (j=3; j<21; j++)

        gotoxymenu(35,j,"°");

    for (j=3; j<21; j++)

        gotoxymenu(119,j,"°");


    ///opções do menu
    gotoxymenu(60,1, "*Menu*");
    gotoxymenu(60,5, "Versão easy");
    gotoxymenu(60,10, "versão até o diabo vai chorar");
    gotoxymenu(60,15,"records");
    gotoxymenu(60,20, "Sair");
}
//////////////////////////////////////////////////////////////////////////////////

int main()
{


    strcpy(jogadores[0].nome, "BBB");
    strcpy(jogadores[1].nome, "BBB");
    strcpy(jogadores[2].nome, "BBB");

    jogadores[0].pontuacao = 32;
    jogadores[1].pontuacao = 54;
    jogadores[2].pontuacao = 546;

    grava_tresmaiores();

    setlocale(LC_ALL, "Portuguese");
    gotoxymenu(50,5, ">>"); ///definição das flexas para orientação do menu
    do
    {
        menu(); ///chamda da função menu antes declarada

        gotoxymenu(50, pbaixolinha, ">>");
        tecla = getch();

        switch(tecla)
        {
        case 72:
            if(pbaixolinha>5) ///para a movimentação para baixo das flexas de orientação
            {
                gotoxymenu(51,pbaixolinha,"  ");
                pbaixolinha = pbaixolinha - 5;
                gotoxymenu(51,pbaixolinha,">>");
            }
            break;
        case 80:
            if(pbaixolinha<20) ///para a movimentação para cima das flexas de orientação
            {
                gotoxymenu(51,pbaixolinha,"  ");
                pbaixolinha = pbaixolinha + 5;
                gotoxymenu(51,pbaixolinha,">>");
                ///pressionou enter para escolha das opções do menu
            }
            break;
        case 13: /// caso das opções , primeira opção
            if(pbaixolinha==5)
            {
                jogo_normal();
            }
            else if(pbaixolinha==10) ///segunda opção
            {
                jogo_modificado();
            }
            else if(pbaixolinha==15) ///terceira opção
            {
                imprime_jogadores();
            }

            else if(pbaixolinha==20) ///saida
                system("exit");
            break;
        }
    }
    while(!(tecla==13) ); ///fazer enquanto o usuario não apertar enter
}
