/*
 * @author Tejus Pratap
 * @desc Jquery plugin to render stylised buttons
 * @params A <a> tag with the attribute sp-button='true'
 */
(function($) {
			$.spButtonCounter=0;
			$.spButton=function(options){
					if(options==undefined && $.spButton.options!=undefined && $.spButton.options!=null)
					{
						options=$.spButton.options;
					}
					var settings=
					{
							padding:"2px",
							borderStyle:"solid",
							borderWidth:"2px",
							backgroundColor:"#cccccc",
							corner:false,
							margin:"0px",
							activeBorderColor:"#6F6F6F #AFAFAF #AFAFAF #6F6F6F",
							inactiveBorderColor:"#AFAFAF #6F6F6F #6F6F6F #AFAFAF"
					}
					$.extend(settings,options);
					var sp_button=function(text,buttonId,image,id,className,imageWidth)
					{
						id=id.length!=0?id:"";
						className=className.length!=0?"sp-button "+className:"sp-button";
						text=text.length!=0?text:"";
						var padding=settings.padding;
						var imagePadding="";
						if(image!=undefined)
						{
							if(imageWidth==undefined)
							{
								var heavyImage = new Image(); 
								heavyImage.src = image;
								if(text=="")
								{
									imagePadding=heavyImage.width;
									text="<span style='margin:"+imagePadding+"px'>&nbsp;</span>";
								}
								else
								{
									imagePadding=heavyImage.width+5;
								}
							}
							else
							{
								imagePadding=parseInt(imageWidth);
								if(text=="")
								{
									text="<span style='width:1px'></span>";
								}
								else
								{
									imagePadding=imagePadding+5;
								}
							}
							imagePadding=imagePadding+"px";
							var out="<span style='width:10%;background-color:"+settings.backgroundColor+";border-color:"+settings.inactiveBorderColor+";padding:"+padding+";border-style:"+settings.borderStyle+";border-width:"+settings.borderWidth+"'><span style='text-align:right;background: transparent url("+image+") no-repeat scroll 0pt 1pt; padding-left:"+imagePadding+"; -moz-background-clip: border; -moz-background-origin: padding; -moz-background-inline-policy: continuous;'>"+text+"</span></span>";
						}
						else
						{
							var out="<span style='width:10%;background-color:"+settings.backgroundColor+";border-color:"+settings.inactiveBorderColor+";padding:"+padding+";border-style:"+settings.borderStyle+";border-width:"+settings.borderWidth+"'><span style='text-align:right;'>"+text+"</span></span>";
						}
						return out;
					}
					
					var sp_button_disabled=function(text,buttonId,image,id,className,imageWidth)
					{
						text=text.length!=0?text:"";
						id=id.length!=0?id:"";
						className=className.length!=0?className:"sp-button";
						inactiveBorderColor="#DFDFDF";
						var padding=settings.padding;
						var imagePadding="";
						if(image!=undefined)
						{
							if(imageWidth==undefined)
							{
								var heavyImage = new Image(); 
								heavyImage.src = image;
								if(text=="")
								{
									imagePadding=heavyImage.width;
								}
								else
								{
									imagePadding=heavyImage.width+5;
								}
							}
							else
							{
								imagePadding=parseInt(imageWidth);
								imagePadding=imagePadding+5;
								if(text=="")
								{
									text="<span style='width:1px'></span>";
								}
								else
								{
									imagePadding=imagePadding+5;
								}
							}
							imagePadding=imagePadding+"px";
							var out="<span disabledBorder='"+settings.inactiveBorderColor+"' style='width:10%;color:#6f6f6f;background-color:"+settings.backgroundColor+";border-color:"+inactiveBorderColor+";padding:"+padding+";border-style:"+settings.borderStyle+";border-width:"+settings.borderWidth+"'><span style='text-align:right;background: transparent url("+image+") no-repeat scroll 0pt 1pt; padding-left: "+imagePadding+"; -moz-background-clip: border; -moz-background-origin: padding; -moz-background-inline-policy: continuous;'>"+text+"</span></span>";
						}
						else
						{
							var out="<span disabledBorder='"+settings.inactiveBorderColor+"' style='width:10%;color:#6f6f6f;background-color:"+settings.backgroundColor+";border-color:"+inactiveBorderColor+";padding:"+padding+";border-style:"+settings.borderStyle+";border-width:"+settings.borderWidth+"'><span style='text-align:right;'>"+text+"</span></span>";
						}
						return out;
					}
					jQuery.extend({
						sp_button_mouseDown:function(obj)
						{
							if($(obj).attr("disabled")!="true" && $(obj).attr("disabled")!=true)
							{
								$(obj).find("> span").css("border-color",settings.activeBorderColor)
							}
						},
						sp_button_mouseUp:function(obj)
						{
							if($(obj).attr("disabled")!="true" && $(obj).attr("disabled")!=true)
							{
								$(obj).find("> span").css("border-color",settings.inactiveBorderColor);
								$(obj).blur();
							}
						}
					});
					
					$("a[sp-button=true]").each(function(){
						var text=$(this).html();
						var buttonId="sp-button-"+$.spButtonCounter++;
						var isDisabled=$(this).attr("disabled");
						var id=$(this).attr("id");
						var className=$(this).attr("class");
						var image = $(this).attr("image");
						var imageWidth=$(this).attr("imagewidth");
						className=className.length!=0?className:"sp-button";
						if(isDisabled=="true" || isDisabled==true)
						{
							$(this).wrapInner(sp_button_disabled(text,buttonId,image,id,className,imageWidth));
							$(this).attr('button','true').attr('disabled','true').attr('class',className).attr('buttonId',buttonId).attr('id',id).attr('onblur','$.sp_button_mouseUp(this)');
							$(this).attr('href','javascript:void(0)').attr('style','text-decoration:none;width:1%;cursor:default');
							$(this).removeAttr("sp-button");
							$(this).mousedown(function(){
								$.sp_button_mouseDown(this);
							}).mouseup(function(){
								$.sp_button_mouseUp(this);
							});
						}
						else
						{
							$(this).wrapInner(sp_button(text,buttonId,image,id,className,imageWidth));
							$(this).attr('button','true').attr('class',className).attr('buttonId',buttonId).attr('id',id).attr('onblur','$.sp_button_mouseUp(this)');
							$(this).attr('href','javascript:void(0)').attr('style','text-decoration:none;width:1%');
							$(this).removeAttr("sp-button");
							$(this).mousedown(function(){
								$.sp_button_mouseDown(this);
							});
							$(this).mouseup(function(){
								$.sp_button_mouseUp(this);
							});
						}
						if(settings.corner && jQuery.browser.msie==false)
						{
							if(typeof($.fn.corner)=="function")
							{
								$("a[buttonId="+buttonId+"] > span").corner(settings.corner);
							}
							else
							{
								alert("Please include corner plugin");
							}
						}
					});
			}
			$.spButton.disable=function(identifier)
			{
				$(identifier).find("> span").each(function(){
						var borderColor=$(this).css("border-color");
						$(this).attr("disabledBorder",borderColor);
						$(this).css("border-color","#DFDFDF").css("color","#6f6f6f");
						var pObj=$(this).parent();
						var clickLink=pObj.attr("onclick");
						clickLink="'"+clickLink+"'";
						clickLink=$.trim(clickLink.toString().substr(26,clickLink.length).slice(0,-3));
						pObj.attr("donclick",clickLink).removeAttr("onclick").attr("disabled","true");
				});
			}
			$.spButton.enable=function(identifier)
			{
				$(identifier).find("> span").each(function(){
						var disabledBorderColor=$(this).attr("disabledBorder");
						$(this).removeAttr("disabledBorder");
						$(this).css("border-color",disabledBorderColor).css("color","");
						var pObj=$(this).parent();
						var clickLink=pObj.attr("donclick");
						pObj.removeAttr("donclick");
						pObj.removeAttr("disabled");
						pObj.attr("onclick",clickLink)
						pObj.focus();
				});
			}
			$.spButton.intervalVar="";
			$.spButton.options=null;
			$.spButton.render=function(options)
			{
				if($("a[sp-button='true']").size()>0)
					{
						$.spButton(options);
						window.clearInterval($.spButton.intervalVar);
						$.spButton.intervalVar=0;
					}
			};
			$.spButton.init=function(realTime,options)
			{
				$.spButton.options=options;
				$.spButton.render(options);
				if(realTime)
				{
					$.spButton.realTime();
				}
			}
			$.spButton.realTime=function()
			{
				$("a,button,input").live("click",function(){  //replace with LiveQuery for jquery v1.2.6
					$.spButton.intervalVar=window.setInterval("$.spButton.render();",50)
				});
			}
		})(jQuery);
		
		$(function(){
			$.spButton.init(true,{corner:"3px"});
		});