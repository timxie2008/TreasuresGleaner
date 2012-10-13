package com.crazyamber.core;

import java.security.NoSuchAlgorithmException;

import javax.crypto.Cipher;
import javax.crypto.NoSuchPaddingException;
import javax.crypto.spec.IvParameterSpec;
import javax.crypto.spec.SecretKeySpec;

public class SimpleCipher
{
	private IvParameterSpec ivspec;
	private SecretKeySpec keyspec;
	private Cipher cipher;

	//private String iv = "fedcba9876543210";//Dummy iv (CHANGE IT!)
	//private String SecretKey = "0123456789abcdef";//Dummy secretKey (CHANGE IT!)

	public SimpleCipher(byte[] iv, byte[] sk)
	{
		ivspec = new IvParameterSpec(iv);
		keyspec = new SecretKeySpec(sk, "AES");

		try
		{
			cipher = Cipher.getInstance("AES/CBC/NoPadding");
		}
		catch (NoSuchAlgorithmException e)
		{
			e.printStackTrace();
			Logger.e(e);
		}
		catch (NoSuchPaddingException e)
		{
			e.printStackTrace();
			Logger.e(e);
		}
	}

	public String encryptString(String text)  throws Exception
	{
		byte[] r = encrypt(text);
		return Utils.bytesToHex(r);
	}
	
	public String decryptString(String text) throws Exception
	{
		byte[] r = decrypt(text);
		return new String(r,"UTF-8");
	}
	
	public byte[] encrypt(String text) throws Exception
	{
		if (text == null || text.length() == 0) 
			throw new Exception("Empty string");

		byte[] encrypted = null;

		try
		{
			cipher.init(Cipher.ENCRYPT_MODE, keyspec, ivspec);
			encrypted = cipher.doFinal(padString(text).getBytes());
		}
		catch (Exception e)
		{
			Logger.e(e);
			throw new Exception("[encrypt] " + e.getMessage());
		}

		return encrypted;
	}

	public byte[] decrypt(String code) throws Exception
	{
		if (code == null || code.length() == 0) 
			throw new Exception("Empty string");

		byte[] decrypted = null;

		try
		{
			cipher.init(Cipher.DECRYPT_MODE, keyspec, ivspec);
			decrypted = cipher.doFinal(Utils.hexToBytes(code));
		}
		catch (Exception e)
		{
			Logger.e(e);
			throw new Exception("[decrypt] " + e.getMessage());
		}
		return decrypted;
	}

	private String padString(String source)
	{
		char paddingChar = ' ';
		int size = 16;
		int align = (source.length() + (size - 1)) / size;
		align *= size;
		int padLength = align - source.length();

		for (int i = 0; i < padLength; i++)
		{
			source += paddingChar;
		}

		return source;
	}
}
