#!/usr/bin/python
# Filename: objvar.py

class Person:
    '''Represents a person.'''
    population = 0                  #类变量

    def __init__(self, name):       #构造函数，自动调用
        '''Initializes the person's data.'''
        self.name = name            #实例对象变量
        print ( '(Initializing %s)' % self.name ) 

        # When this person is created, he/she
        # adds to the population
        Person.population += 1

    def __del__(self):              #析构函数，自动调用
        '''I am dying.'''
        print ( '%s says bye.' % self.name )

        Person.population -= 1

        if Person.population == 0:
            print ( 'I am the last one.' )
        else:
            print ( 'There are still %d people left.' % Person.population )

    def sayHi(self):
        '''Greeting by the person.

        Really, that's all it does.'''
        print ( 'Hi, my name is %s.' % self.name )

    def howMany(self):
        '''Prints the current population.'''
        if Person.population == 1:
            print ( 'I am the only person here.' )
        else:
            print ( 'We have %d persons here.' % Person.population )

swaroop = Person('Swaroop')
swaroop.sayHi()
swaroop.howMany()

kalam = Person('Abdul Kalam')
kalam.sayHi()
kalam.howMany()

swaroop.sayHi()
swaroop.howMany() 

del swaroop
del kalam
