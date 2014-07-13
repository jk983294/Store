#

source_dir = ''

f = open('poem.txt', 'w') # open for 'w'riting
f.write(poem) # write text to file
f.close() # close the file

f = open('poem.txt')
# if no mode is specified, 'r'ead mode is assumed by default
while True:
    line = f.readline()
    if len(line) == 0: # Zero length indicates EOF
        break
    print ( line, )
    # Notice comma to avoid automatic newline added by Python
f.close() # close the file









for item in os.listdir('.'): # 遍历指定目录
    if os.path.isfile(item) and item.endswith('.txt'): # 判断是否为.txt文件
        f = open(item) # 打开文件

        for line in f: # 读入文件的每一行
            if line.startswith('用户名'): # 变量初始化
                uid = age = sex = None    
            elif line.startswith("用户id"): # 根据每行开始内容获取数据
                uid = line.split()[1]
            elif line.startswith("年龄"):
                age = line.split()[1]
            elif line.startswith("性别"):
                sex = line.split()[1]
                users.append([uid, age, sex]) # 将所获得的数据以列表的形式追加到数组中
                
        f.close() # 关闭文件

print(users) # 打印数组内容
