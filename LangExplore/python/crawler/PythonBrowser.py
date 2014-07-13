import urllib, urllib2, cookielib, urlparse
import HtmlLibrary
import os, base64, traceback, time

class PythonBrowser:
    def __init__(self):
        self.__currPage = None
        self.__currPageContent = None
        self.__cj = cookielib.CookieJar()
        self.__opener = urllib2.build_opener(urllib2.HTTPCookieProcessor(self.__cj))
        self.__opener.addheaders = [('User-agent', 'Mozilla/5.0')]        
        
    def goToPage(self, urlToClick, userName = None, password = None,  numOfTries = 3):
        i = 0
        succ = False
        while (not(succ) and i < numOfTries):
            try:
                self.__goPage(urlToClick, userName, password)
                succ = True
            except:
                traceback.print_exc()
                print 'Retrying...'
                if (i +1 < numOfTries):
                    time.sleep(5)
            i = i + 1;
        
    def __goPage(self, urlToClick, userName = None, password = None):
        if (not(urlToClick.startswith('http')) and self.__currPage<> None):
            urlToClick = urlparse.urljoin(self.__currPage.geturl(), urlToClick)
        try:
            if (userName<> None and password<> None):
                req = urllib2.Request(urlToClick)
                base64string = base64.encodestring(
                '%s:%s' % (userName, password))[:-1]
                authheader =  "Basic %s" % base64string
                req.add_header('Authorization', authheader)
                
                self.__currPage = self.__opener.open(req)
                self.__currPageContent = self.__currPage.read()
                self.__currPage.close()
                self.__opener.close()
                return
            self.__currPage = self.__opener.open(urlToClick)
            self.__currPageContent = self.__currPage.read()
            self.__currPage.close()
            self.__opener.close()
        except urllib2.HTTPError, e:
            self.__currPage = e.fp
            self.__currPageContent = e.fp.read()
            raise

    def openHtmlInBrowser(self):
        if (self.__currPage== None):
            raise NameError('no page to open, please go to a page, before calling this')
        tmpFile= open('tmp.html','w')
        tmpFile.write(self.__currPageContent)
        tmpFile.close()
        os.startfile('tmp.html')

    def getCurrentPageContent(self):
        return self.__currPageContent

    def getCurrentPageUrl(self):
        if (self.__currPage== None):
            return None
        return self.__currPage.geturl()

    def submitForm(self, formMethod, formAction, formData):
        if (not(formAction.startswith('http')) and self.__currPage<> None):
            formAction = urlparse.urljoin(self.__currPage.geturl(), formAction)
        
        if (formAction == None or len(formAction)==0):
            raise NameError('ArgumentError: form action cann\'t be empty')
        if (type(formData) <> str and type(formData) <> dict):
            raise NameError('ArgumentError: formData should be string or dictionary')

        if (type(formData) == dict):
            formData = urllib.urlencode(formData)
        
        if (formMethod == 'post'):
            self.__currPage = self.__opener.open(formAction, formData)
            self.__currPageContent = self.__currPage.read()
            self.__currPage.close()
            self.__opener.close()
        elif (formMethod == 'get'):
            self.__currPage = self.__opener.open(formAction.strip('?') + '?%s' % formData)
            self.__currPageContent = self.__currPage.read()
            self.__currPage.close()
            self.__opener.close()
        else:
            raise NameError('Not Supported "%s" method for form.'%formMethod)


    def goToLinkName(self, linkName):
        if (self.__currPage== None):
            raise NameError('no page to open, please go to a page, before calling this')
        
        res = HtmlLibrary.getLinks(self.__currPageContent, linkName)
            
        if (len(res)==1):
            link = res.items()[0][1]
            if (link.startswith('http')):
                nextUrl = linkName
            else:
                nextUrl = urlparse.urljoin(self.getCurrentPageUrl(), link)
            self.goToPage(nextUrl)
        elif (len(res)==0):
            if (type(linkName) == str):
                print linkName
            else:
                print 'Regex: ' + linkName.pattern
            raise NameError('Didn\'t found the link')
        else:
            if (type(linkName) == str):
                print linkName
            else:
                print 'Regex: ' + linkName.pattern
            raise NameError('more then one link for this name, so please choose manually.')
        return res   
