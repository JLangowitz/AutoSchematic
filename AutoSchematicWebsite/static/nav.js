$(document).ready(function(){
    var height=$('nav').height();
    $('.nav-logo').height(height);
    selected=$('#'+window.location.pathname.replace('/',''));
    if(selected){
        selected.addClass('active');        
    }
});