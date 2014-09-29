/**
 * Namespaces defined using object literal notation may be easily extended (or merged)
 * with other objects (or namespaces) such that the properties and functions of both
 * namespaces can be accessible under the same namespace post-merge.
 *
 * Underscore.js extend()
 * jQuery $.extend()
 */
// extend.js
// Written by Andrew Dupont, optimized by Addy Osmani

function extend( destination, source ) {

    var toString = Object.prototype.toString,
        objTest = toString.call({});

    for ( var property in source ) {
        if ( source[property] && objTest === toString.call(source[property]) ) {
            destination[property] = destination[property] || {};
            extend(destination[property], source[property]);
        } else {
            destination[property] = source[property];
        }
    }
    return destination;

};

console.group( "objExtend namespacing tests" );

// define a top-level namespace for usage
var myNS = myNS || {};

// 1. extend namespace with a "utils" object
extend(myNS, {
    utils:{
    }
});

console.log( "test 1" , myNS);
// myNS.utils now exists

// 2. extend with multiple depths (namespace.hello.world.wave)
extend(myNS, {
    hello:{
        world:{
            wave:{
                test: function(){
                    //...
                }
            }
        }
    }
});

// test direct assignment works as expected
myNS.hello.test1 = "this is a test";
myNS.hello.world.test2 = "this is another test";
console.log( "test 2", myNS );

// 3. what if myNS already contains the namespace being added
// (e.g. "library")? we want to ensure no namespaces are being
// overwritten during extension

myNS.library = {
    foo:function () {}
};

extend( myNS, {
    library:{
        bar:function(){
            //...
        }
    }
});

// confirmed that extend is operating safely (as expected)
// myNS now also contains library.foo, library.bar
console.log( "test 3", myNS );


// 4. what if we wanted easier access to a specific namespace without having
// to type the whole namespace out each time?

var shorterAccess1 = myNS.hello.world;
shorterAccess1.test3 = "hello again";
console.log( "test 4", myNS);

//success, myApp.hello.world.test3 is now "hello again"

console.groupEnd();