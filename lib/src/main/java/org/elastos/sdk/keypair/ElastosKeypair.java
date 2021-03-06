package org.elastos.sdk.keypair;

public final class ElastosKeypair extends NativeBridge {
    private ElastosKeypair() {}

    public static class Data {
        public byte[] buf;
    }

    /**
     * \~English
     * Get single address wallet public key.
     *
     * @param
     *      seed     [in] binary conent of seed.
     * @param
     *      seedLen  [in] the length of seed.
     *
     * @return
     *      the public key if succeeded, or nullptr if failed.
     */
    public static native String getSinglePublicKey(Data seed, int seedLen);

    /**
     * \~English
     * Get single address wallet private key.
     *
     * @param
     *      seed     [in] binary conent of seed.
     * @param
     *      seedLen  [in] the length of seed.
     *
     * @return
     *      the private key if succeeded, or nullptr if failed.
     */
    public static native String getSinglePrivateKey(Data seed, int seedLen);

    /**
     * \~English
     * Generate mnemonic.
     *
     * @param
     *      language     [in] language, such as english, chinese etc.
     * @param
     *      words        [in] the words, seperated by ' ', if the language is english, words is empty string.
     *
     * @return
     *      mnemonic if succeeded, or nullptr if failed.
     */
    public static native String generateMnemonic(String language, String words);

    /**
     * \~English
     * Get address from public key.
     *
     * @param
     *      publicKey     [in] the public key.
     *
     * @return
     *      the address if succeeded, or nullptr if failed.
     */
    public static native String getAddress(String publicKey);

    /**
     * \~English
     * Get address from personal info.
     *
     * @param
     *      info     [in] personal info, such as phone number, email address etc.
     *
     * @return
     *      the address if succeeded, or nullptr if failed.
     */
    public static native String getAddressByInfo(String info);

    /**
     * \~English
     * Check the address is valid.
     *
     * @param
     *      address          [in] the address.
     *
     * @return
     *      true if valid address, or false if not.
     */
    public static native boolean isAddressValid(String address);

    /**
     * \~English
     * Get seed from mnemonic.
     *
     * @param
     *      seed                [out] the seed content.
     * @param
     *      mnemonic            [in] mnemonic, seperated by ' '.
     * @param
     *      mnemonicPassword    [in] mnemonic password, empty string or effctive password.
     *
     * @return
     *      the seed buffer length if succeeded, or 0 if failed.
     */
    public static native int getSeedFromMnemonic(Data seed, String mnemonic, String mnemonicPassword);

    /**
     * \~English
     * Sign data.
     *
     * @param
     *      privateKey          [in] the private key to sign the data.
     * @param
     *      data                [in] the data buffer.
     * @param
     *      len                 [in] length of data buffer.
     * @param
     *      signedData          [out] the signed data.
     *
     * @return
     *      the signed data length if succeeded, or 0 if failed.
     */
    public static native int sign(String privateKey, Data data, int len, Data signedData);

    /**
     * \~English
     * Verify data.
     *
     * @param
     *      publicKey           [in] the publik key to verify the data.
     * @param
     *      data                [in] the source data.
     * @param
     *      len                 [in] length of source data.
     * @param
     *      signedData          [in] the signed data.
     * @param
     *      signedLen           [in] the signed data length.
     *
     * @return
     *      true if verification passed, or false if failed.
     */
    public static native boolean verify(String publicKey, Data data, int len, Data signedData, int signedLen);
}
