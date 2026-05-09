#include <iomanip>
#include <jni.h>
#include <sstream>
#include <string>
#include <vector>

extern "C" {

JNIEXPORT jstring JNICALL Java_com_dictor_lobstercrypt_MainActivity_encryptData(
    JNIEnv *env, jobject thiz, jstring input, jstring pin) {
  if (!input || !pin)
    return env->NewStringUTF("");
  const char *in_c = env->GetStringUTFChars(input, nullptr);
  const char *pin_c = env->GetStringUTFChars(pin, nullptr);

  std::string text(in_c), key_s(pin_c);
  std::stringstream ss;
  ss << std::hex << std::setfill('0');

  for (size_t i = 0; i < text.length(); ++i) {
    uint8_t key = (key_s.empty()) ? 0x4C : (uint8_t)key_s[i % key_s.length()];
    uint8_t b = (uint8_t)text[i] ^ key;
    b = (uint8_t)((b << 3) | (b >> 5));
    ss << std::setw(2) << (int)b;
  }

  env->ReleaseStringUTFChars(input, in_c);
  env->ReleaseStringUTFChars(pin, pin_c);
  return env->NewStringUTF(ss.str().c_str());
}

JNIEXPORT jstring JNICALL Java_com_dictor_lobstercrypt_MainActivity_decryptData(
    JNIEnv *env, jobject thiz, jstring input, jstring pin) {
  if (!input || !pin)
    return env->NewStringUTF("");
  const char *in_c = env->GetStringUTFChars(input, nullptr);
  const char *pin_c = env->GetStringUTFChars(pin, nullptr);

  std::string hex(in_c), key_s(pin_c), res = "";

  try {
    for (size_t i = 0; i + 1 < hex.length(); i += 2) {
      uint8_t b = (uint8_t)std::stoul(hex.substr(i, 2), nullptr, 16);
      b = (uint8_t)((b >> 3) | (b << 5));
      uint8_t key =
          (key_s.empty()) ? 0x4C : (uint8_t)key_s[(i / 2) % key_s.length()];
      res += (char)(b ^ key);
    }
  } catch (...) {
    res = "ERROR";
  }

  env->ReleaseStringUTFChars(input, in_c);
  env->ReleaseStringUTFChars(pin, pin_c);
  return env->NewStringUTF(res.c_str());
}
}
