import re

def getAllAttributes(elementNode, changeToLower=False):
    startElement = re.compile('<\s*\S+')
    toStartFrom = startElement.search(elementNode).end()
    
    attributeRe = re.compile('([^=\s]+)\s*=\s*("[^"]*"|\'[^\']*\'|[^\s>]+)')
    allMatches = attributeRe.finditer(elementNode, toStartFrom)
    d = dict()
    for match in allMatches:
        attributeName = match.group(1).strip()
        attributeValue = match.group(2).strip('"\'')
        if (changeToLower):
            attributeName = attributeName.lower()
        d[attributeName] = attributeValue
        
    return d
    

def getAttribute(elementNode, attributeName):
    allAttributes = getAllAttributes(elementNode)
    if (not(allAttributes.has_key(attributeName))):
        return -1
    else:
        return allAttributes[attributeName]

def getElementContent(elementNode):
    rStart = re.compile('(?<=<)(\s*[^\s>]+)[^>]*(?=>)')
    p = rStart.findall(elementNode)
    if (len(p) == 0):
        raise NameError('ArgumentException: elementNode isn\'t xml element, it doesn\'t contains Tag')
    tagName = re.escape(p[0])

    rAll = re.compile('(?:<\s*%s[^>]*>)(?<!/>)(.*?)(?:</\s*%s\s*>)'%(tagName,tagName), re.DOTALL | re.IGNORECASE)
    resI = rAll.finditer(elementNode)
    found =False
    for m in resI:
        found = True
        res = m.group(1)
        break
    if (not(found)):
        Rsingle = re.compile('(?:<\s*%s[^/>]*/>)'%tagName)
        if (Rsingle.match(elementNode) == None):
            print '*********************'
            print elementNode
            print '*********************'
            res = None
            raise NameError('Xml Parse Error end tag wasn\'t found')
        else:
            res = None
    return res
    
    

def findElementNameBegin(htmlContent, elementName):
    elementReg = re.compile('(<' + elementName + '\s+[^>]*>)((?<=>)|(?<=/>))'  , re.DOTALL | re.IGNORECASE)
    elements1 = elementReg.finditer(htmlContent)
    
    d = list()
    dTitle = list()
    for ele in elements1:
        elementTag = ele.group(1)
        allElement = ele.group(0)
        d.append(allElement)
        dTitle.append(elementTag)
        
    return d, dTitle

#Not working on hierarchieal elements. won't work good on "<p> <p> </p> </p>"
def findElementName(htmlContent, elementName):
    elementReg = re.compile('(<' + elementName + '\s+[^>]*>)(?<!/>).*?</\s*' + elementName + '\s*>' , re.DOTALL | re.IGNORECASE)
    elementReg2 = re.compile('(<' + elementName + '\s+[^>]*>)(?<=/>)'  , re.DOTALL | re.IGNORECASE)

    elements1 = elementReg.finditer(htmlContent)
    elements2 = elementReg2.finditer(htmlContent)
    d = list()
    dTitle = list()
    for ele in elements1:
        elementTag = ele.group(1)
        allElement = ele.group(0)
        d.append(allElement)
        dTitle.append(elementTag)
    for ele in elements2:
        elementTag = ele.group(1)
        allElement = ele.group(0)
        d.append(allElement)
        dTitle.append(elementTag)
    
    return d, dTitle

def findFormsInPage(htmlContent):
    allForms, formsTitle = findElementName(htmlContent, 'form')
    #check that they have method and action attributes and fetch them
    afterFilter = list()
    methodValues = list()
    actionValues = list()
    for i in xrange(0,len(allForms)):
        form = formsTitle[i]
        aForm = allForms[i]
        methodAtt = getAttribute(form, 'method')
        actionAtt = getAttribute(form, 'action')
        if (methodAtt == None or actionAtt == None):
            raise NameError('Html is in wrong format, more then one attribute is declared for the same element:\n"' + form + '"')
        elif (actionAtt<>-1):
            afterFilter.append(aForm)
            if (methodAtt<> -1):
                methodValues.append(methodAtt)
            else:
                methodValues.append('get')
            actionValues.append(actionAtt)
    return afterFilter, methodValues, actionValues


