$(document).ready(function(){
    console.log('cut.jade');
    filename=$('#file').text();
    console.log(filename);
    // console.log('err',$('#err').attr('err'));
    if ($('#err').attr('err')=='') {
        $.post('/cut/'+filename,{},function(data){
            $('body').html(data);
        });
    };
});