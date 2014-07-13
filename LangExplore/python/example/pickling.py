#!/usr/bin/python
# Filename: pickling.py

#import cPickle as p
import pickle as p

shoplistfile = 'shoplist.data'
# the name of the file where we will store the object

shoplist = ['apple', 'mango', 'carrot']

# Write to the file
f = open(shoplistfile, 'wb')        #必须以wb打开，因为pickle写成二进制格式
p.dump(shoplist, f) # dump the object to a file
f.close()

del shoplist # remove the shoplist

# Read back from the storage
f = open(shoplistfile,'rb')         #必须以rb打开，因为pickle写成二进制格式
storedlist = p.load(f)
print ( storedlist )