def getLinks(htmlContent, linkNameOrRegex):
    links,linksTitles = findElementName(htmlContent, 'a')
    res = dict()
    for i in xrange(0, len(links)):
        link = links[i]
        lT = linksTitles[i]
        hrefValue = getAttribute(lT,'href')
        try:
            linkN = getElementContent(link)
            if (type(linkNameOrRegex) <> str):
                if (linkN <> None and linkNameOrRegex.match(linkN)<> None):
                    res[link] = hrefValue
            else:
                if (linkN == linkNameOrRegex):
                    res[link] = hrefValue                        
        except:
            print '**********************'
            print link
            print '**********************'
            raise
        
    return res


def __getSelectElement(form):
    inputElement, inputElementsTitles = findElementName(form, 'select')
    d= dict()

    for i in xrange(0,len(inputElement)):
        allProperties = getAllAttributes(inputElementsTitles[i], True)
        if (allProperties.has_key('name')):
            inputName = allProperties['name']
        else:
            continue
        allOptions = getElementContent(inputElement[i])
        optionElement, optionElementsTitles = findElementName(allOptions, 'option')
        for o in optionElementsTitles:
            allO = getAllAttributes(o, True)
            if (not(allO.has_key('value'))):
                print o
                raise NameError('option with no value attribute')
            inputValue = allO['value']
            if (not(d.has_key(inputName))):
                d[inputName] = list()
            d[inputName].append(inputValue)
    return d

def __getTextAreaElement(form):
    inputElement, inputElementsTitles = findElementName(form, 'textarea')
    d = dict()
    
    for i in xrange(0,len(inputElement)):
        allProperties = getAllAttributes(inputElementsTitles[i], True)
        if (allProperties.has_key('name')):
            inputName = allProperties['name']
        else:
            continue
        inputValue = getElementContent(inputElement[i])
        if (not(d.has_key(inputName))):
            d[inputName] = list()
        d[inputName].append(inputValue)
    return d

def __getInputElemetns(form):
    inputElement, inputElementsTitles = findElementNameBegin(form, 'input')
    d = dict()
    
    for inputEle in inputElementsTitles:
        allProperties = getAllAttributes(inputEle, True)
        if (allProperties.has_key('name')):
            inputName = allProperties['name']
        else:
            continue
        inputValue = ''
        if (allProperties.has_key('value')):
            inputValue = allProperties['value']
        if (not(d.has_key(inputName))):
            d[inputName] = list()
        d[inputName].append(inputValue)
        
    return d

def __getButtonElement(form):
    inputElement, inputElementsTitles = findElementNameBegin(form, 'button')
    d = dict()
    
    for inputEle in inputElementsTitles:
        allProperties = getAllAttributes(inputEle, True)
        if (allProperties.has_key('name')):
            inputName = allProperties['name']
        else:
            continue
        inputValue = ''
        if (allProperties.has_key('value')):
            inputValue = allProperties['value']
        if (not(d.has_key(inputName))):
            d[inputName] = list()
        d[inputName].append(inputValue)
        
    return d

def getFormAllInputs(form):
    d = __getInputElemetns(form)
    
    dTextArea = __getTextAreaElement(form)
    for kv in dTextArea.items():
        fieldName = kv[0]
        listFieldValue = kv[1]
        if (not(d.has_key(fieldName))):
            d[fieldName] = list()
        for val in listFieldValue:
            d[fieldName].append(val)

    dSelect = __getSelectElement(form)
    for kv in dSelect.items():
        fieldName = kv[0]
        listFieldValue = kv[1]
        if (not(d.has_key(fieldName))):
            d[fieldName] = list()
        for val in listFieldValue:
            d[fieldName].append(val)

    dButton = __getButtonElement(form)
    for kv in dButton.items():
        fieldName = kv[0]
        listFieldValue = kv[1]
        if (not(d.has_key(fieldName))):
            d[fieldName] = list()
        for val in listFieldValue:
            d[fieldName].append(val)
    
    return d



#Just a Debug method, not really needed
def printFormInputs(form):
    allInputs = getFormAllInputs(form)

    for kv in allInputs.items():
        inputName = kv[0]
        inputValueList = kv[1]
        inputValue = 'ALON_WARNNING:MORE_THEN_ONE_OPTION'
        if (len(inputValueList) == 1):
            inputValue = inputValueList[0]
        else:
            inputValue = inputValueList[0]
            for i in xrange(1,len(inputValueList)):
                inputValue +=  ';' + inputValueList[i]
        if (len(inputValue)> 100):
            inputValue = inputValue[0:100]
        print '%s="%s"'%(inputName, inputValue)
