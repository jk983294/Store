import HtmlLibrary
from PythonBrowser import PythonBrowser
import re
import urlparse

p = PythonBrowser()
p.goToPage('http://www.youku.com/')

print 'going to "%s"...'%p.getCurrentPageUrl()
pageContent = p.getCurrentPageContent()

print 'Opening Page in browser for view..'
p.openHtmlInBrowser()

regexSearchPatternForLinks = re.compile('(signin|sign in|login|log in|browse)', re.IGNORECASE)
links = HtmlLibrary.getLinks(pageContent, regexSearchPatternForLinks)

print 'Links on page matching pattern:'
for link, linkHref in links.items():
    print 'Html Link: "%s"'%link
    print '#############################'


print
print 'Open page after search..'
p.openHtmlInBrowser()


