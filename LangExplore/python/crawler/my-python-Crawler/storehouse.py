#!/usr/bin/python -tt
import sys
import urllib, urllib2
from urllib2 import urlopen

'''  Store is used by engin for maintaining global list of URLs and by Download for maintaing Local list for the current url '''

class Store:
	def __init__(self):
		self.URLs = []
	
	def add_url(self,url):
		'''This condition maintains distinct URLs'''
		if url not in self.URLs:
			self.URLs.append(url)
			print 'found url : ', url