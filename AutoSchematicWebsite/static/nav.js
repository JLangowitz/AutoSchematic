$(document).ready(function(){
    var height=$('nav').height();
    $('.nav-logo').height(height);
    console.log(window.location.pathname);
    selector=window.location.pathname.replace('/','');
    console.log(selector.indexOf('/'));
    if  (selector.indexOf('/')<0){
        selected=$('#'+window.location.pathname.replace('/',''));
        if(selected){
            selected.addClass('active');        
        }
    };
});