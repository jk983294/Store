#!/usr/bin/python -tt
import sys
import httplib
import HTMLParser
import urllib,urllib2
from urllib2 import urlopen

''' Valid is a datastructure that stores a url together with a 
val parameter which is set to true or false depending on whether 
the url is a valid one or not '''

class Valid:
	''' I suppose keeping a temperory repository of last 1000 links would speed up the process . Store here is that cache'''
	store = []
	def __init__(self,the_url):
		if not the_url.startswith('http'):
			the_url = 'http://' + the_url
		self.url = the_url
		self.val = False
	
	''' check function checks thevarious kinds of errors that may be associated with the url '''
	def check(self):
		if self.url not in Valid.store:
			try:
				code = urlopen(self.url).code
			except ValueError:
				self.val = False
			except urllib2.HTTPError:
				self.val = False
			except urllib2.URLError:
				self.val = False
			except httplib.InvalidURL:
				self.val = False
			else:
				if code < 400:
					self.val = True
					
		Valid.store.append(self.url)
		if len(Valid.store)>=20:
			del Valid.store[0]