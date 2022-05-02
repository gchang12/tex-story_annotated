import re
from os import walk, sep

folder='annotations-copy'

def add_bf(line):
    line=line.split(' ')
    line[0]='{\\bf '+line[0]+'}'
    line=' '.join(line)
    return line

def fix_quotes(file):
    with open(file) as rfile:
        file_contents=rfile.readlines()
    pattern='^"|(?<=\s)"'
    with open(file,'w') as wfile:
        while file_contents:
            c=file_contents.pop(0)
            c=add_bf(c)
            c=re.sub(pattern,'``',c)
            wfile.write(c)

def remove_bf(line):
    if line[:4] != '{\\bf':
        return line
    line=line.split(' ')
    line[1]=line[1].replace('}','')
    line=' '.join(line[1:])
    return line

def unfix_quotes(file):
    with open(file) as rfile:
        file_contents=rfile.readlines()
    with open(file,'w') as wfile:
        while file_contents:
            c=file_contents.pop(0)
            c=remove_bf(c)
            c=c.replace('``','"')
            wfile.write(c)

def fix_file(to_raw=False):
    if to_raw:
        action=unfix_quotes
    else:
        action=fix_quotes
    for x,y,filelist in walk(folder):
        while filelist:
            file=filelist.pop(0)
            file=sep.join([x,file])
            action(file)

fix_file(True)
