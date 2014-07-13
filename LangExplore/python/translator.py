#!/usr/bin/python
# -*- coding: utf-8 -*-


import struct
import sys
import binascii 
import pdb
#�ѹ���scel�ʿ���Ǳ�����ı���unicode���룬ÿ�����ֽ�һ���ַ������ĺ��ֻ���Ӣ����ĸ��
#�ҳ���ÿ���ֵ�ƫ��λ�ü���
#��Ҫ������
#1.ȫ��ƴ����ò�������е�ƴ����ϣ��ֵ���
#       ��ʽΪ(index,len,pinyin)���б�
#       index: �����ֽڵ����� �������ƴ��������
#       len: �����ֽڵ����� ƴ�����ֽڳ���
#       pinyin: ��ǰ��ƴ����ÿ���ַ������ֽڣ��ܳ�len
#       
#2.��������
#       ��ʽΪ(same,py_table_len,py_table,{word_len,word,ext_len,ext})��һ���б�
#       same: �����ֽ� ���� ͬ��������
#       py_table_len:  �����ֽ� ����
#       py_table: �����б�ÿ�����������ֽ�,ÿ����������һ��ƴ��������
#
#       word_len:�����ֽ� ���� �������Ĵ����ֽ�������
#       word: ���Ĵ���,ÿ�����ĺ��������ֽڣ��ܳ���word_len
#       ext_len: �����ֽ� ���� ������չ��Ϣ�ĳ��ȣ�������10
#       ext: ��չ��Ϣ ǰ�����ֽ���һ������(��֪���ǲ��Ǵ�Ƶ) ��˸��ֽ�ȫ��0
#
#      {word_len,word,ext_len,ext} һ���ظ�same�� ͬ���� ��ͬƴ����

#ƴ����ƫ�ƣ�
startPy = 0x1540;


#��������ƫ��
startChinese = 0x2628;

#ȫ��ƴ����

GPy_Table ={}

#�������
#Ԫ��(��Ƶ,ƴ��,���Ĵ���)���б�
GTable = []

def byte2str(data):
    '''��ԭʼ�ֽ���תΪ�ַ���'''
    i = 0;
    length = len(data)
    ret = u''
    while i < length:
        x = data[i] + data[i+1]
        t = unichr(struct.unpack('H',x)[0])
        if t == u'\r':
            ret += u'\n'
        elif t != u' ':
            ret += t
        i += 2
    return ret
#��ȡƴ����
def getPyTable(data):

    if data[0:4] != "\x9D\x01\x00\x00":
        return None
    data = data[4:]
    pos = 0
    length = len(data)
    while pos < length:
        index = struct.unpack('H',data[pos]+data[pos+1])[0]
        #print index,
        pos += 2
        l = struct.unpack('H',data[pos]+data[pos+1])[0]
        #print l,
        pos += 2
        py = byte2str(data[pos:pos+l])
        #print py
        GPy_Table[index]=py
        pos += l

#��ȡһ�������ƴ��
def getWordPy(data):
    pos = 0
    length = len(data)
    ret = u''
    while pos < length:
        
        index = struct.unpack('H',data[pos]+data[pos+1])[0]
        ret += GPy_Table[index]
        pos += 2    
    return ret

#��ȡһ������
def getWord(data):
    pos = 0
    length = len(data)
    ret = u''
    while pos < length:
        
        index = struct.unpack('H',data[pos]+data[pos+1])[0]
        ret += GPy_Table[index]
        pos += 2    
    return ret

#��ȡ���ı�    
def getChinese(data):
    #import pdb
    #pdb.set_trace()
    
    pos = 0
    length = len(data)
    while pos < length:
        #ͬ��������
        same = struct.unpack('H',data[pos]+data[pos+1])[0]
        #print '[same]:',same,
        
        #ƴ����������
        pos += 2
        py_table_len = struct.unpack('H',data[pos]+data[pos+1])[0]
        #ƴ��������
        pos += 2
        py = getWordPy(data[pos: pos+py_table_len])

        #���Ĵ���
        pos += py_table_len
        for i in xrange(same):
            #���Ĵ��鳤��
            c_len = struct.unpack('H',data[pos]+data[pos+1])[0]
            #���Ĵ���
            pos += 2  
            word = byte2str(data[pos: pos + c_len])
            #��չ���ݳ���
            pos += c_len        
            ext_len = struct.unpack('H',data[pos]+data[pos+1])[0]
            #��Ƶ
            pos += 2
            count  = struct.unpack('H',data[pos]+data[pos+1])[0]

            #����
            GTable.append((count,py,word))
        
            #���¸��ʵ�ƫ��λ��
            pos +=  ext_len


def deal(file_name):
    print '-'*60
    f = open(file_name,'rb')
    data = f.read()
    f.close()
    
    
    if data[0:12] !="\x40\x15\x00\x00\x44\x43\x53\x01\x01\x00\x00\x00":
        print "ȷ����ѡ������ѹ�(.scel)�ʿ�?"
        sys.exit(0)
    #pdb.set_trace()
    
    print "�ʿ�����" ,byte2str(data[0x130:0x338])#.encode('GB18030')
    print "�ʿ����ͣ�" ,byte2str(data[0x338:0x540])#.encode('GB18030')
    print "������Ϣ��" ,byte2str(data[0x540:0xd40])#.encode('GB18030')
    print "�ʿ�ʾ����",byte2str(data[0xd40:startPy])#.encode('GB18030')
    
    getPyTable(data[startPy:startChinese])
    getChinese(data[startChinese:])
    
        
if __name__ == '__main__':

    #��Ҫת���Ĵʿ����������Ϳ�����
    o = ['�ɶ������ز�С�Դ�ȫ.scel',
    '�ɶ������ز�С�Դ�ȫ.scel',
    ]
    
    for f in o:
        deal(f)
        
    #������  
    f = open('sougou.txt','w')
    for count,py,word in GTable:
        #GTable�����Ž������һ���б�ÿ��Ԫ����һ��Ԫ��(��Ƶ,ƴ��,���Ĵ���)������Ҫ�Ļ����Ա�����Լ���Ҫ����ʽ
        #��û�������Խ���ǰ������������ļ���˳��
        f.write( unicode('{%(count)s}' %{'count':count}+py+' '+ word).encode('GB18030') )#���ձ����ļ��ı��룬�����Ը���
        f.write('\n')
    f.close()    
