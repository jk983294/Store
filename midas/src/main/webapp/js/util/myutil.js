// ��Date����չ���� Date ת��Ϊָ����ʽ��String
// ��(M)����(d)��Сʱ(h)����(m)����(s)������(q) ������ 1-2 ��ռλ���� 
// ��(y)������ 1-4 ��ռλ��������(S)ֻ���� 1 ��ռλ��(�� 1-3 λ������) 
// ���ӣ� 
// (new Date()).Format("yyyy-MM-dd hh:mm:ss.S") ==> 2006-07-02 08:09:04.423 
// (new Date()).Format("yyyy-M-d h:m:s.S")      ==> 2006-7-2 8:9:4.18 
Date.prototype.Format = function (fmt) { //author: meizz 
    var o = {
        "M+": this.getMonth() + 1, //�·� 
        "d+": this.getDate(), //�� 
        "h+": this.getHours(), //Сʱ 
        "m+": this.getMinutes(), //�� 
        "s+": this.getSeconds(), //�� 
        "q+": Math.floor((this.getMonth() + 3) / 3), //���� 
        "S": this.getMilliseconds() //���� 
    };
    if (/(y+)/.test(fmt)) fmt = fmt.replace(RegExp.$1, (this.getFullYear() + "").substr(4 - RegExp.$1.length));
    for (var k in o)
    if (new RegExp("(" + k + ")").test(fmt)) fmt = fmt.replace(RegExp.$1, (RegExp.$1.length == 1) ? (o[k]) : (("00" + o[k]).substr(("" + o[k]).length)));
    return fmt;
}
function urlWithOutHash(url){
	return url.substr(0,url.indexOf('#'));
}
function urlafterHash(url){
	return url.substring(url.indexOf('#')+1,url.length);
}

function extractProperty(data, property){
	var properties = new Array();
	for(var i = 0, len = data.length; i < len; i++ ) properties.push(data[i][property]);
	return properties;
}
function merge2Array(p1, p2){
	var properties = new Array();
	for(var i = 0, len = p1.length; i < len; i++ ) properties.push( [ p1[i] , p2[i] ] );
	return properties;
}