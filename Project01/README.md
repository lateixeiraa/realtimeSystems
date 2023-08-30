# Project: manufacturing system

## Description

A manufacturing system in which there are several producing and consuming machines. For an efficient operation, it is necessary to control the production and processing of these items in a coordinated manner, avoiding bottlenecks and ensuring that all items are properly processed.

### Requirements of project

- Ensure a balance between machine production and item processing to avoid excessive accumulation of items in the buffer.
- Prevent a machine that consumes items from processing the same item as another machine.
- Ensure that all items produced are processed and packaged for shipment.

## How to use

### Requirements of system

1. Use the standard Linux system installation:

```
sudo apt update
sudo apt install build-essential
```

2. Clone this repository

```
git clone https://github.com/lateixeiraa/realtimeSystems.git
```
   
3. Open the terminal and navigate to the directory where projeto_1.c is located.

4. Compile the code using GCC and the necessary options for the pthread libraries:

```
gcc -o nome_do_executavel nome_do_arquivo.c -lpthread
```

Exemplo:
```
gcc -o projeto_1 projeto_1.c -lpthread
```
   
5. Run the compiled program:
   
``` ./projeto_1```

### Example of Expected Output

```
Producer 0: Item 1486160194 produced
Buffer: 1

Producer 1: Item 634822808 produced
Buffer: 2

Consumer 0: Item 1486160194 consumed
Buffer: 1

Producer 2: Item 1013740783 produced
Buffer: 3

Consumer 1: Item 634822808 consumed
Buffer: 2

Producer 0: Item 705653319 produced
Buffer: 4

Producer 1: Item 1091530846 produced
Buffer: 5
```


## Team

| [Larissa Teixeira</sub>](https://github.com/lateixeiraa) |  [Marcus Vinicius</sub>](https://github.com/MarcusPereirinha) |  
| :---: | :---: |
