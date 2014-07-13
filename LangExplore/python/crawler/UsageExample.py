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
print 'Lets search for first 10 images...'
allImages, allImagesTitles = HtmlLibrary.findElementNameBegin(pageContent, 'img')
i  =0;
for pic in allImages:
    print pic
    print '#######################'
    i = i +1;
    if (i>10):
        break

print
forms, formsMethods, formsActions = HtmlLibrary.findFormsInPage(pageContent)
if (len(forms) ==0):
    print 'No forms found on page'
else:
    form = forms[-1]
    formMethod = formsMethods[-1]
    formAction = formsActions[-1]
    print 'found last form: "%s"'%form
    print '********************'
    print 'form Method: "%s"'%formMethod
    print 'form Action: "%s"'%formAction

    print
    print 'Form Inputs:'
    formInputData = HtmlLibrary.getFormAllInputs(form)
    for fieldName, fieldListValues in formInputData.items():
        if (fieldListValues == None):
            fieldListValues = list()
        if (len(fieldListValues) > 1):
            print '%s:=(%s)'%(fieldName, ', '.join(fieldListValues))
        else:
            print '%s:="%s"'%(fieldName, ', '.join(fieldListValues))

    formAll, formTitle = HtmlLibrary.findElementNameBegin(form, 'form')
    formAttributes = HtmlLibrary.getAllAttributes(formTitle[0])

print
print 'Form Attributes:'
print '\n'.join(map(lambda kv: '%s:"%s"'%(kv[0], kv[1]) ,formAttributes.items()))



# now lets use the form to search youtube:
formInputData['search_query'] = 'Blomen'
p.submitForm(formMethod, formAction, formInputData)

print
print 'Open page after search..'
p.openHtmlInBrowser()


