/**
 * promote code re-use
 * offered the ability to add behaviour to existing classes in a system dynamically
 */


/**
 * method 1
 */
function Vehicle( vehicleType ){
    this.vehicleType = vehicleType || "car";
    this.model = "default";
    this.license = "00000-000";
}
// Lets create a new instance of vehicle, to be decorated
var truck = new Vehicle( "truck" );

// New functionality we're decorating vehicle with
truck.setModel = function( modelName ){
    this.model = modelName;
};
truck.setColor = function( color ){
    this.color = color;
};

// Test the value setters and value assignment works correctly
truck.setModel( "CAT" );
truck.setColor( "blue" );
console.log( truck );

// Demonstrate "vehicle" is still unaltered, because it doesn't use prototype to add new function
var secondInstance = new Vehicle( "car" );
console.log( secondInstance );

/**
 * method 2
 */
function MacBook() {
    this.cost = function () { return 997; };
    this.screenSize = function () { return 11.6; };
}
// Decorators
function memory( macbook ) {
    var v = macbook.cost();
    macbook.cost = function() { return v + 75; };
}
function engraving( macbook ){
    var v = macbook.cost();
    macbook.cost = function(){ return  v + 200; };
}
function insurance( macbook ){
    var v = macbook.cost();
    macbook.cost = function(){ return  v + 250; };
}

var mb = new MacBook();
// New functionality we're decorating vehicle with
memory( mb );
engraving( mb );
insurance( mb );

console.log( mb.cost() );
console.log( mb.screenSize() );

/**
 * method 3 interface
 */


