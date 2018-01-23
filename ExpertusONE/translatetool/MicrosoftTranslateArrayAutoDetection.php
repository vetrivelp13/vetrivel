<?php

class AccessTokenAuthentication {
    /*
     * Get the access token.
     *
     * @param string $azure_key    Subscription key for Text Translation API.
     *
     * @return string.
     */
    function getToken($azure_key)
    {
        $url = 'https://api.cognitive.microsoft.com/sts/v1.0/issueToken';
        $ch = curl_init();
        $data_string = json_encode('{body}');
        curl_setopt($ch, CURLOPT_POSTFIELDS, $data_string);
        curl_setopt($ch, CURLOPT_HTTPHEADER, array(
                'Content-Type: application/json',
                'Content-Length: ' . strlen($data_string),
                'Ocp-Apim-Subscription-Key: ' . $azure_key
            )
        );
        curl_setopt($ch, CURLOPT_URL, $url);
        curl_setopt($ch, CURLOPT_HEADER, false);
        curl_setopt($ch, CURLOPT_RETURNTRANSFER, TRUE);
        $strResponse = curl_exec($ch);
        curl_close($ch);
        return $strResponse;
    }
}

/*
 * Class:HTTPTranslator
 *
 * Processing the translator request.
 */
Class HTTPTranslator {
    /*
     * Create and execute the HTTP CURL request.
     *
     * @param string $url        HTTP Url.
     * @param string $authHeader Authorization Header string.
     * @param string $postData   Data to post.
     *
     * @return string.
     *
     */
    function curlRequest($url, $authHeader, $postData=''){
        //Initialize the Curl Session.
        $ch = curl_init();
        //Set the Curl url.
        curl_setopt ($ch, CURLOPT_URL, $url);
        //Set the HTTP HEADER Fields.
        curl_setopt ($ch, CURLOPT_HTTPHEADER, array($authHeader,"Content-Type: text/xml"));
        //CURLOPT_RETURNTRANSFER- TRUE to return the transfer as a string of the return value of curl_exec().
        curl_setopt ($ch, CURLOPT_RETURNTRANSFER, TRUE);
        //CURLOPT_SSL_VERIFYPEER- Set FALSE to stop cURL from verifying the peer's certificate.
        curl_setopt ($ch, CURLOPT_SSL_VERIFYPEER, False);
        if($postData) {
            //Set HTTP POST Request.
            curl_setopt($ch, CURLOPT_POST, TRUE);
            //Set data to POST in HTTP "POST" Operation.
            curl_setopt($ch, CURLOPT_POSTFIELDS, $postData);
        }
        //Execute the  cURL session.
        $curlResponse = curl_exec($ch);
        //Get the Error Code returned by Curl.
        $curlErrno = curl_errno($ch);
        if ($curlErrno) {
            $curlError = curl_error($ch);
            throw new Exception($curlError);
        }
        //Close a cURL session.
        curl_close($ch);
        return $curlResponse;
    }


    /*
     * Create Request XML Format.
     *
     * @param string $fromLanguage   Source language Code.
     * @param string $toLanguage     Target language Code.
     * @param string $contentType    Content Type.
     * @param string $inputStrArr    Input String Array.
     *
     * @return string.
     */
    function createReqXML($fromLanguage,$toLanguage,$contentType,$inputStrArr) {
        //Create the XML string for passing the values.
        $requestXml = "<TranslateArrayRequest>".
            "<AppId/>".
            "<From>$fromLanguage</From>". 
            "<Options>" .
             "<Category xmlns=\"http://schemas.datacontract.org/2004/07/Microsoft.MT.Web.Service.V2\" />" .
              "<ContentType xmlns=\"http://schemas.datacontract.org/2004/07/Microsoft.MT.Web.Service.V2\">$contentType</ContentType>" .
              "<ReservedFlags xmlns=\"http://schemas.datacontract.org/2004/07/Microsoft.MT.Web.Service.V2\" />" .
              "<State xmlns=\"http://schemas.datacontract.org/2004/07/Microsoft.MT.Web.Service.V2\" />" .
              "<Uri xmlns=\"http://schemas.datacontract.org/2004/07/Microsoft.MT.Web.Service.V2\" />" .
              "<User xmlns=\"http://schemas.datacontract.org/2004/07/Microsoft.MT.Web.Service.V2\" />" .
            "</Options>" .
            "<Texts>";
        foreach ($inputStrArr as $inputStr)
        $requestXml .=  "<string xmlns=\"http://schemas.microsoft.com/2003/10/Serialization/Arrays\">$inputStr</string>" ;
        $requestXml .= "</Texts>".
            "<To>$toLanguage</To>" .
          "</TranslateArrayRequest>";
        return $requestXml;
    }
}
function invokeTranslation($arr = '',$toL='it'){

try {
    //Client Secret key of the application.
    $config=getConfig('exp_sp');
        //Client Secret key of the application.
    if(isset($config["microsoft_api_key"]) && trim($config["microsoft_api_key"]) !="")
           $clientSecret = $config["microsoft_api_key"];
        else
        {
            $retArr = array();
            /*for($i = 0; $i < count($arr); $i++)
            {
                $retArr[$i] = $arr[$i];
            }*/
            return $retArr; //translation will be performed manually when key is not present. 
        }

    //Create the AccessTokenAuthentication object.
    $authObj      = new AccessTokenAuthentication();
    //Get the Access token.
    $accessToken  = $authObj->getToken($clientSecret);
    //Create the authorization Header string.
    $authHeader = "Authorization: Bearer ". $accessToken;

    //Set the params.//
    $fromLanguage = "";// "en" - it detects automatically 
    $toLanguage   = $toL;
    $inputStrArr  = $arr;// array("The answer lies in machine translation.", "the best machine translation technology cannot always provide translations tailored to a site or users like a human.");
    $contentType  = 'text/plain';
    //Create the Translator Object.
    $translatorObj = new HTTPTranslator();

    //Get the Request XML Format.
    $requestXml = $translatorObj->createReqXML($fromLanguage,$toLanguage,$contentType,$inputStrArr);

    //HTTP TranslateMenthod URL.
    $translateUrl = "https://api.microsofttranslator.com/v2/Http.svc/TranslateArray";

    //Call HTTP Curl Request.
    $curlResponse = $translatorObj->curlRequest($translateUrl, $authHeader, $requestXml);

    //Interprets a string of XML into an object.
    $xmlObj = simplexml_load_string($curlResponse);
    $i=0;
   // echo "<table border=2px>";
   // echo "<tr>";
   // echo "<td><b>From $fromLanguage</b></td><td><b>To $toLanguage</b></td>";
   // echo "</tr>";

    foreach($xmlObj->TranslateArrayResponse as $translatedArrObj){
     $arr[$i] =    $translatedArrObj->TranslatedText;  
//   echo "<tr><td>".$inputStrArr[$i]."</td><td>". $translatedArrObj->TranslatedText."</td></tr>";
        $i++;
    }
    //echo "</table>";
return $arr;
} catch (Exception $e) {
    echo "Exception: " . $e->getMessage() . PHP_EOL;
}
}
//invokeTranslation();
