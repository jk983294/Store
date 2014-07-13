#!/usr/bin/python -tt
import sys
import urllib, urllib2
from urllib2 import urlopen
from storehouse import Store
from validator import Valid
from url_downloader import Download
from img_downloader import Img_Downloader
from text_downloader import Text_Downloader
from video_downloader import Video_Downloader

class Engine:
	''' initialization of Engine with required store as global LIST for urls,valids for the first_url and max '''
	def __init__(self,first_url,max):
		self.store = Store()
		self.valid = Valid(first_url)
		self.max =  max
		
	''' Adding the first URL after checcking it '''	
	def set_first(self):
		self.valid.check()
		if self.valid.val:
			self.store.add_url(self.valid.url)
		
	def start(self):
		''' Starting the URL retrival cycle '''
		for url in self.store.URLs:
			print '\ncrawlling url : ',url,'...'
			
			''' For each URL in the URL list Engin calls an  image_downloader to download the images from the page of this url'''
			img_Downloader = Img_Downloader(url)
			img_Downloader.start()
			
			''' For each URL in the URL list Engin calls a text_downloader to download the urls from the page of this url'''
			text_Downloader = Text_Downloader(url)
			text_Downloader.start()
			
			''' For each URL in the URL list Engin calls video_downloader to download the vildeo (For now only 1 video per url if  found)'''
			# video_Downloader = Video_Downloader(url)
			# video_Downloader.start()
			
			''' rq is the required no of URLs to be found = self.max - len(self.store.URLs) '''
			rq = self.max - len(self.store.URLs)
			''' For each URL in the URL list Engin calls a url_downloader to download the urls from the page of this url'''
			if len(self.store.URLs)<self.max:
				download = Download(url,rq)
				download.start()
				
				''' Adding the newly found URLs to Global list'''
				for new_url in download.store.URLs:
					self.store.add_url(new_url)
					if len(self.store.URLs)>=self.max:
						break
