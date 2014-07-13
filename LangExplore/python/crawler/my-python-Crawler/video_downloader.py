#!/usr/bin/python -tt
import os
import sys
import urlparse
import HTMLParser
import urllib,urllib2
from validator import Valid
from change_dir import Change_Dir
from BeautifulSoup import BeautifulSoup


class Video_Downloader:	
	store = []
	def __init__(self,url):
		self.url = url
		
	def start(self):
		page = urllib.urlopen(self.url)
		
		change_Dir = Change_Dir('video')
		try:
			soup = BeautifulSoup(page)
			for link_tag in soup.findAll('a'):
				if link_tag ['href'] and len(link_tag['href'])>0:
					new_url = urlparse.urljoin(self.url,link_tag['href'])
					''' after getting the link a valid DS is created for the URL'''
					valid = Valid(new_url)
					valid.check()
					''' If the link is found valid and watch?v is found as a part of url then it is saved in flv format '''
					if valid.val and 'watch?v=' in new_url:
						filename = str(new_url)
						filename = re.sub(r'[^a-bA-Z0-9 ]','',filename)[:40]
						f = open(filename + '.flv','w')
						response = urllib.urlopen(new_url)
						f.write(response.read())
						f.close()
						break
		except HTMLParser.HTMLParseError:
			pass
		except KeyError:
			pass
			
		change_Dir.__del__()