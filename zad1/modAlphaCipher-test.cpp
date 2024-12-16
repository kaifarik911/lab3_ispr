#include <UnitTest++/UnitTest++.h>
#include <string>
#include "modAlphaCipher.h"
#include <locale>
#include <codecvt>

struct KeyB_fixture {
    modAlphaCipher * p;
    KeyB_fixture()
    {
        p = new modAlphaCipher(L"А");
    }
    ~KeyB_fixture()
    {
        delete p;
    }
};
//setup converter
using convert_type = std::codecvt_utf8<wchar_t>;
std::wstring_convert<convert_type, wchar_t> converter;

SUITE(Costructor)
{

    TEST(NormalKey) {
        std::wstring wres=modAlphaCipher(L"ГОД").encrypt(L"ЖЖЖЖЖЖ");
        std::string res = converter.to_bytes(wres);
        CHECK_EQUAL("ГОДГОД", res);
    }
    TEST(LowerlKey) {
        std::wstring wres=modAlphaCipher(L"ро").encrypt(L"ПАКРС");
        std::string res = converter.to_bytes(wres);
        CHECK_EQUAL("РОРОР", res);
    }
    TEST(LongKey) {
        std::wstring wres=modAlphaCipher(L"МАШИНА").encrypt(L"МАРТ");
        std::string res = converter.to_bytes(wres);
        CHECK_EQUAL("МАШИ", res);
    }

    TEST(DigitsInKey) {
        CHECK_THROW(modAlphaCipher cp(L"32"),cipher_error);
    }
    TEST(PunctuationInKey) {
        CHECK_THROW(modAlphaCipher cp(L"рпа.п"),cipher_error);
    }
    TEST(WhitespaceInKey) {
        CHECK_THROW(modAlphaCipher cp(L" РП "),cipher_error);
    }
    TEST(EmptyKey) {
        CHECK_THROW(modAlphaCipher cp(L""),cipher_error);
    }
}
SUITE(encrypt)
{
    TEST_FIXTURE(KeyB_fixture, UpperString) {
        std::wstring wres=p->encrypt(L"ПАОТК");
        std::string res = converter.to_bytes(wres);
        CHECK_EQUAL("ААААА", res);
    }
    TEST_FIXTURE(KeyB_fixture, LowerString) {
        std::wstring wres=p->encrypt(L"ПРпк");
        std::string res = converter.to_bytes(wres);
        CHECK_EQUAL("ААРЛ", res);
    }
    TEST_FIXTURE(KeyB_fixture, WhitespaceInString) {
        CHECK_THROW(p->encrypt(L"КАРАН ДАШ"), cipher_error);
    }
    TEST_FIXTURE(KeyB_fixture, PunctuationInString) {
        CHECK_THROW(p->encrypt(L"КАР,;.АНДАШ"), cipher_error);
    }
    TEST_FIXTURE(KeyB_fixture, DigitsInString) {
        CHECK_THROW(p->encrypt(L"СТОП432"), cipher_error);
    }
    TEST_FIXTURE(KeyB_fixture, Digits) {
        CHECK_THROW(p->encrypt(L"756"), cipher_error);
    }
    TEST_FIXTURE(KeyB_fixture, EmptyString) {
        CHECK_THROW(p->encrypt(L""), cipher_error);
    }
    TEST(MaxStep) {
        std::wstring wres=modAlphaCipher(L"Я").encrypt(L"АРБУЗ");
        std::string res = converter.to_bytes(wres);
        CHECK_EQUAL("ЯЯЯЯЯ", res);
    }
}
SUITE(decrypt)
{
    TEST_FIXTURE(KeyB_fixture, UpperString) {
        std::wstring wres=p->decrypt(L"ЛОПЛЛЛОЛРАО");
        std::string res = converter.to_bytes(wres);
        CHECK_EQUAL("ААААААААААА", res);
    }
    TEST_FIXTURE(KeyB_fixture, LowerString) {
        std::wstring wres=p->decrypt(L"АРРПрРррА");
        std::string res = converter.to_bytes(wres);
        CHECK_EQUAL("ААААСАССА", res);
    }
    TEST_FIXTURE(KeyB_fixture, WhitespaceInString) {
        CHECK_THROW(p->decrypt(L" О П АН Г"), cipher_error);
    }
    TEST_FIXTURE(KeyB_fixture, PunctuationInString) {
        CHECK_THROW(p->decrypt(L".ОЮЩ.)"), cipher_error);
    }
    TEST_FIXTURE(KeyB_fixture, DigitsInString) {
        CHECK_THROW(p->decrypt(L"ПКОР6Р8ЛО85"), cipher_error);
    }
    TEST_FIXTURE(KeyB_fixture, EmptyString) {
        CHECK_THROW(p->decrypt(L""), cipher_error);
    }
    TEST_FIXTURE(KeyB_fixture, MaxStep) {
        std::wstring wres=modAlphaCipher(L"Я").decrypt(L"ЯПАТЖ");
        std::string res = converter.to_bytes(wres);
        CHECK_EQUAL("БББББ", res);
    }
}
int main(int argc, char **argv)
{
    return UnitTest::RunAllTests();
}
