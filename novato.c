#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define TAMANHO_FILA 5
#define TAMANHO_PILHA 3

typedef struct {
    char tipo;
    int id;
} Peca;

typedef struct {
    Peca pecas[TAMANHO_FILA];
    int frente;
    int tras;
    int quantidade;
} FilaCircular;

typedef struct {
    Peca pecas[TAMANHO_PILHA];
    int topo;
} Pilha;

int contadorId = 1;

Peca gerarPeca() {
    char tipos[] = {'I', 'O', 'T', 'L', 'J', 'S', 'Z'};
    Peca novaPeca;
    novaPeca.tipo = tipos[rand() % 7];
    novaPeca.id = contadorId++;
    return novaPeca;
}

void inicializarFila(FilaCircular *fila) {
    fila->frente = 0;
    fila->tras = -1;
    fila->quantidade = 0;
}

int filaCheia(FilaCircular *fila) {
    return fila->quantidade == TAMANHO_FILA;
}

int filaVazia(FilaCircular *fila) {
    return fila->quantidade == 0;
}

int enqueue(FilaCircular *fila, Peca peca) {
    if (filaCheia(fila)) {
        return 0;
    }
    fila->tras = (fila->tras + 1) % TAMANHO_FILA;
    fila->pecas[fila->tras] = peca;
    fila->quantidade++;
    return 1;
}

Peca dequeue(FilaCircular *fila) {
    Peca pecaRemovida = fila->pecas[fila->frente];
    fila->frente = (fila->frente + 1) % TAMANHO_FILA;
    fila->quantidade--;
    return pecaRemovida;
}

void preencherFilaInicial(FilaCircular *fila) {
    for (int i = 0; i < TAMANHO_FILA; i++) {
        enqueue(fila, gerarPeca());
    }
}

void inicializarPilha(Pilha *pilha) {
    pilha->topo = -1;
}

int pilhaCheia(Pilha *pilha) {
    return pilha->topo == TAMANHO_PILHA - 1;
}

int pilhaVazia(Pilha *pilha) {
    return pilha->topo == -1;
}

int push(Pilha *pilha, Peca peca) {
    if (pilhaCheia(pilha)) {
        return 0;
    }
    pilha->topo++;
    pilha->pecas[pilha->topo] = peca;
    return 1;
}

Peca pop(Pilha *pilha) {
    Peca pecaRemovida = pilha->pecas[pilha->topo];
    pilha->topo--;
    return pecaRemovida;
}

Peca pop(Pilha *pilha) {
    Peca pecaRemovida = pilha->pecas[pilha->topo];
    pilha->topo--;
    return pecaRemovida;
}

void visualizarPilha(Pilha *pilha) {
    printf("\n╔════════════════════════════════════╗\n");
    printf("║      PILHA DE RESERVA             ║\n");
    printf("╚════════════════════════════════════╝\n");
    
    if (pilhaVazia(pilha)) {
        printf("  [PILHA VAZIA]\n");
        return;
    }
    
    printf("  Topo ↓\n");
    for (int i = pilha->topo; i >= 0; i--) {
        printf("   [%c#%d]\n", pilha->pecas[i].tipo, pilha->pecas[i].id);
    }
    printf("  Peças reservadas: %d/%d\n", pilha->topo + 1, TAMANHO_PILHA);
}

void visualizarEstado(FilaCircular *fila, Pilha *pilha) {
    visualizarFila(fila);
    visualizarPilha(pilha);
}

// ==================== FUNÇÕES DO JOGO ====================

void jogarPeca(FilaCircular *fila) {
    if (filaVazia(fila)) {
        printf("\n❌ Não há peças para jogar!\n");
        return;
    }
    
    Peca pecaJogada = dequeue(fila);
    printf("\n✓ Peça jogada: [%c#%d]\n", pecaJogada.tipo, pecaJogada.id);
    
    // Mantém a fila sempre cheia
    Peca novaPeca = gerarPeca();
    enqueue(fila, novaPeca);
    printf("✓ Nova peça adicionada à fila: [%c#%d]\n", novaPeca.tipo, novaPeca.id);
}

void reservarPeca(FilaCircular *fila, Pilha *pilha) {
    if (filaVazia(fila)) {
        printf("\n❌ Não há peças na fila para reservar!\n");
        return;
    }
    
    if (pilhaCheia(pilha)) {
        printf("\n❌ A pilha de reserva está cheia! Use uma peça reservada primeiro.\n");
        return;
    }
    
    Peca pecaReservada = dequeue(fila);
    push(pilha, pecaReservada);
    printf("\n✓ Peça reservada: [%c#%d]\n", pecaReservada.tipo, pecaReservada.id);
    
    // Mantém a fila sempre cheia
    Peca novaPeca = gerarPeca();
    enqueue(fila, novaPeca);
    printf("✓ Nova peça adicionada à fila: [%c#%d]\n", novaPeca.tipo, novaPeca.id);
}

void usarPecaReservada(Pilha *pilha) {
    if (pilhaVazia(pilha)) {
        printf("\n❌ Não há peças reservadas!\n");
        return;
    }
    
    Peca pecaUsada = pop(pilha);
    printf("\n✓ Peça reservada usada: [%c#%d]\n", pecaUsada.tipo, pecaUsada.id);
}

// ==================== MENU ====================

void exibirMenu() {
    printf("\n╔════════════════════════════════════╗\n");
    printf("║    TETRIS STACK - AVENTUREIRO     ║\n");
    printf("╠════════════════════════════════════╣\n");
    printf("║ 1 - Jogar peça                    ║\n");
    printf("║ 2 - Reservar peça                 ║\n");
    printf("║ 3 - Usar peça reservada           ║\n");
    printf("║ 0 - Sair                          ║\n");
    printf("╚════════════════════════════════════╝\n");
    printf("Escolha uma opção: ");
}

// ==================== MAIN ====================

int main() {
    srand(time(NULL));
    
    FilaCircular fila;
    Pilha pilha;
    
    inicializarFila(&fila);
    inicializarPilha(&pilha);
    preencherFilaInicial(&fila);
    
    int opcao;
    
    printf("╔════════════════════════════════════╗\n");
    printf("║   BEM-VINDO AO TETRIS STACK!      ║\n");
    printf("║      NÍVEL AVENTUREIRO            ║\n");
    printf("╚════════════════════════════════════╝\n");
    
    visualizarEstado(&fila, &pilha);
    
    do {
        exibirMenu();
        scanf("%d", &opcao);
        
        switch(opcao) {
            case 1:
                jogarPeca(&fila);
                visualizarEstado(&fila, &pilha);
                break;
            case 2:
                reservarPeca(&fila, &pilha);
                visualizarEstado(&fila, &pilha);
                break;
            case 3:
                usarPecaReservada(&pilha);
                visualizarEstado(&fila, &pilha);
                break;
            case 0:
                printf("\n👋 Obrigado por jogar! Até a próxima!\n\n");
                break;
            default:
                printf("\n❌ Opção inválida! Tente novamente.\n");
        }
    } while(opcao != 0);
    
    return 0;
}