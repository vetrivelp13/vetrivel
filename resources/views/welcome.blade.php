<!doctype html>
<html lang="{{ app()->getLocale() }}">
    <head>
        
    </head>
    <body>
   	 	<div class="vclsloginbox">
	        <img src="/images/Vclasslogo.png" alt="" class="vclslogo">
	        <form class="vclsloginform" method="POST" action="/login">
	        {{ csrf_field() }}
	            <div class="vclsid infield">
	                <label for="vclassid">ID</label>
	                <input type="text" class="" name="id" required>
	            </div>
	            <div class="vclsid infield">
	                <label for="vclassname">USER NAME</label>
	                <input type="text" class="" name="username" placeholder="Enter User Name" required>
	            </div>
	            <div class="vclsid infield">
	                <Label for="vclasspassword">PASSWORD</Label>
	                <input type="password" class="" name="password" placeholder="Enter Password" required>
	            </div>
	            <a href="#" class="forgetlink" target="_self">Forget Password?</a>
	            <input type="submit" class="vclssubmit" name="" value="LOGIN">
	        </form>
    	</div>
	</body>
</html>



