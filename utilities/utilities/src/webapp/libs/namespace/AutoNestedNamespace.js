/**
 * method that takes a single string argument for a nest,
 * parses this and automatically populates our base namespace with the objects required.
 */
// top-level namespace being assigned an object literal
var myApp = myApp || {};

// a convenience function for parsing string namespaces and
// automatically generating nested namespaces
function extend( ns, ns_string ) {
    var parts = ns_string.split("."),
        parent = ns,
        pl;

    pl = parts.length;

    for ( var i = 0; i < pl; i++ ) {
        // create a property if it doesn't exist
        if ( typeof parent[parts[i]] === "undefined" ) {
            parent[parts[i]] = {};
        }

        parent = parent[parts[i]];
    }

    return parent;
}

// Usage:
// extend myApp with a deeply nested namespace
var mod = extend(myApp, "modules.module2");

// the correct object with nested depths is output
console.log(mod);

// minor test to check the instance of mod can also
// be used outside of the myApp namesapce as a clone
// that includes the extensions

// Outputs: true
console.log(mod == myApp.modules.module2);

// further demonstration of easier nested namespace
// assignment using extend
extend(myApp, "moduleA.moduleB.moduleC.moduleD");
extend(myApp, "longer.version.looks.like.this");
console.log(myApp);


/**
 * Dependency Declaration pattern
 */
// common approach to accessing nested namespaces
myApp.utilities.math.fibonacci( 25 );
myApp.utilities.math.sin( 56 );
myApp.utilities.drawing.plot( 98,50,60 );

// with local/cached references
var utils = myApp.utilities,
    maths = utils.math,
    drawing = utils.drawing;

// easier to access the namespace
maths.fibonacci( 25 );
maths.sin( 56 );
drawing.plot( 98, 50,60 );

// note that the above is particularly performant when
// compared to hundreds or thousands of calls to nested
// namespaces vs. a local reference to the namespace
