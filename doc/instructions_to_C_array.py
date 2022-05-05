#!/usr/bin/python3
# -*- coding: utf-8 -*-

## Instructions Counter x86_64.
## Author: Manel Lurbe Sempere
## e-mail: malursem@gap.upv.es
## Year: 2022

import sys

arguments =  sys.argv
numargs = len(sys.argv)

print("\nInstructions Counter x86_64.\nAuthor: Manel Lurbe Sempere\ne-mail: malursem@gap.upv.es\nYear: 2022\n")

if numargs != 4:
    print("Missing arguments")
    print("Usage: ./instructions_to_C_array.py Time workloadArray [a->2006 & microbenchmark b->2006 & microbenchmark & geekbench5] Results_path")
    sys.exit(1)

time = arguments[1]
workloadArray = arguments[2]
res_path = arguments[3]
ini = 0
fin = 0
if workloadArray == 'a':
    print("Selected Benchmarks: SPEC CPU 2006 & microbenchmark\n")
    benchmarks = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26]
    ini = 0
    fin = 0
elif workloadArray =='b':
    print("Selected Benchmarks: ALL\n")
    benchmarks = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47]
    ini = 0
    fin = 0
else:
    print("Array value incorrect.")
    sys.exit(1)
listaInstructions=[]

benchmarksNames = ["perlbench checkspam","bzip2","gcc","mcf","gobmk","hmmer","sjeng","libquantum","h264ref","omnetpp","astar","xalancbmk","bwaves","gamess",
"milc","zeusmp","gromacs","cactusADM","leslie3d","namd","microbench","soplex","povray","GemsFDTD","lbm",
"perlbench diffmail","calculix","AES-XTS","Text Compression","Image Compression","Navigation","HTML5","SQLite",
"PDF Rendering","Text Rendering","Clang","Camera","N-Body Physics","Rigid Body Physics","Gaussian Blur","Face Detection",
"Horizon Detection","Image Inpainting","HDR","Ray Tracing","Structure from Motion","Speech Recognition","Machine Learning"]

for bench in benchmarks:
    existe = 0
    try:
        fitx = open(str(res_path)+"instructions["+time+"]/Instructions["+str(bench)+"].txt","r")
        for linea in fitx.read().split("\n"):
            if "PMU_COUNTS:" in linea:
                existe = 1
                instructions = linea.split("\t")[2]
                listaInstructions.append(instructions)
                break
        if existe == 0:
            print("Error with benchmark number: "+str(bench)+"\nCan't find PMU_COUNTS\n")
            listaInstructions.append(str(0))
    except:
        print("Error with benchmark number: "+str(bench)+"\nCan't find the file\n")
        listaInstructions.append(str(0))

escribir_array = open("array["+time+"].txt","w")
escribir_names = open("array_names["+time+"].txt","w")
escribir_array.write("unsigned long int bench_Instructions [] = {\n\t")
escribir_names.write("char *bench_Names [] = {\n\t")

not_working = open("not_working["+time+"].txt","w")
not_working.write("Benchmarks not working\n")

working = open("working["+time+"].txt","w")
working.write("Benchmarks working\n")

print(str(len(listaInstructions)))
cont = 0
conta = 0

for value in listaInstructions:
    if value == '0':
        not_working.write(str(cont)+" ")
    else:
        working.write(str(cont)+" ")
    if cont != len(listaInstructions)-1:
        escribir_array.write(value+",")
        escribir_names.write('"'+benchmarksNames[cont]+'"'+",")
    else:
        escribir_array.write(value)
        escribir_names.write('"'+benchmarksNames[cont]+'"')
    if conta == 7:
        fin = ini+conta
        escribir_array.write("//"+str(ini)+"--"+str(fin)+"\n\t")
        escribir_names.write("//"+str(ini)+"--"+str(fin)+"\n\t")
        ini = fin+1
        conta = 0
    else:
        conta = conta+1
    cont=cont+1
escribir_array.write("\n};")
escribir_names.write("\n};")
escribir_array.close()
escribir_names.close()
not_working.close()
working.close()