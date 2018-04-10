<!doctype html>
<html>
    <head>
        <title>My Page</title>
    </head>
    
    <body>
        <div style="align:'center'" >
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
