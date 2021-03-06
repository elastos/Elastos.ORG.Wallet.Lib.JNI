#include <jni.h>
#include <string>

#include "Elastos.Wallet.Utility.h"

#include "JniUtils.hpp"
#include "Log.hpp"

static const char* JavaClassName_ElastosKeypair_Data = "org/elastos/sdk/keypair/ElastosKeypair$Data";

static jobject NewData(JNIEnv *jEnv)
{
    jclass jClass_Data = JniUtils::GetJavaClass(jEnv, JavaClassName_ElastosKeypair_Data);
    jmethodID jMethod_DataConstructor = jEnv->GetMethodID(jClass_Data, "<init>", "()V");
    jobject jData = jEnv->NewObject(jClass_Data, jMethod_DataConstructor);

    return jData;
}

static void SetDataBuffer(JNIEnv *jEnv, jobject jData, int8_t *buf, int size)
{
    auto jBuf = JniUtils::GetByteArraySafely(jEnv, buf, size);

    jclass jClass_Data = JniUtils::GetJavaClass(jEnv, JavaClassName_ElastosKeypair_Data);
    jfieldID jField_DataBuf = jEnv->GetFieldID(jClass_Data, "buf", JniUtils::JavaClassNameByteArray);
    jEnv->SetObjectField(jData, jField_DataBuf, jBuf.get());

    return;
}

static std::shared_ptr<int8_t> GetDataBuffer(JNIEnv *jEnv, jobject data)
{
    jclass jClass_Data = JniUtils::GetJavaClass(jEnv, JavaClassName_ElastosKeypair_Data);
    jfieldID jField_DataBuf = jEnv->GetFieldID(jClass_Data, "buf", JniUtils::JavaClassNameByteArray);
    jobject jObject_DataBuf = jEnv->GetObjectField(data, jField_DataBuf);

    auto buf = JniUtils::GetByteArraySafely(jEnv, static_cast<jbyteArray>(jObject_DataBuf));

    return buf;
}

extern "C"
JNIEXPORT jstring JNICALL
Java_org_elastos_sdk_keypair_ElastosKeypair_generateMnemonic(JNIEnv *jEnv, jclass jType,
                                                           jstring jLanguage, jstring jWords)
{

    auto language = JniUtils::GetStringSafely(jEnv, jLanguage);
    auto words = JniUtils::GetStringSafely(jEnv, jWords);

    char* mnemonic = generateMnemonic(language.get(), words.get());
    if(mnemonic == nullptr) {
        return nullptr;
    }

    auto jMnemonic = JniUtils::GetStringSafely(jEnv, mnemonic, false);
    freeBuf(mnemonic);

    return jMnemonic.get();
}

extern "C"
JNIEXPORT jint JNICALL
Java_org_elastos_sdk_keypair_ElastosKeypair_getSeedFromMnemonic(JNIEnv *jEnv, jclass jType,
                                                              jobject jSeed, jstring jMnemonic,
                                                              jstring jMnemonicPassword) {
    auto mnemonic = JniUtils::GetStringSafely(jEnv, jMnemonic);
    auto mnemonicPassword = JniUtils::GetStringSafely(jEnv, jMnemonicPassword);

    void *seed = nullptr;
    int seedLen = getSeedFromMnemonic(&seed, mnemonic.get(), mnemonicPassword.get());
    if(seedLen <= 0 || seed == nullptr) {
        return seedLen;
    }

    SetDataBuffer(jEnv, jSeed, static_cast<int8_t *>(seed), seedLen);
    freeBuf(seed);

    return seedLen;
}

extern "C"
JNIEXPORT jstring JNICALL
Java_org_elastos_sdk_keypair_ElastosKeypair_getSinglePrivateKey(JNIEnv *jEnv, jclass jType,
                                                              jobject jSeed, jint jSeedLen) {

    auto seed = GetDataBuffer(jEnv, jSeed);
    int seedLen = jSeedLen;

    char* privKey = getSinglePrivateKey(seed.get(), seedLen);
    if(privKey == nullptr) {
        return nullptr;
    }

    auto jPrivateKey = JniUtils::GetStringSafely(jEnv, privKey, false);
    freeBuf(privKey);

    return jPrivateKey.get();
}

extern "C"
JNIEXPORT jstring JNICALL
Java_org_elastos_sdk_keypair_ElastosKeypair_getSinglePublicKey(JNIEnv *jEnv, jclass jType,
                                                             jobject jSeed, jint jSeedLen) {

    auto seed = GetDataBuffer(jEnv, jSeed);
    int seedLen = jSeedLen;

    char* pubKey = getSinglePublicKey(seed.get(), seedLen);
    if(pubKey == nullptr) {
        return nullptr;
    }

    auto jPublicKey = JniUtils::GetStringSafely(jEnv, pubKey, false);
    freeBuf(pubKey);

    return jPublicKey.get();
}

