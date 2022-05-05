#!/usr/bin/python3
# -*- coding: utf-8 -*-

## Instructions Counter x86_64.
## Author: Manel Lurbe Sempere
## e-mail: malursem@gap.upv.es
## Year: 2022

print("\nInstructions Counter x86_64.\nAuthor: Manel Lurbe Sempere\ne-mail: malursem@gap.upv.es\nYear: 2022\n")

benchmarks = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47]
benchmarksNames = ["perlbench checkspam","bzip2","gcc","mcf","gobmk","hmmer","sjeng","libquantum","h264ref","omnetpp","astar","xalancbmk","bwaves","gamess",
"milc","zeusmp","gromacs","cactusADM","leslie3d","namd","microbench","soplex","povray","GemsFDTD","lbm",
"perlbench diffmail","calculix","AES-XTS","Text Compression","Image Compression","Navigation","HTML5","SQLite",
"PDF Rendering","Text Rendering","Clang","Camera","N-Body Physics","Rigid Body Physics","Gaussian Blur","Face Detection",
"Horizon Detection","Image Inpainting","HDR","Ray Tracing","Structure from Motion","Speech Recognition","Machine Learning"]

file = open("benchmark_names_nums.txt","w")

for h in benchmarks:
    if h == 47:
        file.write(benchmarksNames[h]+" -> "+str(benchmarks[h]))
    else:
        file.write(benchmarksNames[h]+" -> "+str(benchmarks[h])+"\n")
file.close()

file = open("benchmark_names_array.txt","w")
file.write("char *bench_Names [] = {\n\t")
cont = 0
ini = 0
fin = 0
for h in benchmarks:
    if h == 47:
        file.write('"'+benchmarksNames[h]+'"')
    else:
        file.write('"'+benchmarksNames[h]+'",')
    if cont == 7:
        fin = ini+cont
        file.write("//"+str(ini)+"--"+str(fin)+"\n\t")
        ini = fin+1
        cont = 0
    else:
        cont = cont+1

if cont != 0:
    fin = fin+cont
    file.write("//"+str(ini)+"--"+str(fin)+"\n")
file.write("};")
file.close()