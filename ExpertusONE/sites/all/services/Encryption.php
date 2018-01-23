<?php
/**
 * @desc A class to encrypt and decrypt strings
 * @author Tejus Pratap
 * @method Encrypt, Decrypt
 */
class Encrypt
{
	private $td;
	public function __construct($key = 'holdSmartConnect', $iv = '87654321', $algorithm = MCRYPT_BLOWFISH, $mode = MCRYPT_MODE_CBC)
	{
		if(extension_loaded('mcrypt') === FALSE)
		{
			$prefix = (PHP_SHLIB_SUFFIX == 'dll') ? 'php_' : '';
			dl($prefix . 'mcrypt.' . PHP_SHLIB_SUFFIX) or die('The Mcrypt module could not be loaded.');
		}
		if($mode!='ecb' && $iv===false)
		{
			die('In order to use encryption modes other then ecb, you must specify a unique and consistent initialization vector.');
		}

		$this->td = mcrypt_module_open($algorithm, '', $mode, '') ;
		$random_seed = strstr(PHP_OS, "WIN") ? MCRYPT_RAND : MCRYPT_DEV_RANDOM;
		$iv = ($iv === false) ? mcrypt_create_iv(mcrypt_enc_get_iv_size($this->td), $random_seed) : substr($iv, 0, mcrypt_enc_get_iv_size($this->td));
		$expected_key_size = mcrypt_enc_get_key_size($this->td);
		mcrypt_generic_init($this->td, $key, $iv);
	}

	public function __destruct()
	{
		mcrypt_generic_deinit($this->td);
		mcrypt_module_close($this->td);
	}

	/**
	 * The method accepts the string to be encrypted and returns the encrypted string.
	 *
	 * @param String $plain_string
	 * @return Encrypted String
	 */
	public function encrypt($plain_string)
	{
		return bin2hex(mcrypt_generic($this->td, $plain_string));
	}
	 
	/**
	 * The method accepts the string to be decrypted and returns the decrypted string.
	 *
	 * @param String $encrypted_string
	 * @return Decrypted String
	 */
	public function decrypt($encrypted_string)
	{
		return trim(mdecrypt_generic($this->td, $this->hex2bin($encrypted_string)));
	}
	
	public function decryptAES($AESkey,$AESiv,$inputData)
	{
	return rtrim(mcrypt_decrypt(MCRYPT_RIJNDAEL_128,$AESkey,$inputData,MCRYPT_MODE_CBC,$AESiv),"\x00..\x1F");
	}

	public function hex2bin ($RawInput)
	{
		$BinStr = '';
		for ($i = 0; $i < strlen ($RawInput); $i += 2)
		{ $BinStr .= '%'.substr ($RawInput, $i, 2);}
		return rawurldecode ($BinStr);
	}
	
	public function getAESDecryptedValue($encryptedValue) {
		
		$encryptedArray = explode("~~x~",$encryptedValue);
		$toBeDecryted = $this->hex2bin($encryptedArray[0]);
		$saltHex = $this->hex2bin($encryptedArray[1]);
		
		$keyAndIV = $this->getAESkeyAndIV("x-ExpertusONEKey",$saltHex);
		return $this->decryptAES($keyAndIV["key"],$keyAndIV["iv"],$toBeDecryted);
	}
	
	public function getAESkeyAndIV($password, $salt, $keySize = 8, $ivSize = 4, $iterations = 1, $hashAlgorithm = "md5") {
		$targetKeySize = $keySize + $ivSize;
		$derivedBytes = "";
		$numberOfDerivedWords = 0;
		$block = NULL;
	
		$hasher = hash_init($hashAlgorithm);
		while ($numberOfDerivedWords < $targetKeySize) {
			if ($block != NULL) {
				hash_update($hasher, $block);
			}
			hash_update($hasher, $password);
			hash_update($hasher, $salt);
			$block = hash_final($hasher, TRUE);
			$hasher = hash_init($hashAlgorithm);
	
			// Iterations
			for ($i = 1; $i < $iterations; $i++) {
				hash_update($hasher, $block);
				$block = hash_final($hasher, TRUE);
				$hasher = hash_init($hashAlgorithm);
			}
	
			$derivedBytes .= substr($block, 0, min(strlen($block), ($targetKeySize - $numberOfDerivedWords) * 4));
	
			$numberOfDerivedWords += strlen($block)/4;
		}
	
		return array(
				"key" => substr($derivedBytes, 0, $keySize * 4),
				"iv"  => substr($derivedBytes, $keySize * 4, $ivSize * 4)
		);
	}
	
}
?>
