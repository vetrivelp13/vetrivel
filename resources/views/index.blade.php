<!doctype html>
<html>
    <head>
        <title></title>
    </head>
    
    <body>
        <div style="align:'center'" >
        	<h1>About my family</h1>
        		<ul>
        			@foreach ($tasks as $task)
        
        				<li>
        					<a href="/detail/{{$task->id}}">
        						{{ $task->name }}
        					</a>
        				</li>
        
        			@endforeach
        		</ul>
        </div>
    </body>
</html>
