/**
 * example for jquery based AMD
 */

/*
define(["js/jquery.js","js/jquery.color.js","js/underscore.js"],

    function( $, colorPlugin, _ ){
        // Here we've passed in jQuery, the color plugin and Underscore
        // None of these will be accessible in the global scope, but we
        // can easily reference them below.

        // Pseudo-randomize an array of colors, selecting the first
        // item in the shuffled array
        var shuffleColor = _.first( _.shuffle( "#666","#333","#111"] ) );

        // Animate the background-color of any elements with the class
        // "item" on the page using the shuffled color
        $( ".item" ).animate( {"backgroundColor": shuffleColor } );

        // What we return can be used by other modules
        return {};
    }
);
*/