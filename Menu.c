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


int posy = POSICAO_BIRD; /// defini��o da variavel responsavel pela posi��o do passaro
TIPO_jogador jogadores[3];


int ocorreu_colisao_mod(int posy, TIPO_cano cano, CHAR_INFO buf[][DRAW_AREA_X], TIPO_tiro tiro)
///checa se houve colis�o com as bordas, e executa uma "tela de game over"
{
    int i,j;
    int stop;

    if (posy>25 || posy<0)///verifica se ocorreu colis�o com o teto e com o ch�o
    {
        printf("\ngame over");/// se sim, game-over
        return 1;
    }
    else if(buf[(int)posy][POSICAO_BIRD].Attributes != 67) ///verifica se ocorreu colis�o com o ch�o
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
///checa se houve colis�o com as bordas, e executa uma "tela de game over"
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
///limpa o buf para fazer a mudan�as dos objetos
{
    int i, j;
    for(i=0; i< DRAW_AREA_Y; i++)
        for(j=0; j< DRAW_AREA_X; j++)
        {
            buf[i][j].Char.AsciiChar = ' ';
            buf[i][j].Attributes = 78; ///preenchendo cada espa�o com a cor vermelha
        }
}


void imprime_cano_na_tela(TIPO_cano cano, CHAR_INFO buf[][DRAW_AREA_X])
///fun��o que faz a cria��o do cano
{
    int i,j,coluna_cima=0;

    cano.coluna_cima =  cano.coluna_baixo - TAM_BURACO; /// calcula o tamanho que a coluna tem que ter para que haja um v�o de 3 espa�os

    if(cano.coluna_cima < 0)///verifica se o valor da coluna de baixo � menor que 0
    {
        ///caso seja
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
///fun��o que mostra a barra preta lateral para mostrar as informa��es
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

    ///fun��o que valida se o passaro n�o colidiu com o cano
    if(buf[(int)posy][POSICAO_BIRD].Attributes != 47)
    {
        ///caso n�o
        buf[(int)posy][POSICAO_BIRD].Char.AsciiChar = '@';///desloca o passaro na tela
        buf[(int)posy][POSICAO_BIRD].Attributes = 67;
    }





    clear_win(buf); /// chamamento da fun��o que limpa a tela
    buf[(int)posy][POSICAO_BIRD].Char.AsciiChar = '@'; ///imprime o passaro para a nova posi��o
    buf[(int)posy][POSICAO_BIRD].Attributes = 67;
    imprime_cano_na_tela(cano, buf);///chamamento da fun��o que imprime os canos
    barra_lateral(buf);///chamamento da fun��o que imprime a barra lateral



    ///fun��o que faz o dezenho na tela
    draw_quads(console_out, buf, cano); /// chamamento da fun��o que imprime o jogo
    draw_text(console_out, pontuacao, 50, 3); ///chamamento da fun��o que imprime as informa��es
}
void atualiza_tela_mod(HANDLE console_out, CHAR_INFO buf[][DRAW_AREA_X], TIPO_cano cano,int pontuacao, TIPO_inimigo inimigo, TIPO_tiro tiro)
///atualiza tela da vers�o modificada
{

    ///fun��o que limpa a tela
    clear_win(buf);
    ///fun��o que valida se o passaro n�o colidiu com o cano
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

    ///desenha o personagem na nova posi��o
    buf[(int)posy][POSICAO_BIRD].Char.AsciiChar = '@';
    buf[(int)posy][POSICAO_BIRD].Attributes = 67;
    ///desenha o inimigo na nova posi��o
    buf[inimigo.posicaoy-1][inimigo.posicaox-1].Char.AsciiChar = '8';
    buf[inimigo.posicaoy-1][inimigo.posicaox-1].Attributes = 207;

    imprime_cano_na_tela(cano, buf);
    barra_lateral(buf);

    ///fun��o que faz o dezenho na tela
    draw_quads(console_out, buf, cano);/// chamamento da fun��o que imprime o jogo

    draw_text(console_out, pontuacao, 50, 3);///chamamento da fun��o que imprime as informa��es
}


float move_bird(float posy, float *velocidade)
///fun��o que faz a gravidade agir sobre o passaro
{
    *velocidade = *velocidade * 1.1; ///calcula a velocidade
    posy =  posy + *velocidade;///soma a velocidade a posi��o do passaro

    return posy; ///retorna a nova posi��o
}

int aleatorio(TIPO_cano cano, CHAR_INFO buf[][DRAW_AREA_X])
///fun��o que gera o numero aleatorio
{
    int i, j, coluna_baixo;
    cano.coluna_baixo = clock()%14 +1;

    return cano.coluna_baixo;
}



int faz_pontuacao(TIPO_cano cano, int *pontuacao)
///fun��o que imcrementa a pontua��o
{
    if(cano.posicaoX == POSICAO_BIRD)
    {
        (*pontuacao)++;
    }
    return *pontuacao; ///retorna a nova pontua��o
}

draw_text(HANDLE output, int pontuacao, int x, int y)
///fun��o que imprime as informa��es na tela
{
    setlocale(LC_ALL, "Portuguese");
    COORD coord = {(SHORT)48, (SHORT)0};

    SetConsoleCursorPosition(output, coord);
    printf("Pontuacao %d", pontuacao);
}

void grava_tresmaiores()
///fun��o que grava os tres maiores valores e o nome de cada um deles
{
    TIPO_jogador jogador_aux;
    jogador_aux.pontuacao = 0;
    FILE *arq2;
    int i,j;
    for(i = 0; i<3; i++)
    {
        for(j = 0; j<3; j++)
        {
            if(jogadores[i].pontuacao <= jogadores[j].pontuacao)
            {
                jogador_aux = jogadores[i];
                jogadores[i] = jogadores[j];
                jogadores[j] = jogador_aux;
            }
        }
    }
    if(!(arq2 = fopen("ranqueV1.bin", "wb"))) ///verifica se abriu
        printf("Falha ao abrir o arquivo para grava��o"); /// se ocorreu falha
    else///se n�o
    {
        if(!fwrite(jogadores, sizeof(TIPO_jogador), 3, arq2))///escreve
        {
            printf("erro");///se deu falha
        }
    }

    fclose(arq2);///fecha o arquivo
}

void salvadadosV1(int pontuacaoV1)
///salva a pontua��o do jogo normal
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
///fun��o que da inicio ao jogo
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
    printf("aperte qualquer bot�o para jogar");
    scanf(" %c", &aperta);
    while(!ocorreu_colisao((int)posy,  cano1, buf))///come�a o loop e verifica se ocorreu colis�o
    {
        faz_pontuacao(cano1,&pontuacao); /// faz a pontua��o do jogador
        Sleep(TEMPOCAIR); /// para o programa
        cano1.posicaox -= 1; ///decrementa o inicio do cano
        cano1.posicaoX -= 1;///decrementa o fim do cano

        if(_kbhit())///testa se algum bot�o foi precionado
        {
            c = _getch();
            if (c== ' ') /// verifica se o retorno da fun��o getch � igual a espa�o
            {
                posy -=1;///se for, faz o pulo
                aceleracao = 0.1;/// e "zera" a acelera��o
            }
        }
        else
        {
            posy = move_bird(posy, &aceleracao); ///caso contrario faz a "gravidade"
        }
        if(cano1.posicaox < 0)
            ///verifica se cano inicio � menor que 0
        {
            cano1.posicaox = 0; ///se sim, mantem o cano em zero
        }
        if(cano1.posicaoX == 0)
            ///verifica se cano fim � menor que 0
        {
            cano1.posicaox = POSICAO_DO_CANO_inicio;   ///zera cano inicio
            cano1.posicaoX = POSICAO_DO_CANO_fim;       ///zera cano fim

            cano1.coluna_baixo = aleatorio(cano1, buf); /// adiciona um valor aleatorio a coluna
        }

        atualiza_tela(console_out, buf, cano1, pontuacao);///atualiza a tela
    }
    salvadadosV1(pontuacao); ///chama a fun��o para salvamento
    return pontuacao; ///retorna a pontua��o
}

int entra_jogo_mod(CHAR_INFO buf[][DRAW_AREA_X])
///fun��o que da inicio ao jogo
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
    tiro.posicaox = 53; ///inicia o tiro na posi��ox
    tiro.posicaoy = 2; ///inicia o tiro na posi��oy
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
                tiro.posicaox = POSICAO_BIRD + 1; ///adiciona a posi��o do passaro a posi��o x do tiro
                tiro.posicaoy = (int)posy; ///adiciona a posi��o do passaro a posi��o y do tiro
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

        ///valida se o cano mais perto do ponto 0 � menor que zero
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
    grava_tresmaiores(pontuacao, nome);///chama a fun��o para o salvamento dos tres maiores records
    return pontuacao; ///retorna pontua��o
}

