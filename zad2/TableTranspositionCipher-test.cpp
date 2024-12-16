#include <UnitTest++/UnitTest++.h>
#include <string>
#include "TableTranspositionCipher.h"
#include <locale>
#include <codecvt>
struct Key4_fixture {
    TableTranspositionCipher * p;
    Key4_fixture()
    {
        p = new TableTranspositionCipher(4);
    }
    ~Key4_fixture()
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
        std::wstring wres=TableTranspositionCipher(7).encrypt(L"АВТОМОБИЛЬ");
        std::string res = converter.to_bytes(wres);
        CHECK_EQUAL("Б*О*М*О*ТЬВЛАИ", res);
    }
    TEST(UselessKey) {
        CHECK_THROW(TableTranspositionCipher tb(1), cipher_error);
    }    
    TEST(NegativeKey) {
        CHECK_THROW(TableTranspositionCipher tb(-7), cipher_error);
    }
    TEST(LongKey) {
        std::wstring wres=TableTranspositionCipher(10).encrypt(L"КАРШЕРИНГ");
        std::string res = converter.to_bytes(wres);
        CHECK_EQUAL("*ГНИРЕШРАК", res);
    }

}
SUITE(encrypt)
{
    TEST_FIXTURE(Key4_fixture, UpperString) {
        std::wstring wres=p->encrypt(L"ВИДЕООБЗОР");
        std::string res = converter.to_bytes(wres);
        CHECK_EQUAL("ЕЗ*ДБ*ИОРВОО", res);
    }
    TEST_FIXTURE(Key4_fixture, WhitespaceInString) {
        CHECK_THROW(p->encrypt(L"ВИ ДЕ ОО БЗ ОР"), cipher_error);
    }
    TEST_FIXTURE(Key4_fixture, PunctuationInString) {
        std::wstring wres=p->encrypt(L"В.,И)Д(ЕО;:ОБЗОР");
        std::string res = converter.to_bytes(wres);
        CHECK_EQUAL("ИЕОР,(:О.Д;ЗВ)ОБ", res);
    }
    TEST_FIXTURE(Key4_fixture, DigitsInString) {
        std::wstring wres=p->encrypt(L"ВИДЕ54ОО89БЗОР");
        std::string res = converter.to_bytes(wres);
        CHECK_EQUAL("ЕОЗ*ДОБ*И49РВ58О", res);
    }
    TEST_FIXTURE(Key4_fixture, EmptyString) {
        CHECK_THROW(p->encrypt(L" "), cipher_error);
    }
    TEST(MainTest) {
        std::wstring wres=TableTranspositionCipher(7).encrypt(L"ЯНДЕКСБРАУЗЕР");
        std::string res = converter.to_bytes(wres);
        CHECK_EQUAL("Б*СРКЕЕЗДУНАЯР", res);
    }
}
SUITE(decrypt)
{
    TEST_FIXTURE(Key4_fixture, UpperString) {
        std::wstring wres=p->decrypt(L"ЕЗ*ДБ*ИОРВОО");
        std::string res = converter.to_bytes(wres);
        CHECK_EQUAL("ВИДЕООБЗОР**", res);
    }
    TEST_FIXTURE(Key4_fixture, WhitespaceInString) {
        CHECK_THROW(p->decrypt(L"ЕЗ*Д Б*ИО РВ ОО"), cipher_error);
    }
    TEST_FIXTURE(Key4_fixture, PunctuationInString) {
        std::wstring wres=p->decrypt(L"ИЕОР,(:О.Д;ЗВ)ОБ");
        std::string res = converter.to_bytes(wres);
        CHECK_EQUAL("В.,И)Д(ЕО;:ОБЗОР", res);
    }
    TEST_FIXTURE(Key4_fixture, DigitsInString) {
        std::wstring wres=p->decrypt(L"ЕОЗ*ДОБ*И49РВ58О");
        std::string res = converter.to_bytes(wres);
        CHECK_EQUAL("ВИДЕ54ОО89БЗОР**", res);
    }
    TEST_FIXTURE(Key4_fixture, EmptyString) {
        CHECK_THROW(p->decrypt(L" "), cipher_error);
    }
    TEST(MainTest) {
        std::wstring wres=TableTranspositionCipher(7).decrypt(L"Б*СРКЕЕЗДУНАЯР");
        std::string res = converter.to_bytes(wres);
        CHECK_EQUAL("ЯНДЕКСБРАУЗЕР*", res);
    }
}
int main(int argc, char **argv)
{
    return UnitTest::RunAllTests();
}
