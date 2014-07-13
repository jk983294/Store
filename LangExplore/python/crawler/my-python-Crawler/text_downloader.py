#!/usr/bin/python -tt
import sys
import os
import re
import urllib, urllib2
import HTMLParser
import urlparse
import nltk
from urllib import urlopen
from storehouse import Store
from change_dir import Change_Dir
from BeautifulSoup import BeautifulSoup

class Text_Downloader:
	
	def __init__(self, url):
		self.url = url
		
	def start(self):
		change_Dir = Change_Dir('Text')
		filename = str(self.url)
		filename = re.sub(r'[^a-zA-Z0-9 ]', '', filename)[:40]
		print 'Writing in file : ',filename,'.txt'
		f = open(filename + '.txt','w')
		try:
			html = urlopen(self.url).read()
			raw = nltk.clean_html(html)
			raw = re.sub(r' +',' ',raw)
			list = raw.splitlines()
			for line in list:
				if len(line)>1:
					f.write(line)
					f.write('\n')
			
		except IOError:
			pass
		except HTMLParser.HTMLParseError:
			pass
			
		f.close()
		change_Dir.__del__()