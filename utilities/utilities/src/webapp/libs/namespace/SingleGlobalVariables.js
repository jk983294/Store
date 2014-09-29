/**
 * the biggest challenge with the single global variable pattern is ensuring that
 * no one else has used the same global variable name as we have in the page.
 */
var myApplication =  (function () {
    function a(){
        //...
    };
    return{
        //...
    }
})();