import re as regex
import matplotlib.pyplot as plt
import sys
import signal, os

from matplotlib.backends.backend_pdf import PdfPages

if __name__ == "__main__":
    # The horrible output when CTRL+C was annoying me
    def int_handle(signum,frame):
        print()
        exit(0)
    signal.signal(signal.SIGINT, int_handle)

signal.signal(signal.SIGINT, int_handle)

# args: <INPUT_FILENAME> <OUTPUT_FILENAME>
def main(args):
    if args.__len__() != 2:
        print("Incorrect number of arguments!")
        return 0

    INPUT_FILE  = args[1]

    res_store = {}
    name_aux  = []
    res_aux   = []

    fp = open(INPUT_FILE, "r")

    for line in fp:
        elems = line.split(',')

        if elems.__len__() == 1:
            # NAME
            name_aux = line.split('\n')[0]

        else:
            # RESULTS
            for res in elems:
                if res != "" and res != "\n":
                    res_aux.append(int(res))

            res_aux.sort()

            k_best = ( res_aux[0] + res_aux[1] + res_aux[2] ) / 3;
            
            # store the results
            res_store[name_aux] = k_best
            # reset the auxiliary list
            res_aux = []

    for (k,v) in res_store.items():
        print(k + " : " + str(v))

if __name__ == "__main__":
    # main
    main(sys.argv)