void imprime_jogadores()///imprime todos os jogadores salvos
{
    int i;

    for(i=0; i<3; i++)
    {
        printf("\nnome :");
        puts(jogadores[i].nome);
        printf("\npontua��o: %d",jogadores[i].pontuacao);
    }
}

void le_tresmaiores()
///fun��o que le os tres maiores records
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
                printf("N�o foi possivel ler");
            }

        }
        fclose(arq2);
    }
}

void jogo_normal()
///fun��o que chama a fun��o para iniciar o jogo normal
{
    setlocale(LC_ALL, "Portuguese");
    TIPO_jogador jogador;
    jogador.pontuacao = 0;
    system("mode 60, 26");
    CHAR_INFO buf[DRAW_AREA_Y][DRAW_AREA_X];
    int posicao;
    char pergunta1;
    char pergunta2;
    int recomecar = 1;

    do
    {
        jogador.pontuacao = entra_jogo(buf);
        le_tresmaiores();
        printf("\nsua pontuacao e: %d\n", jogador.pontuacao);
        printf("Deseja recome�ar o jogo?\n");
        fflush(stdin);

        while(pergunta1 != 'N' && pergunta1 != 'S')
        {
            printf("Digite N para nao ou S para sim.\n");
            fflush(stdin);
            scanf("%c", &pergunta1);
            pergunta1 = toupper(pergunta1);
        }
        if(pergunta1 == 'N')
        {
            recomecar = 0;
        }
    }
    while(recomecar);

    printf("salvar sua pontuacao?");
    while(pergunta2 != 'N' && pergunta2 != 'S')
    {
        printf("Digite N para nao ou S para sim.\n");
        fflush(stdin);
        scanf("%c", &pergunta2);
        pergunta2 = toupper(pergunta2);
    }
    if(pergunta2 == 'S')
        salvadadosV1(jogador.pontuacao);
}



