// A null signifies separate line segments
var d3 = [[0, 12], [7, 12], null, [7, 2.5], [12, 2.5]];
/**
* different series types
*/
$.plot("#placeholder", [{
	data: d1,
	lines: { show: true, fill: true }	//show line, and fill color under line to axis
}, {
	data: d2,
	bars: { show: true }					// bar chart
}, {
	data: d3,
	points: { show: true }					//only show points, not line
} {
	data: d6,
	lines: { show: true, steps: true }	//step chart for series
}]);

/**
* categories and textual data in x axis
*/
var data = [ ["January", 10], ["February", 8], ["March", 4], ["April", 13], ["May", 17], ["June", 9] ];
$.plot("#placeholder", [ data ], {
	series: {
		bars: {
			show: true,
			barWidth: 0.6,
			align: "center"
		}
	},
	xaxis: {
		mode: "categories",		
		tickLength: 0
	}
});

/**
* options for plot
*/
$.plot("#placeholder", [
	{ label: "sin(x)", data: d1 },		//legend
	{ label: "cos(x)", data: d2 },
	{ label: "tan(x)", data: d3 }
], {
	series: {
		lines: { show: true },
		points: { show: true }
	},
	xaxis: {
		ticks: [
			0, [ Math.PI/2, "\u03c0/2" ], [ Math.PI, "\u03c0" ],		// [value, name] for ticks, \u03c0 means pi
			[ Math.PI * 3/2, "3\u03c0/2" ], [ Math.PI * 2, "2\u03c0" ]
		]
	},
	yaxis: {
		ticks: 10,
		min: -2,
		max: 2,
		tickDecimals: 3
	},
	grid: {
		markings: [
			{ color: "#f6f6f6", yaxis: { from: 1 } },
			{ color: "#f6f6f6", yaxis: { to: -1 } },
			{ color: "#000", lineWidth: 1, xaxis: { from: 2, to: 2 } },
			{ color: "#000", lineWidth: 1, xaxis: { from: 8, to: 8 } }
		],
		backgroundColor: { colors: [ "#fff", "#eee" ] },
		borderWidth: {
			top: 1,
			right: 1,
			bottom: 2,
			left: 2
		}
	}
});
// Append annotation to the placeholder that Flot already uses for positioning
var o = plot.pointOffset({ x: 2, y: -1.2});
placeholder.append("<div style='position:absolute;left:" + (o.left + 4) + "px;top:" + o.top + "px;color:#666;font-size:smaller'>Warming up</div>");