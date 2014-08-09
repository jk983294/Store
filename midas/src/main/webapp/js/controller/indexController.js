function indexController(){
	getIndexChart();
}

function getIndexChart() {
	var url_index = urlWithOutHash(window.location.href) + "rest/stocks/stock/IDX999999";
//	console.log(url_stockoverview);
	$.ajax({
				type : "get",
				url : url_index,
				success : drawIndexChart,
				error : function() {}
			});
}

function drawIndexChart(rawdata){	
	//change format
	var times = extractProperty(rawdata.records, 'date');
	var ends = extractProperty(rawdata.records, 'end');
//	console.log(rawdata);
//	console.log(times);
	var d = merge2Array(times,ends);
	
	var plot = $.plot("#detailIndex", [d], {
			xaxis: {
				mode: "time",
				tickLength: 5
			},
			selection: {
				mode: "x"
			}
//			,
//			grid: {
//				markings: weekendAreas
//			}
		});
	var overview = $.plot("#overviewIndex", [d], {
			series: {
				lines: {
					show: true,
					lineWidth: 1
				},
				shadowSize: 0
			},
			xaxis: {
				ticks: [],
				mode: "time"
			},
			yaxis: {
				ticks: [],
				min: 0,
				autoscaleMargin: 0.1
			},
			selection: {
				mode: "x"
			}
		});
	
	// now connect the two
	$("#detailIndex").bind("plotselected", function (event, ranges) {
		// do the zooming
		$.each(plot.getXAxes(), function(_, axis) {
			var opts = axis.options;
			opts.min = ranges.xaxis.from;
			opts.max = ranges.xaxis.to;
		});
		plot.setupGrid();
		plot.draw();
		plot.clearSelection();
		// don't fire event on the overview to prevent eternal loop
		overview.setSelection(ranges, true);
	});
	
	$("#overviewIndex").bind("plotselected", function (event, ranges) {
			plot.setSelection(ranges);
		});
	
	//draw chart

	//register dblclick event handler
//	$('#stockstable tbody').on('dblclick', 'tr', function () {
//        var name = $('td', this).eq(0).text();
//        alert( 'You clicked on '+name+'\'s row' );
//    } );
}

// helper for returning the weekends in a period
function weekendAreas(axes) {
	var markings = [], d = new Date(axes.xaxis.min);
	// go to the first Saturday
	d.setUTCDate(d.getUTCDate() - ((d.getUTCDay() + 1) % 7))
	d.setUTCSeconds(0);
	d.setUTCMinutes(0);
	d.setUTCHours(0);

	var i = d.getTime();
	// when we don't set yaxis, the rectangle automatically
	// extends to infinity upwards and downwards
	do {
		markings.push({ xaxis: { from: i, to: i + 2 * 24 * 60 * 60 * 1000 } });
		i += 7 * 24 * 60 * 60 * 1000;
	} while (i < axes.xaxis.max);
	return markings;
}
