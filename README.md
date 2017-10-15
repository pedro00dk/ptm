# PMT - Patter Matcher Tool

PMT é uma ferramenta para busca de padrões em arquivos, similar ao GNU grep. Ela pode ser utilizada tanto para filtrar as linhas em que há ocorrência quanto para contar a quantidade de ocorrências. De acordo com a escolha, a busca pode ser exata ou aproximada (utilizando [distância de edição](https://pt.wikipedia.org/wiki/Dist%C3%A2ncia_Levenshtein)). Além disso, o usuário pode escolher o algoritmo de busca utilizado.

## Algoritmos disponíveis

* [Brute Force](https://en.wikipedia.org/wiki/String_searching_algorithm#Na.C3.AFve_string_search): Algoritmo "ingênuo" de busca exata
* [Boyer Moore](https://en.wikipedia.org/wiki/Boyer%E2%80%93Moore_string_search_algorithm): Algoritmo de busca exata com uso de heurísticas
* [Knuth-Morris-Pratt](https://en.wikipedia.org/wiki/Knuth%E2%80%93Morris%E2%80%93Pratt_algorithm): Algoritmo de busca exata com uso do conceito de borda para evitar comparações repetidas
* [Shift-Or](http://www-igm.univ-mlv.fr/~lecroq/string/node6.html): Algoritmo de busca exata que faz uso de operações bit a bit
* [Rabin Karp](https://en.wikipedia.org/wiki/Rabin%E2%80%93Karp_algorithm): Algoritmo de busca exata que faz uso de hash e uma janela deslizante
* [Sellers](http://www.cin.ufpe.br/~paguso/courses/if767/bib/Sellers_1980.pdf): Algoritmo de busca aproximada com uso de programação dinâmica
* [Ukkonen](http://www.cin.ufpe.br/~paguso/courses/if767/bib/Ukkonen_1985.pdf): Algoritmo de busca aproximada com uso de uma máquina de estados

## Pré-requisitos

Para executar a ferramente, é necessário que a máquina possua o compilador de C++11.

## Setup

1. Baixe o projeto
2. Execute o Makefile para compilar o executável do projeto
3. Configure a variável de ambiente para apontar para o executável

## Opções de linha de comando

```
Usage:
  ptm [<option>] pattern textfile [textfile...]  ->  search files
  ptm [<option>] pattern  ->  search input stream

Options:
-h,--help            Display this information
-a,--algorithm <arg> Specify the search algorithm
-e,--edit <arg>      Specify the edit distance (required) for approx algorithms\n"
-c,--count           Display only the number of found occurrences
-p,--pattern <arg>   Specify a file with patterns to search
                     If specified, the first global argument (pattern) is treated as a text file

Algorithm <arg> options:
  exact: <bf>, <kmp>, <bm>, <ac>, <so>, <rk>
  approx: <se>, <uk>
```

## Exemplo

```
ptm -a kmp -c abra abracadrabra.txt
```

## Mais informações
Mais informações estão disponíveis no relatório, na pasta doc.


## Autores

* **Giovanni Barros** - [gaabs](https://github.com/gaabs)
* **Pedro Henrique** - [pedro00dk](https://github.com/pedro00dk)
