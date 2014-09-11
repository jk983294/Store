function overviewController(){
	getStockOverView();
}

function getStockOverView() {
	var url_stockoverview = urlWithOutHash(window.location.href) + "rest/stocks";
//	console.log(url_stockoverview);
	$.ajax({
				type : "get",
				url : url_stockoverview,
				success : drawStockOverviewTable,
				error : function() {}
			});
}

function drawStockOverviewTable(data){	
	//change format
	$.each(data, function(i,val){   
		val.latest.date = (new Date(val.latest.date)).Format("yyyy-MM-dd");
		val.latest.change = (val.latest.change*100).toFixed(2);
	});
	//draw table
	$('#stockstable').dataTable({
				"data" : data,
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
	//register dblclick event handler
	$('#stockstable tbody').on('dblclick', 'tr', function () {
        var name = $('td', this).eq(0).text();
        alert( 'You clicked on '+name+'\'s row' );
    } );
}