#! /usr/bin/env python
import sys
import random
import math

random.seed()

def gera(nomeArq, numPontos):	
  f=open(nomeArq,'w')
  f.write("N "+str(numPontos)+"\n")

  #numero de conjuntos esta entre  log N e 10*N
  numConjuntos = random.randint(int(math.log(numPontos)),10*numPontos)
  f.write("M "+str(numConjuntos)+"\n")

  conjuntos = []
  for i in range(numConjuntos):
    conjuntos.append([])

  #garante existencia de uma solucao
  for i in range(1,numPontos+1):
    j = random.randint(0,numConjuntos-1)
    conjuntos[j].append(i)
    
  #Cada conjunto tera ate D elementos
  D = max(2,random.randint(numPontos/10,numPontos))

  #para cada conjunto sao atribuidos uniform(1,D) elementos
  #escolhidos em uniform(1,numPontos)
  for c in range(numConjuntos):
    d = random.randint(1,D)
    for i in range(d):
      k = random.randint(1,numPontos)
      conjuntos[c].append(k)
    conjuntos[c] = removeDuplicates(conjuntos[c])

  #escreve conjuntos em arquivo
  for c in range(numConjuntos):
    f.write("S"+str(c+1)+" ")
    #cada conjunto tem peso entre 5 e 25
    f.write(str(random.random()*20+5)+" ")
    for j in conjuntos[c]:
      f.write(str(j) + " ")
    f.write("\n")
  f.close()
  

def removeDuplicates(list):
  if list==[]:
    return
  list = sorted(list)
  list2 = []
  ant = list[0]
  list2.append(ant)
  for i in range(1,len(list)):
    n = list[i]
    if n == ant:
      continue
    else:
      ant = n
      list2.append(ant)
  return list2

#Entrada nome-arq, numero de elementos
if(len(sys.argv) != 3):
  print "Entre com\n geradorInst nome-arq numero-de-elementos"
else:
  if(int(sys.argv[2])<2):
    print("Numero de elementos deve ser maior que 1")
  else:
    gera(sys.argv[1],int(sys.argv[2]))
