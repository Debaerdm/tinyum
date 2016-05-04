$(document).ready(function(){
    $("#flip").click(function(){
	$("#panel").slideToggle("slow");
    });
    
    $("#object").mouseenter(function(){
	$("#object li").css("font-size",  "24px");
    });
    
    $("#object").mouseenter(function(){
	$("#object li").css("font-size",  "16px");
    });
    
});

