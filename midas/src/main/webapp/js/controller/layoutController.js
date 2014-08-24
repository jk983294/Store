// controller should register to global/controller.js
function layoutController(){
	init_layout();
	window.onhashchange=onHashChange;
}

function init_layout(){
	for(var i =0; i <stateMgr.menu.length; i++ )
		msgbus.publish("hideCmp", stateMgr.menu[i] );

	if(window.location.hash && window.location.hash.length > 1) 
		onHashChange();
}

function onHashChange(){
	msgbus.publish("hideCmp", stateMgr.targetlocation );
	stateMgr.targetlocation = urlafterHash(window.location.hash);
	msgbus.publish("showCmp", stateMgr.targetlocation );
}

function hideCmp(oldcmp){
	$("#"+oldcmp).removeClass("active");
	$("#"+oldcmp+"_Content").hide();
}

function showCmp(newcmp){
	$("#"+newcmp).removeClass("active");
	$("#"+newcmp+"_Content").show();
}