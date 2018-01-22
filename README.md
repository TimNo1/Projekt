# Projekt

C++ implementacija alata za preklapanje dugačkih greškovitih očitanja treće generacije sekvenciranja.

## Dependencies

### Linux

1. gcc 4.8+
2. cmake 3.2+

## Usage

Program se pokreće iz naredbenog retka sa dva argumenta od kojih je drugi opcionalan. Prvi argument je putanja do .fasta datoteke koja se želi obraditi, drugi argument služi za odabir moda rada. Ako je 1 onda se obrada obavlja samo pomoću jedne dretve, dok ne navođenje ničega razultira izvođenjem u više dretvi. Višedretvenost je izvedena uz pomoć https://github.com/rvaser/thread_pool.

## Description

Algoritam pronalaženja očitanja koje se preklapaju temelji se na nalaženju minimizera unutar očitanja, a za samo poravnava očitanja koristi se algoritam lis (https://en.wikipedia.org/wiki/Longest_increasing_subsequence). Algoritam je inspiriran minmapom i minmapom2 (https://github.com/lh3/minimap2).