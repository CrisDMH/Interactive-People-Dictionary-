#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#define NOME_TAM 100
#define TAM_DICT 100
#define CPF_DIGITOS 11
#define CPF_TAM 13
#define CPF_FORMATADO 16

struct pessoa
{ 
  char nome[NOME_TAM];
  char cpf[CPF_TAM];
  uint16_t idade;
};

char* formata_cpf(char *cpf)
{
  if (!cpf || strlen(cpf) != CPF_DIGITOS) 
  {
    return NULL; // CPF inválido
  }

  char *formatado = malloc(CPF_FORMATADO);
  if (!formatado)
    return NULL;

  // formata para "123.456.789-00"
  snprintf(formatado, CPF_FORMATADO, "%.3s.%.3s.%.3s-%.2s",
           cpf, cpf + 3, cpf + 6, cpf + 9);

  return formatado;  
}

int cpf_valido(const char *cpf) 
{
  if (strlen(cpf) != 11)
    return 0;

  for (int i = 0; i < 11; i++) 
  {
    if (cpf[i] < '0' || cpf[i] > '9')
      return 0;
  }

  return 1;
}

struct dicionario_pessoa
{
  struct pessoa* pessoas[TAM_DICT];
  size_t numItens;
};

void inicializar(struct dicionario_pessoa* dict)
{
  dict->numItens = 0;
}

struct pessoa* buscar(char *cpf, struct dicionario_pessoa* dict)
{
  int inicio = 0;
  int fim = dict->numItens - 1;

  while (inicio <= fim)
  {
    int meio = (inicio + fim) / 2;
    
    int cmp = strcmp(dict->pessoas[meio]->cpf, cpf);
    if (cmp == 0)
      return dict->pessoas[meio];
    else if (cmp < 0)
      inicio = meio + 1;
    else
      fim = meio - 1;
  }

  return NULL;
}

int inserir(struct pessoa* nova, struct dicionario_pessoa* dict)
{
  if (dict->numItens >= TAM_DICT)
    return 0;

  if (buscar(nova->cpf, dict) != NULL)
    return 0;

  int i = dict->numItens - 1;

  while (i >= 0 && strcmp(nova->cpf, dict->pessoas[i]->cpf) < 0)
  {
    dict->pessoas[i + 1] = dict->pessoas[i];
    i--;
  }

  dict->pessoas[i + 1] = nova;
  dict->numItens++;

  return 1;
}

struct pessoa* remover(char *cpf, struct dicionario_pessoa* dict)
{
  int i;
  for (i = 0; i < dict->numItens; i++)
  {
    if (strcmp(dict->pessoas[i]->cpf, cpf) == 0)
    {
      struct pessoa* removida = dict->pessoas[i];
      for (int j = i; j < dict->numItens - 1; j++)
        dict->pessoas[j] = dict->pessoas[j + 1];
      
      dict->numItens--;

      return removida;
    }
  }
  
  return NULL;
}

void imprimir(const struct dicionario_pessoa* dict)
{
  if (dict->numItens == 0) 
  {
    printf("\nNenhuma pessoa cadastrada.\n");
    
    return;
  }  
  
  printf("\n           |Lista de Pessoas|\n");
  printf("---------------------------------------------\n");
  for (size_t i = 0; i < dict->numItens; i++)
  {
    struct pessoa* p = dict->pessoas[i];
    char *cpf_formatado = formata_cpf(dict->pessoas[i]->cpf);

    printf("Nome: %s\n", p->nome); 
    printf("CPF: %s\n", cpf_formatado);
    printf("Idade: %u anos\n", p->idade);
    printf("---------------------------------------------\n");

    free(cpf_formatado);
  }
}

struct pessoa* ler_pessoa()
{
  struct pessoa* nova = malloc(sizeof(struct pessoa));
  if (!nova)
    return NULL;

  printf("\nDigite o nome: ");
  fgets(nova->nome, NOME_TAM, stdin);
  nova->nome[strcspn(nova->nome, "\n")] = '\0';

