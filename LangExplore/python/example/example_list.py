#!/usr/bin/python
# Filename: using_list.py

# This is my shopping list
shoplist = ['apple', 'mango', 'carrot', 'banana']   #声明

print ( 'I have', len(shoplist),'items to purchase.' )  #获得list大小

print ( 'These items are:',  )       # Notice the comma at end of the line
for item in shoplist:
    print ( item, )

print ( '\nI also have to buy rice.' )
shoplist.append('rice')                 #list添加元素
print ( 'My shopping list is now', shoplist )

print ( 'I will sort my list now' )
shoplist.sort()
print ( 'Sorted shopping list is', shoplist )

print ( 'The first item I will buy is', shoplist[0] )
olditem = shoplist[0]
del shoplist[0]         #list删除元素
print ( 'I bought the', olditem )
print ( 'My shopping list is now', shoplist )

age = 22
name = 'Swaroop'

print ( '%s is %d years old' % (name, age) )
print ( 'Why is %s playing with that python?' % name )
