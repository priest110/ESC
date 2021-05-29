from re import split
import sys
import signal, os
import matplotlib.pyplot as plt
import statistics
from matplotlib.backends.backend_pdf import PdfPages

if __name__ == "__main__":
    # The horrible output when CTRL+C was annoying me
    def int_handle(signum,frame):
        print()
        exit(0)
    signal.signal(signal.SIGINT, int_handle)


def main(args):

    in_files = [ "deb_1.txt","deb_1c.txt","deb_1k.txt","deb_2_5k.txt",
                 "io_1.txt","io_1c.txt","io_1k.txt","io_2_5k.txt",
                 "ram_1.txt","ram_1c.txt","ram_1k.txt","ram_2_5k.txt",
                 "tt_1c.txt","tt_2_5k.txt","tt_1.txt","tt_1k.txt" ]

    size = { 0 : 1,
             1 : 100,
             2 : 1000,
             3 : 2500 }

    deb   = {}
    io_p  = {}    
    io_g  = {}
    ram_p = {}
    ram_g = {}
    tt_p  = {}
    tt_g  = {}

    # Débito
    for i in range(0,4):
        in_file = open("output/" + in_files[i], "r")
        deb[size[i % 4]] = [ float(line) for line in in_file ]
    
    # Tempo de Acesso ao Ficheiro
    for i in range(4,8):
        in_file = open("output/" + in_files[i], "r")
        tmp_p = []
        tmp_g = []
        for line in in_file:
            try:
                elems = line.split(" ")
                if elems[0] == "p":
                    tmp_p.append(float(elems[1]))
                elif elems[0] == "g":
                    tmp_g.append(float(elems[1]))
            except ValueError:
                continue
        io_p[size[i % 4]] = tmp_p
        io_g[size[i % 4]] = tmp_g

    # Tempo de Acesso à Estrutura
    for i in range(8,12):
        in_file = open("output/" + in_files[i], "r")
        tmp_p = []
        tmp_g = []
        for line in in_file:
            elems = line.split(" ")
            if elems[0] == "p":
                tmp_p.append(float(elems[1]))
            elif elems[0] == "g":
                tmp_g.append(float(elems[1]))
        ram_p[size[i % 4]] = tmp_p
        ram_g[size[i % 4]] = tmp_g

    # Tempo para Ambos tipos de pedido
    for i in range(12,16):
        in_file = open("output/" + in_files[i], "r")
        tmp_p = []
        tmp_g = []
        for line in in_file:
            elems = line.split(" ")
            if elems[0] == "p":
                tmp_p.append(float(elems[1]))
            elif elems[0] == "g":
                tmp_g.append(float(elems[1]))
        tt_p[size[i % 4]] = tmp_p
        tt_g[size[i % 4]] = tmp_g    
        
    # Calculate Median and Average
    #stuff = [ deb, io_p, io_g, ram_p, ram_g, tt_p, tt_g ]
#
    #medians = []
    #avgs    = []
#
    #for s in stuff:
    #    for v in s.values():
    #        medians.append(statistics.median(v))
    #        avgs.append(statistics.mean(v))


    pp = { 1    : tt_p[1],
           100  : tt_p[100],
           1000 : tt_p[1000]         }

    pg = { 1    : tt_g[1],
           100  : tt_g[100],
           1000 : tt_g[1000]
         }

    print("PUTS:")
    for p in pp.values():
        print(p)
        print("Mean:  %1.8f" % (statistics.mean(p) * 1000.0), sep="", end="; ")
        print("Median: %1.8f" % (statistics.median(p) * 1000.0), sep="")
    
    
    print("GETS: ")
    for p in pg.values():
        print("Mean:  %1.8f" % (statistics.mean(p) * 1000.0), sep="", end="; ")
        print("Median: %1.8f" % (statistics.median(p) * 1000.0), sep="")
    
    #print("Medians: ", medians, sep="")
    #print("Means: ", avgs, sep="")


    #with PdfPages("graficos_da_pinta.pdf") as export_file:
    #    # Débito
    #    plt.scatter(list(deb.keys()), medians[:4], color='b', label="Débito (mediana)")
    #    plt.scatter(list(deb.keys()), avgs[:4], color='r', label="Débito (média)")
    #    plt.xlabel("Número de Clientes")
    #    plt.ylabel("Pedidos por segundo")
    #    plt.title("Débito")
    #    plt.grid(True)
    #    plt.legend()
    #    plt.savefig("throughput.png")
    #    export_file.savefig()
    #    plt.close()



if __name__ == "__main__":
    signal.signal(signal.SIGINT, int_handle)
    main(sys.argv)