@extends('layouts.app')

@section('content')
<link href="{{ asset('/css/style.css') }}" rel="stylesheet">
    <div class="vclsloginbox">
    <img src="/images/Vclasslogo.png" alt="" class="vclslogo">
                    <form method="POST" action="{{ route('login') }}">
                        @csrf
                        <div class="vclsid infield">
                            <label for="vclassid">{{ __('ID') }}</label>
                            <input id="vclassid" type="text" name="vclassid" value="{{ old('mamber_id') }}" required autofocus>
                        </div>                        
                        
                        <div class="vclsid infield">
                            <label for="vclassname">{{ __('User Name') }}</label>
							<input id="username" type="text" name="username" value="{{ old('username') }}" required autofocus>
                        </div>

                        <div class="vclsid infield">
                            <label for="vclasspassword">{{ __('Password') }}</label>
                            <input id="vclasspassword" type="password" name="vclasspassword" required>
                        </div>
						<a href="#" class="forgetlink" target="_self">Forget Password?</a>
                        <input type="submit" class="vclssubmit" name="" value="LOGIN">

                    </form>
</div>
@endsection
