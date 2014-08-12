function MidasController(){
	this.onRead = function(){
		layoutController();
		indexController();
		overviewController();
		adminController();
	};	
}
