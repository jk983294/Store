#!/usr/bin/python -tt
import sys
import os
import urllib, urllib2
import HTMLParser
import urlparse
from urllib import urlretrieve
from urllib2 import urlopen
from storehouse import Store
from change_dir import Change_Dir
from BeautifulSoup import BeautifulSoup

class Img_Downloader:
	store = []
	def __init__(self, url):
		self.url = url
		
	def start(self):
		change_Dir = Change_Dir('image')
		try:
			page = urllib.urlopen(self.url)
			soup = BeautifulSoup(page)
			
			for img in soup.findAll('img'):
				img_url = urlparse.urljoin(self.url, img['src'])
				
				if img_url not in Img_Downloader.store:
					print "Image found : %(src)s" % img
					filename = img["src"].split("/")[-1]
					response = urllib.urlopen(img_url)
					f = open(filename,'wb')
					f.write(response.read())
					f.close()
					Img_Downloader.store.append(img_url)
					
		except IOError:
			pass
		except HTMLParser.HTMLParseError:
			pass
		
		change_Dir.__del__()