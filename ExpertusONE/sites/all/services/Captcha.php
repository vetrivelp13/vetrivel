<?php
include_once "Encryption.php";

//Send a generated image to the browser
create_image();
exit();

function create_image()
{
    //read the encrypted string from request
    $pass = $_GET["captcha"];
    $enc = new Encrypt();
    $pass = $enc->decrypt($pass);

    //Set the image width and height
    $width = 110;
    $height = 35;

    //Create the image resource
    $image = ImageCreate($width, $height);

    //We are making three colors, white, black and gray
    $white = ImageColorAllocate($image, 255, 255, 255);
    $black = ImageColorAllocate($image, 82, 97, 16);
    $grey = ImageColorAllocate($image, 204, 204, 204);

    //Make the background black
    ImageFill($image, 0, 0, $black);

    //Add randomly generated string in white to the image
    $pageview_letters = preg_split('//', $pass, -1 ); // Form are original array of letters.
    $minus = 15; // The letter spacing in pixels
    $yminus =8;
    $first = true; // Whether or not we have started the string
    $x = 100; // X Location of imagestring
    $y = 18; // Y Location of imagestring
    $letters = array(); // Initiate the array o letters.

    foreach ( $pageview_letters as $letter ) {
        $letters[] = $letter;        
    }
     
    $letters = array_reverse( $letters );
    $count=0; 
    foreach ( $letters as $letter ) {        
        if ( $first ) {          
            imagestring( $image, 5, $x, $y, $letter, $white );
            $first = false;          
        } else {             
            $x = ( $x - $minus );
            if($count < 2 )$y = ($y - $yminus +5); else $y = ($y + $yminus - 10);
            imagestring( $image, 5, $x, $y, $letter, $white );           
        }        
        $count++;
    }

    //ImageString($image, 5, 10, 3, $pass, $white);

    //Throw in some lines to make it a little bit harder for any bots to break
     /*ImageRectangle($image,0,0,$width-1,$height-1,$grey);
     imageline($image, 0, $height/2, $width, $height/6, $grey);
     imageline($image, $width/2, 0, $width/5, $height, $grey);
    */
    //Tell the browser what kind of file is come in
    header("Content-Type: image/jpeg");

    //Output the newly created image in jpeg format
    ImageJpeg($image);
     
    //Free up resources
    ImageDestroy($image);
}
?>