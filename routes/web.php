<?php

/*
|--------------------------------------------------------------------------
| Web Routes
|--------------------------------------------------------------------------
|
| Here is where you can register web routes for your application. These
| routes are loaded by the RouteServiceProvider within a group which
| contains the "web" middleware group. Now create something great!
|
*/

Route::get('/', function () {
    
    $tasks = DB::table('vel_family')->get();
    
    return view('index', compact('tasks'));
});


Route::get('/detail/{id}', function ($id) {
    
    $details = DB::table('vel_family')->find($id);
    
    //dd($details);
    
    return view('show', compact('details'));
});
        