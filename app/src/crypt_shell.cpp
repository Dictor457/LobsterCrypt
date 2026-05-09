#include <iomanip>
#include <jni.h>
#include <sstream>
#include <string>
#include <vector>

const uint8_t MASTER_KEY[] = "LOBSTER_STRONG_CRYPT_2026_PROTOT";
const std::string MAGIC_MARKER = "LBS_OK:";

extern "C" {

// ШИФРОВАНИЕ
JNIEXPORT jstring JNICALL Java_com_dictor_lobstercrypt_MainActivity_encryptData(
    JNIEnv *env, jobject thiz, jbyteArray input, jint user_pin) {

  if (!input)
    return env->NewStringUTF("");

  jsize len = env->GetArrayLength(input);
  jbyte *bytes = env->GetByteArrayElements(input, nullptr);

  std::string plainText(reinterpret_cast<char *>(bytes), len);
  std::string markedText = MAGIC_MARKER + plainText;

  std::stringstream ss;
  ss << std::hex << std::uppercase << std::setfill('0');

  for (size_t i = 0; i < markedText.length(); ++i) {
    uint8_t b = (uint8_t)markedText[i];
    b ^= MASTER_KEY[i % 32];
    b ^= (uint8_t)user_pin;
    b = (uint8_t)((b << 3) | (b >> 5));
    ss << std::setw(2) << (int)b;
  }

  env->ReleaseByteArrayElements(input, bytes, JNI_ABORT);
  return env->NewStringUTF(ss.str().c_str());
}

// ДЕШИФРОВАНИЕ
JNIEXPORT jstring JNICALL Java_com_dictor_lobstercrypt_MainActivity_decryptData(
    JNIEnv *env, jobject thiz, jstring hexInput, jint user_pin) {

  const char *hexStr = env->GetStringUTFChars(hexInput, nullptr);
  if (!hexStr)
    return env->NewStringUTF("");
  std::string hex(hexStr);
  env->ReleaseStringUTFChars(hexInput, hexStr);

  std::vector<uint8_t> decryptedBytes;
  for (size_t i = 0; i < hex.length(); i += 2) {
    if (i + 2 > hex.length())
      break;
    std::string byteString = hex.substr(i, 2);
    uint8_t b = (uint8_t)strtol(byteString.c_str(), nullptr, 16);

    b = (uint8_t)((b >> 3) | (b << 5));
    b ^= (uint8_t)user_pin;
    b ^= MASTER_KEY[(i / 2) % 32];
    decryptedBytes.push_back(b);
  }

  std::string result(decryptedBytes.begin(), decryptedBytes.end());

  if (result.find(MAGIC_MARKER) == 0) {
    return env->NewStringUTF(result.substr(MAGIC_MARKER.length()).c_str());
  } else {
    return env->NewStringUTF("ERROR_WRONG_PIN");
  }
}
}
