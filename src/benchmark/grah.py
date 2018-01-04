#!/usr/bin/python3

from sys import argv
from math import log2

FILE_SIZE_ON_READ = {1: 2097152, 2: 4194304, 3: 4194304, 4: 4194304}

directory = argv[1]

fin = open(directory + "/streams.csv")
input1 = {}
input2 = {}
input3 = {}
input3notpow = {}
input3pow = {}
input4 = {}
output1 = {}
output2 = {}
output3 = {}
output3notpow = {}
output3pow = {}
output4 = {}
inputs  = [input1, input2, input3, input4]
outputs = [output1, output2, output3, output4]
input3filen = {}
input4filen = {}
output3filen = {}
output4filen = {}
inputfilens = [None, None, input3filen, input4filen]
outputfilens = [None, None, output3filen, output4filen]
output3_1024 = {}
output4_32 = {}
output4_1024 = {}
output4_4_128 = {}
data = {"input": inputs, "output": outputs}

for line in fin :
    if line.startswith('"./streams') :
        output = None
        line = line.split(',')
        command = line[0][1:-1].split(' ')
        action = command[1]
        impl = int(command[2])
        output = data[action][impl-1]
        majorpf = int(line[5])
        vw = int(line[6])

        filec = int(command[3])
        files = None
        param = None
        if action == 'input' :
            files = FILE_SIZE_ON_READ[impl]
            param = int(command[4])
        else :
            files = int(command[4])
            param = int(command[5])




        # to plots
        # as a function of B
        d = output.get(param, [[], [], []])
        size = int(filec*files*4)
        time = float(line[1])+float(line[2])
        d[0].append((size,time))
        d[1].append(majorpf)
        d[2].append(vw)
        output[param] = d
        if impl == 3 :
            if action == 'output' :
                if abs(log2(param*4)-int(log2(param*4))) > 0.001 :
                    output = output3notpow
                else:
                    output = output3pow
            else :
                if abs(log2(param*4)-int(log2(param*4))) > 0.001 :
                    output = input3notpow
                else :
                    output = input3pow
            d = output.get(param, [[], [], []])
            d[0].append((size,time))
            d[1].append(majorpf)
            d[2].append(vw)
            output[param] = d


        # as a function of files
        if impl == 3 or impl == 4:
            if action == 'output' :
                output = outputfilens[impl-1]
            else :
                output = inputfilens[impl-1]
            d = output.get(filec, [[],])
            d[0].append((size,time))
            output[filec] = d
        if impl == 3 and param == 1024 and action == 'output' :
            output = output3_1024
            d = output.get(filec, [[],])
            d[0].append((size,time))
            output[filec] = d
        if impl == 4 and param == 32 and action == 'output' :
            output = output4_32
            d = output.get(filec, [[],])
            d[0].append((size,time))
            output[filec] = d
        if impl == 4 and param == 1024 and action == 'output' :
            output = output4_1024
            d = output.get(filec, [[],])
            d[0].append((size,time))
            output[filec] = d
        if impl == 4 and 4 <= param and param <= 128 and action == 'output' :
            output = output4_4_128
            d = output.get(filec, [[],])
            d[0].append((size,time))
            output[filec] = d






#exit(0)
import matplotlib.pyplot as plt

def overallavg(line): 
    div = sum(map(lambda x:x[1],line))
    if div == 0:
        return 0
    return sum(map(lambda x:x[0],line))/div

def plotoutput(output, line, value) :
    outputx = []
    outputy = []

    keys = sorted(output.keys())
    for k in keys :
        outputx.append(k)
        outputy.append(value(output[k][line]))

    plt.plot(outputx, outputy, "o-")

def saveplot(outputs, line, name, xlabel, ylabel, log = True, value = lambda line : sum(line)/len(line)) :
    fig, ax = plt.subplots()
    if log:
        ax.set_xscale('log')

    for output in outputs :
        plotoutput(output, line, value)

    plt.ylabel(ylabel)
    plt.xlabel(xlabel)
    fig.savefig(directory + "/" + name + '.png', bbox_inches='tight')
    plt.close(fig)

saveplot((input3, input4), 0, "is34", "B [stream3 in int32 / stream4 in pages]", "speed in B/s", False, value=overallavg)
saveplot((input3,), 0, "is3full", "B in int32 (= 4 bytes)", "speed in B/s", True, value = overallavg)
saveplot((input3,), 1, "is3pf", "B in int32 (= 4 bytes)", "page faults", True)
saveplot((input3,), 2, "is3vw", "B in int32 (= 4 bytes)", "volontary context switch", True)
saveplot((input3filen, ), 0, "is3f", "files", "speed in B/s", False, value=overallavg)
saveplot((input3pow,input3notpow), 0, "is3", "B in int32 (= 4 bytes)", "speed in B/s", True, value = overallavg)
saveplot((input4,), 0, "is4", "B in pages (system dependent, here 1024B)", "speed in B/s", True, value = overallavg)
saveplot((input4,), 1, "is4pf", "B in pages (system dependent, here 1024B)", "page faults", True)
saveplot((input4,), 2, "is4vw", "B in pages (system dependent, here 1024B)", "volontary context switch", True)
saveplot((input4filen, ), 0, "is4f", "files", "speed in B/s", False, value=overallavg)
saveplot((output3, output4), 0, "os34", "B [stream3 in int32 / stream4 in pages]", "speed in B/s", False, value=overallavg)
saveplot((output3,), 0, "os3full", "B in int32 (= 4 bytes)", "speed in B/s", True, value = overallavg)
saveplot((output3,), 1, "os3pf", "B in int32 (= 4 bytes)", "page faults", True)
saveplot((output3,), 2, "os3vw", "B in int32 (= 4 bytes)", "volontary context switch", True)
saveplot((output3_1024, output4_1024), 0, "os34f1024f1024", "files", "speed in B/s", False, value=overallavg)
saveplot((output3_1024, output4_32), 0, "os34f1024f32", "files", "speed in B/s", False, value=overallavg)
saveplot((output3_1024, output4_4_128), 0, "os34f1024f4_128", "files", "speed in B/s", False, value=overallavg)
saveplot((output3_1024, output4filen), 0, "os34f1024f", "files", "speed in B/s", False, value=overallavg)
saveplot((output3filen, ), 0, "os3f", "files", "speed in B/s", False, value=overallavg)
saveplot((output3filen, output4filen), 0, "os34f", "files", "speed in B/s", False, value=overallavg)
saveplot((output3pow,output3notpow), 0, "os3", "B in int32 (= 4 bytes)", "speed in B/s", True, value = overallavg)
saveplot((output4,), 0, "os4", "B in pages (system dependent, here 1024B)", "speed in B/s", True, value = overallavg)
saveplot((output4,), 1, "os4pf", "B in pages (system dependent, here 1024B)", "page faults", True)
saveplot((output4,), 2, "os4vw", "B in pages (system dependent, here 1024B)", "volontary context switch", True)
saveplot((output4filen, ), 0, "os4f", "files", "speed in B/s", False, value=overallavg)
