<script type="text/javascript" src="my_jquery_functions.js"></script>

//selector
$("p") 														//选取 <p> 元素
$("p.intro") 											//选取所有 class="intro" 的 <p> 元素
$("p#demo") 											//选取所有 id="demo" 的 <p> 元素
$("[href]") 												//选取所有带有 href 属性的元素
$("[href='#']") 										//选取所有带有 href 值等于 "#" 的元素
$("[href!='#']") 										//选取所有带有 href 值不等于 "#" 的元素
$("[href$='.jpg']") 									//选取所有 href 值以 ".jpg" 结尾的元素
$("p").css("background-color","red");	//change all p elements css background color red
$("ul li:first")											//每个 <ul> 的第一个 <li> 元素
$("div#intro .head")								//id="intro" 的 <div> 元素中的所有 class="head" 的元素

//get content
$("#test").text();					$("#test1").text(function(i,origText){ return  " New text";});  			//get set
$("#test").html();
$("#test").val();										//form value
$("#w3s").attr("href");		$("#w3s").attr("href","http://www.w3school.com.cn/jquery");
$("p").append("Some appended text.");				//prepend() before() after()
$("#div1").remove();								//remove all including itself
$("#div1").empty(); 								//remove all children
//css
$("h1,h2,p").addClass("blue");
$("h1,h2,p").removeClass("blue");
 $("h1,h2,p").toggleClass("blue");
 $("p").css("background-color");		//query css attribute
 $("p").css("background-color","yellow");		 $("p").css({"background-color":"yellow","font-size":"200%"});
 
//dom traverse
$("span").parent(); 							//direct parent	
$("span").parents();  							//until html element
$("span").parents("ul");						//parents which is ul element
$("span").parentsUntil("div");			//parents between span and div
$("div").children();								//only one level down
$("div").find("*");								//all children		
$("h2").siblings("p");							//all p element siblings	 
$("h2").next();									//only one element													prev()
$("h2").nextAll();								//all next siblings in same level								prevAll()
$("h2").nextUntil("h6");						//all siblings between this and h6							prevUntil()
//element filter
$("div p").first();
$("div p").last();
$("p").eq(1);										//index start from 0
$("p").filter(".intro");							//with class name intro
$("p").not(".intro");							//the opposite of filter

//AJAX
$("#div1").load("demo_test.txt #p1");			//load txt class p1 content to div1
$("#div1").load("demo_test.txt",function(responseTxt,statusTxt,xhr){
    if(statusTxt=="success")
      alert("外部内容加载成功！");
    if(statusTxt=="error")
      alert("Error: "+xhr.status+": "+xhr.statusText);
  });
$.ajax({
  url: "/api/getWeather",
  data: {
    zipcode: 97201
  },
  success: function( data ) {
    $( "#weather-temp" ).html( "<strong>" + data + "</strong> degrees" );
  }
});
$.get("demo_test.asp",function(data,status){
    alert("Data: " + data + "\nStatus: " + status);
  });
$.post("demo_test_post.asp",
  {
    name:"Donald Duck",		//parameters to server
    city:"Duckburg"
  },
  function(data,status){
    alert("Data: " + data + "\nStatus: " + status);
  });

//event 
//bind and unbind
$("button").bind("click",function(){  $("p").slideToggle(); });									//bind click event handler
$("button").click(function(){  $("p").unbind();});														//unbind event handler
$("p").one("click",function(){  $(this).animate({fontSize:"+=6px"});});					//fire only once
$("div").delegate("button","click",function(){ $("p").slideToggle(); });						//bind handler to child element of selected element, affect future elements
$("body").undelegate();																								//remove all handler added by delegate
$("p").live("click",function(){ $(this).slideToggle(); });												//affect future elements
$("button").click(function(){ $("p").die();  });																//remove all handler added through live

//element event
$("button").click(function() {  $("p").hide(); } )
$("input").blur(function(){  $("input").css("background-color","#D6D6FF"); });		//input box blur event   $(selector).blur();	 //fire blur event 	
$("input").focus(function(){  $("input").css("background-color","#FFFFCC");});																									
$(".field").change(function(){ $(this).css("background-color","#FFFFCC"); });			//input field text change   $(selector).change();													
$("img").error(function(){  $("img").replaceWith("Missing image!"); });					//if error happens, then fire
$("img").load(function(){  $("div").text("Image loaded");});										//image load event
$(window).unload(function(){  alert("Goodbye!");});
$(document).ready(function(){});																				//document loaded and rendered
$(window).resize(function() {  $('span').text(x+=1);});												
$("div").scroll(function() {  $("span").text(x+=1);});													//text area or window scroll event
$("input").select(function(){  $("input").after(" Text marked!");});							//select text in input field
$("form").submit(function(e){  alert("Submitted");});
$(selector).toggle(function1(),function2(),functionN(),...);										//toggle functions like hide() and show()
$("button").click(function(){  $("input").trigger("select");});										//trigger some events happen
$("button").click(function(){  $("input").triggerHandler("select");});						//compare with trigger, no default action taken like submit, only affect first matched element

//key event
$("input").keydown(function(){  $("input").css("background-color","#FFFFCC");});
$("input").keypress(function(){  $("span").text(i+=1);});											//on text then no matter focus or blur
$("input").keyup(function(){  $("input").css("background-color","#D6D6FF");});

//mouse event
$("button").mousedown(function(){  $("p").slideToggle();});
$("button").mouseup(function(){  $("p").slideToggle();});
$("p").mouseenter(function(){  $("p").css("background-color","yellow");});
$("p").mouseleave(function(){  $("p").css("background-color","#E9E9E4");});
$("p").mouseover(function(){  $("p").css("background-color","yellow");});				//unlike mouseenter, it will count any child element mouseover event
$("p").mouseout(function(){  $("p").css("background-color","#E9E9E4");});			//unlike mouseleave, it will count any child element mouseout event
$(document).mousemove(function(e){  $("span").text(e.pageX + ", " + e.pageY);});

//event attributes
$("a").click(function(event){   event.preventDefault(); });											//prevent href being open
$(document).mousemove(function(event){ $("span").text("X: " + event.pageX + ", Y: " + event.pageY);});											//show mouse position
$("button").click(function(event) {  $("p").html(event.result); });											//e.result return last function return value
$("p, button").click(function(event){$("div").html("Triggered by a " + event.target.nodeName + " element.");}); 			//which dom element trigger event
$("button").click(function(event){  $("span")html(event.timeStamp);});					//event happen timestamp
$("p").bind('click dblclick mouseover mouseout',function(event){  $("div").html("Event: " + event.type); });		//event.type like click dblclick mouseover mouseout
$("input").keydown(function(event){  $("div").html("Key: " + event.which); });		//event.which indicate which key has been pressed			