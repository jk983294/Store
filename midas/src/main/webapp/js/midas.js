$(document).ready(function() {
	init_states();
	var stocks = new Object();
	getStockOverView(stocks);			
	
	window.onhashchange=onHashChange;
	$('.typeahead').typeahead({
				hint : true,
				highlight : true
			}, {
				name : 'states',
				displayKey : 'value',
				source : substringMatcher(states)
			});
});

stateMgr = {
	menu : ["Index","Overview","Analytics","Comparison","Admin"],
	targetlocation : "Index"
};

function init_states(){
	for(var i =0; i <stateMgr.menu.length; i++ ){
		$("#"+stateMgr.menu[i]).removeClass("active");
		$("#"+stateMgr.menu[i]+"_Content").hide();
	}
	if(window.location.hash && window.location.hash.length > 1) {
		onHashChange();
	}	
}
function onHashChange(){
//	console.log("old "+stateMgr.targetlocation);
	$("#"+stateMgr.targetlocation).removeClass("active");
	$("#"+stateMgr.targetlocation+"_Content").hide();
	stateMgr.targetlocation = urlafterHash(window.location.hash);
//	console.log("new "+stateMgr.targetlocation);
	$("#"+stateMgr.targetlocation).addClass("active");
	$("#"+stateMgr.targetlocation+"_Content").show();
}
function getStockOverView(stocks) {
	var url_stockoverview = urlWithOutHash(window.location.href) + "rest/stocks";
	console.log(url_stockoverview);
	$.ajax({
				type : "get",
				url : url_stockoverview,
				success : function(data, textStatus) {
					stocks = data;					
					$.each(stocks, function(i,val){   
						val.latest.date = (new Date(val.latest.date)).Format("yyyy-MM-dd");
						val.latest.change = (val.latest.change*100).toFixed(2);
  					});
					$('#stockstable').dataTable({
								"data" : stocks,
								"columns" : [
									{ "title" : "Name", data: "name"}, 
									{ "title" : "Desp" , data: "desp" ,"orderable": false }, 
									{ "title" : "Date", data: "latest.date"  }, 
									{ "title" : "Start" , data: "latest.start","orderable": false }, 
									{ "title" : "Max", data: "latest.max" ,"orderable": false},
									{ "title" : "Min", data: "latest.min" ,"orderable": false},
									{ "title" : "End", data: "latest.end" ,"orderable": false},
									{ "title" : "Total", data: "latest.total" },
									{ "title" : "%Change", data: "latest.change" }],
								"createdRow": function ( row, data, index ) {
						            if ( data.latest.change > 0 )  $(row).addClass( 'up' );
						            else if( data.latest.change  < 0 )  $(row).addClass( 'down' );
						        }
							});
					$('#stockstable tbody').on('dblclick', 'tr', function () {
				        var name = $('td', this).eq(0).text();
				        alert( 'You clicked on '+name+'\'s row' );
				    } );
				},
				error : function() {
				}
			});
}

var substringMatcher = function(strs) {
	return function findMatches(q, cb) {
		var matches, substrRegex;
		// an array that will be populated with substring matches
		matches = [];
		// regex used to determine if a string contains the substring `q`
		substrRegex = new RegExp(q, 'i');
		// iterate through the pool of strings and for any string that
		// contains the substring `q`, add it to the `matches` array
		$.each(strs, function(i, str) {
					if (substrRegex.test(str)) {
						// the typeahead jQuery plugin expects suggestions to a
						// JavaScript object, refer to typeahead docs for more
						// info
						matches.push({
									value : str
								});
					}
				});
		cb(matches);
	};
};

var states = ['Alabama', 'Alaska', 'Arizona', 'Arkansas', 'California',
		'Colorado', 'Connecticut', 'Delaware', 'Florida', 'Georgia', 'Hawaii',
		'Idaho', 'Illinois', 'Indiana', 'Iowa', 'Kansas', 'Kentucky',
		'Louisiana', 'Maine', 'Maryland', 'Massachusetts', 'Michigan',
		'Minnesota', 'Mississippi', 'Missouri', 'Montana', 'Nebraska',
		'Nevada', 'New Hampshire', 'New Jersey', 'New Mexico', 'New York',
		'North Carolina', 'North Dakota', 'Ohio', 'Oklahoma', 'Oregon',
		'Pennsylvania', 'Rhode Island', 'South Carolina', 'South Dakota',
		'Tennessee', 'Texas', 'Utah', 'Vermont', 'Virginia', 'Washington',
		'West Virginia', 'Wisconsin', 'Wyoming'];
