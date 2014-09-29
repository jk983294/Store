/**
 * an object containing a collection of key:value pairs with a colon separating each pair of keys
 * and values where keys can also represent new namespaces.
 */
var myApplication = {

    // As we've seen, we can easily define functionality for
    // this object literal..
    getInfo:function(){
        //...
    },

    // but we can also populate it to support
    // further object namespaces containing anything
    // anything we wish:
    models : {},
    views : {
        pages : {}
    },
    collections : {}
};

/**
 * extend namespace
 */
myApplication.foo = function(){
    return "bar";
}

myApplication.utils = {
    toString:function(){
        //...
    },
    export: function(){
        //...
    }
}



/**
 * check if exist
 */
// This doesn't check for existence of "myApplication" in
// the global namespace. Bad practice as we can easily
// clobber an existing variable/namespace with the same name
var myApplication = {};

// The following options *do* check for variable/namespace existence.
// If already defined, we use that instance, otherwise we assign a new
// object literal to myApplication.
//
// Option 1: var myApplication = myApplication || {};
// Option 2  if( !MyApplication ){ MyApplication = {} };
// Option 3: window.myApplication || ( window.myApplication = {} );
// Option 4: var myApplication = $.fn.myApplication = function() {};
// Option 5: var myApplication = myApplication === undefined ? {} : myApplication;
