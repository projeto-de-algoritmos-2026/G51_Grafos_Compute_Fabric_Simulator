# Compute Fabric Simulator & Visualizer

**Número da Lista**: 1<br>
**Conteúdo da Disciplina**: Grafos 1 (Caminhos Mínimos e Árvore Geradora Mínima)<br>

## Alunos

| Matrícula | Aluno |
| -- | -- |
| 180113097 | Daniel Coimbra dos Santos |
| 180066161 | Luis Henrique Luz Costa |

## Sobre

O **Compute Fabric Simulator** é um simulador e visualizador em C++ desenvolvido para modelar, otimizar e analisar a infraestrutura de rede de um Data Center sob condições dinâmicas de tráfego e degradação física de hardware. O projeto simula 50 racks organizados em piso de processamento e opera em duas fases principais baseadas nos algoritmos da disciplina:

* **Stage 1 — Otimização de Topologia Física (Kruskal) [Inspirado no LeetCode 1584 (Min Cost to Connect All Points)]:** Calcula a *Minimum Spanning Tree* (MST) para conectar todos os racks com o menor custo total em metros de cabeamento de fibra.
* **Stage 2 — Roteamento Lógico e Latência (Dijkstra) [Inspirado no LeetCode 743 (Network Delay Time)]:** Calcula a *Shortest Path Tree* (SPT) a partir do servidor Master (Nó 0) para monitorar latências de broadcast em milissegundos e identificar congestionamentos (disparados quando a latência supera 65 ms).

### Snapshots Temporais ($t_1 \to t_{10}$)
O sistema evolui ao longo de 10 snapshots de tempo:
1. **$t_1$ (Estado Inicial):** Define o grafo base e executa a otimização física e lógica inicial.
2. **$t_2 \dots t_{10}$ (Mutações Dinâmicas):** Flutuações na rede alteram aleatoriamente a latência de links em tempo real (via `updateEdgeWeight`). Se a flag de topologia dinâmica estiver ativa, o sistema também simula a degradação física dos cabos. O Dijkstra e o Kruskal reavaliam a topologia a cada passo e geram visualizações dinâmicas em SVG.

## Screenshots

*(Substitua pelos caminhos ou links das imagens salvas no seu repositório)*

![Execução do Terminal](path/to/screenshot_terminal.png)
*Execução do simulador exibindo a evolução dos snapshots e alertas de congestionamento no terminal.*

![Visualização da Árvore de Roteamento - Dijkstra](path/to/snapshot_t1.png)
*Mapa SVG mostrando o roteamento lógico do nó Master (amarelo) para os racks em t = 1.*

![Visualização de Roteamento Dinâmico](path/to/snapshot_t10.png)
*Mapa SVG em t = 10 demonstrando a reconfiguração dos caminhos azuis do Dijkstra para contornar nós engarrafados.*

## Instalação

Linguagem: C++ (padrão C++17)<br>
Framework: Não se aplica (utiliza biblioteca padrão e gerador SVG próprio)<br>

### Pré-requisitos
* Compilador C++ com suporte ao C++17 (`g++` ou `clang++`)
* Utilitário `make`

### Comandos de Compilação

1. Clone o repositório para a sua máquina local:
   ```bash
   git clone <URL_DO_REPOSITORIO>
   cd G51_Grafos_Compute_Fabric_Simulator

### Limpe artefatos anteriores e compile o projeto:

`make clean && make`

### Execute o binário gerado na raiz do projeto:


`./simulator`

### Escolha uma das opções no menu interativo CLI:

#### Opção 1: 10 Snapshots Determinísticos (Topologia Estática) — usa seed fixa (1337).

#### Opção 2: 10 Snapshots Determinísticos (Topologia Dinâmica) — inclui degradação física de cabos.

#### Opção 3: 10 Snapshots Não-Determinísticos (Topologia Dinâmica) — usa entropia do sistema para variações inéditas a cada execução.

#### Opção 4: Benchmark de Carga Massivo — gera grafos em grande escala (100 a 3000 nós).

#### Opção 5: Execução Estática com Arquivos Locais — executa a simulação baseada na pasta data/.

Os arquivos vetoriais em SVG serão exportados automaticamente nas pastas output_deterministic/ ou output_random/. Abra os arquivos .svg em qualquer navegador web para visualizar as topologias renderizadas.


### Estrutura dos Arquivos SVG Renderizados

#### Nó Amarelo (#d29922): Servidor Master / Ponto de origem do broadcast (Nó 0).

#### Nós Verdes (#238636): Racks de processamento do Data Center (Nós 1 a 49).

#### Linhas Azuis (#2f81f7): Árvore de caminhos mínimos do Dijkstra (snapshot_t*.svg).

#### Linhas Verdes (#3fb950): Conexões da Árvore Geradora Mínima do Kruskal (topology_t*.svg).

