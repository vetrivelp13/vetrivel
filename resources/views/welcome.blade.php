<!doctype html>
<html>
    <head>
        <title></title>
    </head>
    
    <body>
        <div style="Align:'center'" >
        	<h1>About my family</h1>
        		<ul>
        			@foreach ($tasks as $task)
        
        				<li>{{ $task->name }}</li>
        
        			@endforeach
        		</ul>
        </div>
    </body>
</html>
