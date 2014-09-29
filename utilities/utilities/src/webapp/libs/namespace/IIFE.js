/**
 * immediately-invoked function expression
 * an unnamed function, immediately invoked after it's been defined
 */
// an (anonymous) immediately-invoked function expression
(function () { /*...*/ })();

// a named immediately-invoked function expression
(function foobar () { /*..*/ })();


var namespace = namespace || {};
// here a namespace object is passed as a function
// parameter, where we assign public methods and
// properties to it
(function( o ){
    o.foo = "foo";
    o.bar = function(){
        return "bar";
    };
})( namespace );

console.log( namespace );


// let's extend the namespace with new functionality
(function( namespace, undefined ){

    // public method
    namespace.sayGoodbye = function () {
        namespace.say( "goodbye" );
    }
})( window.namespace = window.namespace || {});

// Outputs: goodbye
namespace.sayGoodbye();

/**
 * anonymous self-executing function
 */
var foobar = function () {
    console.log("called");
    arguments.callee();     // recursive call
}