extern "C"
JNIEXPORT jstring JNICALL
Java_org_elastos_sdk_keypair_ElastosKeypair_getAddress(JNIEnv *jEnv, jclass jType,
                                                     jstring jPublicKey) {
    auto pubKey = JniUtils::GetStringSafely(jEnv, jPublicKey);

    char *address = getAddress(pubKey.get());
    if(address == nullptr) {
        return nullptr;
    }

    auto jAddress = JniUtils::GetStringSafely(jEnv, address, false);
    freeBuf(address);

    return jAddress.get();
}

extern "C"
JNIEXPORT jstring JNICALL
Java_org_elastos_sdk_keypair_ElastosKeypair_getAddressByInfo(JNIEnv *jEnv, jclass jType,
                                                       jstring jInfo) {
    auto info = JniUtils::GetStringSafely(jEnv, jInfo);

    char *address = getAddressByInfo(info.get());
    if(address == nullptr) {
        return nullptr;
    }

    auto jAddress = JniUtils::GetStringSafely(jEnv, address, false);
    freeBuf(address);

    return jAddress.get();
}

extern "C"
JNIEXPORT jboolean JNICALL
Java_org_elastos_sdk_keypair_ElastosKeypair_isAddressValid(JNIEnv *jEnv, jclass jType,
                                                           jstring jAddress) {
    auto address = JniUtils::GetStringSafely(jEnv, jAddress);

    bool ret = isAddressValid(address.get());

    return ret;
}

extern "C"
JNIEXPORT jint JNICALL
Java_org_elastos_sdk_keypair_ElastosKeypair_sign(JNIEnv *jEnv, jclass jType,
                                               jstring jPrivateKey,
                                               jobject jData, jint jLen, jobject jSignedData)
{
    auto privateKey = JniUtils::GetStringSafely(jEnv, jPrivateKey);
    auto data = GetDataBuffer(jEnv, jData);
    int len = jLen;
    void *signedData = nullptr;

    int signedLen = sign(privateKey.get(), data.get(), len, &signedData);
    if(signedLen <= 0 || signedData == nullptr) {
        return signedLen;
    }

    SetDataBuffer(jEnv, jSignedData, static_cast<int8_t *>(signedData), signedLen);
    freeBuf(signedData);

    return signedLen;
}

extern "C"
JNIEXPORT jboolean JNICALL
Java_org_elastos_sdk_keypair_ElastosKeypair_verify(JNIEnv *jEnv, jclass jType,
                                                 jstring jPublicKey,
                                                 jobject jData, jint jLen,
                                                 jobject jSignedData, jint jSignedLen)
{
    auto publicKey = JniUtils::GetStringSafely(jEnv, jPublicKey);
    auto data = GetDataBuffer(jEnv, jData);
    int len = jLen;
    auto signedData = GetDataBuffer(jEnv, jSignedData);
    int signedLen = jSignedLen;

    bool verified = verify(publicKey.get(), data.get(), len, signedData.get(), signedLen);

    return verified;
}

extern "C"
JNIEXPORT jobject JNICALL
Java_org_elastos_sdk_keypair_ElastosKeypairHD_getMasterPublicKey(JNIEnv *jEnv, jclass jType,
                                                               jobject jSeed, jint jSeedLen,
                                                               jint jCoinType) {
    auto seed = GetDataBuffer(jEnv, jSeed);
    int seedLen = jSeedLen;
    int coinType = jCoinType;

    MasterPublicKey* masterPublicKey = getMasterPublicKey(seed.get(), seedLen, coinType);


    jobject jMasterPublicKey = NewData(jEnv);
    SetDataBuffer(jEnv, jMasterPublicKey,
                  reinterpret_cast<int8_t *>(masterPublicKey), sizeof(*masterPublicKey));
    delete masterPublicKey;

    return jMasterPublicKey;
}

extern "C"
JNIEXPORT jstring JNICALL
Java_org_elastos_sdk_keypair_ElastosKeypairHD_generateSubPrivateKey(JNIEnv *jEnv, jclass jType,
                                                                  jobject jSeed, jint jSeedLen,
                                                                  jint jCoinType, jint jChain,
                                                                  jint jIndex) {
    auto seed = GetDataBuffer(jEnv, jSeed);
    int seedLen = jSeedLen;
    int coinType = jCoinType;
    int chain = jChain;
    int index = jIndex;

    char *subPrivKey = generateSubPrivateKey(seed.get(), seedLen, coinType, chain, index);

    auto jSubPrivateKey = JniUtils::GetStringSafely(jEnv, subPrivKey, false);
    freeBuf(subPrivKey);

    return jSubPrivateKey.get();
}

