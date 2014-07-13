#!/usr/bin/python -tt
import sys
import urlparse
import HTMLParser
import urllib, urllib2
from urllib2 import urlopen
from storehouse import Store
from validator import Valid
from BeautifulSoup import BeautifulSoup

''' Download is a the module that finds and stores the urls found and extracted from a given url '''

class Download:
	
	def __init__(self,url,max):
		self.url = url
		self.store = Store()
		''' here max no. of urls that is extracted from a given link is set as 12 this may be changed as desired or passed from main function by taking as input from user'''
		max = (10 if max > 10 else max)
		self.max = max
		
	def start(self):
		print 'searching for urls in : ',self.url,'...'
		page = urllib.urlopen(self.url)
		''' Basic link extraction procedure with BeautifulSoup (I had version 3.1.0.1) '''
		try:
			soup = BeautifulSoup(page)
			for link_tag in soup.findAll('a'):
				if link_tag ['href'] and len(link_tag['href'])>0:
					new_url = urlparse.urljoin(self.url,link_tag['href'])
					''' after getting the link a valid DS is created for the URL'''
					valid = Valid(new_url)
					valid.check()
					''' If the link is found valid it is appended to the local store of download '''
					if valid.val:
						self.store.URLs.append(new_url)
				if len(self.store.URLs)>self.max:
					break
		except HTMLParser.HTMLParseError:
			pass
		except KeyError:
			pass
			