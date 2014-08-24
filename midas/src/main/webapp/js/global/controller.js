/**
 * controller should register to global/controller.js
 */
function MidasController(){
	this.onReady = function(){
		layoutController();
		indexController();
		overviewController();
		adminController();
	};	
}
