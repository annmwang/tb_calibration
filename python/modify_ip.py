"""
modify_ip.py -- a script to modify the boardId 

Run like:
> python modify_ip.py
"""
import argparse
import array
import copy
import sys
import time
import ROOT

def main():

    ops = options()

    # gather ye inputs
    fCALIB = ops.i or fatal("Please provide a -i input ROOT file")

    fCALIB = ROOT.TFile.Open(fCALIB)

    trCALIB = fCALIB.Get("calib")

    if not trCALIB: fatal("Couldnt find TTree (calib) within %s" % (ops.i))

    entsCALIB = trCALIB.GetEntries()
    newip = int(ops.ip) or fatal("Please provide an IP to change to")

    print
    print "input CALIB  : %s" % (fCALIB.GetName())
    print "output       : %s" % (ops.o)
    print "new IP       : %d" % (newip)
    print
    print "CALIB events : %i" % (entsCALIB)
    print

    start_time = time.time()

    boardid = ROOT.vector("int")()

    if (ops.xadc):
        boardid = array.array("i",[0])
    
    # configure output
    # add branches to our cloned tree
    outfile = ROOT.TFile(ops.o, "recreate")

    clonetree = trCALIB.CloneTree(0)

    clonetree.SetBranchAddress("boardId", boardid)

    # MM loop
    for entCALIB in xrange(entsCALIB):

        _ = trCALIB.GetEntry(entCALIB)
        if not ops.xadc:
            boardid.clear()

        if ops.xadc:
            boardid[0] = newip
        else:
            entsize = len(trCALIB.boardId)
            for i in range(entsize):
                boardid.push_back(newip)
        clonetree.Fill()
        if (entCALIB % (entsCALIB/10) == 0):
            progress(time.time() - start_time, entCALIB, entsCALIB)

    print

    # save
    clonetree.GetCurrentFile().Write()
    clonetree.GetCurrentFile().Close()

def fatal(msg):
    sys.exit("Fatal error: %s" % (msg))

class color:
    BLUE  = "\033[94m"
    GREEN = "\033[92m"
    RED   = "\033[91m"
    GRAY  = "\033[90m"
    END   = "\033[0m"

def options():
    parser = argparse.ArgumentParser(usage=__doc__, formatter_class=argparse.ArgumentDefaultsHelpFormatter)
    parser.add_argument("-i",      help="input file")
    parser.add_argument("-o",      help="output file")
    parser.add_argument("--xadc",  help="flag for xADC files",  action="store_true")
    parser.add_argument("--ip",    help="ip to change board id to")
    return parser.parse_args()

def progress(time_diff, nprocessed, ntotal):
    nprocessed, ntotal = float(nprocessed), float(ntotal)
    rate = (nprocessed+1)/time_diff
    msg = "\r > %6i / %6i | %2i%% | %8.2fHz | %6.1fm elapsed | %6.1fm remaining"
    msg = msg % (nprocessed, ntotal, 100*nprocessed/ntotal, rate, time_diff/60, (ntotal-nprocessed)/(rate*60))
    sys.stdout.write(msg)
    sys.stdout.flush()


if __name__ == "__main__":
    main()
