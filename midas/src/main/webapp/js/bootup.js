(function() {
	var jsLibs = [
        	"jquery-1.11.1","bootstrap", "typeahead0.10.4","jquery.dataTables","mediator",
        	"jquery.flot","jquery.flot.selection","jquery.flot.time"
        ],
        cssFiles = [
        	"bootstrap.min","bootstrap-theme.min","jquery.dataTables.min","mytheme"
        ];
        
    var scripts = document.getElementsByTagName('script'),
        localhostTests = [
            /^localhost$/,
            /\b(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)(:\d{1,5})?\b/ // IP v4
        ],
        host = window.location.hostname,
        isDevelopment = null,
        queryString = window.location.search,
        test, path, i, ln, scriptSrc, match;
	
    for (i = 0, ln = scripts.length; i < ln; i++) {
        scriptSrc = scripts[i].src;
        match = scriptSrc.match(/bootup\.js$/);
        if (match) {
            path = scriptSrc.substring(0, scriptSrc.length - match[0].length);
            break;
        }
    }

    if (isDevelopment === null) {
        for (i = 0, ln = localhostTests.length; i < ln; i++) {
            test = localhostTests[i];
            if (host.search(test) !== -1) {
                isDevelopment = true;
                break;
            }
        }
    }
    if (isDevelopment === null && window.location.protocol === 'file:') {
        isDevelopment = true;
    }
    
//    isDevelopment = false;
	// load js libs
	for (i = 0, ln = jsLibs.length; i < ln; i++) {
    		document.write('<script type="text/javascript" src="' + path + 'lib/' + ((isDevelopment) ? 'dev/' : 'prod/')+ jsLibs[i]+((isDevelopment) ? '' : '.min') + '.js"></script>');
	}
	// load css libs
	var head = document.getElementsByTagName('head').item(0);
	for (i = 0, ln = cssFiles.length; i < ln; i++) {
		css = document.createElement('link');
	    css.href = "css/"+cssFiles[i]+".css";
	    css.rel = 'stylesheet';
	    css.type = 'text/css';
	    head.appendChild(css);
	}

	//load user defined 
	document.write('<script type="text/javascript" src="js/util/myutil.js"></script>');
	document.write('<script type="text/javascript" src="js/controller/layoutController.js"></script>');
	document.write('<script type="text/javascript" src="js/controller/indexController.js"></script>');
	document.write('<script type="text/javascript" src="js/controller/overviewController.js"></script>');
	document.write('<script type="text/javascript" src="js/controller/analyticsController.js"></script>');
	document.write('<script type="text/javascript" src="js/controller/comparisonController.js"></script>');
	document.write('<script type="text/javascript" src="js/controller/adminController.js"></script>');
    document.write('<script type="text/javascript" src="js/global/stateMgr.js"></script>');
    document.write('<script type="text/javascript" src="js/global/msgBus.js"></script>');
    document.write('<script type="text/javascript" src="js/global/controller.js"></script>');

})();