# G51_Grafos1_2026.2

# Compute Fabric Simulator

**Número da Lista**: 2<br>
**Conteúdo da Disciplina**: Grafos 2 (Caminhos Mínimos e Árvore Geradora Mínima)<br>

## Alunos

|Matrícula | Aluno |
| -- | -- |
| 180113097 | Daniel Coimbra dos Santos |
| 180066161 | Luis Henrique Luz Costa |

## Sobre

O **Compute Fabric Simulator** é um projeto dividido em duas etapas que aplica algoritmos de grafos para simular o design físico e o roteamento de dados em um cluster de alta performance (Data Center). O projeto adapta as lógicas de problemas clássicos do LeetCode para um cenário do mundo real:

1. **Stage 1 (Topology & Setup):** Baseado no LeetCode 1584 (*Min Cost to Connect All Points*). Utiliza o algoritmo de **Kruskal** com a estrutura *Union-Find* (Disjoint Sets) para calcular a topologia ideal (Árvore Geradora Mínima). O algoritmo define o layout mais barato de cabos de fibra ótica para conectar todos os racks de servidores no andar.
2. **Stage 2 (Runtime & Routing):** Baseado no LeetCode 743 (*Network Delay Time*). Utiliza o algoritmo de **Dijkstra** para simular o tempo de latência de rede, encontrando o caminho mais rápido para transmitir uma atualização de dados de um servidor central (nó origem) para todos os outros nós pela rede recém-criada.

## Screenshots

*(Substitua os links abaixo pelas imagens do seu projeto rodando)*

![Topologia de Rede Gerada](placeholder-imagem-1.png)
![Cálculo de Rotas no Terminal](placeholder-imagem-2.png)
![Simulação de Falha e Redundância](placeholder-imagem-3.png)

## Instalação

**Linguagem**: C++ (C++17 recomendado)<br>
**Framework**: (Nenhum)<br>

**Pré-requisitos:**
* Ter um compilador C++ instalado (como o `g++` via GCC, ou `clang++`).
* GNU Make (opcional, caso decida criar um Makefile).

Comandos necessários para baixar e preparar o projeto:

```bash
# Clone o repositório
git clone [https://github.com/SEU-USUARIO/Grafos2_Compute-Fabric-Simulator.git](https://github.com/SEU-USUARIO/Grafos2_Compute-Fabric-Simulator.git)

# Acesse a pasta do projeto
cd Grafos2_Compute-Fabric-Simulator

# Compile o código fonte (ajuste o nome do arquivo .cpp conforme necessário)
g++ -std=c++17 main.cpp -o simulator

Quaisquer outras informações sobre seu projeto podem ser descritas abaixo.
