#!/usr/bin/python -tt
import sys
import urllib, urllib2
from urllib2 import urlopen

URL_List = []                                          # This list is used to store the URLs
MAX = 25                                               # No of URLs you want

def check_n_push(url):
	try:
		code = urlopen(url).code
	except ValueError:                                 # Eliminating URLs with ValueErrors
		pass
	except urllib2.HTTPError:                          # Eliminating URLs with HTTPErrors
		pass
	except urllib2.URLError:                           # Eliminating URLs with other URLErrors
		pass
	else:
		if (url not in URL_List) and (code < 400):     # To store distinct URLs
			URL_List.append(url)                       # Pushing the good URLs obtained in list
			print 'got',len(URL_List), 'urls ',url     # Printing URL at output screen

def get_all_links(url):
	
	start_tag = '<a href=\"'                           # For pattern based search. I avoided using regular expression for comparision cause this was giving better results for http://recruiterbox.com/
	end_tag = '\"'
	
	page = urllib.urlopen(url)                         # To store page source
	data = page.read().split("</a>")                   # To split page source based on "</a>"
	print page
	for item in data:
		if "<a href" in item:                          # Pattern based
			try:                                       # search
				ind = item.index(start_tag)			   # to
				item = item[ind + len(start_tag):]     # find
				end = item.index(end_tag)              # URL
			except: pass                               # Eliminating cases where end_tag < -1
			else:
				new_url = item[:end]                   # The new URL obtained
				check_n_push(new_url)                  # To check n push the url in URL_List
				
				if (new_url not in URL_List):          # To store distinct URLs
					if not new_url.startswith(url):
						now_new_url = url + new_url    # The actual new URL may be preceeded by the url of the page containing it
						check_n_push(now_new_url)      # To check n push the url in URL_List
					if not new_url.startswith('http'):
						now_new_url = 'http' + new_url # The actual new URL may be preceeded by http
						check_n_push(now_new_url)      # To check n push the url in URL_List
						
		if len(URL_List)>=MAX:                          # break if No of URLs exceed MAX allowed value
			break
			
def main():
	
	firstURL = 'http://recruiterbox.com'              # The assumed firstURL in case u want to enter the first URL ur self uncomment the next line
	#firstURL = raw_input('-->')
	check_n_push(firstURL)                            # Adding the first url to the list
	
	for url in URL_List:
		get_all_links(url)                            # calling get_all_link to get all links in this url
		if len(URL_List)>=MAX:                        # break if No of URLs exceed MAX allowed value
			break
	
	file_w = open('go_crawls_URL_LIST.txt','w')       # Storing all those link in this go_crawls_URL_List.txt file
	for url in URL_List:
		file_w.write(url)
		file_w.write('\n')
	file_w.close()

if __name__ == '__main__':
	main()