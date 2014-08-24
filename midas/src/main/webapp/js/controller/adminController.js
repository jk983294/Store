/**
 * for Admin menu, provide serveral button's function implementation
 */
function adminController(){
	$("#LoadDataAdmin").click(loadDataAdmin);
	$("#TestAdmin").click(testAdmin);
	$("#DeleteDataAdmin").click(deleteDataAdmin);
}
/**
 * trigger server side to load all stocks file 
 */
function loadDataAdmin() {
	var url_loaddataadmin = urlWithOutHash(window.location.href) + "rest/admin/stocks";
//	console.log(url_loaddataadmin);
	$.ajax({
				type : "put",
				url : url_loaddataadmin,
				success : function(){},
				error : function() {}
			});
}
/**
 * trigger server side to drop DB collection 
 */
function deleteDataAdmin() {
	var url_deletedataadmin = urlWithOutHash(window.location.href) + "rest/admin/stocks";
//	console.log(url_deletedataadmin);
	$.ajax({
				type : "delete",
				url : url_deletedataadmin,
				success : function(){},
				error : function() {}
			});
}

/**
 * trigger server side to test function
 */
function testAdmin() {
	var url_testadmin = urlWithOutHash(window.location.href) + "rest/stocks/test";
//	console.log(url_testadmin);
	$.ajax({
				type : "get",
				url : url_testadmin,
				success : function(){},
				error : function() {}
			});
}