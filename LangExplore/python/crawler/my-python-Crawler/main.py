#!/usr/bin/python -tt
import sys
import urllib, urllib2
from urllib2 import urlopen
from storehouse import Store
from engin import Engine
from validator import Valid
from change_dir import Change_Dir

def main(first_url,max):
	''' Engine is the class that fetches a url from global Store of URLs and calls downloader to find URLs in the page of that url'''
	engine = Engine(first_url,max)
	''' This function sets the first URL in the global store afterchecking it '''
	engine.set_first()
	''' The next 4 lines starts the retrival process of URLs if the first URL was ok '''
	if len(engine.store.URLs)>0:
		engine.start()
	else:
		print "The first URL you entered was not correct"
	
	change_Dir = Change_Dir('URLs')
	file_w = open('URL_Names.txt','w')
	for url in engine.store.URLs:
		print url
		file_w.write(url)
		file_w.write('\n')
	file_w.close()
	change_Dir.__del__()
	
if __name__ == '__main__':
	''' collecting basic two inputs '''
	first_url = raw_input('Enter a url -> ')
	max = raw_input('Enter the no of links you want to obtain -> ')
	max = (20 if max == '' else int(max))
	''' calling main() with the two values '''
	main(first_url,max)