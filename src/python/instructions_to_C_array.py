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
    print("Usage: ./instructions_to_C_array.py Time workloadArray [a->GAPS b->Geekbench5 c->SPEC ALL d->Phoronix] Results_path")
    sys.exit(1)

time = arguments[1]
workloadArray = arguments[2]
res_path = arguments[3]
ini = 0
fin = 0
if workloadArray == 'a':
    print("Selected Benchmarks: GAPS\n")
    benchmarks = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45]
    benchmarksNamesComplete = ["bfs_g_24","bfs_u_24","bc_g_24","bc_u_24","cc_g_24","cc_u_24","pr_g_24","pr_u_24",##0--7
        "sssp_g_24","sssp_u_24","tc_g_24","tc_u_24","cc_sv_g_24","cc_sv_u_24","pr_spmv_g_24","pr_spmv_u_24",##8--15
        "bfs-twitter","pr-twitter","cc-twitter","bc-twitter","bfs-web","pr-web","cc-web","bc-web",##16--23
        "bfs-road","pr-road","cc-road","bc-road","bfs-kron","pr-kron","cc-kron","bc-kron",##24--31
        "bfs-urand","pr-urand","cc-urand","bc-urand","tc-kron","tc-urand","tc-twitter","tc-web","tc-road",##32--39
        "sssp-kron","sssp-urand","sssp-twitter","sssp-web","sssp-road"]##41--45
    benchmarksNames = ["bfs","bfs","bc","bc","cc","cc","pr","pr",##0--7
        "sssp","sssp","tc","tc","cc_sv","cc_sv","pr_spmv","pr_spmv",##8--15
        "bfs","pr","cc","bc","bfs","pr","cc","bc",##16--23
        "bfs","pr","cc","bc","bfs","pr","cc","bc",##24--31
        "bfs","pr","cc","bc","tc","tc","tc","tc","tc",##32--39
        "sssp","sssp","sssp","sssp","sssp"]##41--45
elif workloadArray =='b':
    print("Selected Benchmarks: Geekbench5\n")
    benchmarks = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20]
    benchmarksNamesComplete = ["AES-XTS","Text Compression","Image Compression","Navigation","HTML5",##0--4
	    "SQLite","PDF Rendering","Text Rendering","Clang","Camera","N-Body Physics","Rigid Body Physics","Gaussian Blur",##5--12
	    "Face Detection","Horizon Detection","Image Inpainting","HDR","Ray Tracing","Structure from Motion","Speech Recognition","Machine Learning"]##13--20
    benchmarksNames = ["geekbench5","geekbench5","geekbench5","geekbench5","geekbench5",##0--4
	    "geekbench5","geekbench5","geekbench5","geekbench5","geekbench5","geekbench5","geekbench5","geekbench5",##5--12
	    "geekbench5","geekbench5","geekbench5","geekbench5","geekbench5","geekbench5","geekbench5","geekbench5"]##13--20
