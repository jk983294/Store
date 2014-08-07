controllerBase = Object.create(null);
 
Object.defineProperties
(
    controllerBase,
    {
        'name'  : {  value: 'controllerBase'},
    }
);
controllerBase.toString = function(){
	return this.name;
}