void jogo_modificado()
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
            printf("Deseja recome�ar o jogo?\n");
            fflush(stdin);
            scanf("%c", &pergunta1);
        }
        while(pergunta1!='s' || pergunta1!='n');
        do
        {
            printf("Deseja recome�ar ou salvar sua pontuacao?");
            fflush(stdin);
            scanf("%c", &pergunta2);
        }
        while(pergunta2!='s' || pergunta2!='n');
    }
    while(pergunta1 == 's');

}

/* desliga buffering em stdout para que mensagens sejam imprimidas na
 * funcao draw_text */


void gotoxymenu(int coluna, int linha, char opcao[10]);
void menu();

///////////////////////////////////////////////////////////////////////////////////
void gotoxymenu(int coluna, int linha, char opcao[10])
{
    HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD position = {coluna,linha}; ///fun��o para otuliza��o do gotoxy
    ///Recupera um identificador para o dispositivo padr�o especificado (entrada padr�o, sa�da padr�o, ou erro padr�o).
    SetConsoleCursorPosition(hStdout,position);
    ///Para colocarmos o cursor numa dada posi��o da janela fun��o do Windows.h
    printf("%s", opcao);
}
///variaveis
char tecla;
int pcimacoluna;
int pbaixolinha = 5;
///////////////////////////////////////////////////////////////////////////////////
void menu() ///fun��o para o desenho do menu e das op��es
{

    system("cls"); ///limpa janela
    system("color 0E"); ///defini��o das cores de texto (D) e fundo (0)

    int i;
    int j;

    ///i=coluna
    ///j=linha

    ///desenho do quadrado que envolve o menu
    for (i=35; i<120; i++)

        gotoxymenu(i,2,"�");

    for (i=35; i<120; i++)

        gotoxymenu(i,21,"�");

    for (j=3; j<21; j++)

        gotoxymenu(35,j,"�");

    for (j=3; j<21; j++)

        gotoxymenu(119,j,"�");


    ///op��es do menu
    gotoxymenu(60,1, "*Menu*");
    gotoxymenu(60,5, "Vers�o easy");
    gotoxymenu(60,10, "vers�o at� o diabo vai chorar");
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
	int sair = 0;

    setlocale(LC_ALL, "Portuguese");
    gotoxymenu(50,5, ">>"); ///defini��o das flexas para orienta��o do menu
    do
    {
        menu(); ///chamda da fun��o menu antes declarada

        gotoxymenu(50, pbaixolinha, ">>");
        tecla = getch();

        switch(tecla)
        {
        case CIMA:
            if(pbaixolinha>5) ///para a movimenta��o para baixo das flexas de orienta��o
            {
                gotoxymenu(51,pbaixolinha,"  ");
                pbaixolinha = pbaixolinha - 5;
                gotoxymenu(51,pbaixolinha,">>");
            }
            break;
        case BAIXO:
            if(pbaixolinha<20) ///para a movimenta��o para cima das flexas de orienta��o
            {
                gotoxymenu(51,pbaixolinha,"  ");
                pbaixolinha = pbaixolinha + 5;
                gotoxymenu(51,pbaixolinha,">>");
                ///pressionou enter para escolha das op��es do menu
            }
            break;
        case ENTER: /// caso das op��es , primeira op��o
            if(pbaixolinha==5)
            {
                jogo_normal();
            }
            else if(pbaixolinha==10) ///segunda op��o
            {
                jogo_modificado();
            }
            else if(pbaixolinha==15) ///terceira op��o
            {
                imprime_jogadores();
            }

            else if(pbaixolinha==20) ///saida
                sair = 1;
            break;
        }
    }
    while(!sair); ///fazer enquanto o usuario n�o apertar enter
}