elif workloadArray =='c':
    print("Selected Benchmarks: SPEC CPU 2006\n")
    benchmarks = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50]
    benchmarksNames = ["perlbench_base.i386","bzip2_base.i386","gcc_base.i386","mcf_base.i386","gobmk_base.i386","hmmer_base.i386","sjeng_base.i386","libquantum_base.i386",##0--7
        "h264ref_base.i386","omnetpp_base.i386","astar_base.i386","xalancbmk_base.i386","bwaves_base.i386","gamess_base.i386","milc_base.i386","zeusmp_base.i386",##8--15
        "gromacs_base.i386","cactusADM_base.i386","leslie3d_base.i386","namd_base.i386","soplex_base.i386","povray_base.i386","GemsFDTD_base.i386",##16--22
        "lbm_base.i386","perlbench_base.i386","calculix_base.i386","perlbench_r_base.InitialTest-m64","cpugcc_r_base.InitialTest-m64",##23--27
        "mc_r_base.InitialTest-m64","omnetpp_r_base.InitialTest-m64","cpuxalan_r_base.InitialTest-m64","x264_r_base.InitialTest-m64","deepsjeng_r_base.InitialTest-m64","leela_r_base.InitialTest-m64",##28--33
        "exchange2_r_base.InitialTest-m64","xz_r_base.InitialTest-m64","bwaves_r_base.InitialTest-m64","cactuBSSN_r_base.InitialTest-m64","lbm_r_base.InitialTest-m64","wrf_r_base.InitialTest-m64","imagick_r_base.InitialTest-m64",##34--40
        "nab_r_base.InitialTest-m64","fotonik3d_r_base.InitialTest-m64","roms_r_base.InitialTest-m64","namd_r_base.InitialTest-m64","parest_r_base.InitialTest-m64","povray_r_base.InitialTest-m64","xz_r_base.InitialTest-m64",##41--47
        "xz_r_base.InitialTest-m64","exchange2_r_base.InitialTest-m64","perlbench_r_base.InitialTest-m64"]##48--50
    benchmarksNamesComplete = ["perlbench checkspam","bzip2","gcc","mcf","gobmk","hmmer","sjeng","libquantum",##0--7
        "h264ref","omnetpp","astar","xalancbmk","bwaves","gamess","milc","zeusmp",##8--15
        "gromacs","cactusADM","leslie3d","namd","soplex","povray","GemsFDTD",##16--22
        "lbm","perlbench diffmail","calculix","perlbench_r checkspam","gcc_r",##23--27
        "mcf_r","omnetpp_r","xalancbmk_r","x264_r","deepsjeng_r","leela_r",##28--33
        "exchange2_r","xz_r 1","bwaves_r","cactuBSSN_r","lbm_r","wrf_r","imagick_r",##34--40
        "nab_r","fotonik3d_r","roms_r","namd_r","parest_r","povray_r","xz_r 2",##41--47
        "xz_r 3","exchange2_r","perlbench_r diffmail"]##48--50
elif workloadArray =='d':
    print("Selected Benchmarks: Phoronix\n")
    benchmarks = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15]
    benchmarksNames = ["perlbench checkspam","bzip2","gcc","mcf","gobmk","hmmer","sjeng","libquantum",##0--7
        "h264ref","omnetpp","astar","xalancbmk","bwaves","gamess","milc","zeusmp"]##8--15
    benchmarksNamesComplete = ["perlbench checkspam","bzip2","gcc","mcf","gobmk","hmmer","sjeng","libquantum",##0--7
        "h264ref","omnetpp","astar","xalancbmk","bwaves","gamess","milc","zeusmp"]##8--15
else:
    print("Array value incorrect.")
    sys.exit(1)
listaInstructions=[]

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
escribir_names_complete = open("array_names_complete["+time+"].txt","w")
escribir_array.write("unsigned long int bench_Instructions [NUM_BENCHMARKS] = {\n\t")
escribir_names.write("char *bench_Names [NUM_BENCHMARKS] = {\n\t")
escribir_names_complete.write("char *bench_Names_complete [NUM_BENCHMARKS] = {\n\t")

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
        escribir_names_complete.write('"'+benchmarksNamesComplete[cont]+'"'+",")
    else:
        escribir_array.write(value)
        escribir_names.write('"'+benchmarksNames[cont]+'"')
        escribir_names_complete.write('"'+benchmarksNamesComplete[cont]+'"')
    if conta == 7:
        fin = ini+conta
        escribir_array.write("//"+str(ini)+"--"+str(fin)+"\n\t")
        escribir_names.write("//"+str(ini)+"--"+str(fin)+"\n\t")
        escribir_names_complete.write("//"+str(ini)+"--"+str(fin)+"\n\t")
        ini = fin+1
        conta = 0
    else:
        conta = conta+1
    cont=cont+1
escribir_array.write("\n};")
escribir_names.write("\n};")
escribir_names_complete.write("\n};")
escribir_array.close()
escribir_names.close()
escribir_names_complete.close()
not_working.close()
working.close()