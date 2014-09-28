/**
 * example for commonJS spec
 */
// package/lib is a dependency we require
var lib = require( "package/lib" );

// behaviour for our module
function foo(){
    lib.log( "hello world!" );
}

// export (expose) foo to other modules
exports.foo = foo;


/**
 * Basic consumption of exports
 */

// define more behaviour we would like to expose
function foobar(){
    this.foo = function(){
        console.log( "Hello foo" );
    }

    this.bar = function(){
        console.log( "Hello bar" );
    }
}

// expose foobar to other modules
exports.foobar = foobar;

// an application consuming "foobar"

// access the module relative to the path
// where both usage and module files exist
// in the same directory

var foobar = require("./foobar").foobar,
    test   = new foobar();

// Outputs: "Hello bar"
test.bar();