extern "C"
JNIEXPORT jstring JNICALL
Java_org_elastos_sdk_keypair_ElastosKeypairHD_generateSubPublicKey(JNIEnv *jEnv, jclass jType,
                                                                 jobject jMasterPublicKey,
                                                                 jint jChain, jint jIndex) {
    auto masterPublicKey = GetDataBuffer(jEnv, jMasterPublicKey);
    int chain = jChain;
    int index = jIndex;

    char * subPubKey = generateSubPublicKey(reinterpret_cast<const MasterPublicKey *>(masterPublicKey.get()),
                                            chain, index);

    auto jSubPublicKey = JniUtils::GetStringSafely(jEnv, subPubKey, false);
    freeBuf(subPubKey);

    return jSubPublicKey.get();
}

extern "C"
JNIEXPORT jstring JNICALL
Java_org_elastos_sdk_keypair_ElastosKeypairDID_getDid(JNIEnv *jEnv, jclass jType,
                                                    jstring jIdPublicKey) {
    auto idPubKey = JniUtils::GetStringSafely(jEnv, jIdPublicKey);

    char *did = getDid(idPubKey.get());
    if(did == nullptr) {
        return nullptr;
    }

    auto jDid = JniUtils::GetStringSafely(jEnv, did, false);
    freeBuf(did);

    return jDid.get();
}

extern "C"
JNIEXPORT jstring JNICALL
Java_org_elastos_sdk_keypair_ElastosKeypairDID_getDidByInfo(JNIEnv *jEnv, jclass jType,
                                                      jstring jInfo) {
    auto info = JniUtils::GetStringSafely(jEnv, jInfo);

    char *did = getDidByInfo(info.get());
    if(did == nullptr) {
        return nullptr;
    }

    auto jDid = JniUtils::GetStringSafely(jEnv, did, false);
    freeBuf(did);

    return jDid.get();
}

extern "C"
JNIEXPORT jstring JNICALL
Java_org_elastos_sdk_keypair_ElastosKeypairSign_generateRawTransaction(JNIEnv *jEnv, jclass jType,
                                                                       jstring jTransaction,
                                                                       jstring jAssertId) {
    auto transaction = JniUtils::GetStringSafely(jEnv, jTransaction);
    auto assertId = JniUtils::GetStringSafely(jEnv, jAssertId);

    char* rawTx = generateRawTransaction(transaction.get(),
                                         assertId.get() == nullptr ? ELA_ASSERT_ID : assertId.get());
    if(rawTx == nullptr) {
        return nullptr;
    }

    auto jRawTx = JniUtils::GetStringSafely(jEnv, rawTx, false);
    freeBuf(rawTx);

    return jRawTx.get();
}

extern "C"
JNIEXPORT jstring JNICALL
Java_org_elastos_sdk_keypair_ElastosKeypairSign_getMultiSignAddress(JNIEnv *jEnv, jclass jType,
                                                                  jobjectArray jPublicKeys,
                                                                  jint jLength,
                                                                  jint jRequiredSignCount) {
//    JniUtils::GetByteArraySafely()
    auto jPubKeyArray = JniUtils::GetObjectArraySafely(jEnv, jPublicKeys);
    int jPubKeyArraySize = jEnv->GetArrayLength(jPublicKeys);
    auto pubKeyArray = new std::shared_ptr<const char>[jPubKeyArraySize];
    for(int idx = 0; idx < jPubKeyArraySize; idx++) {
        pubKeyArray[idx] = JniUtils::GetStringSafely(jEnv, (jstring)jPubKeyArray.get()[idx]);
    }
    const char* publicKeys[jPubKeyArraySize];
    for(int idx = 0; idx < jPubKeyArraySize; idx++) {
        publicKeys[idx] = pubKeyArray[idx].get();
    }

    int length = jLength;
    int requiredSignCount = jRequiredSignCount;

    char* address = getMultiSignAddress(const_cast<char **>(publicKeys), length, requiredSignCount);
    delete[] pubKeyArray;
    if(address == nullptr) {
        return nullptr;
    }

    auto jAddress = JniUtils::GetStringSafely(jEnv, address, false);
    freeBuf(address);

    return jAddress.get();
}

