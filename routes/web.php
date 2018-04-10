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
    
    $tasks = DB::table('vc_users')->get();
    
    return view('index', compact('tasks'));
});


Route::get('/detail/{uid}', function ($uid) {
    
    $details = DB::table('vc_users')->find($uid);
    
    return view('show', compact('details'));
});
        