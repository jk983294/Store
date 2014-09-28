/**
 * Asynchronous Module Definition
 * both the module and dependencies can be asynchronously loaded.
 */
// A module_id (myModule) is used here for demonstration purposes only
define( "myModule",

    ["foo", "bar"],



    // module definition function
    // dependencies (foo and bar) are mapped to function parameters
    function ( foo, bar ) {
        // return a value that defines the module export
        // (i.e the functionality we want to expose for consumption)

        // create your module here
        var myModule = {
            doStuff:function () {
                console.log( "Yay! Stuff" );
            }
        };

        return myModule;
    });

// An alternative version could be..
define( "myModule",

    ["math", "graph"],

    function ( math, graph ) {

        // Note that this is a slightly different pattern
        // With AMD, it's possible to define modules in a few
        // different ways due to it's flexibility with
        // certain aspects of the syntax
        return {
            plot: function( x, y ){
                return graph.drawPie( math.randomGrid( x, y ) );
            }
        };
    }
);

define(function ( require ) {
    var isReady = false, foobar;

    // note the inline require within our module definition
    require(["foo", "bar"], function ( foo, bar ) {
        isReady = true;
        foobar = foo() + bar();
    });

    // we can still return a module
    return {
        isReady: isReady,
        foobar: foobar
    };
});