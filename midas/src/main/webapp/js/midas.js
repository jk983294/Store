$(document).ready(function() {
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
function onHashChange(){
	console.log(window.location.hash);
//	var number=obj[window.location.hash.substring(1)] || '';//substring(1)用来减去地址栏的地址中的#号
//	alert("change "+number);
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
					$('#stockstable tbody').on('click', 'tr', function () {
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
