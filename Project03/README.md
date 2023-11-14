# Jogo de artilharia

O projeto envolve o desenvolvimento de um sistema de simulação de combate de mísseis, implementado como um sistema em tempo real usando FreeRTOS. O objetivo principal é simular um cenário onde um lado lança mísseis e o outro se defende usando um sistema de defesa. As principais características do projeto incluem:

*Processo de Ataque*: Simula o lançamento de mísseis pelo adversário. Os mísseis são gerados com atributos aleatórios como trajetória e tempo de impacto. O sistema lança múltiplos mísseis em intervalos aleatórios.

*Processo de Defesa*: Representa um sistema de defesa, similar ao Iron Dome de Israel. Este processo intercepta os mísseis lançados com base em sua trajetória, priorizando aqueles com menor tempo de impacto e direcionados a áreas habitadas.

*Monitoramento de Áreas Habitadas*: Um componente que monitora as áreas que necessitam de proteção, informando ao sistema de defesa sobre mísseis ameaçadores.

*Comunicação e Sincronização*: Utiliza semáforos para gerenciar a comunicação e o acesso aos recursos compartilhados entre os processos de ataque e defesa.

*Gestão de Memória*: Implementa a alocação e liberação dinâmica de memória para os mísseis, garantindo uma gestão eficiente dos recursos.

*Ordenação e Processamento*: Os mísseis são ordenados e processados com base em critérios específicos, como menor tempo de impacto e trajetória, para simular uma estratégia de defesa eficaz.

## Tecnologias e Ferramentas Utilizadas

- Sistema Operacional: Windows.
- Linguagem de Programação: C.
- Sistema Operacional em Tempo Real: FreeRTOS.
- Ambiente de Desenvolvimento: Visual Studio Code (VSCode).
- Controle de Versão: Git.

## Funcionamento

Siga as instruções em video para configurações do ambiente e um exemplo utilizando os mesmos recursos:
https://github.com/vsserafim/twotasks-win32-msvc

