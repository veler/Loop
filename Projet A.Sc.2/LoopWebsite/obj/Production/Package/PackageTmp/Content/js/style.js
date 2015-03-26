//A chaque appui sur 'Reset' ou chargement de la page, un b&ckground sera choisie au hasard.
var link = [];
link[0] = "../../Content/css/style1.css";
link[1] = "../../Content/css/style2.css";
link[2] = "../../Content/css/style3.css";
link[3] = "../../Content/css/style4.css";
link[4] = "../../Content/css/style5.css";

$(function (){
	var style = link [Math.floor(Math.random() * 5)];
	$('<link />',{
		rel : 'stylesheet',
		type: 'text/css',
		id: 'myStyles',
		href: style
	}).appendTo('head');
});


/*function ShowResult(){
	location.href="../index.html";
};*/