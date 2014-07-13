ifs = open('log_whole_sim_window8_optimize1.txt','r');
while 1:
    line = ifs.readline()
    if not line:    break;
    if len(line) <= 2 : continue;
    content = line.split(' ');
#    print (content)
    if content[0] =='paras' :
        line = ifs.readline();  line= line[:len(line)-1]; content = line.split('\t');
        print (content)
