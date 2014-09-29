/**
 * Lazy Initialization Pattern
 */
$( document ).ready( function () {

    // The ajax request won't attempt to execute until
    // the DOM is ready

    var jqxhr = $.ajax({
        url: "http://domain.com/api/",
        data: "display=latest&order=ascending"
    }).done( function( data ) {
        $(".status").html( "content loaded" );
        console.log( "Data output:" + data );
    });

});

/**
 * Iterator Pattern
 */
$.each( ["john","dave","rick","julian"] , function( index, value ) {
    console.log( index + ": " + value);
});

$( "li" ).each( function ( index ) {
    console.log( index + ": " + $( this ).text());
});


/**
 * Composite Pattern
 */
// Single elements
$( "#singleItem" ).addClass( "active" );
$( "#container" ).addClass( "active" );

// Collections of elements
$( "div" ).addClass( "active" );
$( ".item" ).addClass( "active" );
$( "input" ).addClass( "active" );


/**
 * Adapter Pattern
 */
// Setting opacity
$( ".container" ).css( { opacity: .5 } );

// Getting opacity
var currentOpacity = $( ".container" ).css('opacity');



/**
 * Facade Pattern
 */
var url = 'localhost:8080';
var data = 'stock';
var dataType = 'string';
function callback(){
    console.log("called");
}
$.get( url, data, callback, dataType );
$.post( url, data, callback, dataType );
$.getJSON( url, data, callback );
$.getScript( url, callback );