  printf("Digite o CPF(Apenas números): ");
  fgets(nova->cpf, CPF_TAM, stdin);
  nova->cpf[strcspn(nova->cpf, "\n")] = '\0';

  if (!cpf_valido(nova->cpf)) 
  {
    printf("\nCPF com formato inválido. Operação cancelada.\n");
    
    free(nova);
    return NULL;
  }

  char buffer[16];
  printf("Digite a idade: ");
  fgets(buffer, sizeof(buffer), stdin);
  nova->idade = (uint16_t) atoi(buffer);

  return nova;
}

int main()
{
  struct dicionario_pessoa dict;
  char cpf[CPF_TAM];
  char *cpf_formatado;

  inicializar(&dict);

  char opcao;
  char linha[10];

  do 
  {
    printf("\nMenu:\n");
    printf("-----------------------");
    printf("\ni - Inserir pessoa\n");
    printf("r - Remover pessoa\n");
    printf("b - Buscar pessoa\n");
    printf("p - Imprimir lista\n");
    printf("s - Sair\n");
    printf("-----------------------\n");

    printf("\nOpção: ");
    if (!fgets(linha, sizeof(linha), stdin))
      break; // erro de leitura ou EOF

    linha[strcspn(linha, "\n")] = '\0'; 

    if (strlen(linha) == 0)
      continue;

    opcao = linha[0];

    switch (opcao)
    {
      case 'i': 
      {
        struct pessoa* nova = ler_pessoa();

        if (!nova)
          break;

        if (inserir(nova, &dict))
          printf("\nPessoa inserida com sucesso!\n");
        else 
        {
          printf("\nFalha ao inserir pessoa (CPF duplicado ou lista cheia).\n");
          free(nova);
        }
        break;
      }

      case 'r': 
      {
        if (dict.numItens == 0)
        {
          printf("\nNenhuma pessoa cadastrada\n");

          break;
        }        
        
        printf("\nDigite o CPF da pessoa a remover(Apenas números): ");
        fgets(cpf, CPF_TAM, stdin);
        cpf[strcspn(cpf, "\n")] = '\0';

        if (!cpf_valido(cpf)) 
        {
          printf("\nCPF inválido. Operação cancelada.\n");
          
          break;
        }

        struct pessoa* removida = remover(cpf, &dict);
        cpf_formatado = formata_cpf(cpf);

        if (removida) 
        {
          printf("\nPessoa removida: %s\n", removida->nome);
          
          free(removida);
        } 
        else 
        {
          printf("\nPessoa com CPF %s não encontrada.\n", cpf_formatado);
        }
        
        free(cpf_formatado);
        break;
      }

      case 'b': 
      {
        printf("\nDigite o CPF da pessoa a buscar(Apenas números): ");
        fgets(cpf, CPF_TAM, stdin);
        cpf[strcspn(cpf, "\n")] = '\0';

        struct pessoa* encontrada = buscar(cpf, &dict);
        cpf_formatado = formata_cpf(cpf);

        if (encontrada) 
        {
          printf("\n");  
          printf("Pessoa encontrada:\n");
          printf("\nNome: %s\n", encontrada->nome);
          printf("CPF: %s\n", cpf_formatado);
          printf("Idade: %u\n", encontrada->idade);
        } 
        else 
        {
          printf("Pessoa com CPF %s não encontrada.\n", cpf_formatado);
        }
        
        free(cpf_formatado);
        break;
      }

      case 'p':
        imprimir(&dict);
        
        break;

      case 's':
        printf("\nSaindo...\n");
        
        break;

      default:
        printf("\nOpção inválida.\n");
        
        break;
    }

  } while (opcao != 's');

  // liberar memória de todas as pessoas
  for (size_t i = 0; i < dict.numItens; i++) 
  {
    free(dict.pessoas[i]);
  }

  return 0;
}