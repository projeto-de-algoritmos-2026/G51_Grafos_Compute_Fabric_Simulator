# Compute Fabric Simulator & Tester

Projeto desenvolvido para a disciplina de Projetos de Algoritmos na Universidade de Brasília (UnB). O Compute Fabric Simulator é uma ferramenta em C++17 projetada para otimizar a topologia física e analisar o roteamento dinâmico e a latência de tráfego em infraestruturas de data centers e clusters de alta performance.

---

## Estrutura do Projeto

O simulador é dividido em duas etapas principais (Stages), complementadas por módulos de visualização vetorial e análise temporal:

* **Stage 1 - Otimização de Topologia Física (Algoritmo de Kruskal):** Determina a Árvore Geradora Mínima (MST) para minimizar a quantidade e o comprimento dos cabos de fibra ótica necessários para interconectar todos os racks de servidores no piso do data center.
* **Stage 2 - Roteamento Lógico e Análise de Latência (Algoritmo de Dijkstra):** Calcula a Árvore de Caminhos Mínimos (Shortest Path Tree - SPT) a partir de um servidor mestre (Master Node), determinando o tempo máximo de propagação e identificando gargalos na rede.
* **Simulação Temporal (10 Snapshots):** Avalia o comportamento adaptativo do roteamento lógico em tempo real frente a flutuações de tráfego e congestionamentos temporais.
* **Exportador Vetorial (VisualizerSVG):** Gera mapas gráficos em formato SVG das topologias físicas e das rotas ativas de comunicação.

---

## Simulação Temporal Dinâmica (10 Snapshots)

A latência dos enlaces de rede em um ambiente de computação de alto desempenho varia dinamicamente conforme o volume de requisições. O simulador executa 10 passos de tempo (t_1 a t_10) reavaliando a latência dos links e recomputando a árvore de caminhos mínimos do Dijkstra para redirecionar o tráfego fora de zonas congestionadas.

### Modos de Execução Temporais

1. **Determinístico (Seed Fixa: 1337):**
   * Proporciona reprodutibilidade técnica total dos testes.
   * Gera variações idênticas em qualquer ambiente de execução.
   * Exporta as imagens SVG correspondentes para a pasta `output_deterministic/`.

2. **Não-Determinístico (Seed Dinâmica: std::random_device):**
   * Simula condições reais e imprevisíveis de operação em data centers.
   * Aplica flutuações randômicas de latência em 20% dos enlaces a cada snapshot.
   * Exporta as imagens SVG correspondentes para a pasta `output_random/`.

---

## Visualização Gráfica Vetorial (SVG)

A cada snapshot t, o módulo VisualizerSVG gera um arquivo vetorial representando a topologia de rede no instante analisado:

* **Nó Dourado:** Servidor Mestre / Nó de Origem (Master Node).
* **Nós Verdes:** Racks de servidores da infraestrutura.
* **Linhas Azuis:** Conexões ativas selecionadas pela Árvore de Caminhos Mínimos do Dijkstra no instante t.

### Estrutura de Arquivos Gerados

```text
G51_Grafos_Compute_Fabric_Simulator/
├── output_deterministic/      # SVGs gerados no modo Determinístico (snapshot_t1.svg ... snapshot_t10.svg)
├── output_random/             # SVGs gerados no modo Não-Determinístico (snapshot_t1.svg ... snapshot_t10.svg)
├── topology_output.svg        # Topologia Física Base (Stage 1 - Kruskal)
├── data/                      # Arquivos de entrada estáticos
└── src/                       # Código-fonte da aplicação