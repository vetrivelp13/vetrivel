(function($) {
    jQuery.fn.setPageLayout = function(arrFuncs, objWidget){

        return this.each(function(){
            $(this).html($.fn.renderPage(arrFuncs,objWidget));
            $.fn.setContent(arrFuncs,objWidget);
        });

    };


    $.fn.renderPage = function(arrFuncs,objWidget){
        if(document.getElementById(objWidget.getUniqueWidgetId()+"msgDiv"))
            $('#'+objWidget.getUniqueWidgetId()+'msgDiv').remove();
        var divs = {titleDiv:"0", mainDiv:"0", parentDiv:"0", tabDiv:"0",linkDiv:"0",bottDiv:"0"};
        for(var i=0;i<arrFuncs.length;i++){
            divs[arrFuncs[i].divId]="1";
        }
        var html = "";

        html += "<div id='"+objWidget.getUniqueWidgetId()+"parentPageLayout'>";

        if(divs.titleDiv=="1")
            html += '<div id="'+objWidget.getUniqueWidgetId()+'titleDiv" class="titleDiv" ></div>';

        //html += '<div class="sp_msgwrap" ><div id="msgDiv" class="sp_msgbox" style="display:none;"></div></div>';
        html += '<div id="'+objWidget.getUniqueWidgetId()+'msgDiv" class="sp_msgbox" style="display:none;border:1px solid #CCCCCC;"></div><div class="sp_msgwrap" style="display:none"></div>';

        html += "<div class='FindTraining' style='border-left: 1px solid #CCCCCC;border-right: 1px solid #CCCCCC;background:#ffffff;float:left;padding-top:10px;width:99.8%;'>";


        if(divs.linkDiv=="0")
            html += '<div id="'+objWidget.getUniqueWidgetId()+'leftDiv" style="width:100%" class="leftDiv">';
        else
            html += '<div id="'+objWidget.getUniqueWidgetId()+'leftDiv" class="leftDiv">';

        if(divs.parentDiv=="1")
            html += '<div id="'+objWidget.getUniqueWidgetId()+'parentDiv" class="parentDiv" ></div>';

        if(divs.mainDiv=="1")
            html += '<div id="'+objWidget.getUniqueWidgetId()+'mainDiv"  class="mainDiv"></div>';

        html += '</div>';


        if(divs.linkDiv=="1")
            html += '<div id="'+objWidget.getUniqueWidgetId()+'linkDiv" class="linkDiv" ></div>';

        //html += '</div><br>';
        if(divs.buttonDiv=="1")
            html += '<div id="'+objWidget.getUniqueWidgetId()+'buttonDiv" class="buttonDiv"></div>';

        html += '</div>';

        if(divs.tabDiv=="1")
            html += '<div id="'+objWidget.getUniqueWidgetId()+'tabDiv" class="tabDiv"></div>';

        if(divs.bottDiv=="1")
            html += '<div id="'+objWidget.getUniqueWidgetId()+'bottDiv" class="bottDiv" ></div>';

        html += '<div style="clear:both;height:0px;"></div>';
        html += '<div class="block-footer-left"><div class="block-footer-right"><div class="block-footer-middle">&nbsp;</div></div></div></div>';
        return html;
    };

    $.fn.setContent = function(arrFuncs, objWidget){
        var divs = {titleDiv:"0", mainDiv:"0", buttonDiv:"0",parentDiv:"0", tabDiv:"0",linkDiv:"0",bottDiv:"0" };
        for(var i=0;i<arrFuncs.length;i++){
            divs[arrFuncs[i].divId] ="1";
            if(arrFuncs[i].params!=undefined)
                var content = eval(objWidget.getWidgetInstanceName() +"."+ arrFuncs[i].funcName+"("+ JSON.stringify(arrFuncs[i].params) +")");
            else
                var content = eval(objWidget.getWidgetInstanceName() +"."+ arrFuncs[i].funcName+"()");
            $("#"+objWidget.getUniqueWidgetId()+arrFuncs[i].divId).html(content);
            if(arrFuncs[i].callBack!=undefined)
                eval(objWidget.getWidgetInstanceName()+"."+arrFuncs[i].callBack+"()");
        }
        for(var props in divs){
            if(divs[props]=="0")
                $("#"+props).css("display","none");
        }
        $.fn.setLinks();
    };

    $.fn.setLinks = function(){
        $("#related_links a").click(function(){
            $("#related_links .bullet:all").html("&nbsp;");
            $("#related_links a:all").removeClass("rel_active");
            $(this).addClass("rel_active");
            $(this).parent().prev().children().each(function(){
                $(this).html("&rsaquo;");
            });

        });
    };

})(jQuery);




