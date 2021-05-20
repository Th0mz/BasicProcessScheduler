#!/bin/bash

numArgs=3

# Check if the number of arguments is valid
if [ $# -ne $numArgs ]
then
	echo Error : Invalid number of arguments
	exit 1
fi

# Check if the first argument is an integer
re='^[0-9]+$'
if ! [[ $1 =~ $re ]]
then
	echo Error : $1 must be an integer
	exit 1
fi

# Check if the second argument is an integer
if ! [[ $2 =~ $re ]]
then
	echo Error : $2 must be an integer
	exit 1
fi

# Check if the third argument is an integer
if ! [[ $3 =~ $re ]]
then
	echo Error : $3 must be an integer
	exit 1
fi

liminf=$1
limsup=$2
step=$3

for process in $(seq $1 $3 $2)
do
	# Gerar teste
	../GeradorDeTestes/randomDAG $process 50 > testfile.txt
	echo ======: Processes $process :======
	# Obter a primeira lina do testefile e calcular a complexidado do algoritmo
	python3 -c "firstline = open(\"./testfile.txt\").readline().split(); print(\"V * E = \", end = \"\"); print(int(firstline[0]) * ( 2 * int(firstline[0]) + int(firstline[1])))"

	# Correr codigo com o grafo gerado dando display do tempo
	time ./a.out < ./testfile.txt
	echo ===============================
	echo
done
