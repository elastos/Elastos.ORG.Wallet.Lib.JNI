package org.elastos.sdk.keypair;

public final class ElastosKeypairDID extends NativeBridge {
    private ElastosKeypairDID() {}

// Apis for DID
    /**
     * \~English
     * Generate DID from public key.
     *
     * @param
     *      idPublicKey          [in] the public key of ID chain.
     *
     * @return
     *      the DID if succeeded, or nullptr if failed.
     */
    public static native String getDid(String idPublicKey);
}
