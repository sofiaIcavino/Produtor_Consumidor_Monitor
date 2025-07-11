# Tarefa: Produtores e Consumidores Concorrentes

## Descrição

Esta tarefa tem como objetivo implementar um sistema concorrente com múltiplas threads produtoras e consumidoras que acessam um buffer circular de forma segura e sincronizada, utilizando `pthread_mutex_t` e `pthread_cond_t`.

## Objetivo

Implementar um buffer compartilhado com:
- N threads produtoras
- M threads consumidoras
- Acesso concorrente com exclusão mútua
- Condições de espera e sinalização usando variáveis de condição

## Regras

- O buffer tem tamanho fixo (`#define TAM_BUFFER 5`)
- Um produtor só pode inserir se houver espaço disponível
- Um consumidor só pode remover se houver item disponível
- Toda inserção/remoção deve ser registrada com um log:
  - `[Produtor X] Inseriu: VALOR`
  - `[Consumidor Y] Removeu: VALOR`

## Como executar

```bash
make
./buffer
```