extern "C"
JNIEXPORT jstring JNICALL
Java_org_elastos_sdk_keypair_ElastosKeypairSign_multiSignTransaction(JNIEnv *jEnv, jclass jType,
                                                                   jstring jPrivateKey,
                                                                   jobjectArray jPublicKeys,
                                                                   jint jLength,
                                                                   jint jRequiredSignCount,
                                                                   jstring jTransaction) {
    auto privateKey = JniUtils::GetStringSafely(jEnv, jPrivateKey);
    auto jPubKeyArray = JniUtils::GetObjectArraySafely(jEnv, jPublicKeys);
    int jPubKeyArraySize = jEnv->GetArrayLength(jPublicKeys);
    auto pubKeyArray = new std::shared_ptr<const char>[jPubKeyArraySize];
    for(int idx = 0; idx < jPubKeyArraySize; idx++) {
        pubKeyArray[idx] = JniUtils::GetStringSafely(jEnv, (jstring)jPubKeyArray.get()[idx]);
    }
    const char* publicKeys[jPubKeyArraySize];
    for(int idx = 0; idx < jPubKeyArraySize; idx++) {
        publicKeys[idx] = pubKeyArray[idx].get();
    }

    int length = jLength;
    int requiredSignCount = jRequiredSignCount;
    auto transaction = JniUtils::GetStringSafely(jEnv, jTransaction);

    char* signedTx = multiSignTransaction(privateKey.get(), const_cast<char **>(publicKeys), length, requiredSignCount, transaction.get());
    delete[] pubKeyArray;
    if(signedTx == nullptr) {
        return nullptr;
    }

    auto jSignedTx = JniUtils::GetStringSafely(jEnv, signedTx, false);
    freeBuf(signedTx);

    return jSignedTx.get();
}

extern "C"
JNIEXPORT jstring JNICALL
Java_org_elastos_sdk_keypair_ElastosKeypairSign_serializeMultiSignTransaction(JNIEnv *jEnv, jclass jType,
                                                                            jstring jTransaction) {
    auto transaction = JniUtils::GetStringSafely(jEnv, jTransaction);

    char* serialize = serializeMultiSignTransaction(transaction.get());
    if(serialize == nullptr) {
        return nullptr;
    }

    auto jSerialize = JniUtils::GetStringSafely(jEnv, serialize, false);
    freeBuf(serialize);

    return jSerialize.get();
}

extern "C"
JNIEXPORT jobjectArray JNICALL
Java_org_elastos_sdk_keypair_ElastosKeypairSign__1getSignedSigners(JNIEnv *jEnv, jclass jType,
                                                                   jstring jTransaction,
                                                                   jobject jOutLen) {
    auto transaction = JniUtils::GetStringSafely(jEnv, jTransaction);

    int signerCnt = 0;
    char** signerArray = getSignedSigners(transaction.get(), &signerCnt);

    if(signerArray == nullptr) {
        return nullptr;
    }

    jobjectArray ret = jEnv->NewObjectArray(signerCnt, jEnv->FindClass("java/lang/String"), jEnv->NewStringUTF(""));

    for(int i = 0; i < signerCnt; i++) {
        jEnv->SetObjectArrayElement(ret, i, jEnv->NewStringUTF(signerArray[i]));
    }

//    auto jSignerArray = JniUtils::GetStringArraySafely(jEnv, const_cast<const char**>(signerArray), signerCnt, false);
    for(int idx = 0; idx < signerCnt; idx++) {
        free(signerArray[idx]);
    }
    free(signerArray);

//    jclass jIntegerClass = jEnv->GetObjectClass(jOutLen);
//    jfieldID jIntegerFieldID = jEnv->GetFieldID(jIntegerClass, "value", "I");
//    jEnv->SetIntField(jOutLen, jIntegerFieldID, signerCnt);

//    return jSignerArray.get();
    return ret;
}

extern "C"
JNIEXPORT jstring JNICALL
Java_org_elastos_sdk_keypair_ElastosKeypairCrypto_eciesEncrypt(JNIEnv *jEnv, jclass jType,
                                                               jstring jPublicKey,
                                                               jbyteArray jPlainData) {
    auto publicKey = JniUtils::GetStringSafely(jEnv, jPublicKey);
    auto plainText = JniUtils::GetByteArraySafely(jEnv, jPlainData);
    auto len = jEnv->GetArrayLength (jPlainData);

    char* cipherText = eciesEncrypt(publicKey.get(),
            reinterpret_cast<unsigned char *>(plainText.get()), len);

    auto jCipherText = JniUtils::GetStringSafely(jEnv, cipherText, false);
    free(cipherText);

    return jCipherText.get();
}

extern "C"
JNIEXPORT jbyteArray JNICALL
Java_org_elastos_sdk_keypair_ElastosKeypairCrypto_eciesDecrypt(JNIEnv *jEnv, jclass jType,
                                                               jstring jPrivateKey,
                                                               jstring jCipherText) {
    auto privateKey = JniUtils::GetStringSafely(jEnv, jPrivateKey);
    auto cipherText = JniUtils::GetStringSafely(jEnv, jCipherText);

    int plainTextLen = 0;
    unsigned char* plainText = eciesDecrypt(privateKey.get(), cipherText.get(), &plainTextLen);

    jbyteArray jdata = jEnv->NewByteArray(plainTextLen);
    jEnv->SetByteArrayRegion(jdata, 0, plainTextLen, reinterpret_cast<int8_t *>(plainText));
    free(plainText);

    return jdata;
}
